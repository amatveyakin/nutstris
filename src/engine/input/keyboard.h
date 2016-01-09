#pragma once

#include <string>

#include <SFML/Window/Keyboard.hpp>


namespace engine {

using sf::Keyboard;

std::string getKeyboardKeyName(Keyboard::Key key);
bool parseKeyboardKeyName(Keyboard::Key key, std::string& result);

}  // namespace engine
