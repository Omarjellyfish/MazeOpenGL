#pragma once

#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "gl/glm/glm.hpp"
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>
#include <cstddef>  
#include "CubeStruct.h"
#include "EnumCubeFace.h"
#include "VertexStruct.h"
#include "PointLight.h"

void AddLightToCube(Cube& cube, glm::vec3 lightColor, float intensity);

GLuint LoadTexture(const std::string& filepath);

void SetTexture(Cube& cube, const std::string& wallPath,
    const std::string& floorPath, const std::string& ceilingPath);

Cube CreateCube(glm::vec3 position, glm::vec3 size, glm::vec3( color),int facesToRemove);

Cube CreateCubeExterior(glm::vec3 position, glm::vec3 size, glm::vec3 color, int facesToRemove);