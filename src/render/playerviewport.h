#pragma once

#include <vector>


namespace render {

struct PlayerViewport {
  int x;
  int y;
  int width;
  int height;
};

std::vector<PlayerViewport> createPlayerViewports(int nPlayers, int screenWidth, int screenHeight);

}
