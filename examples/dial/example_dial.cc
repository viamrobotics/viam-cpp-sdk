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

using viam::robot::v1::Status;

extern "C" void *init_rust_runtime();
extern "C" int free_rust_runtime(void *ptr);
extern "C" void free_string(char *s);
extern "C" char *dial(const char *uri, const char *payload, bool allow_insecure,
		      void *ptr);

int main() {
	const char *uri = "webrtc-test-main.jkek76kqnh.viam.cloud";
	DialOptions dial_options = DialOptions();
	std::string payload =
	    "ytexnwei4fu1xv9csoqxfv4ckl3htsb49mzzey5t15xo9swy";
	Credentials credentials(payload);
	dial_options.credentials = credentials;
	boost::optional<DialOptions> opts(dial_options);
	std::string address(uri);
	Options options = Options(1, opts);
	std::shared_ptr<RobotClient> robot =
	    RobotClient::at_address(address, options);
	robot->refresh();
	std::vector<ResourceName> *resource_names = robot->resource_names();
	ResourceName the_one_we_care_about = resource_names->at(0);
	for (ResourceName resource : *resource_names) {
		std::cout << "Resource name: " << resource.name()
			  << resource.type() << resource.subtype() << std::endl;
	}
	std::vector<Status> status_plural = robot->get_status();
	std::cout << "Status plural len " << status_plural.size() << std::endl;
	for (Status s : status_plural) {
		std::cout << " Status! " << s.name().subtype() << std::endl;
	}

	std::vector<ResourceName> just_one = std::vector<ResourceName>();
	just_one.push_back(the_one_we_care_about);
	std::vector<Status> status_singular = robot->get_status(just_one);
	std::cout << "Status singular len " << status_singular.size()
		  << std::endl;
	for (Status s : status_singular) {
		std::cout << " Status! " << s.name().subtype() << std::endl;
	}

	robot->close();
	return 0;
}
