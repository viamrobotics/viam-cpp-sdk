#pragma once

#include <viam/sdk/common/proto_value.hpp>

#include <viam/sdk/components/generic.hpp>
#include <viam/sdk/services/generic.hpp>

namespace viam {
namespace sdktests {
namespace generic {

using namespace viam::sdk;

class MockGenericComponent : public GenericComponent {
   public:
    MockGenericComponent(std::string name) : GenericComponent(std::move(name)) {}
    ProtoStruct do_command(const ProtoStruct& command) override;

    static std::shared_ptr<MockGenericComponent> get_mock_generic();
    std::vector<GeometryConfig> get_geometries(const ProtoStruct& extra) override;

   private:
    ProtoStruct map_;
    std::vector<GeometryConfig> geometries_;
};

class MockGenericService : public GenericService {
   public:
    MockGenericService(std::string name) : GenericService(std::move(name)) {};
    ProtoStruct do_command(const ProtoStruct& command) override;

    static std::shared_ptr<MockGenericService> get_mock_generic();

   private:
    ProtoStruct map_;
};

}  // namespace generic
}  // namespace sdktests
}  // namespace viam
