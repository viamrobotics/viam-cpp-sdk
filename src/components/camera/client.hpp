#include <component/camera/v1/camera.grpc.pb.h>
#include <grpcpp/channel.h>

#include <components/camera/camera.hpp>
#include <components/camera/server.hpp>

class CameraClient: public Camera {
  public:
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(std::string name, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command) override; 
    raw_image get_image(std::string name, std::string mime_type) override;
    point_cloud get_point_cloud(std::string name, std::string mime_type) override;
    properties get_properties(std::string name) override;
    CameraClient(std::shared_ptr<grpc::Channel> channel_);

    
    protected: 
     CameraClient();
    std::unique_ptr<viam::component::camera::v1::CameraService::StubInterface> stub_;

    private:
    std::shared_ptr<grpc::Channel> channel_;
    
};
 std::vector<unsigned char> string_to_bytes(std::string const &s);
bool has_suffix(const std::string &str, const std::string &suffix);



