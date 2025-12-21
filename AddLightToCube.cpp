#include "CreateCube.h"
void AddLightToCube(Cube& cube, glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f), float intensity = 1.0f) {
    cube.hasLight = true;
    cube.light.position = cube.position; // Light at center of cube
    cube.light.ambient = lightColor * 0.2f * intensity;
    cube.light.diffuse = lightColor * 0.8f * intensity;
    cube.light.specular = lightColor * intensity;
}