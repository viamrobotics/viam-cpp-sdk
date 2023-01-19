#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>

#include <string>
#include <thread>
#include <unordered_map>

#include "../common/utils.h"
#include "../components/component_base.h"
#include "../components/service_base.h"
#include "../robot/client.h"
#include "common/v1/common.pb.h"
#include "grpcpp/server_context.h"
#include "robot/v1/robot.grpc.pb.h"
#include "robot/v1/robot.pb.h"

using google::protobuf::RepeatedPtrField;
using viam::common::v1::ResourceName;
using viam::robot::v1::Status;

class RobotService_ : public ComponentServiceBase, public viam::robot::v1::RobotService::Service {
   public:
    ::grpc::Status ResourceNames(::grpc::ServerContext* context,
                                 const ::viam::robot::v1::ResourceNamesRequest* request,
                                 ::viam::robot::v1::ResourceNamesResponse* response) override;
    ::grpc::Status GetStatus(::grpc::ServerContext* context,
                             const ::viam::robot::v1::GetStatusRequest* request,
                             ::viam::robot::v1::GetStatusResponse* response) override;
    ::grpc::Status StreamStatus(
        ::grpc::ServerContext* context,
        const ::viam::robot::v1::StreamStatusRequest* request,
        ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>* writer) override;
    ::grpc::Status StopAll(::grpc::ServerContext* context,
                           const ::viam::robot::v1::StopAllRequest* request,
                           ::viam::robot::v1::StopAllResponse* response) override;

   private:
    std::vector<ResourceName> generate_metadata();
    std::vector<Status> generate_status(RepeatedPtrField<ResourceName> resources);

    void stream_status(const ::viam::robot::v1::StreamStatusRequest* request,
                       ::grpc::ServerWriter<::viam::robot::v1::StreamStatusResponse>* writer,
                       int interval);
};

std::vector<ResourceName> RobotService_::generate_metadata() {
    std::vector<ResourceName> metadata;
    for (const auto& key_and_val : manager.components) {
        for (ResourceName resource : resource_names_for_component(key_and_val.second)) {
            metadata.push_back(resource);
        }
    }
    return metadata;
}

std::vector<Status> RobotService_::generate_status(RepeatedPtrField<ResourceName> resource_names) {
    std::vector<Status> statuses;
    for (auto cmp : manager.components) {
        ComponentBase component = cmp.second;
        for (auto registry : Registry::registered_components()) {
            ComponentRegistration registration = registry.second;
            if (registration.component_type == component.type) {
                bool component_present = false;
                ResourceName component_name = component.get_resource_name(component.name);
                for (auto resource_name : resource_names) {
                    if (&resource_name == &component_name) {
                        component_present = true;
                        break;
                    }
                }

                if (component_present) {
                    Status status = registration.create_status(component);
                    statuses.push_back(status);
                }
            }
        }
    }
    std::vector<Status> returnable_statuses;
    for (auto status : statuses) {
        bool status_name_is_known = false;
        for (auto resource_name : resource_names) {
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
    if (request == nullptr) {
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
    if (request == nullptr) {
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
        viam::robot::v1::StreamStatusResponse* response;
        RepeatedPtrField<Status>* response_status = response->mutable_status();
        for (Status status : statuses) {
            *response_status->Add() = status;
        }

        const viam::robot::v1::StreamStatusResponse* resp = response;
        writer->Write(*resp);
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
    std::unordered_map<std::string, std::unordered_map<std::string, ProtoType>> extra;
    grpc::StatusCode status = grpc::StatusCode::OK;
    for (auto ex : request->extra()) {
        google::protobuf::Struct struct_ = ex.params();
        std::unordered_map<std::string, ProtoType> value_map = struct_to_map(struct_);
        std::string name = ex.name().SerializeAsString();
        std::pair<std::string, std::unordered_map<std::string, ProtoType>> pair_(name, value_map);
        extra.insert(pair_);

        for (auto comp : manager.components) {
            ComponentBase component = comp.second;
            ResourceName rn = component.get_resource_name(component.name);
            std::string rn_ = rn.SerializeAsString();
            if (extra.find(rn_) != extra.end()) {
                grpc::StatusCode status = component.stop(extra.at(rn_));
                if (status != grpc::StatusCode::OK) {
                    status = component.stop();
                }
            } else {
                status = component.stop();
            }
        }
    }

    return grpc::Status(status, "");
}
