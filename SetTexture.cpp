#include "CreateCube.h"
void SetTexture(Cube& cube, const std::string& wallPath,
    const std::string& floorPath, const std::string& ceilingPath) {
    cube.wallTexture = LoadTexture(wallPath);
    cube.floorTexture = LoadTexture(floorPath);
    cube.ceilingTexture = LoadTexture(ceilingPath);
}