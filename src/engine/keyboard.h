#ifndef ENGINE_KEYBOARD_H
#define ENGINE_KEYBOARD_H

#include <map>
#include <string>

#include <SFML/Window/Keyboard.hpp>


namespace engine {

using sf::Keyboard;

std::string getKeyboardKeyName(Keyboard::Key key);
bool parseKeyboardKeyName(Keyboard::Key key, std::string& result);

}  // namespace engine

#endif  // ENGINE_KEYBOARD_H
