#pragma once

#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>

using namespace viam::sdk;

// `MyBase` inherits from the `Base` class defined in the viam C++ SDK and
// implements some of the relevant methods along with `reconfigure`. It also
// specifies a static `validate` method that checks config validity.
class MyBase : public Base {
   public:
    MyBase(const Dependencies& deps, const ResourceConfig& cfg) : Base(cfg.name()) {
        this->reconfigure(deps, cfg);
    };
    void reconfigure(const Dependencies& deps, const ResourceConfig& cfg) override;
    static std::vector<std::string> validate(ResourceConfig cfg);

    bool is_moving() override;
    void stop(const AttributeMap& extra) override;
    void set_power(const Vector3& linear,
                   const Vector3& angular,
                   const AttributeMap& extra) override;

    AttributeMap do_command(const AttributeMap& command) override;
    std::vector<GeometryConfig> get_geometries(const AttributeMap& extra) override;
    Base::properties get_properties(const AttributeMap& extra) override;

    void move_straight(int64_t distance_mm, double mm_per_sec, const AttributeMap& extra) override {
        throw std::runtime_error("move_straight unimplemented");
    }
    void spin(double angle_deg, double degs_per_sec, const AttributeMap& extra) override {
        throw std::runtime_error("spin unimplemented");
    }
    void set_velocity(const Vector3& linear,
                      const Vector3& angular,
                      const AttributeMap& extra) override {
        throw std::runtime_error("set_velocity unimplemented");
    }

   private:
    std::shared_ptr<Motor> left_;
    std::shared_ptr<Motor> right_;
};
