#define BOOST_LOG_DYN_LINK 1
#include <boost/algorithm/string.hpp>
#include <boost/log/trivial.hpp>
#include <boost/optional/optional.hpp>
#include <resource/resource.hpp>
#include <resource/resource_base.hpp>
#include <services/service_base.hpp>
#include <string>
#include <subtype/subtype.hpp>

boost::optional<ResourceBase&> SubtypeService::resource(std::string name) {
    lock.lock();
    if (resources.find(name) != resources.end()) {
        lock.unlock();
        return resources.at(name);
    }

    std::string short_name = short_names.at(name);
    if (resources.find(short_name) != resources.end()) {
        lock.unlock();
        return resources.at(short_name);
    }

    lock.unlock();
    return boost::none;
};

void SubtypeService::replace_all(std::unordered_map<Name, ResourceBase> new_map) {
    lock.lock();
    std::unordered_map<std::string, ResourceBase> new_resources;
    std::unordered_map<std::string, std::string> new_short_names;
    this->resources = new_resources;
    this->short_names = new_short_names;

    for (auto& resource : new_map) {
        try {
            do_add(resource.first, resource.second);
        } catch (std::exception& exc) {
            BOOST_LOG_TRIVIAL(error) << "Error replacing all resources" << exc.what();
            lock.unlock();
            return;
        }
    }

    lock.unlock();
};

std::string get_shortcut_name(std::string name) {
    std::vector<std::string> name_split;
    boost::split(name_split, name, boost::is_any_of(":"));
    return name_split.at(name_split.size() - 1);
}

void SubtypeService::do_add(Name name, ResourceBase resource) {
    if (name.name == "") {
        throw "Empty name used for resource: " + name.to_string();
    }
    std::string short_name = name.short_name();
    if (resources.find(short_name) != resources.end()) {
        throw "Attempted to add resource that already existed: " + name.to_string();
    }

    resources.emplace(short_name, resource);

    std::string shortcut = get_shortcut_name(short_name);
    if (shortcut != short_name) {
        if (short_names.find(shortcut) != short_names.end()) {
            short_names.emplace(shortcut, "");
        } else {
            short_names.emplace(shortcut, short_name);
        }
    }
};

void SubtypeService::add(Name name, ResourceBase resource) {
    lock.lock();
    try {
        do_add(name, resource);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "Error adding resource to subtype service: " << exc.what();
    }
    lock.unlock();
};

void SubtypeService::do_remove(Name name) {
    std::string short_name = name.short_name();
    if (resources.find(short_name) == resources.end()) {
        throw "attempted to remove resource " + name.to_string() + " but it didn't exist!";
    }
    resources.erase(short_name);

    std::string shortcut = get_shortcut_name(short_name);
    if (short_names.find(shortcut) != short_names.end()) {
        short_names.erase(shortcut);
    }

    // case: remote1:nameA and remote2:nameA both existed, and remote2:nameA is being deleted,
    // restore shortcut to remote1:nameA
    for (auto& res : resources) {
        std::string key = res.first;
        if (shortcut == get_shortcut_name(key) && short_name != get_shortcut_name(key)) {
            if (short_names.find(shortcut) != short_names.end()) {
                short_names.emplace(shortcut, "");
            } else {
                short_names.emplace(shortcut, key);
            }
        }
    }
}

void SubtypeService::remove(Name name) {
    lock.lock();
    try {
        do_remove(name);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "unable to remove resource: " << exc.what();
    };
    lock.unlock();
};
void SubtypeService::replace_one(Name name, ResourceBase resource) {
    lock.lock();
    try {
        do_remove(name);
        do_add(name, resource);
    } catch (std::exception& exc) {
        BOOST_LOG_TRIVIAL(error) << "failed to replace resource " << name.to_string() << ": "
                                 << exc.what();
    }
    lock.unlock();
};

SubtypeService::SubtypeService(){};

