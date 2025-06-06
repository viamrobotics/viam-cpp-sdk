// This file is adapted from examples/modules/simple/main.cpp, but it runs the module
// service in a thread which is killed after two seconds. This is a workaround to conform with the
// expected format of a conan test package executable.
#include <csignal>

#include <iostream>
#include <memory>
#include <sstream>
#include <thread>

#include <viam/sdk/common/exception.hpp>
#include <viam/sdk/common/instance.hpp>
#include <viam/sdk/common/proto_value.hpp>
#include <viam/sdk/components/sensor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/log/logging.hpp>
#include <viam/sdk/module/service.hpp>
#include <viam/sdk/registry/registry.hpp>
#include <viam/sdk/resource/reconfigurable.hpp>

using namespace viam::sdk;

// Implements a trivial sensor component, constructed with a ResourceConfig that specifies a
// "multiplier" value which is then returned as the only sensor reading.
class MySensor : public Sensor, public Reconfigurable {
   public:
    MySensor(const ResourceConfig& cfg) : Sensor(cfg.name()) {
        this->reconfigure({}, cfg);
    }

    static std::vector<std::string> validate(const ResourceConfig&);

    void reconfigure(const Dependencies&, const ResourceConfig&) override;

    ProtoStruct do_command(const ProtoStruct&) override;

    std::vector<GeometryConfig> get_geometries(const ProtoStruct&) override {
        throw Exception("method not supported");
    }

    ProtoStruct get_readings(const ProtoStruct&) override;

   private:
    double multiplier_{1.0};
};

std::vector<std::string> MySensor::validate(const ResourceConfig& cfg) {
    auto itr = cfg.attributes().find("multiplier");
    if (itr != cfg.attributes().end()) {
        const double* multiplier = itr->second.get<double>();
        if (!multiplier) {
            throw Exception("multiplier must be a number value");
        }

        if (*multiplier == 0.0) {
            throw Exception("multiplier cannot be zero");
        }
    }

    return {};
}

void MySensor::reconfigure(const Dependencies&, const ResourceConfig& cfg) {
    auto itr = cfg.attributes().find("multiplier");
    if (itr != cfg.attributes().end()) {
        const double* multiplier = itr->second.get<double>();
        if (multiplier) {
            multiplier_ = *multiplier;
        }
    }
}

ProtoStruct MySensor::do_command(const ProtoStruct& command) {
    for (const auto& entry : command) {
        // The VIAM_RESOURCE_LOG macro will associate log messages to the current resource and can
        // be filtered upon
        VIAM_RESOURCE_LOG(info) << "Command entry " << entry.first;
    }

    return command;
}

ProtoStruct MySensor::get_readings(const ProtoStruct&) {
    return {{"signal", multiplier_}};
}

int main(int argc, char** argv) try {
    // Every Viam C++ SDK program must have one and only one Instance object which is created before
    // any other C++ SDK objects and stays alive until all Viam C++ SDK objects are destroyed.
    Instance inst;

    // Write general log statements with the VIAM_SDK_LOG macro
    VIAM_SDK_LOG(info) << "Starting up simple sensor module";

    Model mysensor_model("viam", "sensor", "mysensor");

    std::shared_ptr<ModelRegistration> mr = std::make_shared<ModelRegistration>(
        API::get<Sensor>(),
        mysensor_model,
        [](Dependencies, ResourceConfig cfg) { return std::make_unique<MySensor>(cfg); },
        &MySensor::validate);

    std::vector<std::shared_ptr<ModelRegistration>> mrs = {mr};
    auto my_mod = std::make_shared<ModuleService>(argc, argv, mrs);

    std::thread t{[&my_mod] { my_mod->serve(); }};
    std::this_thread::sleep_for(std::chrono::seconds{2});

    pthread_kill(t.native_handle(), SIGTERM);

    t.join();

    return EXIT_SUCCESS;
} catch (const viam::sdk::Exception& ex) {
    std::cerr << "main failed with exception: " << ex.what() << "\n";
    return EXIT_FAILURE;
}
