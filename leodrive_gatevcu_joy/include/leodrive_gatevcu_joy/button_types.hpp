#pragma once

#include <string>

namespace leodrive_gatevcu_joy
{

enum gamepad_button {
  X_BUTTON,
  SQUARE_BUTTON,
  CIRCLE_BUTTON,
  TRIANGLE_BUTTON,
  RIGHT_BUTTON, // R1 - Gear Up
  LEFT_BUTTON, // L1 - Gear Down
  RIGHT_TRIGGER_BUTTON, // R2
  LEFT_TRIGGER_BUTTON, // L2
  SHARE_BUTTON,
  OPTIONS_BUTTON,
  PS4_BUTTON,
  LEFT_JOYSTICK_BUTTON,
  RIGHT_JOYSTICK_BUTTON,
  ENTER_BUTTON
};

enum gamepad_axes_button { UP_BUTTON, DOWN_BUTTON };

inline std::string button_to_string(gamepad_button button)
{
  switch (button) {
    case X_BUTTON:
      return "X Button";
    case CIRCLE_BUTTON:
      return "Circle Button";
    case TRIANGLE_BUTTON:
      return "Triangle Button";
    case SQUARE_BUTTON:
      return "Square Button";
    case LEFT_BUTTON:
      return "Left Button";
    case RIGHT_BUTTON:
      return "Right Button";
    case LEFT_TRIGGER_BUTTON:
      return "Left Trigger Button";
    case RIGHT_TRIGGER_BUTTON:
      return "Right Trigger Button";
    case SHARE_BUTTON:
      return "Share Button";
    case OPTIONS_BUTTON:
      return "Options Button";
    case PS4_BUTTON:
      return "PS4 Button";
    case LEFT_JOYSTICK_BUTTON:
      return "Left Joystick Button";
    case RIGHT_JOYSTICK_BUTTON:
      return "Right Joystick Button";
    case ENTER_BUTTON:
      return "Enter Button";
    default:
      return "Unknown Button";
  }
}

inline std::string button_to_string(gamepad_axes_button button)
{
  switch (button) {
    case UP_BUTTON:
      return "Up Button";
    case DOWN_BUTTON:
      return "Down Button";
    default:
      return "Unknown Button";
  }
}

}  // namespace leodrive_gatevcu_joy