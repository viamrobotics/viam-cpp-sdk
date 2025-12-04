/// @file rpc/grpc_context_observer.hpp
///
/// @brief Provides access to gRPC server context information during request processing.
#pragma once

#include <boost/optional/optional.hpp>

#include <viam/sdk/common/grpc_fwd.hpp>

namespace viam {
namespace sdk {

/// @class GrpcContextObserver grpc_context_observer.hpp "rpc/grpc_context_observer.hpp"
/// @brief Provides thread-local access to the current gRPC server context during request handling.
///
/// This class allows code to access gRPC metadata and context information without explicit
/// parameter passing. The context is automatically made available when processing gRPC requests
/// and remains valid for the duration of the request.
///
/// @note Access to the context is thread-safe via thread-local storage.
class GrpcContextObserver {
   public:
    ///
    /// @brief Returns the current thread-local gRPC context observer, if one is active.
    ///
    /// @return An optional containing the active observer, or an empty optional if no
    ///         gRPC request is currently being processed on this thread.
    ///
    /// @warning The returned `GrpcContextObserver` reference is valid only for the duration
    ///          of the gRPC call and only on the thread that received it. Do not store the
    ///          reference or pass it to other threads.
    ///
    static const boost::optional<const GrpcContextObserver>& current() noexcept;

    ///
    /// @brief Returns the underlying gRPC server context.
    ///
    /// @return Reference to the gRPC server context for the current request.
    ///
    /// @warning The returned `GrpcServerContext` reference is valid only for the duration
    ///          of the gRPC call and only on the thread that received it. Do not store the
    ///          reference or pass it to other threads.
    ///
    const GrpcServerContext& context() const noexcept;

    ///
    /// @brief Retrieves all values for a given metadata key from the client's initial metadata.
    ///
    /// Client metadata is sent at the beginning of each gRPC call and can contain multiple
    /// values for the same key. This method returns all values associated with the specified key.
    ///
    /// @param key The metadata key to look up (case-sensitive).
    /// @return A vector of all string values associated with the key. Returns an empty vector
    ///         if the key is not found in the metadata. The returned strings are copies and
    ///         remain valid after the gRPC call completes.
    ///
    std::vector<std::string> get_client_metadata_field_values(const std::string& key) const;

    ///
    /// @class Enable
    /// @brief RAII helper for activating the gRPC context observer for the current thread.
    ///
    /// Instances of this class establish the thread-local context observer when constructed
    /// and automatically remove it when destroyed. This ensures proper cleanup even in the
    /// presence of exceptions.
    ///
    /// @note This class is for internal SDK use only. Users of the SDK do not need to
    ///       instantiate this class directly.
    ///
    class Enable;
    friend class Enable;

   private:
    explicit GrpcContextObserver(const GrpcServerContext& context) noexcept;
    const GrpcServerContext& context_;
};

}  // namespace sdk
}  // namespace viam
