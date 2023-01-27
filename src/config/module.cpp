#include <filesystem>
#include <regex>
#include <string>

const std::string RESERVED_MODULE_NAME = "parent";

class ModuleConfig {
    std::string name;
    std::string exe_path;

    void validate(std::string path);

    ModuleConfig(std::string name, std::string exe_path);
};

void ModuleConfig::validate(std::string path) {
    if (!std::__fs::filesystem::exists(exe_path)) {
        throw "module " + path + " executable path error. Module's listed exe_path: " + exe_path;
    }

    std::regex regex_match("^[A-Za-z0-9_-]+$");

    if (!std::regex_match(name, regex_match)) {
        throw "module " + path +
            " must only contain letters, numbers, underscores, and "
            "hyphens. Provided name: " +
            name;
    }

    if (name == RESERVED_MODULE_NAME) {
        throw "module " + path + " cannot use the reserved module name " + RESERVED_MODULE_NAME;
    }
}

ModuleConfig::ModuleConfig(std::string name, std::string exe_path) {
    validate(exe_path);
    name = name;
    exe_path = exe_path;
}
