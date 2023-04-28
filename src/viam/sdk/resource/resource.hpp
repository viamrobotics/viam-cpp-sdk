#pragma once

#include <string>

#include <google/protobuf/descriptor.h>

#include <viam/api/common/v1/common.pb.h>

namespace viam {
namespace sdk {

class Type {
   public:
    Type(std::string namespace_, std::string resource_type);
    Type(){};
    virtual std::string to_string() const;

    const std::string& type_namespace() const;
    const std::string& resource_type() const;

    void set_namespace(const std::string type_namespace);
    void set_resource_type(const std::string resource_type);

   private:
    std::string namespace_;
    std::string resource_type_;
};

class Subtype : public Type {
   public:
    virtual std::string to_string() const override;
    Subtype(){};
    Subtype(std::string namespace_, std::string resource_type, std::string resource_subtype);
    Subtype(Type type, std::string resource_subtype);
    static Subtype from_string(std::string subtype);

    const std::string& resource_subtype() const;
    void set_resource_subtype(const std::string subtype);
    bool is_component_type();
    bool is_service_type();
    friend bool operator==(Subtype const& lhs, Subtype const& rhs);
    friend bool operator<(const Subtype& lhs, const Subtype& rhs);

   private:
    std::string resource_subtype_;
};

// TODO: instead of inheriting from Subtype probably this should just have a
// Subtype as a member
class Name : public Subtype {
   public:
    const std::string short_name() const;
    virtual std::string to_string() const override;
    // TODO: this isn't necessary, instead this->Subtype::to_string();
    const Subtype* to_subtype() const;
    const viam::common::v1::ResourceName to_proto() const;
    static Name from_string(std::string name);
    Name(Subtype subtype, std::string remote_name, std::string name);
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
        return (subtype_.to_string() + proto_service_name_ + descriptor_.DebugString()) <
               (rhs.subtype_.to_string() + rhs.proto_service_name_ + rhs.descriptor_.DebugString());
    };
    const std::string& proto_service_name() const;
    const Subtype& subtype() const;

    RPCSubtype(Subtype subtype, const google::protobuf::ServiceDescriptor& descriptor);
    RPCSubtype(Subtype subtype,
               std::string proto_service_name,
               const google::protobuf::ServiceDescriptor& descriptor);
    friend bool operator==(const RPCSubtype& lhs, const RPCSubtype& rhs);

   private:
    const google::protobuf::ServiceDescriptor& descriptor_;
    std::string proto_service_name_;
    Subtype subtype_;
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
        ::viam::sdk::Subtype subtype = key.subtype();
        std::string hash = subtype.to_string() + key.proto_service_name();
        return std::hash<std::string>()(hash);
    };
};

template <>
struct std::hash<::viam::sdk::Subtype> {
    size_t operator()(const ::viam::sdk::Subtype& key) const noexcept {
        return std::hash<std::string>()(key.to_string());
    };
};
