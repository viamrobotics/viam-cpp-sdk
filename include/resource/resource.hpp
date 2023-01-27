#pragma once

#include <google/protobuf/descriptor.h>

#include <components/component_base.hpp>
#include <string>

class Type {
   public:
    std::string namespace_;
    std::string resource_type;

    std::string to_string();
};

class Subtype : public Type {
   public:
    std::string resource_subtype;

    const std::string to_string();
    Subtype(std::string subtype);
    Subtype(std::string namespace_, std::string resource_type, std::string resource_subtype);
    Subtype(Type type, std::string resource_subtype);
    bool is_component_type();
    bool is_service_type();
    friend bool operator==(const Subtype& lhs, const Subtype& rhs);
};

class Name : public Subtype {
   public:
    std::string remote_name;
    std::string name;

    const std::string to_string() const;
    const Subtype to_subtype() const;
    viam::common::v1::ResourceName to_proto();
    Name(std::string name);
    Name(Subtype subtype, std::string remote_name, std::string name);
    Name();
    friend bool operator==(const Name& lhs, const Name& rhs);
};

class RPCSubtype {
   public:
    Subtype subtype;
    std::string proto_service_name;
    google::protobuf::Descriptor descriptor;
    size_t operator()(RPCSubtype const& key) const {
        Subtype subtype = key.subtype;
        std::string hash = subtype.to_string() + proto_service_name;
        return std::hash<std::string>()(hash);
    }

    RPCSubtype(Subtype subtype, const google::protobuf::Descriptor& descriptor);
    RPCSubtype(Subtype subtype,
               std::string proto_service_name,
               google::protobuf::Descriptor& descriptor);
    friend bool operator==(const RPCSubtype& lhs, const RPCSubtype& rhs);
};

class ModelFamily {
   public:
    std::string namespace_;
    std::string family;

    ModelFamily(std::string namespace_, std::string family);
    std::string to_string();
};

class Model : public ModelFamily {
   public:
    std::string model_name;

    Model(std::string namespace_, std::string family, std::string model_name);
    Model(ModelFamily model, std::string model_name);
    Model(std::string model);
    std::string to_string();
    friend bool operator==(Model& lhs, Model& rhs);
};

template <>
struct std::hash<Name> {
    size_t operator()(Name const& key) const noexcept;
};

template <>
struct std::hash<RPCSubtype> {
    size_t operator()(RPCSubtype const& key) const noexcept;
};

template <>
struct std::hash<Subtype> {
    size_t operator()(const Subtype& key) const noexcept;
};
