#pragma once

#include <string>

#include <google/protobuf/descriptor.h>

#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {

class APIType {
   public:
    APIType(std::string namespace_, std::string resource_type);
    APIType(){};
    virtual std::string to_string() const;

    const std::string& type_namespace() const;
    const std::string& resource_type() const;

    void set_namespace(const std::string type_namespace);
    void set_resource_type(const std::string resource_type);

   private:
    std::string namespace_;
    std::string resource_type_;
};

class API : public APIType {
   public:
    virtual std::string to_string() const override;
    API(){};
    API(std::string namespace_, std::string resource_type, std::string resource_subtype);
    API(APIType type, std::string resource_subtype);
    static API from_string(std::string api);

    const std::string& resource_subtype() const;
    void set_resource_subtype(const std::string subtype);
    bool is_component_type();
    bool is_service_type();
    friend bool operator==(API const& lhs, API const& rhs);
    friend bool operator<(const API& lhs, const API& rhs);

   private:
    std::string resource_subtype_;
};

// TODO: instead of inheriting from API probably this should just have a
// API as a member
class Name : public API {
   public:
    std::string short_name() const;
    virtual std::string to_string() const override;
    // TODO: this isn't necessary, instead this->API::to_string();
    const API* to_api() const;
    viam::common::v1::ResourceName to_proto() const;
    static Name from_string(std::string name);
    Name(API api, std::string remote_name, std::string name);
    Name();
    const std::string& name() const;
    const std::string& remote_name() const;
    friend bool operator==(const Name& lhs, const Name& rhs);

   private:
    std::string remote_name_;
    std::string name_;
};

class RPCSubtype {
   public:
    bool operator<(const RPCSubtype& rhs) const {
        return (api_.to_string() + proto_service_name_ + descriptor_.DebugString()) <
               (rhs.api_.to_string() + rhs.proto_service_name_ + rhs.descriptor_.DebugString());
    };
    const std::string& proto_service_name() const;
    const API& api() const;

    RPCSubtype(API api, const google::protobuf::ServiceDescriptor& descriptor);
    RPCSubtype(API api,
               std::string proto_service_name,
               const google::protobuf::ServiceDescriptor& descriptor);
    friend bool operator==(const RPCSubtype& lhs, const RPCSubtype& rhs);

   private:
    const google::protobuf::ServiceDescriptor& descriptor_;
    std::string proto_service_name_;
    API api_;
};

class ModelFamily {
   public:
    ModelFamily(std::string namespace_, std::string family);
    std::string to_string() const;

   private:
    std::string namespace_;
    std::string family_;
};

class Model {
   public:
    std::string to_string() const;

    Model(std::string namespace_, std::string family, std::string model_name);
    Model(ModelFamily model, std::string model_name);
    Model();
    /// Parses a single model string into a Model. If only a model_name is
    /// included, then default values will be used for namespace and family.
    ///
    /// Raises:
    /// 	Will throw an error if an invalid model string is passed (i.e., using
    /// non-word characters)
    static Model from_str(std::string model);
    friend bool operator==(const Model& lhs, const Model& rhs);

   private:
    ModelFamily model_family_;
    std::string model_name_;
};

}  // namespace sdk
}  // namespace viam

template <>
struct std::hash<::viam::sdk::Name> {
    size_t operator()(::viam::sdk::Name const& key) const noexcept {
        return std::hash<std::string>()(key.to_string());
    }
};

template <>
struct std::hash<::viam::sdk::RPCSubtype> {
    size_t operator()(::viam::sdk::RPCSubtype const& key) const noexcept {
        const ::viam::sdk::API& api = key.api();
        std::string hash = api.to_string() + key.proto_service_name();
        return std::hash<std::string>()(hash);
    };
};

template <>
struct std::hash<::viam::sdk::API> {
    size_t operator()(const ::viam::sdk::API& key) const noexcept {
        return std::hash<std::string>()(key.to_string());
    };
};
