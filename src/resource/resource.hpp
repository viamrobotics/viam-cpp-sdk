#pragma once

#include <google/protobuf/descriptor.h>

#include <string>

#include "common/v1/common.pb.h"

class Type {
   public:
    std::string namespace_;
    std::string resource_type;
    Type(std::string namespace_, std::string resource_type);
    Type(){};

    virtual std::string to_string() const;
};

class Subtype : public Type {
   public:
    std::string resource_subtype;

    std::string to_string() const;
    Subtype(){};
    Subtype(std::string namespace_, std::string resource_type, std::string resource_subtype);
    Subtype(Type type, std::string resource_subtype);
    static Subtype from_string(std::string subtype);
    bool is_component_type();
    bool is_service_type();
    friend bool operator==(Subtype const& lhs, Subtype const& rhs);
    friend bool operator<(const Subtype& lhs, const Subtype& rhs);
};

// TODO: instead of inheriting from Subtype probably this should just have a
// Subtype as a member
class Name : public Subtype {
   public:
    std::string remote_name;
    std::string name;

    std::string short_name() const;
    std::string to_string() const;
    // TODO: this isn't necessary, instead this->Subtype::to_string();
    const Subtype* to_subtype() const;
    viam::common::v1::ResourceName to_proto();
    static Name from_string(std::string name);
    Name(Subtype subtype, std::string remote_name, std::string name);
    Name();
    friend bool operator==(const Name& lhs, const Name& rhs);
};

class RPCSubtype {
   public:
    Subtype subtype;
    std::string proto_service_name;
    const google::protobuf::ServiceDescriptor* descriptor;

    bool operator<(const RPCSubtype& rhs) const {
        return (subtype.to_string() + proto_service_name + descriptor->DebugString()) <
               (rhs.subtype.to_string() + rhs.proto_service_name + rhs.descriptor->DebugString());
    };

    RPCSubtype(Subtype subtype, const google::protobuf::ServiceDescriptor& descriptor);
    RPCSubtype(Subtype subtype,
               std::string proto_service_name,
               const google::protobuf::ServiceDescriptor& descriptor);
    friend bool operator==(const RPCSubtype& lhs, const RPCSubtype& rhs);
};

class ModelFamily {
   public:
    std::string namespace_;
    std::string family;

    ModelFamily(std::string namespace_, std::string family);
    std::string to_string() const;
};

class Model {
   public:
    ModelFamily model_family;
    std::string model_name;
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
};

template <>
struct std::hash<Name> {
    size_t operator()(Name const& key) const noexcept {
        return std::hash<std::string>()(key.to_string());
    }
};

template <>
struct std::hash<RPCSubtype> {
    size_t operator()(RPCSubtype const& key) const noexcept {
        Subtype subtype = key.subtype;
        std::string hash = subtype.to_string() + key.proto_service_name;
        return std::hash<std::string>()(hash);
    };
};

template <>
struct std::hash<Subtype> {
    size_t operator()(const Subtype& key) const noexcept {
        return std::hash<std::string>()(key.to_string());
    };
};
