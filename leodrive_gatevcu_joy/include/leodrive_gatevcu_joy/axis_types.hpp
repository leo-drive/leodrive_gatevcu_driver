#pragma once

#include <string>

namespace leodrive_gatevcu_joy
{

enum gamepad_axis {
  LEFT_JOYSTICK_HORIZONTAL, // Steering
  LEFT_JOYSTICK_VERTICAL, // Empty
  RIGHT_TRIGGER, // Gas
  LEFT_TRIGGER, // Brake
  RIGHT_JOYSTICK_HORIZONTAL, // Empty
  RIGHT_JOYSTICK_VERTICAL, // Empty
  DPAD_HORIZONTAL, // Empty
  DPAD_VERTICAL // Gear
};

static std::string axis_to_string(gamepad_axis axis)
{
  switch (axis) {
    case LEFT_JOYSTICK_HORIZONTAL:
      return "Left Joystick Horizontal";
    case LEFT_JOYSTICK_VERTICAL:
      return "Left Joystick Vertical";
    case LEFT_TRIGGER:
      return "Left Trigger";
    case RIGHT_JOYSTICK_HORIZONTAL:
      return "Right Trigger Horizontal";
    case RIGHT_JOYSTICK_VERTICAL:
      return "Right Joystick Vertical";
    case RIGHT_TRIGGER:
      return "Right Trigger";
    case DPAD_HORIZONTAL:
      return "DPad Horizontal";
    case DPAD_VERTICAL:
      return "DPad Vertical";
    default:
      return "Unknown Axis";
  }
}

}  // namespace leodrive_gatevcu_joy