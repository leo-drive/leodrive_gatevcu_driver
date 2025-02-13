#include "leodrive_gatevcu_adapter/can_interface/drivedb.h"
#include "rclcpp/rclcpp.hpp"

#include "can_msgs/msg/frame.hpp"
#include "leodrive_gatevcu_msgs/msg/longitudinal.hpp"
#include "leodrive_gatevcu_msgs/msg/steering_wheel.hpp"
#include "leodrive_gatevcu_msgs/msg/vehicle.hpp"

namespace leodrive_gatevcu_adapter
{

using CanFrame = can_msgs::msg::Frame;
using SteeringMsg = leodrive_gatevcu_msgs::msg::SteeringWheel;
using LongitudinalMsg = leodrive_gatevcu_msgs::msg::Longitudinal;
using VehicleMsg = leodrive_gatevcu_msgs::msg::Vehicle;

constexpr std::string_view frame_id{"can"};

class VcuSender : public rclcpp::Node
{
public:
  explicit VcuSender(const rclcpp::NodeOptions & options);
  CanFrame create_frame();
  void steering_callback(const SteeringMsg & msg);
  void longitudinal_callback(const LongitudinalMsg & msg);
  void vehicle_callback(const VehicleMsg & msg);

private:
  rclcpp::Publisher<CanFrame>::SharedPtr can_frame_pub_;
  rclcpp::Subscription<SteeringMsg>::SharedPtr steering_sub_;
  rclcpp::Subscription<LongitudinalMsg>::SharedPtr longitudinal_sub_;
  rclcpp::Subscription<VehicleMsg>::SharedPtr vehicle_sub_;
};

}  // namespace leodrive_gatevcu_adapter
