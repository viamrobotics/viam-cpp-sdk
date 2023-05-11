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

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/resource.hpp>
#include <viam/sdk/resource/resource_api.hpp>
#include <viam/sdk/rpc/server.hpp>

namespace viam {
namespace sdk {

std::shared_ptr<Resource> ResourceManager::resource(std::string name) {
    std::lock_guard<std::mutex> lock(lock_);

    if (resources_.find(name) != resources_.end()) {
        return resources_.at(name);
    }

    if (short_names_.find(name) != short_names_.end()) {
        std::string short_name = short_names_.at(name);
        if (resources_.find(short_name) != resources_.end()) {
            return resources_.at(short_name);
        }
    }
    throw ViamException("Unable to find resource named " + name);
}

void ResourceManager::replace_all(std::unordered_map<Name, std::shared_ptr<Resource>> resources) {
    std::lock_guard<std::mutex> lock(lock_);
    std::unordered_map<std::string, std::shared_ptr<Resource>> new_resources;
    std::unordered_map<std::string, std::string> new_short_names;
    this->resources_ = new_resources;
    this->short_names_ = new_short_names;

    for (auto& resource : resources) {
        try {
            do_add(resource.first, resource.second);
        } catch (std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << "Error replacing all resources" << exc.what();
            return;
        }
    }
}

std::string get_shortcut_name(std::string name) {
    std::vector<std::string> name_split;
    // clang-tidy thinks this is a possible memory leak
    // NOLINTNEXTLINE
    boost::split(name_split, name, boost::is_any_of(":"));
    return name_split.at(name_split.size() - 1);
}

void ResourceManager::do_add(Name name, std::shared_ptr<Resource> resource) {
    if (name.name().empty()) {
        throw ViamException("Empty name used for resource: " + name.to_string());
    }
    std::string short_name = name.short_name();

    do_add(short_name, resource);
}

void ResourceManager::do_add(std::string name, std::shared_ptr<Resource> resource) {
    if (resources_.find(name) != resources_.end()) {
        throw DuplicateResourceException("Attempted to add resource that already existed: " + name);
    }

    resources_.emplace(name, resource);

    std::string shortcut = get_shortcut_name(name);
    if (shortcut != name) {
        if (short_names_.find(shortcut) != short_names_.end()) {
            short_names_.emplace(shortcut, "");
        } else {
            short_names_.emplace(shortcut, name);
        }
    }
}

void ResourceManager::add(Name name, std::shared_ptr<Resource> resource) {
    std::lock_guard<std::mutex> lock(lock_);
    try {
        do_add(name, resource);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "Error adding resource to subtype service: " << exc.what();
    }
};

void ResourceManager::do_remove(Name name) {
    std::string short_name = name.short_name();
    if (resources_.find(short_name) == resources_.end()) {
        throw ViamException("attempted to remove resource " + name.to_string() +
                            " but it didn't exist!");
    }
    resources_.erase(short_name);

    std::string shortcut = get_shortcut_name(short_name);
    if (short_names_.find(shortcut) != short_names_.end()) {
        short_names_.erase(shortcut);
    }

    // case: remote1:nameA and remote2:nameA both existed, and remote2:nameA is
    // being deleted, restore shortcut to remote1:nameA
    for (auto& res : resources_) {
        std::string key = res.first;
        if (shortcut == get_shortcut_name(key) && short_name != get_shortcut_name(key)) {
            if (short_names_.find(shortcut) != short_names_.end()) {
                short_names_.emplace(shortcut, "");
            } else {
                short_names_.emplace(shortcut, key);
            }
        }
    }
}

void ResourceManager::remove(Name name) {
    std::lock_guard<std::mutex> lock(lock_);
    try {
        do_remove(name);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "unable to remove resource: " << exc.what();
    };
};

void ResourceManager::replace_one(Name name, std::shared_ptr<Resource> resource) {
    std::lock_guard<std::mutex> lock(lock_);
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

void ResourceManager::add(std::string name, std::shared_ptr<Resource> resource) {
    std::lock_guard<std::mutex> lock(lock_);
    // NOLINTNEXTLINE(clang-analyzer-cplusplus.NewDeleteLeaks)
    do_add(name, resource);
}

}  // namespace sdk
}  // namespace viam
