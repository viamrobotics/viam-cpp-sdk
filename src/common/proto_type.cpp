#include <google/protobuf/struct.pb.h>

#include <boost/blank.hpp>
#include <boost/variant/get.hpp>
#include <boost/variant/variant.hpp>
#include <common/proto_type.hpp>
#include <unordered_map>
#include <vector>

using google::protobuf::Struct;
using google::protobuf::Value;

Struct map_to_struct(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> dict);
std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> struct_to_map(Struct struct_);

// float, string, char, struct, array, struct(map from string to any of the above)

ProtoType ProtoType::of_value(const Value& value) {
    switch (value.kind_case()) {
        case Value::KindCase::kBoolValue: {
            return ProtoType(value.bool_value());
        }
        case Value::KindCase::kStringValue: {
            std::string stg = std::string(value.string_value());
            return ProtoType(stg);

        }
        case Value::KindCase::kNumberValue: {
            return ProtoType(value.number_value());
        }
        case Value::KindCase::kListValue: {
            std::vector<ProtoType*> vec;
            google::protobuf::ListValue list_val = value.list_value();
            for (auto& val : value.list_value().values()) {
                ProtoType p = ProtoType::of_value(val);
                vec.push_back(&p);
            }
            return ProtoType(vec);
        }
        case Value::KindCase::kStructValue: {
            std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map = struct_to_map(value.struct_value());
            return ProtoType(map);
        }
        case Value::KindCase::KIND_NOT_SET:
        case Value::KindCase::kNullValue: {
            return ProtoType();
        }
    }
};

Struct map_to_struct(std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> dict) {
    Struct s;
    for (auto& key_and_value : *dict) {
        const std::string key = key_and_value.first;
        Value value = key_and_value.second->proto_value();
        const std::string stg = boost::get<std::string>(key_and_value.second->proto_type);

        google::protobuf::MapPair<std::string, Value> val(key, value);
            
        s.mutable_fields()->insert(val);
    }

    return s;
}



std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> struct_to_map(Struct struct_) {
    google::protobuf::Map<std::string, Value> struct_map = struct_.fields();
    std::unordered_map<std::string, std::shared_ptr<ProtoType>> map;

    for (auto& val : struct_.fields()) {
        std::string key = val.first;
        std::shared_ptr<Value> val_ptr = std::make_shared<Value>(val.second); 
        std::shared_ptr<ProtoType> value = std::make_shared<ProtoType>(ProtoType::of_value(val.second));
        map.emplace(key, value);
    }

    return std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);
    
};

Value ProtoType::proto_value() {
    Value v;
       try {
            int i = proto_type.which();
        }
        catch(std::exception &err) {
                std::cout << "error" << err.what();
        } 
    switch (proto_type.which()) {
        case 0: {
            ::google::protobuf::NullValue value;
            v.set_null_value(value);
            break;
        }
        case 1: {
            const bool b = boost::get<bool>(proto_type);
            v.set_bool_value(b);
            break;
        }
        case 2: {
            const std::string s = boost::get<std::string>(proto_type);
            *v.mutable_string_value() = s;
            break;
        }
       case 3: {
            char const* c = boost::get<char const*>(proto_type);
            *v.mutable_string_value() = c;
            break;
        } 
        case 4: {
            const int i = boost::get<int>(proto_type);
            v.set_number_value(i);
            break;
        }
        case 5: {
            const double i = boost::get<double>(proto_type);
            v.set_number_value(i);
            break;
        }
        case 6: {
            const std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map =
            boost::get<std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>>(proto_type);
            Struct s = map_to_struct(map);
            *v.mutable_struct_value() = s;
            break;
        }
        case 7: {
            ::google::protobuf::ListValue l = *v.mutable_list_value();
            google::protobuf::RepeatedPtrField<Value>* values = l.mutable_values();
            std::vector<ProtoType*> vec = boost::get<std::vector<ProtoType*>>(proto_type);
            for (auto& val : vec) {
                *values->Add() = val->proto_value();
            }
            break;
        }
        default: {
            throw "Invalid proto_value conversion type. This should never happen;\
						please file a bug report.";
        }
    }

    return v;
}

bool operator==(const ProtoType &lhs, const ProtoType &rhs) {
    if (lhs.proto_type.which() != rhs.proto_type.which()) {
        return false;
    }




    switch(lhs.proto_type.which()) {
          case 0: {
            // both null values
            return true;
            break;
        }
        case 1: {
            const bool lhs_b = boost::get<bool>(lhs.proto_type);
            const bool rhs_b = boost::get<bool>(rhs.proto_type);
            if(lhs_b == rhs_b) {
                return true;
            }
            return false;
            break;
        }
        case 2: {
            const std::string lhs_s = boost::get<std::string>(lhs.proto_type);
            const std::string rhs_s = boost::get<std::string>(rhs.proto_type);
             if(lhs_s == rhs_s) {
                return true;
            }
            return false;
            break;
        }
         case 3: {
            char const* lhs_c = boost::get<char const*>(lhs.proto_type);
            char const* rhs_c = boost::get<char const*>(rhs.proto_type);
             if(lhs_c == rhs_c) {
                return true;
            }
            return false;
            break;
        }
        case 4: {
            const int lhs_i = boost::get<int>(lhs.proto_type);
            const int rhs_i = boost::get<int>(rhs.proto_type);
            if(lhs_i == rhs_i) {
                return true;
            }
            return false;
            break;

        }
        case 5: {
            const double lhs_i = boost::get<double>(lhs.proto_type);
            const double rhs_i = boost::get<double>(rhs.proto_type);
            if(lhs_i == rhs_i) {
                return true;
            }
            return false;
            break;
        }
        case 6: {
            std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> lhs_map =
                boost::get<std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>>(lhs.proto_type);
            std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> rhs_map =
                boost::get<std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>>(rhs.proto_type);
            
            auto it1 = *lhs_map->begin();
            auto it2 = *rhs_map->begin();


            ProtoType lhs_type = *lhs_map->at(it1.first);
            ProtoType rhs_type = *rhs_map->at(it2.first);

            if(it1.first == it2.first) {
                    if(lhs_type == rhs_type) {
                return true;
            }
            }

            return false;
            break;
        }
        case 7: {
            std::vector<ProtoType*> lhs_vec = boost::get<std::vector<ProtoType*>>(lhs.proto_type);
            std::vector<ProtoType*> rhs_vec = boost::get<std::vector<ProtoType*>>(rhs.proto_type);
              if(lhs_vec == rhs_vec) {
                return true;
            }
            return false;
            break;
        }
        default: {
            throw "Invalid proto_value conversion type. This should never happen;\
						please file a bug report.";
        }
    }




    
    

}


