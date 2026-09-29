#pragma once

#include <memory>
#include <string>
#include <vector>

#include <viam/sdk/app/viam_client.hpp>
#include <viam/sdk/common/utils.hpp>

namespace viam {
namespace sdk {

class AppClient {
   public:
    /// @brief A machine which the calling user has marked as a favorite.
    struct favorite_machine {
        std::string machine_id;
        std::string organization_id;
        time_pt created_on;
    };

    static AppClient from_viam_client(const ViamClient&);

    AppClient(AppClient&&) noexcept;

    AppClient& operator=(AppClient&&) noexcept;

    ~AppClient();

    const ViamChannel& channel() const;

    /// @brief Mark a machine as a favorite for the calling user.
    /// @param machine_id The ID of the machine to favorite.
    /// @return The newly created favorite.
    favorite_machine add_favorite_machine(const std::string& machine_id);

    /// @brief Remove a machine from the calling user's favorites.
    /// @param machine_id The ID of the machine to unfavorite.
    void remove_favorite_machine(const std::string& machine_id);

    /// @brief List the calling user's favorite machines.
    std::vector<favorite_machine> list_favorite_machines();

   private:
    AppClient(const ViamChannel& channel);

    struct impl;

    std::unique_ptr<impl> pimpl_;
};

}  // namespace sdk
}  // namespace viam
