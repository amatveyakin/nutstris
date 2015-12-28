#include "engine/keyboard.h"

#include <exception>


namespace engine {

std::string getKeyboardKeyName(Keyboard::Key key) {
  switch (key) {
    case Keyboard::Unknown   : break;
    case Keyboard::A         : return "A";
    case Keyboard::B         : return "B";
    case Keyboard::C         : return "C";
    case Keyboard::D         : return "D";
    case Keyboard::E         : return "E";
    case Keyboard::F         : return "F";
    case Keyboard::G         : return "G";
    case Keyboard::H         : return "H";
    case Keyboard::I         : return "I";
    case Keyboard::J         : return "J";
    case Keyboard::K         : return "K";
    case Keyboard::L         : return "L";
    case Keyboard::M         : return "M";
    case Keyboard::N         : return "N";
    case Keyboard::O         : return "O";
    case Keyboard::P         : return "P";
    case Keyboard::Q         : return "Q";
    case Keyboard::R         : return "R";
    case Keyboard::S         : return "S";
    case Keyboard::T         : return "T";
    case Keyboard::U         : return "U";
    case Keyboard::V         : return "V";
    case Keyboard::W         : return "W";
    case Keyboard::X         : return "X";
    case Keyboard::Y         : return "Y";
    case Keyboard::Z         : return "Z";
    case Keyboard::Num0      : return "Num0";
    case Keyboard::Num1      : return "Num1";
    case Keyboard::Num2      : return "Num2";
    case Keyboard::Num3      : return "Num3";
    case Keyboard::Num4      : return "Num4";
    case Keyboard::Num5      : return "Num5";
    case Keyboard::Num6      : return "Num6";
    case Keyboard::Num7      : return "Num7";
    case Keyboard::Num8      : return "Num8";
    case Keyboard::Num9      : return "Num9";
    case Keyboard::Escape    : return "Escape";
    case Keyboard::LControl  : return "LControl";
    case Keyboard::LShift    : return "LShift";
    case Keyboard::LAlt      : return "LAlt";
    case Keyboard::LSystem   : return "LSystem";
    case Keyboard::RControl  : return "RControl";
    case Keyboard::RShift    : return "RShift";
    case Keyboard::RAlt      : return "RAlt";
    case Keyboard::RSystem   : return "RSystem";
    case Keyboard::Menu      : return "Menu";
    case Keyboard::LBracket  : return "LBracket";
    case Keyboard::RBracket  : return "RBracket";
    case Keyboard::SemiColon : return "SemiColon";
    case Keyboard::Comma     : return "Comma";
    case Keyboard::Period    : return "Period";
    case Keyboard::Quote     : return "Quote";
    case Keyboard::Slash     : return "Slash";
    case Keyboard::BackSlash : return "BackSlash";
    case Keyboard::Tilde     : return "Tilde";
    case Keyboard::Equal     : return "Equal";
    case Keyboard::Dash      : return "Dash";
    case Keyboard::Space     : return "Space";
    case Keyboard::Return    : return "Return";
    case Keyboard::BackSpace : return "BackSpace";
    case Keyboard::Tab       : return "Tab";
    case Keyboard::PageUp    : return "PageUp";
    case Keyboard::PageDown  : return "PageDown";
    case Keyboard::End       : return "End";
    case Keyboard::Home      : return "Home";
    case Keyboard::Insert    : return "Insert";
    case Keyboard::Delete    : return "Delete";
    case Keyboard::Add       : return "Add";
    case Keyboard::Subtract  : return "Subtract";
    case Keyboard::Multiply  : return "Multiply";
    case Keyboard::Divide    : return "Divide";
    case Keyboard::Left      : return "Left";
    case Keyboard::Right     : return "Right";
    case Keyboard::Up        : return "Up";
    case Keyboard::Down      : return "Down";
    case Keyboard::Numpad0   : return "Numpad0";
    case Keyboard::Numpad1   : return "Numpad1";
    case Keyboard::Numpad2   : return "Numpad2";
    case Keyboard::Numpad3   : return "Numpad3";
    case Keyboard::Numpad4   : return "Numpad4";
    case Keyboard::Numpad5   : return "Numpad5";
    case Keyboard::Numpad6   : return "Numpad6";
    case Keyboard::Numpad7   : return "Numpad7";
    case Keyboard::Numpad8   : return "Numpad8";
    case Keyboard::Numpad9   : return "Numpad9";
    case Keyboard::F1        : return "F1";
    case Keyboard::F2        : return "F2";
    case Keyboard::F3        : return "F3";
    case Keyboard::F4        : return "F4";
    case Keyboard::F5        : return "F5";
    case Keyboard::F6        : return "F6";
    case Keyboard::F7        : return "F7";
    case Keyboard::F8        : return "F8";
    case Keyboard::F9        : return "F9";
    case Keyboard::F10       : return "F10";
    case Keyboard::F11       : return "F11";
    case Keyboard::F12       : return "F12";
    case Keyboard::F13       : return "F13";
    case Keyboard::F14       : return "F14";
    case Keyboard::F15       : return "F15";
    case Keyboard::Pause     : return "Pause";
    case Keyboard::KeyCount  : break;
  }
  throw std::logic_error("Unknown key");
}

bool parseKeyboardKeyName(const std::string& name, Keyboard::Key& result) {
  // TODO(Andrei): Ignore case (?)
  for (auto key = static_cast<Keyboard::Key>(0); key < Keyboard::KeyCount; key = static_cast<Keyboard::Key>(key + 1)) {
    if (getKeyboardKeyName(key) == name) {
      result = key;
      return true;
    }
  }
  result = Keyboard::Unknown;
  return false;
}

}  // namespace engine
