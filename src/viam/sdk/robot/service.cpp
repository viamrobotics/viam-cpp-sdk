#include <viam/sdk/robot/service.hpp>

#include <memory>
#include <mutex>
#include <stdexcept>
#include <string>
#include <thread>
#include <unordered_map>

#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/stoppable.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/services/service.hpp>

namespace viam {
namespace sdk {

using google::protobuf::RepeatedPtrField;
using viam::common::v1::ResourceName;

namespace {
std::vector<Name> registered_models_for_resource(const std::shared_ptr<Resource>& resource) {
    std::string resource_type;
    std::string resource_subtype;
    std::vector<Name> resource_names;
    for (const auto& kv : Registry::registered_models()) {
        const std::shared_ptr<const ModelRegistration> reg = kv.second;
        if (reg->api() == resource->api()) {
            resource_type = reg->api().resource_type();
            resource_subtype = reg->api().resource_subtype();
        } else {
            continue;
        }

        if (resource_subtype.empty()) {
            resource_subtype = resource->name();
        }

        resource_names.push_back({{kRDK, resource_type, resource_subtype}, "", resource->name()});
    }
    return resource_names;
}
}  // namespace

RobotService_::RobotService_(const std::shared_ptr<ResourceManager>& manager, Server& server)
    : ResourceServer(manager) {
    server.register_service(this);
    // register all managed resources with the appropriate resource servers.
    for (const auto& resource : manager->resources()) {
        server.add_resource(resource.second);
    }
}

std::vector<ResourceName> RobotService_::generate_metadata_() {
    std::vector<ResourceName> metadata;
    for (const auto& key_and_val : resource_manager()->resources()) {
        for (const Name& name : registered_models_for_resource(key_and_val.second)) {
            metadata.push_back(name.to_proto());
        }
    }
    return metadata;
}

::grpc::Status RobotService_::ResourceNames(::grpc::ServerContext*,
                                            const viam::robot::v1::ResourceNamesRequest* request,
                                            viam::robot::v1::ResourceNamesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [ResourceNames] without a request");
    };
    RepeatedPtrField<ResourceName>* p = response->mutable_resources();
    for (const ResourceName& name : generate_metadata_()) {
        *p->Add() = name;
    }

    return ::grpc::Status();
}

::grpc::Status RobotService_::StopAll(::grpc::ServerContext*,
                                      const ::viam::robot::v1::StopAllRequest* request,
                                      ::viam::robot::v1::StopAllResponse*) {
    const ResourceName r;
    std::unordered_map<std::string, ProtoStruct> extra;
    for (const auto& ex : request->extra()) {
        const google::protobuf::Struct& struct_ = ex.params();
        const ProtoStruct value_map = v2::from_proto(struct_);
        const std::string name = ex.name().SerializeAsString();
        extra.emplace(name, value_map);
    }

    grpc::StatusCode status = grpc::StatusCode::OK;
    std::string status_message;

    for (const auto& r : resource_manager()->resources()) {
        const std::shared_ptr<Resource> resource = r.second;
        const ResourceName rn = resource->get_resource_name().to_proto();
        const std::string rn_ = rn.SerializeAsString();
        if (extra.find(rn_) != extra.end()) {
            try {
                Stoppable::stop_if_stoppable(resource, extra.at(rn_));
            } catch (const std::runtime_error& err) {
                try {
                    status_message = err.what();
                    Stoppable::stop_if_stoppable(resource);
                } catch (std::runtime_error& err) {
                    status_message = err.what();
                    status = grpc::UNKNOWN;
                } catch (...) {
                    status_message = "unknown error";
                    status = grpc::UNKNOWN;
                }
            }
        } else {
            try {
                Stoppable::stop_if_stoppable(resource);
            } catch (std::runtime_error& err) {
                status_message = err.what();
                status = grpc::UNKNOWN;
            } catch (...) {
                status_message = "unknown error";
                status = grpc::UNKNOWN;
            }
        }
    }

    return grpc::Status(status, status_message);
}

std::shared_ptr<Resource> RobotService_::resource_by_name(const Name& name) {
    std::shared_ptr<Resource> r;
    const std::lock_guard<std::mutex> lock(lock_);
    auto resources = resource_manager()->resources();
    if (resources.find(name.name()) != resources.end()) {
        r = resources.at(name.name());
    }

    return r;
}

}  // namespace sdk
}  // namespace viam
