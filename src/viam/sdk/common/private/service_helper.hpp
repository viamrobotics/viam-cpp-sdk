#pragma once

#include <type_traits>

#include <grpcpp/support/status.h>

#include <viam/sdk/resource/resource_server_base.hpp>

namespace viam {
namespace sdk {

class ServiceHelperBase {
   public:
    ::grpc::Status fail(::grpc::StatusCode code, const char* message) const noexcept;

    ::grpc::Status failNoRequest() const noexcept;

    ::grpc::Status failNoResource(const std::string& name) const noexcept;

    ::grpc::Status failStdException(const std::exception& xcp) const noexcept;

    ::grpc::Status failUnknownException() const noexcept;

   protected:
    explicit ServiceHelperBase(const char* method) noexcept : method_{method} {}

   private:
    const char* method_;
};

template <typename ServiceType, typename RequestType>
class ServiceHelper : public ServiceHelperBase {
   public:
    ServiceHelper(const char* method, ResourceServer* rs, RequestType* request) noexcept
        : ServiceHelperBase{method}, rs_{rs}, request_{request} {};

    template <typename Callable>
    ::grpc::Status operator()(Callable&& callable) const noexcept try {
        if (!request_) {
            return failNoRequest();
        }
        const auto resource = rs_->resource_manager()->resource<ServiceType>(request_->name());
        if (!resource) {
            return failNoResource(request_->name());
        }
        return invoke_(std::forward<Callable>(callable), std::move(resource));
    } catch (const std::exception& xcp) {
        return failStdException(xcp);
    } catch (...) {
        return failUnknownException();
    }

    auto getExtra() const {
        return request_->has_extra() ? from_proto(request_->extra()) : ProtoStruct{};
    }

   private:
#if __cplusplus >= 201703L
    template <typename Callable, typename... Args>
    using is_void_result = std::is_void<std::invoke_result_t<Callable, Args...>>;
#else
    template <typename Callable, typename... Args>
    using is_void_result = std::is_void<std::result_of_t<Callable(Args...)>>;
#endif

    // Implementation of `invoke_` for a Callable returning non-void,
    // presumably an error return, which we return as a
    // ::grpc::Status.
    template <typename Callable,
              typename ResourcePtrType,
              std::enable_if_t<!is_void_result<Callable, ServiceHelper&, ResourcePtrType&&>::value,
                               bool> = true>
    ::grpc::Status invoke_(Callable&& callable, ResourcePtrType&& resource) const {
        return std::forward<Callable>(callable)(*this, std::forward<ResourcePtrType>(resource));
    }

    // Implementation of `invoke_` for a Callable returning void,
    // which is therefore either non-failing or communicates errors by
    // throwing exceptions. We return an OK status automatically.
    template <typename Callable,
              typename ResourcePtrType,
              std::enable_if_t<is_void_result<Callable, ServiceHelper&, ResourcePtrType&&>::value,
                               bool> = true>
    ::grpc::Status invoke_(Callable&& callable, ResourcePtrType&& resource) const {
        std::forward<Callable>(callable)(*this, std::forward<ResourcePtrType>(resource));
        return {};
    }

    ResourceServer* rs_;
    RequestType* request_;
};

template <typename ServiceType, typename RequestType>
auto make_service_helper(const char* method, ResourceServer* rs, RequestType* request) {
    return ServiceHelper<ServiceType, RequestType>{method, rs, request};
}

}  // namespace sdk
}  // namespace viam
