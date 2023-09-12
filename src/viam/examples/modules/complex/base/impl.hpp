#include <viam/sdk/components/base/base.hpp>
#include <viam/sdk/components/component.hpp>
#include <viam/sdk/components/motor/motor.hpp>
#include <viam/sdk/config/resource.hpp>
#include <viam/sdk/resource/resource.hpp>

using namespace viam::sdk;

// MyBase implements a base that only supports set_power (basic forward, back
// and turn controls), is_moving (check if in motion), and stop (stop all
// motion).
//
// It inherits from the built-in resource subtype Base and implements
// `reconfigure`. It also specifies a static `validate` method that checks
// config validity and returns implicit dependencies for the model.
class MyBase : public Base {
   public:
    MyBase(Dependencies deps, ResourceConfig cfg) : Base(cfg.name()) {
        this->reconfigure(deps, cfg);
    };
    void reconfigure(Dependencies deps, ResourceConfig cfg) override;
    static std::vector<std::string> validate(ResourceConfig cfg);

    bool is_moving() override;
    grpc::StatusCode stop(const AttributeMap& extra) override;
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
