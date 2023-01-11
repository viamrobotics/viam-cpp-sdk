#include <string>

// CR erodkin: do we actually need a config setup here? might that be a little
// specific to go? find out.
class Module {
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

	Module(std::string name, std::string exe_path);
};
