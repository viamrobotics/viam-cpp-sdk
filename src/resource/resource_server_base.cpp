#include <resource/resource_server_base.hpp>

// CR erodkin: for some reason having this here leads to a missing vtable complaint, but defining in
// header doesn't? why?
// void ResourceServerBase::register_server() {
// throw "cannot register the abstract Resource Server base";
// return;
//}
//
