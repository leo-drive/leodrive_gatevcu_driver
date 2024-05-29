#include "leodrive_gatevcu_joy/vcu_joy_handler.hpp"

namespace leodrive_gatevcu_joy
{
VcuJoyHandler::VcuJoyHandler(const rclcpp::NodeOptions & options) : Node{"vcu_sender", options}
{
  RCLCPP_INFO_STREAM(get_logger(), "Hello");
  get_params();

  joy_sub_ = create_subscription<sensor_msgs::msg::Joy>(
    "joy", 10, std::bind(&VcuJoyHandler::joy_callback, this, std::placeholders::_1));
  state_machine_timer_ = create_wall_timer(
    rclcpp::Rate(100).period(), std::bind(&VcuJoyHandler::state_machine_callback, this));

  vehicle_pub_ = create_publisher<VehicleMsg>("vehicle", rclcpp::SensorDataQoS());
  longitudinal_pub_ = create_publisher<LongitudinalMsg>("longitudinal", rclcpp::SensorDataQoS());
  steering_pub_ = create_publisher<SteeringMsg>("steering_wheel", rclcpp::SensorDataQoS());

  vehicle_msg_.gear = 1;

  register_buttons();
  register_axes();
}

void VcuJoyHandler::get_params()
{
  params_.joy_timeout = declare_parameter<long>("joy_timeout");
  params_.max_gas_pedal_pos = declare_parameter<double>("max_gas_pedal_pos");
  params_.max_steering_angle = declare_parameter<double>("max_steering_angle");
  params_.steering_wheel_torque = declare_parameter<long>("steering_wheel_torque");
  params_.enable_ramp = declare_parameter<bool>("enable_ramp");
  params_.steering_rate = declare_parameter<double>("steering_rate", 2.0);

  RCLCPP_INFO_STREAM(get_logger(), "joy_timeout: " << params_.joy_timeout);
  RCLCPP_INFO_STREAM(get_logger(), "max_gas_pedal_pos: " << params_.max_gas_pedal_pos);
  RCLCPP_INFO_STREAM(get_logger(), "max_steering_angle: " << params_.max_steering_angle);
  RCLCPP_INFO_STREAM(get_logger(), "steering_wheel_torque: " << params_.steering_wheel_torque);
  RCLCPP_INFO_STREAM(get_logger(), "enable_ramp: " << params_.enable_ramp);
  RCLCPP_INFO_STREAM(get_logger(), "steering_rate: " << params_.steering_rate);
}

void VcuJoyHandler::joy_callback(const sensor_msgs::msg::Joy & msg)
{
  last_joy_time_ = now();
  button_handler_.update(msg);
  axis_handler_.update(msg);
}

void VcuJoyHandler::state_machine_callback()
{
  if (
    last_joy_time_.has_value() &&
    (now() - *last_joy_time_) > rclcpp::Duration(std::chrono::milliseconds(params_.joy_timeout))) {
    RCLCPP_ERROR_STREAM_THROTTLE(get_logger(), *get_clock(), 1000, "Couldn't receive joy.");
    vehicle_msg_.mode = 0;
    vehicle_pub_->publish(vehicle_msg_);
    return;
  }

  button_handler_.tick();
  axis_handler_.tick();

  vehicle_pub_->publish(vehicle_msg_);
  longitudinal_pub_->publish(longitudinal_msg_);
  steering_pub_->publish(steering_msg_);
}

void VcuJoyHandler::register_buttons()
{
  Button wiper{gamepad_button::X_BUTTON};
  wiper.set_log_fields("wiper", &vehicle_msg_.wiper);
  wiper.on_click([this]() {
    if (vehicle_msg_.wiper == VehicleMsg::WIPER_OFF)
      vehicle_msg_.wiper = VehicleMsg::WIPER_ON;
    else
      vehicle_msg_.wiper = VehicleMsg::WIPER_OFF;
  });
  button_handler_.add_button(wiper);

  Button mode{gamepad_button::PS4_BUTTON};
  mode.set_log_fields("mode", &vehicle_msg_.mode);
  mode.on_click([this]() {
    if (vehicle_msg_.mode == VehicleMsg::MODE_OFF)
      vehicle_msg_.mode = VehicleMsg::MODE_ON;
    else
      vehicle_msg_.mode = VehicleMsg::MODE_OFF;
  });
  button_handler_.add_button(mode);

  Button handbrake{gamepad_button::SQUARE_BUTTON};
  handbrake.set_log_fields("handbrake", &vehicle_msg_.hand_brake);
  handbrake.on_click([this]() { vehicle_msg_.hand_brake = VehicleMsg::HANDBRAKE_PULL; });
  handbrake.on_hold([this]() { vehicle_msg_.hand_brake = VehicleMsg::HANDBRAKE_RELEASE; });
  button_handler_.add_button(handbrake);

  Button gear_up{gamepad_button::RIGHT_BUTTON};
  gear_up.set_log_fields("gear change", &vehicle_msg_.gear);
  gear_up.on_click([this]() {
      if (vehicle_msg_.gear > 2) {
          --vehicle_msg_.gear;
      }
  });
  button_handler_.add_button(gear_up);

  Button gear_down{gamepad_button::LEFT_BUTTON};
  gear_down.set_log_fields("gear change", &vehicle_msg_.gear);
  gear_down.on_click([this]() {
      if (vehicle_msg_.gear < 4) {
          ++vehicle_msg_.gear;
      }
  });
  button_handler_.add_button(gear_down);

  Button left_blinker{gamepad_button::LEFT_TRIGGER_BUTTON};
  left_blinker.set_log_fields("left blinker", &vehicle_msg_.blinker);
  left_blinker.on_click([this]() {
    if (vehicle_msg_.blinker == VehicleMsg::BLINKER_LEFT)
      vehicle_msg_.blinker = VehicleMsg ::BLINKER_OFF;
    else {
      vehicle_msg_.blinker = VehicleMsg ::BLINKER_LEFT;
    }
  });
  button_handler_.add_button(left_blinker);

  Button right_blinker{gamepad_button::RIGHT_TRIGGER_BUTTON};
  right_blinker.set_log_fields("right blinker", &vehicle_msg_.blinker);
  right_blinker.on_click([this]() {
    if (vehicle_msg_.blinker == VehicleMsg::BLINKER_RIGHT)
      vehicle_msg_.blinker = VehicleMsg ::BLINKER_OFF;
    else {
      vehicle_msg_.blinker = VehicleMsg ::BLINKER_RIGHT;
    }
  });
  button_handler_.add_button(right_blinker);

  Button hazard_blinker{gamepad_button::TRIANGLE_BUTTON};
  hazard_blinker.set_log_fields("hazard blinker", &vehicle_msg_.blinker);
  hazard_blinker.on_click([this]() {
    if (vehicle_msg_.blinker == VehicleMsg::BLINKER_HAZARD)
      vehicle_msg_.blinker = VehicleMsg ::BLINKER_OFF;
    else {
      vehicle_msg_.blinker = VehicleMsg ::BLINKER_HAZARD;
    }
  });
  button_handler_.add_button(hazard_blinker);

  Button gear_park{gamepad_button::SHARE_BUTTON};
  gear_park.set_log_fields("gear park", &vehicle_msg_.gear);
  gear_park.on_click([this]() { vehicle_msg_.gear = 1; });
  button_handler_.add_button(gear_park);
}

void VcuJoyHandler::register_axes()
{
  Axis gas_pedal{gamepad_axis::RIGHT_TRIGGER};
  gas_pedal.set_log_fields("gas pedal", &longitudinal_msg_.gas_pedal);
  gas_pedal.on_update([this](const float & joy_input) {
    longitudinal_msg_.gas_pedal =
      mapOneRangeToAnother(joy_input, -1.0, 1.0, 0.0, params_.max_gas_pedal_pos, 2);
  });
  axis_handler_.add_axis(gas_pedal);

  Axis brake_pedal{gamepad_axis::LEFT_TRIGGER};
  brake_pedal.set_log_fields("brake pedal", &longitudinal_msg_.brake_pedal);
  brake_pedal.on_update([this](const float & joy_input) {
    const auto shifted = (joy_input * 1) + 1;
    const auto mapped_gas_pedal_pos = 56.371 * std::pow(shifted, 6) + 243.63 * shifted;
    longitudinal_msg_.brake_pedal = static_cast<uint16_t>(mapped_gas_pedal_pos);
  });
  axis_handler_.add_axis(brake_pedal);

  Axis steering{gamepad_axis::LEFT_JOYSTICK_HORIZONTAL};
  steering.set_log_fields("steering angle", &steering_msg_.angle);
  steering.on_update([this](const float & joy_input) {
    target_steering_angle_ = mapOneRangeToAnother(
      joy_input, 1.0, -1.0, -1 * params_.max_steering_angle, params_.max_steering_angle, 2);
    if (!params_.enable_ramp) steering_msg_.angle = target_steering_angle_;
    steering_msg_.torque = params_.steering_wheel_torque;
  });
  steering.on_tick([this]() {
    if (params_.enable_ramp) {
      const auto diff = std::abs(set_steering_angle_ - target_steering_angle_);
      if (diff < 2.0) {
        if (set_steering_angle_ < target_steering_angle_) {
          set_steering_angle_ += diff;
        } else if (set_steering_angle_ > target_steering_angle_) {
          set_steering_angle_ -= params_.steering_rate;
        }

      } else if (set_steering_angle_ < target_steering_angle_) {
        set_steering_angle_ += params_.steering_rate;
      } else if (set_steering_angle_ > target_steering_angle_) {
        set_steering_angle_ -= params_.steering_rate;
      }

      steering_msg_.angle = set_steering_angle_;
    }
  });
  axis_handler_.add_axis(steering);
}

}  // namespace leodrive_gatevcu_joy

#include <rclcpp_components/register_node_macro.hpp>

RCLCPP_COMPONENTS_REGISTER_NODE(leodrive_gatevcu_joy::VcuJoyHandler)