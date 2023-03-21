#include <component/generic/v1/generic.grpc.pb.h>
#include <grpcpp/channel.h>

#include <components/generic/generic.hpp>

class GenericClient : public Generic {
   public:
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(std::string name, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) override;
    GenericClient(std::shared_ptr<grpc::Channel> channel_);

    protected:
    GenericClient();
    std::unique_ptr<viam::component::generic::v1::GenericService::StubInterface> stub_;

   private:
    std::shared_ptr<grpc::Channel> channel_;
};
