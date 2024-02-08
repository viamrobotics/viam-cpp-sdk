#include <viam/sdk/resource/resource_manager.hpp>

#include <exception>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>
#include <boost/optional/optional.hpp>
#include <grpcpp/impl/service_type.h>
#include <grpcpp/support/status.h>

#include <viam/api/component/generic/v1/generic.grpc.pb.h>

#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

std::shared_ptr<Resource> ResourceManager::resource(const std::string& name) {
    const std::lock_guard<std::mutex> lock(lock_);

    if (resources_.find(name) != resources_.end()) {
        return resources_.at(name);
    }

    if (short_names_.find(name) != short_names_.end()) {
        const std::string short_name = short_names_.at(name);
        if (resources_.find(short_name) != resources_.end()) {
            return resources_.at(short_name);
        }
    }

    return nullptr;
}

void ResourceManager::replace_all(
    const std::unordered_map<Name, std::shared_ptr<Resource>>& resources) {
    const std::lock_guard<std::mutex> lock(lock_);
    const std::unordered_map<std::string, std::shared_ptr<Resource>> new_resources;
    const std::unordered_map<std::string, std::string> new_short_names;
    this->resources_ = new_resources;
    this->short_names_ = new_short_names;

    for (const auto& resource : resources) {
        try {
            do_add(resource.first, resource.second);
        } catch (std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << "Error replacing all resources" << exc.what();
            return;
        }
    }
}

std::string get_shortcut_name(const std::string& name) {
    std::vector<std::string> name_split;
    // clang-tidy thinks this is a possible memory leak
    // NOLINTNEXTLINE
    boost::split(name_split, name, boost::is_any_of(":"));
    return name_split.at(name_split.size() - 1);
}

void ResourceManager::do_add(const Name& name, const std::shared_ptr<Resource>& resource) {
    if (name.name().empty()) {
        throw "Empty name used for resource: " + name.to_string();
    }
    const std::string short_name = name.short_name();

    do_add(short_name, resource);
}

void ResourceManager::do_add(const std::string& name, const std::shared_ptr<Resource>& resource) {
    if (resources_.find(name) != resources_.end()) {
        throw "Attempted to add resource that already existed: " + name;
    }

    resources_.emplace(name, resource);

    const std::string shortcut = get_shortcut_name(name);
    if (shortcut != name) {
        if (short_names_.find(shortcut) != short_names_.end()) {
            short_names_.emplace(shortcut, "");
        } else {
            short_names_.emplace(shortcut, name);
        }
    }
}

void ResourceManager::add(const Name& name, const std::shared_ptr<Resource>& resource) {
    const std::lock_guard<std::mutex> lock(lock_);
    try {
        do_add(name, resource);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "Error adding resource to subtype service: " << exc.what();
    }
};

void ResourceManager::do_remove(const Name& name) {
    const std::string short_name = name.short_name();
    if (resources_.find(short_name) == resources_.end()) {
        throw "attempted to remove resource " + name.to_string() + " but it didn't exist!";
    }
    resources_.erase(short_name);

    std::string const shortcut = get_shortcut_name(short_name);
    if (short_names_.find(shortcut) != short_names_.end()) {
        short_names_.erase(shortcut);
    }

    // case: remote1:nameA and remote2:nameA both existed, and remote2:nameA is
    // being deleted, restore shortcut to remote1:nameA
    for (auto& res : resources_) {
        const std::string key = res.first;
        if (shortcut == get_shortcut_name(key) && short_name != get_shortcut_name(key)) {
            if (short_names_.find(shortcut) != short_names_.end()) {
                short_names_.emplace(shortcut, "");
            } else {
                short_names_.emplace(shortcut, key);
            }
        }
    }
}

void ResourceManager::remove(const Name& name) {
    const std::lock_guard<std::mutex> lock(lock_);
    try {
        do_remove(name);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "unable to remove resource: " << exc.what();
    };
};

void ResourceManager::replace_one(const Name& name, const std::shared_ptr<Resource>& resource) {
    const std::lock_guard<std::mutex> lock(lock_);
    try {
        do_remove(name);
        do_add(name, resource);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "failed to replace resource " << name.to_string() << ": "
                                 << exc.what();
    }
}

const std::unordered_map<std::string, std::shared_ptr<Resource>>& ResourceManager::resources()
    const {
    return resources_;
}

void ResourceManager::add(const std::string& name, const std::shared_ptr<Resource>& resource) {
    const std::lock_guard<std::mutex> lock(lock_);
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    do_add(name, resource);
}

}  // namespace sdk
}  // namespace viam
