#include "playerviewport.h"
#include "render/defs.h"

namespace render {
  
std::vector<PlayerViewport> createPlayerViewports(int nPlayers, int screenWidth, int screenHeight) {
  float vpWidth  = float(screenWidth) / nPlayers;
  float vpHeight = float(screenHeight);

  vpWidth  = std::min(vpWidth,  vpHeight * render::VP_ASPECT);
  vpHeight = std::min(vpHeight, vpWidth  / render::VP_ASPECT);

  float leftIndent = (screenWidth - nPlayers * vpWidth) / (2.0f * nPlayers);
  float topIndent  = (screenHeight - vpHeight) / 2.0f;
  
  std::vector<PlayerViewport> res(nPlayers);
  for (int i = 0; i < nPlayers; ++i)
  {
    res[i].x = int(leftIndent + i * screenWidth / nPlayers);
    res[i].y = int(topIndent);
    res[i].width =  int(vpWidth);
    res[i].height = int(vpHeight);
  }
  
  return res;
}
 
}
