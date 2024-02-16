#pragma once

#include "viam/sdk/common/proto_type.hpp"
#include <viam/api/common/v1/common.pb.h>

#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/components/private/generic_client.hpp>
#include <viam/sdk/components/private/generic_server.hpp>
#include <viam/sdk/services/generic/client.hpp>
#include <viam/sdk/services/generic/generic.hpp>
#include <viam/sdk/services/generic/server.hpp>

namespace viam {
namespace sdktests {
namespace generic {

using namespace viam::sdk;

class MockGenericComponent : public GenericComponent {
   public:
    MockGenericComponent(std::string name) : GenericComponent(std::move(name)){};
    AttributeMap do_command(const AttributeMap& command) override;

    static std::shared_ptr<MockGenericComponent> get_mock_generic();
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

   private:
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map_;
    std::vector<GeometryConfig> geometries_;
};

class MockGenericService : public GenericService {
   public:
    MockGenericService(std::string name) : GenericService(std::move(name)){};
    AttributeMap do_command(const AttributeMap& command) override;

    static std::shared_ptr<MockGenericService> get_mock_generic();

   private:
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map_;
};

}  // namespace generic
}  // namespace sdktests
}  // namespace viam
