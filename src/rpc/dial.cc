#include <grpcpp/channel.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <boost/none.hpp>
#include <boost/optional.hpp>
#include <istream>
#include <string>

#include "../../src/gen/robot/v1/robot.grpc.pb.h"
#include "../../src/gen/robot/v1/robot.pb.h"

class Credentials {
       public:
	std::string type_;
	std::string payload;
	Credentials(std::string payload);
};

extern "C" void *init_rust_runtime();
extern "C" int free_rust_runtime(void *ptr);
extern "C" void free_string(const char *s);
extern "C" char *dial(const char *uri, const char *payload, bool allow_insecure,
		      void *ptr);

class DialOptions {
       public:
	// Bypass webRTC and connect directly to the robot
	// TODO (RSDK-917): This field is currently just for show, we should
	// update dial logic to actually care about this
	bool disable_webrtc;

	// the URL to authenticate against
	boost::optional<std::string> auth_entity;

	// Credentials for connecting to the robot
	boost::optional<Credentials> credentials;

	// Determine if the connection is TLS-based. Must be set to `true` to
	// establish an insecure connection. Otherwise, a TLS-based connection
	// will be assumed
	bool insecure;

	// allow the RPC connection to be downgraded to an insecure connection
	// if detected. This is only used when credentials are not present
	bool allow_insecure_downgrade;

	// allow the RPC connection to be downgraded to an insecure connection
	// if detected, even with credentials present. This is generally unsafe
	// to do, but can be requested
	bool allow_insecure_with_creds_downgrade;
	DialOptions() {
		disable_webrtc = false;
		auth_entity = boost::none;
		credentials = boost::none;
		insecure = false;
		allow_insecure_downgrade = false;
		allow_insecure_with_creds_downgrade = false;
	}
};

class Options {
	// How often to refresh the status/parts of the robot, in seconds.
	// if set to 0, the robot will not automatically refresh.
       public:
	unsigned int refresh_interval;
	boost::optional<DialOptions> dial_options;
	Options(unsigned int refresh_interval_,
		boost::optional<DialOptions> dial_options_) {
		refresh_interval = refresh_interval_;
		if (!dial_options_) {
			std::cout
			    << "dial options don't exist when creating optdions"
			    << std::endl;
		} else {
			std::cout << "dial options exist at creation no problo"
				  << std::endl;
		}
		dial_options = dial_options_;
	}
};

class ViamChannel {
       public:
	std::shared_ptr<grpc::Channel> channel;
	ViamChannel(std::shared_ptr<grpc::Channel> channel_, const char *path_,
		    void *runtime) {
		channel = channel_;
		path = path_;
		closed = false;
		rust_runtime = runtime;
	}
	void close();

       private:
	const char *path;
	bool closed;
	void *rust_runtime;
};

void ViamChannel::close() {
	if (closed) {
		return;
	}
	closed = true;
	free_string(path);
	free_rust_runtime(rust_runtime);
};

ViamChannel dial(const char *uri, boost::optional<DialOptions> options) {
	void *ptr = init_rust_runtime();
	DialOptions opts = options.get_value_or(DialOptions());
	const char *payload;

	if (opts.credentials) {
		payload = opts.credentials->payload.c_str();
	}
	char *socket_path =
	    dial(uri, payload, opts.allow_insecure_downgrade, ptr);
	if (socket_path == NULL) {
		free_rust_runtime(ptr);
		throw "Unable to establish connecting path!";
	}

	std::string address("unix://");
	address += socket_path;
	std::shared_ptr<grpc::Channel> channel =
	    grpc::CreateChannel(address, grpc::InsecureChannelCredentials());
	std::unique_ptr<viam::robot::v1::RobotService::Stub> st =
	    viam::robot::v1::RobotService::NewStub(channel);
	const char *path = address.c_str();
	ViamChannel viam_channel = ViamChannel(channel, path, ptr);
	return viam_channel;
};

Credentials::Credentials(std::string payload_) {
	type_ = "robot-location-secret";
	payload = payload_;
};
