#include <viam/sdk/robot/service.hpp>

#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unordered_map>

#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/robot/v1/robot.grpc.pb.h>
#include <viam/api/robot/v1/robot.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/component_base.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/robot/client.hpp>
#include <viam/sdk/services/service_base.hpp>

namespace viam {
namespace sdk {

using google::protobuf::RepeatedPtrField;
using viam::common::v1::ResourceName;
using viam::robot::v1::Status;

std::vector<ResourceName> RobotService_::generate_metadata() {
    std::vector<ResourceName> metadata;
    for (const auto& key_and_val : manager().resources()) {
        for (ResourceName resource : resource_names_for_resource(key_and_val.second)) {
            metadata.push_back(resource);
        }
    }
    return metadata;
}

std::vector<Status> RobotService_::generate_status(RepeatedPtrField<ResourceName> resource_names) {
    std::vector<Status> statuses;
    for (auto& cmp : manager().resources()) {
        std::shared_ptr<ResourceBase> resource = cmp.second;
        for (auto& registry : Registry::registered_resources()) {
            std::shared_ptr<ModelRegistration> registration = registry.second;
            if (registration->resource_type() == resource->type()) {
                bool resource_present = false;
                ResourceName name = resource->get_resource_name(resource->name());
                for (auto& resource_name : resource_names) {
                    if (&name == &resource_name) {
                        resource_present = true;
                        break;
                    }
                }

                if (resource_present) {
                    Status status = registration->create_status(resource);
                    statuses.push_back(status);
                }
            }
        }
    }

    std::vector<Status> returnable_statuses;
    for (auto& status : statuses) {
        bool status_name_is_known = false;
        for (auto& resource_name : resource_names) {
            if (status.name().SerializeAsString() == resource_name.SerializeAsString()) {
                status_name_is_known = true;
                break;
            }
        }
        if (status_name_is_known) {
            returnable_statuses.push_back(status);
        }
    }
    return returnable_statuses;
}

::grpc::Status RobotService_::ResourceNames(::grpc::ServerContext* context,
                                            const viam::robot::v1::ResourceNamesRequest* request,
                                            viam::robot::v1::ResourceNamesResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [ResourceNames] without a request");
    };
    RepeatedPtrField<ResourceName>* p = response->mutable_resources();
    for (ResourceName name : generate_metadata()) {
        *p->Add() = name;
    }

    return ::grpc::Status();
}

::grpc::Status RobotService_::GetStatus(::grpc::ServerContext* context,
                                        const ::viam::robot::v1::GetStatusRequest* request,
                                        ::viam::robot::v1::GetStatusResponse* response) {
    if (!request) {
        return ::grpc::Status(::grpc::StatusCode::INVALID_ARGUMENT,
                              "Called [GetStatus] without a request");
    };

    RepeatedPtrField<Status>* response_status = response->mutable_status();
    std::vector<Status> statuses = generate_status(request->resource_names());
    for (Status status : statuses) {
        *response_status->Add() = status;
    }

    return ::grpc::Status();
}

void RobotService_::stream_status(
    const viam::robot::v1::StreamStatusRequest* request,
    ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>* writer,
    int interval) {
    while (true) {
        std::vector<Status> statuses = generate_status(request->resource_names());
        viam::robot::v1::StreamStatusResponse response;
        RepeatedPtrField<Status>* response_status = response.mutable_status();
        for (Status status : statuses) {
            *response_status->Add() = status;
        }

        writer->Write(response);
        std::this_thread::sleep_for(std::chrono::seconds(interval));
    }
}

::grpc::Status RobotService_::StreamStatus(
    ::grpc::ServerContext* context,
    const ::viam::robot::v1::StreamStatusRequest* request,
    ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>* writer) {
    int interval = 1;
    if (request->every().seconds() > 0) {
        interval = request->every().seconds();
    }
    RepeatedPtrField<ResourceName> resource_names = request->resource_names();
    std::thread t(&RobotService_::stream_status, this, request, writer, interval);
    t.detach();
    return ::grpc::Status();
}

::grpc::Status RobotService_::StopAll(::grpc::ServerContext* context,
                                      const ::viam::robot::v1::StopAllRequest* request,
                                      ::viam::robot::v1::StopAllResponse* response) {
    ResourceName r;
    std::unordered_map<std::string, AttributeMap> extra;
    grpc::StatusCode status = grpc::StatusCode::OK;
    for (auto& ex : request->extra()) {
        google::protobuf::Struct struct_ = ex.params();
        AttributeMap value_map = struct_to_map(struct_);
        std::string name = ex.name().SerializeAsString();
        extra.emplace(name, value_map);

        for (auto& r : manager().resources()) {
            std::shared_ptr<ResourceBase> resource = r.second;
            ResourceName rn = resource->get_resource_name(resource->name());
            std::string rn_ = rn.SerializeAsString();
            if (extra.find(rn_) != extra.end()) {
                grpc::StatusCode status = resource->stop(extra.at(rn_));
                if (status != grpc::StatusCode::OK) {
                    status = resource->stop();
                }
            } else {
                status = resource->stop();
            }
        }
    }

    return grpc::Status(status, "");
}

std::shared_ptr<ResourceBase> RobotService_::resource_by_name(Name name) {
    std::shared_ptr<ResourceBase> r;
    std::lock_guard<std::mutex> lock(lock_);
    if (manager().resources().find(name.name()) != manager().resources().end()) {
        r = manager().resources().at(name.name());
    }

    return r;
}

RobotService_::RobotService_(){};

std::shared_ptr<RobotService_> RobotService_::create() {
    return std::make_shared<RobotService_>();
};

}  // namespace sdk
}  // namespace viam
