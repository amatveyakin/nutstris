#ifndef PLAYERVIEWPORT_H
#define PLAYERVIEWPORT_H
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

#endif // PLAYERVIEWPORT_H
