#include <component/generic/v1/generic.grpc.pb.h>
#include <grpcpp/channel.h>

#include <components/generic/generic.hpp>

class GenericClient : public Generic {
   public:
    ProtoType do_command(std::unordered_map<std::string, ProtoType*> command) override;
    GenericClient(std::shared_ptr<grpc::Channel> channel_);

   private:
    std::shared_ptr<grpc::Channel> channel_;
    std::unique_ptr<viam::component::generic::v1::GenericService::Stub> stub_;
};
