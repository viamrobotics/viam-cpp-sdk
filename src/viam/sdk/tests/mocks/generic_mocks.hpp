#pragma once

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/camera/v1/camera.grpc.pb.h>
#include <viam/api/component/camera/v1/camera.pb.h>

#include <viam/sdk/components/generic/client.hpp>
#include <viam/sdk/components/generic/generic.hpp>
#include <viam/sdk/components/generic/server.hpp>

namespace viam {
namespace sdktests {
namespace generic {

using namespace viam::sdk;

class MockGeneric : public Generic {
   public:
    MockGeneric(std::string name) : Generic(std::move(name)){};
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(
        std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command)
        override;

    static std::shared_ptr<MockGeneric> get_mock_generic();
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;

   private:
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> map_;
    std::vector<GeometryConfig> geometries_;
};

}  // namespace generic
}  // namespace sdktests
}  // namespace viam
