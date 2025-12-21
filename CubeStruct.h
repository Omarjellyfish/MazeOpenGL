#pragma once
#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "gl/glm/glm.hpp"
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>
#include "PointLight.h"

using namespace std;
using namespace sf;
using namespace glm;

struct AABB {
    glm::vec3 min;
    glm::vec3 max;
};

struct Cube {
    GLuint VAO, VBO, IBO;
    int indexCount;
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 min, max;

    // Texture IDs
    GLuint wallTexture;
    GLuint floorTexture;
    GLuint ceilingTexture;

    // Face index ranges
    struct FaceIndices {
        int start;
        int count;
    };

    FaceIndices frontFace;
    FaceIndices rightFace;
    FaceIndices backFace;
    FaceIndices leftFace;
    FaceIndices topFace;
    FaceIndices bottomFace;

    bool hasFront, hasRight, hasBack, hasLeft, hasTop, hasBottom;

    // Light inside the cube
    PointLight light;
    bool hasLight;


    // Collision data
    AABB collider;
    bool hasCollision;

    // Individual face colliders (optional but useful for selective collision)
    AABB frontCollider;
    AABB backCollider;
    AABB leftCollider;
    AABB rightCollider;
    AABB topCollider;
    AABB bottomCollider;
};