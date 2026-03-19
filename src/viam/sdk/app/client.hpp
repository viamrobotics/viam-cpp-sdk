```cpp
#include <viam/sdk/common/proto_type.hpp>
#include <viam/sdk/common/utils.hpp>
#include <viam/sdk/config/resource.hpp>

#include <google/protobuf/timestamp.pb.h>
#include <grpcpp/channel.h>
#include <grpcpp/client_context.h>
#include <grpcpp/create_channel.h>
#include <grpcpp/security/credentials.h>

#include <boost/optional.hpp>

#include "app.grpc.pb.h"
#include "app.pb.h"

namespace viam {
namespace sdk {

class AppClient {
   public:
    AppClient(std::shared_ptr<grpc::Channel> channel)
        : stub_(App::NewStub(channel)) {}

    // Create a new robot part
    void CreateRobotPart(const std::string& robot_name,
                         const std::string& part_name,
                         const std::vector<std::string>& component_names,
                         const std::vector<std::string>& service_names,
                         const AttributeMap& extra = {});

    // Update an existing robot part
    void UpdateRobotPart(const std::string& id,
                         const boost::optional<std::string>& name,
                         const boost::optional<ProtoStruct>& robot_config,
                         const boost::optional<std::string>& robot_config_json,
                         const boost::optional<google::protobuf::Timestamp>& last_known_update,
                         const AttributeMap& extra = {});

    // Delete a robot part
    void DeleteRobotPart(const std::string& id);

    // Get a list of robot parts
    std::vector<RobotPart> ListRobotParts(const std::string& robot_name);

    // Get a specific robot part
    RobotPart GetRobotPart(const std::string& id);

    // Get a list of all robots
    std::vector<Robot> ListRobots();

    // Get a specific robot
    Robot GetRobot(const std::string& name);

    // Create a new robot
    void CreateRobot(const std::string& name, const AttributeMap& extra = {});

    // Delete a robot
    void DeleteRobot(const std::string& name);

    // Get logs for a robot part
    std::vector<Log> GetRobotPartLogs(const std::string& id, const boost::optional<std::string>& filter = {});

    // Get logs for a robot
    std::vector<Log> GetRobotLogs(const std::string& robot_name, const boost::optional<std::string>& filter = {});

    // Send a command to a robot part
    std::vector<std::string> SendRobotPartCommand(const std::string& id,
                                                  const std::string& command,
                                                  const std::vector<std::string>& args = {},
                                                  const AttributeMap& extra = {});

    // Send a command to a robot
    std::vector<std::string> SendRobotCommand(const std::string& robot_name,
                                              const std::string& command,
                                              const std::vector<std::string>& args = {},
                                              const AttributeMap& extra = {});

    // Get a list of all services
    std::vector<Service> ListServices();

    // Get a specific service
    Service GetService(const std::string& id);

    // Create a new service
    void CreateService(const std::string& name,
                       const std::string& part_id,
                       const std::string& service_type,
                       const AttributeMap& extra = {});

    // Delete a service
    void DeleteService(const std::string& id);

    // Get a list of all components
    std::vector<Component> ListComponents();

    // Get a specific component
    Component GetComponent(const std::string& id);

    // Create a new component
    void CreateComponent(const std::string& name,
                         const std::string& part_id,
                         const std::string& component_type,
                         const AttributeMap& extra = {});

    // Delete a component
    void DeleteComponent(const std::string& id);

    // Get a list of all sensors
    std::vector<Sensor> ListSensors();

    // Get a specific sensor
    Sensor GetSensor(const std::string& id);

    // Create a new sensor
    void CreateSensor(const std::string& name,
                      const std::string& part_id,
                      const std::string& sensor_type,
                      const AttributeMap& extra = {});

    // Delete a sensor
    void DeleteSensor(const std::string& id);

    // Get a list of all movements
    std::vector<Movement> ListMovements();

    // Get a specific movement
    Movement GetMovement(const std::string& id);

    // Create a new movement
    void CreateMovement(const std::string& name,
                        const std::string& part_id,
                        const std::string& movement_type,
                        const AttributeMap& extra = {});

    // Delete a movement
    void DeleteMovement(const std::string& id);

    // Get a list of all cameras
    std::vector<Camera> ListCameras();

    // Get a specific camera
    Camera GetCamera(const std::string& id);

    // Create a new camera
    void CreateCamera(const std::string& name,
                      const std::string& part_id,
                      const std::string& camera_type,
                      const AttributeMap& extra = {});

    // Delete a camera
    void DeleteCamera(const std::string& id);

    // Get a list of all sensors for a robot part
    std::vector<Sensor> ListSensorsForRobotPart(const std::string& id);

    // Get a list of all services for a robot part
    std::vector<Service> ListServicesForRobotPart(const std::string& id);

    // Get a list of all components for a robot part
    std::vector<Component> ListComponentsForRobotPart(const std::string& id);

    // Get a list of all movements for a robot part
    std::vector<Movement> ListMovementsForRobotPart(const std::string& id);

    // Get a list of all cameras for a robot part
    std::vector<Camera> ListCamerasForRobotPart(const std::string& id);

    // Get a list of all robot parts for a robot
    std::vector<RobotPart> ListRobotPartsForRobot(const std::string& robot_name);

    // Get a list of all robot parts for a robot part
    std::vector<RobotPart> ListRobotPartsForRobotPart(const std::string& id);

    // Get a list of all robot parts for a service
    std::vector<RobotPart> ListRobotPartsForService(const std::string& id);

    // Get a list of all robot parts for a component
    std::vector<RobotPart> ListRobotPartsForComponent(const std::string& id);

    // Get a list of all robot parts for a movement
    std::vector<RobotPart> ListRobotPartsForMovement(const std::string& id);

    // Get a list of all robot parts for a camera
    std::vector<RobotPart> ListRobotPartsForCamera(const std::string& id);

    // Get a list of all services for a robot
    std::vector<Service> ListServicesForRobot(const std::string& robot_name);

    // Get a list of all components for a robot
    std::vector<Component> ListComponentsForRobot(const std::string& robot_name);

    // Get a list of all movements for a robot
    std::vector<Movement> ListMovementsForRobot(const std::string& robot_name);

    // Get a list of all cameras for a robot
    std::vector<Camera> ListCamerasForRobot(const std::string& robot_name);

    // Get a list of all sensors for a robot
    std::vector<Sensor> ListSensorsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for a component
    std::vector<Log> ListLogsForComponent(const std::string& id);

    // Get a list of all logs for a movement
    std::vector<Log> ListLogsForMovement(const std::string& id);

    // Get a list of all logs for a camera
    std::vector<Log> ListLogsForCamera(const std::string& id);

    // Get a list of all logs for a sensor
    std::vector<Log> ListLogsForSensor(const std::string& id);

    // Get a list of all logs for a robot
    std::vector<Log> ListLogsForRobot(const std::string& robot_name);

    // Get a list of all logs for a robot part
    std::vector<Log> ListLogsForRobotPart(const std::string& id);

    // Get a list of all logs for a service
    std::vector<Log> ListLogsForService(const std::string& id);

    // Get a list of all logs for