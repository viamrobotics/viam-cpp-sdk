#pragma once

#include <string>

class ModuleConfig {
    std::string name;
    std::string exe_path;

    // Validates a module and a given path.
    // Args:
    // 		path: an identifier for the particular module
    //
    // Raises:
    // 		throws an error if the name is invalid, due to invalid
    // characters or use of the reserved "parent" name.
    // 		Additionally throws if no file exists at the exe_path
    void validate(std::string path);

    ModuleConfig(std::string name, std::string exe_path);
};
