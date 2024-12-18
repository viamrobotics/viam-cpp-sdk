#pragma once

#include <string>

#include <google/protobuf/descriptor.h>

#include <viam/sdk/common/proto_convert.hpp>

namespace viam {
namespace common {
namespace v1 {

class ResourceName;

}
}  // namespace common
}  // namespace viam

namespace viam {
namespace sdk {

/// @class APIType
/// @brief Defines a resource's namespace (e.g., `RDK`) and type (e.g., component or service).
class APIType {
   public:
    APIType() = default;
    APIType(std::string namespace_, std::string resource_type);

    virtual std::string to_string() const;

    const std::string& type_namespace() const;
    const std::string& resource_type() const;

    void set_namespace(const std::string& type_namespace);
    void set_resource_type(const std::string& resource_type);

   private:
    std::string namespace_;
    std::string resource_type_;
};

/// @class API
/// @brief Extends `APIType` to additionally define a resource's subtype (e.g., `camera`).
// TODO: Maybe just merge these two classes or at least use composition rather than inheritance
class API : public APIType {
   public:
    static API from_string(std::string api);

    API() = default;
    API(std::string namespace_, std::string resource_type, std::string resource_subtype);
    API(APIType type, std::string resource_subtype);

    virtual std::string to_string() const override;

    const std::string& resource_subtype() const;
    void set_resource_subtype(const std::string& subtype);
    bool is_component_type();
    bool is_service_type();
    friend bool operator==(API const& lhs, API const& rhs);
    friend bool operator<(const API& lhs, const API& rhs);
    friend std::ostream& operator<<(std::ostream& os, const API& v);

    template <typename T>
    struct traits;

    template <typename T>
    static API get() {
        return traits<T>::api();
    }

   private:
    std::string resource_subtype_;
};

/// @class Name
/// @brief A name for specific instances of resources.
class Name {
   public:
    static Name from_string(std::string name);

    Name(API api, std::string remote_name, std::string name);
    Name() = default;

    std::string short_name() const;
    std::string to_string() const;

    const API& api() const;
    const std::string& name() const;
    const std::string& remote_name() const;

    friend bool operator==(const Name& lhs, const Name& rhs);
    friend std::ostream& operator<<(std::ostream& os, const Name& v);

   private:
    API api_;
    std::string remote_name_;
    std::string name_;
};

namespace proto_convert_details {

template <>
struct to_proto<Name> {
    void operator()(const Name&, common::v1::ResourceName*) const;
};

template <>
struct from_proto<common::v1::ResourceName> {
    Name operator()(const common::v1::ResourceName*) const;
};

}  // namespace proto_convert_details

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

/// @class Model
/// @brief Defines the namespace_, family, and name for a particular resource model.
class Model {
   public:
    std::string to_string() const;

    Model(std::string namespace_, std::string family, std::string model_name);
    Model(ModelFamily model, std::string model_name);
    Model();

    /// @brief Parses a single model string into a Model, using default values for namespace and
    /// family if not provided.
    ///
    /// @throws `Exception` if given an invalid model (i.e., one with non-word characters).
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
