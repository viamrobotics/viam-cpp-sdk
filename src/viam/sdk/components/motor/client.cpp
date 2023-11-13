#include <viam/sdk/components/motor/client.hpp>

#include <algorithm>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

#include <viam/api/common/v1/common.pb.h>
#include <viam/api/component/motor/v1/motor.grpc.pb.h>

#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/robot/client.hpp>

namespace viam {
namespace sdk {

MotorClient::MotorClient(std::string name, std::shared_ptr<grpc::Channel> channel)
    : Motor(std::move(name)),
      stub_(viam::component::motor::v1::MotorService::NewStub(channel)),
      channel_(std::move(channel)){};

template <typename ClientType, typename StubType, typename RequestType, typename ResponseType>
class ClientHelper {
    static void default_rsc(RequestType&) {}
    static void default_rhc(const ResponseType&) {}
    static void default_ehc(const ::grpc::Status& status) {
        throw std::runtime_error(status.error_message());
    }

   public:
    using PFn = ::grpc::Status (StubType::*)(::grpc::ClientContext*,
                                             const RequestType&,
                                             ResponseType*);
    explicit ClientHelper(ClientType* client, StubType& stub, PFn pfn)
        : client_(client), stub_(stub), pfn_(pfn) {}

    template <typename RequestSetupCallable = decltype(default_rsc),
              typename ResponseHandlerCallable = decltype(default_rhc),
              typename ErrorHandlerCallable = decltype(default_ehc)>
    auto operator()(RequestSetupCallable&& rsc = default_rsc,
                    ResponseHandlerCallable&& rhc = default_rhc,
                    ErrorHandlerCallable&& ehc = default_ehc) {
        RequestType request;
        *request.mutable_name() = client_->name();
        std::forward<RequestSetupCallable>(rsc)(request);

        ::grpc::ClientContext ctx;
        set_client_ctx_authority(ctx);

        ResponseType response;
        const auto result = (stub_.*pfn_)(&ctx, request, &response);
        if (!result.ok()) {
            std::forward<ErrorHandlerCallable>(ehc)(result);
            default_ehc(result);
        }

        return std::forward<ResponseHandlerCallable>(rhc)(
            const_cast<const ResponseType&>(response));
    }

   private:
    ClientType* client_;
    StubType& stub_;
    PFn pfn_;
};

// TODO: EXTRA
// TODO: Stop returns the status???

template <typename ClientType, typename StubType, typename RequestType, typename ResponseType>
auto make_client_helper(ClientType* client,
                        StubType& stub,
                        ::grpc::Status (StubType::*method)(::grpc::ClientContext*,
                                                           const RequestType&,
                                                           ResponseType*)) {
    return ClientHelper<ClientType, StubType, RequestType, ResponseType>(client, stub, method);
}

void MotorClient::set_power(double power_pct, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::SetPower)(
        [&](auto& request) { request.set_power_pct(power_pct); });
}

void MotorClient::go_for(double rpm, double revolutions, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GoFor)([&](auto& request) {
        request.set_rpm(rpm);
        request.set_revolutions(revolutions);
    });
}

void MotorClient::go_to(double rpm, double position_revolutions, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GoTo)([&](auto& request) {
        request.set_rpm(rpm);
        request.set_position_revolutions(position_revolutions);
    });
}

void MotorClient::reset_zero_position(double offset, const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::ResetZeroPosition)(
        [&](auto& request) { request.set_offset(offset); });
}

Motor::position MotorClient::get_position(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetPosition)(
        [](auto) {}, [](auto& response) { return from_proto(response); });
}

Motor::properties MotorClient::get_properties(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetProperties)(
        [](auto) {}, [](auto& response) { return from_proto(response); });
}

grpc::StatusCode MotorClient::stop(const AttributeMap& extra) {
    viam::component::motor::v1::StopRequest request;
    viam::component::motor::v1::StopResponse response;

    grpc::ClientContext ctx;
    set_client_ctx_authority(ctx);

    *request.mutable_name() = this->name();
    *request.mutable_extra() = map_to_struct(extra);

    const grpc::Status status = stub_->Stop(&ctx, request, &response);
    return status.error_code();
}

Motor::power_status MotorClient::get_power_status(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::IsPowered)(
        [](auto) {}, [](auto& response) { return from_proto(response); });
}

std::vector<GeometryConfig> MotorClient::get_geometries(const AttributeMap& extra) {
    return make_client_helper(this, *stub_, &StubType::GetGeometries)(
        [](auto) {},
        [](auto& response) { return GeometryConfig::from_proto(response); });
}

bool MotorClient::is_moving() {
    return make_client_helper(this, *stub_, &StubType::IsMoving)(
        [](auto) {}, [](auto& response) { return response.is_moving(); });
}

AttributeMap MotorClient::do_command(const AttributeMap& command) {
    return make_client_helper(this, *stub_, &StubType::DoCommand)(
        [&](auto& request) { *request.mutable_command() = map_to_struct(command); },
        [](auto& response) { return struct_to_map(response.result()); });
}

}  // namespace sdk
}  // namespace viam
