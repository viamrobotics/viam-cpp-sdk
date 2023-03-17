#define BOOST_TEST_MODULE test module test_generic_component
#include <boost/test/included/unit_test.hpp> 
#include <common/v1/common.pb.h>
#include <component/camera/v1/camera.pb.h>
#include <component/camera/v1/camera.grpc.pb.h>

BOOST_AUTO_TEST_SUITE(generic_suite)

#include <common/utils.hpp>
#include <components/camera/camera.hpp>
#include <components/camera/client.hpp>
#include <components/camera/server.hpp>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <vector>


class MockCamera : public Camera {
  public:

      std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> do_command(
      std::string name, std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>command) override {
      return map;
    }
    raw_image get_image(std::string name, std::string mime_type)  override{
      return image;

    }
    point_cloud get_point_cloud(std::string name, std::string mime_type) override {
      return pc;

    }
    properties get_properties(std::string name) override {
      return camera_properties;
    }

    Camera::point_cloud pc;
    Camera::intrinsic_parameters intrinsic_parameters;
    Camera::distortion_parameters distortion_parameters;
    Camera::properties camera_properties;
    Camera::raw_image image;
    std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<ProtoType>>> map;
    
};


 Camera::raw_image raw_image() {
   Camera::raw_image image;
    image.mime_type = "JPEG";

   std::vector<unsigned char> bytes = {'a', 'b', 'c'};

   image.bytes = bytes;

    return image;
}

Camera::point_cloud point_cloud() {
  Camera::point_cloud point_cloud;
  point_cloud.mime_type = "pointcloud/pcd";
  std::vector<unsigned char> bytes = {'a', 'b', 'c'};
  point_cloud.pc= bytes;
  return point_cloud;
}

Camera::intrinsic_parameters intrinsic_parameters() {
  Camera::intrinsic_parameters intrinsic_parameters;
    intrinsic_parameters.width_px = 1;
    intrinsic_parameters.height_px = 2;
    intrinsic_parameters.focal_x_px = 3;
    intrinsic_parameters.focal_y_px = 4;
    intrinsic_parameters.center_x_px = 5;
    intrinsic_parameters.center_y_px = 6;
    return intrinsic_parameters;
}

Camera::distortion_parameters distortion_parameters() {
    Camera::distortion_parameters distortion_parameters;
    distortion_parameters.model = "no distortion";
    return distortion_parameters;
}

Camera::properties properties() {
    Camera::properties properties;
    properties.supports_pcd = true;
    properties.intrinsic_parameters = intrinsic_parameters();
    properties.distortion_parameters = distortion_parameters();
    return properties;
} 

std::shared_ptr<std::unordered_map<std::string,std::shared_ptr<ProtoType>>> map() {

      ProtoType prototype = ProtoType(std::string("hello"));
     std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(prototype);
      std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {{std::string("test"), proto_ptr}};
      return std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);

}

 std::shared_ptr<MockCamera> get_mock_camera() {
    MockCamera camera;

    camera.image.mime_type = "JPEG";


   std::vector<unsigned char> b = {'a', 'b', 'c'};

    camera.image.bytes = b;

    camera.pc.mime_type = "pointcloud/pcd";
    camera.pc.pc = b;

    camera.intrinsic_parameters.width_px = 1;
    camera.intrinsic_parameters.height_px = 2;
    camera.intrinsic_parameters.focal_x_px = 3;
    camera.intrinsic_parameters.focal_y_px = 4;
    camera.intrinsic_parameters.center_x_px = 5;
    camera.intrinsic_parameters.center_y_px = 6;

    camera.distortion_parameters.model = "no distortion";

    camera.camera_properties.supports_pcd = true;
    camera.camera_properties.intrinsic_parameters = camera.intrinsic_parameters;
    camera.camera_properties.distortion_parameters = camera.distortion_parameters;

    camera.map = map();

    return std::make_shared<MockCamera>(camera);
}

class MockStub : public viam::component::camera::v1::CameraService::StubInterface {
   public:
    CameraServer server;   


   
    MockStub(): server(CameraServer(std::make_shared<SubtypeService>())){
        this->server.sub_svc->add(std::string("camera"), get_mock_camera());
      };



      ::grpc::Status GetImage(::grpc::ClientContext* context,
                             const ::viam::component::camera::v1::GetImageRequest& request,
                             ::viam::component::camera::v1::GetImageResponse* response) override {
                            //  std::cout << "in stub get_image";
                            grpc::ServerContext* ctx; 
                          return server.GetImage(ctx, &request, response);
                             }
    ::grpc::Status RenderFrame(::grpc::ClientContext* context,
                             const ::viam::component::camera::v1::RenderFrameRequest& request,
                             ::google::api::HttpBody* response) override  {
                                grpc::ServerContext* ctx;
                                return server.RenderFrame(ctx, &request, response);

                             }                    
    ::grpc::Status GetPointCloud(::grpc::ClientContext* context,
                             const ::viam::component::camera::v1::GetPointCloudRequest& request,
                             ::viam::component::camera::v1::GetPointCloudResponse* response) override {
                                 grpc::ServerContext* ctx;
                                return server.GetPointCloud(ctx, &request, response);
                             }     
    ::grpc::Status GetProperties(::grpc::ClientContext* context,
                             const ::viam::component::camera::v1::GetPropertiesRequest& request,
                             ::viam::component::camera::v1::GetPropertiesResponse* response) override {
                                 grpc::ServerContext* ctx;
                                return server.GetProperties(ctx, &request, response);
                             } 
     ::grpc::Status DoCommand(::grpc::ClientContext* context,
                             const ::viam::common::v1::DoCommandRequest& request,
                             ::viam::common::v1::DoCommandResponse* response) override {
                                 grpc::ServerContext* ctx;
                                return server.DoCommand(ctx, &request, response);
                             } 

    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>> AsyncGetImage(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>>(AsyncGetImageRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>> PrepareAsyncGetImage(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>>(PrepareAsyncGetImageRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>> AsyncRenderFrame(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>>(AsyncRenderFrameRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>> PrepareAsyncRenderFrame(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>>(PrepareAsyncRenderFrameRaw(context, request, cq));
    }

    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>> AsyncGetPointCloud(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>>(AsyncGetPointCloudRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>> PrepareAsyncGetPointCloud(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>>(PrepareAsyncGetPointCloudRaw(context, request, cq));
    }

    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>> AsyncGetProperties(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>>(AsyncGetPropertiesRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>> PrepareAsyncGetProperties(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>>(PrepareAsyncGetPropertiesRaw(context, request, cq));
    }

    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>> AsyncDoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>>(AsyncDoCommandRaw(context, request, cq));
    }
    std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>> PrepareAsyncDoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) {
      return std::unique_ptr< ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>>(PrepareAsyncDoCommandRaw(context, request, cq));
    }



    class async final :
      public StubInterface::async_interface {
     public:
      void GetImage(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest* request, ::viam::component::camera::v1::GetImageResponse* response, std::function<void(::grpc::Status)>) override {
        return;
      }
      void GetImage(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest* request, ::viam::component::camera::v1::GetImageResponse* response, ::grpc::ClientUnaryReactor* reactor) override {
        return;
      }
      void RenderFrame(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest* request, ::google::api::HttpBody* response, std::function<void(::grpc::Status)>) override {
        return;
      }
      void RenderFrame(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest* request, ::google::api::HttpBody* response, ::grpc::ClientUnaryReactor* reactor) override {
        return;
      }
      void GetPointCloud(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest* request, ::viam::component::camera::v1::GetPointCloudResponse* response, std::function<void(::grpc::Status)>) override {
        return;
      }
      void GetPointCloud(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest* request, ::viam::component::camera::v1::GetPointCloudResponse* response, ::grpc::ClientUnaryReactor* reactor) override {
        return;
      }
      void GetProperties(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest* request, ::viam::component::camera::v1::GetPropertiesResponse* response, std::function<void(::grpc::Status)>) override {
        return;
      }
      void GetProperties(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest* request, ::viam::component::camera::v1::GetPropertiesResponse* response, ::grpc::ClientUnaryReactor* reactor) override {
        return;
      }
      void DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, std::function<void(::grpc::Status)>) override {
        return;
      }
      void DoCommand(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest* request, ::viam::common::v1::DoCommandResponse* response, ::grpc::ClientUnaryReactor* reactor) override {
        return;
      }


     public:
     friend class Stub;
      explicit async(MockStub* stub): stub_(stub) { }
      MockStub* stub() { return stub_; }
      MockStub* stub_;
    };

    class async* async() override { return &async_stub_; }

    std::shared_ptr< ::grpc::ChannelInterface> channel_;
    class async async_stub_{this};
    ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>* AsyncGetImageRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest& request, ::grpc::CompletionQueue* cq) override {
        return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetImageResponse, ::viam::component::camera::v1::GetImageRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetImageResponse>* PrepareAsyncGetImageRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetImageRequest& request, ::grpc::CompletionQueue* cq) override {
      return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetImageResponse, ::viam::component::camera::v1::GetImageRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>* AsyncRenderFrameRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest& request, ::grpc::CompletionQueue* cq) override {
      return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<::google::api::HttpBody, ::viam::component::camera::v1::RenderFrameRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::google::api::HttpBody>* PrepareAsyncRenderFrameRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::RenderFrameRequest& request, ::grpc::CompletionQueue* cq) override {
       return ::grpc::internal::ClientAsyncResponseReaderHelper::Create<::google::api::HttpBody, ::viam::component::camera::v1::RenderFrameRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
      
    }
    ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>* AsyncGetPointCloudRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest& request, ::grpc::CompletionQueue* cq) override {
       return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetPointCloudResponse,::viam::component::camera::v1::GetPointCloudRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPointCloudResponse>* PrepareAsyncGetPointCloudRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPointCloudRequest& request, ::grpc::CompletionQueue* cq) override {
             return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetPointCloudResponse,::viam::component::camera::v1::GetPointCloudRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>* AsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) override {
             return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetPropertiesResponse,::viam::component::camera::v1::GetPropertiesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::viam::component::camera::v1::GetPropertiesResponse>* PrepareAsyncGetPropertiesRaw(::grpc::ClientContext* context, const ::viam::component::camera::v1::GetPropertiesRequest& request, ::grpc::CompletionQueue* cq) override {
             return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::component::camera::v1::GetPropertiesResponse,::viam::component::camera::v1::GetPropertiesRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* AsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) override {
             return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse,::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    ::grpc::ClientAsyncResponseReader< ::viam::common::v1::DoCommandResponse>* PrepareAsyncDoCommandRaw(::grpc::ClientContext* context, const ::viam::common::v1::DoCommandRequest& request, ::grpc::CompletionQueue* cq) override {
             return ::grpc::internal::ClientAsyncResponseReaderHelper::Create< ::viam::common::v1::DoCommandResponse,::viam::common::v1::DoCommandRequest, ::grpc::protobuf::MessageLite, ::grpc::protobuf::MessageLite>(channel_.get(), cq, rpcmethod_GetImage_, context, request);
    }
    const ::grpc::internal::RpcServiceMethod::RpcType type = ::grpc::internal::RpcServiceMethod::RpcType();
    const ::grpc::internal::RpcMethod rpcmethod_GetImage_ =  ::grpc::internal::RpcMethod("name", type);
    const ::grpc::internal::RpcMethod rpcmethod_RenderFrame_ = ::grpc::internal::RpcMethod("name", type);
    const ::grpc::internal::RpcMethod rpcmethod_GetPointCloud_ = ::grpc::internal::RpcMethod("name", type);
    const ::grpc::internal::RpcMethod rpcmethod_GetProperties_ = ::grpc::internal::RpcMethod("name", type);
    const ::grpc::internal::RpcMethod rpcmethod_DoCommand_ = ::grpc::internal::RpcMethod("name", type);
   



  };

// mock camera tests!!!!
  std::shared_ptr<MockCamera> camera = get_mock_camera();


BOOST_AUTO_TEST_CASE(test_get_image) {
    Camera::raw_image expected_image = raw_image();
    Camera::raw_image image = camera->get_image("camera", "JPEG");

    BOOST_CHECK(expected_image == image);
} 

BOOST_AUTO_TEST_CASE(test_get_point_cloud) {
   Camera::point_cloud expected_pc = point_cloud();
   Camera::point_cloud pc = camera->get_point_cloud("camera", "pointcloud/pcd");

    BOOST_CHECK(expected_pc == pc);

}


BOOST_AUTO_TEST_CASE(test_get_properties) {
    Camera::properties expected_props = properties(); 
    Camera::properties properties = camera->get_properties("camera");

     BOOST_CHECK(expected_props == properties);
}

BOOST_AUTO_TEST_CASE(test_do) {
   ProtoType prototype = ProtoType(std::string("hello"));

  std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(prototype);
  std::unordered_map<std::string, std::shared_ptr<ProtoType>> expected_map = {{std::string("test"), proto_ptr}};
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> expected = std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(expected_map);


  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command;
  std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> result_map = camera->do_command("camera", command);

   ProtoType expected_pt = *(expected->at(std::string("test")));
   ProtoType result_pt = *(result_map->at(std::string("test")));


   BOOST_CHECK(result_pt == expected_pt);
}    




// service tests

   MockStub mock =  MockStub();

BOOST_AUTO_TEST_CASE(test_get_image_service) {

    grpc::ClientContext ctx;
    viam::component::camera::v1::GetImageRequest req;
    viam::component::camera::v1::GetImageResponse resp;

    *req.mutable_name() = "camera";
    *req.mutable_mime_type() = "JPEG"; 

   grpc::Status status = mock.GetImage(&ctx, req, &resp);

    Camera::raw_image image = raw_image();

    std::vector<unsigned char> bytes = string_to_bytes(resp.image());

    BOOST_CHECK(image.bytes == bytes);
} 


BOOST_AUTO_TEST_CASE(test_get_point_cloud_service) {

    grpc::ClientContext ctx;
    viam::component::camera::v1::GetPointCloudRequest req;
    viam::component::camera::v1::GetPointCloudResponse resp;

    *req.mutable_name() = "camera";
    *req.mutable_mime_type() = "pointcloud/pcd"; 
    

    grpc::Status status = mock.GetPointCloud(&ctx, req, &resp);

    Camera::point_cloud expected_pc = point_cloud();


    std::vector<unsigned char> bytes = string_to_bytes(resp.point_cloud());

     BOOST_CHECK(expected_pc.pc == bytes);
}

BOOST_AUTO_TEST_CASE(test_render_frame_service) {

    grpc::ClientContext ctx;
    viam::component::camera::v1::RenderFrameRequest req;
    ::google::api::HttpBody resp;

    Camera::raw_image image = raw_image();


    *req.mutable_name() = "camera";
    *req.mutable_mime_type() = "JPEG"; 
    grpc::Status status = mock.RenderFrame(&ctx, req, &resp);


    std::vector<unsigned char> bytes = string_to_bytes(resp.data());

    if(resp.content_type() != image.mime_type ) {
      return 1;
    }

     BOOST_CHECK(image.bytes == bytes);
}
 
BOOST_AUTO_TEST_CASE(test_get_properties_service) {
    grpc::ClientContext ctx;
    viam::component::camera::v1::GetPropertiesRequest req;
    viam::component::camera::v1::GetPropertiesResponse resp;
    *req.mutable_name() = "camera";
    grpc::Status status = mock.GetProperties(&ctx, req, &resp);
    Camera::properties expected = properties();

     BOOST_CHECK(expected.supports_pcd!= resp.supports_pcd);

   Camera::intrinsic_parameters in_params;
   Camera::distortion_parameters di_params;
   

     BOOST_CHECK(expected.intrinsic_parameters == in_params.from_proto(resp.intrinsic_parameters()))
     BOOST_CHECK(expected.distortion_parameters == di_params.from_proto(resp.distortion_parameters()));
} 

BOOST_AUTO_TEST_CASE(test_do_service) {
    grpc::ClientContext ctx;
    viam::common::v1::DoCommandRequest req;
    viam::common::v1::DoCommandResponse resp;

     std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command = 
     std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();


     *req.mutable_command() = map_to_struct(command);

     *req.mutable_name() = "camera";

     ProtoType prototype = ProtoType(std::string("hello"));
     std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(prototype);
     std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {{std::string("test"), proto_ptr}};

     std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> expected_map = 
     std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);

    grpc::Status status = mock.DoCommand(&ctx, req, &resp);
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> result_map = struct_to_map(resp.result());

    std::shared_ptr<ProtoType> expected_pt = expected_map->at(std::string("test"));

    std::shared_ptr<ProtoType> result_pt = result_map->at(std::string("test"));


     BOOST_CHECK(*expected_pt ==  *result_pt);
}


  class MockClient : public CameraClient {
    public:
      MockClient() : CameraClient()
  {
        stub_ = std::make_unique<MockStub>();
    }
  }; 


    MockClient client = MockClient();


//client tests 
BOOST_AUTO_TEST_CASE(test_image_client) {


    Camera::raw_image image = client.get_image("camera", "JPEG");
    Camera::raw_image expected_image = raw_image();


     BOOST_CHECK(expected_image == image);
} 



  BOOST_AUTO_TEST_CASE(test_get_point_cloud_client) {


    Camera::point_cloud pc = client.get_point_cloud("camera", "pointcloud/pcd");

    Camera::point_cloud expected = point_cloud();

     BOOST_CHECK(expected == pc);
} 




 

  BOOST_AUTO_TEST_CASE(test_get_properties_client) {

    Camera::properties props = client.get_properties("camera");

    Camera::properties expected = properties(); 

     BOOST_CHECK(expected == props);
  }


BOOST_AUTO_TEST_CASE(test_do_client) {
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> command = 
    std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>();

    ProtoType prototype = ProtoType(std::string("hello"));
    std::shared_ptr<ProtoType> proto_ptr = std::make_shared<ProtoType>(prototype);
    std::unordered_map<std::string, std::shared_ptr<ProtoType>> map = {{std::string("test"), proto_ptr}};
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> expected_map = 
    std::make_shared<std::unordered_map<std::string, std::shared_ptr<ProtoType>>>(map);

    
    std::shared_ptr<std::unordered_map<std::string, std::shared_ptr<ProtoType>>> result_map = client.do_command("camera", command);

    ProtoType expected_pt = *(expected_map->at(std::string("test")));
    ProtoType result_pt = *(result_map->at(std::string("test")));


    BOOST_CHECK(expected_pt == result_pt);
}

BOOST_AUTO_TEST_SUITE_END()













