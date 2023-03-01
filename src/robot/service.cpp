#include <common/v1/common.pb.h>
#include <google/protobuf/struct.pb.h>
#include <grpcpp/support/status.h>
#include <robot/v1/robot.grpc.pb.h>
#include <robot/v1/robot.pb.h>

#include <common/utils.hpp>
#include <components/component_base.hpp>
#include <memory>
#include <mutex>
#include <registry/registry.hpp>
#include <resource/resource.hpp>
#include <robot/client.hpp>
#include <robot/service.hpp>
#include <services/service_base.hpp>
#include <string>
#include <thread>
#include <unordered_map>

using google::protobuf::RepeatedPtrField;
using viam::common::v1::ResourceName;
using viam::robot::v1::Status;

std::vector<ResourceName> RobotService_::generate_metadata() {
    std::vector<ResourceName> metadata;
    for (const auto& key_and_val : manager.resources) {
        for (ResourceName resource : resource_names_for_resources(key_and_val.second)) {
            metadata.push_back(resource);
        }
    }
    return metadata;
}

std::vector<Status> RobotService_::generate_status(RepeatedPtrField<ResourceName> resource_names) {
    std::vector<Status> statuses;
    for (auto& cmp : manager.resources) {
        std::shared_ptr<ResourceBase> resource = cmp.second;
        for (auto& registry : Registry::registered_resources()) {
            std::shared_ptr<ModelRegistration> registration = registry.second;
            if (registration->resource_type == resource->type) {
                bool resource_present = false;
                ResourceName* name;
                try {
                    std::shared_ptr<ComponentBase> cb =
                        std::dynamic_pointer_cast<ComponentBase>(resource);
                    ResourceName rn = cb->get_resource_name(resource->name);
                    name = &rn;
                } catch (std::exception& exc) {
                };
                if (name == nullptr) {
                    try {
                        std::shared_ptr<ServiceBase> sb =
                            std::dynamic_pointer_cast<ServiceBase>(resource);
                        ResourceName rn = sb->get_resource_name(resource->name);
                        name = &rn;
                    } catch (std::exception& exc) {
                        throw "unable to create status; provided resource was of an unknown type";
                    };
                }
                for (auto& resource_name : resource_names) {
                    if (name == &resource_name) {
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
    std::unordered_map<std::string, std::unordered_map<std::string, ProtoType*>> extra;
    grpc::StatusCode status = grpc::StatusCode::OK;
    for (auto& ex : request->extra()) {
        google::protobuf::Struct struct_ = ex.params();
        std::unordered_map<std::string, ProtoType*> value_map = struct_to_map(struct_);
        std::string name = ex.name().SerializeAsString();
        extra.emplace(name, value_map);

        for (auto& comp : manager.components) {
            std::shared_ptr<ComponentBase> component = comp.second;
            ResourceName rn = component->get_resource_name(component->name);
            std::string rn_ = rn.SerializeAsString();
            if (extra.find(rn_) != extra.end()) {
                grpc::StatusCode status = component->stop(extra.at(rn_));
                if (status != grpc::StatusCode::OK) {
                    status = component->stop();
                }
            } else {
                status = component->stop();
            }
        }
    }

    return grpc::Status(status, "");
}

std::shared_ptr<ResourceBase> RobotService_::resource_by_name(Name name) {
    std::shared_ptr<ResourceBase> r;
    lock.lock();
    if (manager.components.find(name.name) != manager.components.end()) {
        r = manager.components.at(name.name);
    } else if (manager.services.find(name.name) != manager.services.end()) {
        r = manager.services.at(name.name);
    }

    lock.unlock();
    return r;
}

RobotService_::RobotService_(){};

std::shared_ptr<RobotService_> RobotService_::create() {
    return std::make_shared<RobotService_>();
};

