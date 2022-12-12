#include <grpcpp/client_context.h>
#include <grpcpp/grpcpp.h>
#include <unistd.h>

#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <vector>

#include "../../src/gen/robot/v1/robot.grpc.pb.h"
#include "../../src/gen/robot/v1/robot.pb.h"
#include "../../src/robot/client.h"
#include "../../src/rpc/dial.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using viam::robot::v1::ResourceNamesRequest;
using viam::robot::v1::ResourceNamesResponse;
using viam::robot::v1::RobotService;

extern "C" void *init_rust_runtime();
extern "C" int free_rust_runtime(void *ptr);
extern "C" void free_string(char *s);
extern "C" char *dial(const char *uri, const char *payload, bool allow_insecure,
		      void *ptr);

class RobotServiceClient {
       public:
	RobotServiceClient(std::shared_ptr<Channel> channel)
	    : stub_(RobotService::NewStub(channel)) {}

	void Resources() {
		ResourceNamesRequest req;

		ResourceNamesResponse resp;
		grpc::ClientContext context;

		Status status = stub_->ResourceNames(&context, req, &resp);
		if (!status.ok()) {
			std::cout << "Rpc failed " << status.error_code()
				  << status.error_message() << std::endl;
			return;
		}
		for (auto i = 0; i < resp.resources_size(); i++) {
			std::cout << "Resource " << i << " "
				  << resp.resources(i).type() << std::endl;
		}
	}

       private:
	std::unique_ptr<RobotService::Stub> stub_;
};

int main() {
	const char *uri = "naveed-pi-main.60758fe0f6.viam.cloud";
	DialOptions dial_options = DialOptions();
	std::string payload = "pem1epjv07fq2cz2z5723gq6ntuyhue5t30boohkiz3iqht4";
	Credentials credentials(payload);
	dial_options.credentials = credentials;
	boost::optional<DialOptions> opts(dial_options);
	std::string address(uri);
	Options options = Options(1, opts);
	std::shared_ptr<RobotClient> robot =
	    RobotClient::at_address(address, options);
	robot->refresh();
	std::vector<ResourceName> *resource_names = robot->resource_names();
	for (ResourceName &resource : *resource_names) {
		std::cout << "Resource name: " << resource.name()
			  << resource.type() << resource.subtype() << std::endl;
	}
	robot->close();
	return 0;
}
