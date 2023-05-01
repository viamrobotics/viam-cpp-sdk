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
#include <viam/sdk/resource/resource_base.hpp>
#include <viam/sdk/rpc/server.hpp>
#include <viam/sdk/services/service_base.hpp>

namespace viam {
namespace sdk {

std::shared_ptr<ResourceBase> ResourceManager::resource(std::string name) {
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
    throw std::runtime_error("Unable to find resource named " + name);
}

void ResourceManager::replace_all(std::unordered_map<Name, std::shared_ptr<ResourceBase>> new_map) {
    std::lock_guard<std::mutex> lock(lock_);
    std::unordered_map<std::string, std::shared_ptr<ResourceBase>> new_resources;
    std::unordered_map<std::string, std::string> new_short_names;
    this->resources_ = new_resources;
    this->short_names_ = new_short_names;

    for (auto& resource : new_map) {
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
    boost::split(name_split, name, boost::is_any_of(":"));
    return name_split.at(name_split.size() - 1);
}

void ResourceManager::do_add(Name name, std::shared_ptr<ResourceBase> resource) {
    if (name.name() == "") {
        throw "Empty name used for resource: " + name.to_string();
    }
    std::string short_name = name.short_name();

    do_add(short_name, resource);
}

void ResourceManager::do_add(std::string name, std::shared_ptr<ResourceBase> resource) {
    if (resources_.find(name) != resources_.end()) {
        throw "Attempted to add resource that already existed: " + name;
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

void ResourceManager::add(Name name, std::shared_ptr<ResourceBase> resource) {
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
        throw "attempted to remove resource " + name.to_string() + " but it didn't exist!";
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

void ResourceManager::replace_one(Name name, std::shared_ptr<ResourceBase> resource) {
    std::lock_guard<std::mutex> lock(lock_);
    try {
        do_remove(name);
        do_add(name, resource);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "failed to replace resource " << name.to_string() << ": "
                                 << exc.what();
    }
}

const std::unordered_map<std::string, std::shared_ptr<ResourceBase>>& ResourceManager::resources()
    const {
    return resources_;
}

void ResourceManager::add(std::string name, std::shared_ptr<ResourceBase> resource) {
    std::lock_guard<std::mutex> lock(lock_);
    do_add(name, resource);
}

}  // namespace sdk
}  // namespace viam
