#pragma once
#include "CubeStruct.h"
#include "EnumCubeFace.h"
#include "VertexStruct.h"
using namespace std;
using namespace sf;


bool CheckAnyCollision(const glm::vec3& position, float playerRadius,
    const std::vector<Cube>& cubes);

// AABB-AABB collision check
bool CheckAABBCollision(const AABB& a, const AABB& b) {
    return (a.min.x <= b.max.x && a.max.x >= b.min.x) &&
        (a.min.y <= b.max.y && a.max.y >= b.min.y) &&
        (a.min.z <= b.max.z && a.max.z >= b.min.z);
}

// Player collision with a single cube (checks individual faces)
bool CheckPlayerCubeCollision(const glm::vec3& playerPos, float playerRadius, const Cube& cube) {
    if (!cube.hasCollision) return false;

    // Create player AABB from position and radius
    AABB playerAABB;
    playerAABB.min = playerPos - glm::vec3(playerRadius, playerRadius, playerRadius);
    playerAABB.max = playerPos + glm::vec3(playerRadius, playerRadius, playerRadius);

    // Check collision with individual faces (only if they exist)
    if (cube.hasFront && CheckAABBCollision(playerAABB, cube.frontCollider)) return true;
    if (cube.hasBack && CheckAABBCollision(playerAABB, cube.backCollider)) return true;
    if (cube.hasLeft && CheckAABBCollision(playerAABB, cube.leftCollider)) return true;
    if (cube.hasRight && CheckAABBCollision(playerAABB, cube.rightCollider)) return true;
    if (cube.hasTop && CheckAABBCollision(playerAABB, cube.topCollider)) return true;
    if (cube.hasBottom && CheckAABBCollision(playerAABB, cube.bottomCollider)) return true;

    return false;
}

// Helper function to check if position collides with any cube
bool CheckAnyCollision(const glm::vec3& position, float playerRadius,
    const std::vector<Cube>& cubes) {
    for (const auto& cube : cubes) {
        if (CheckPlayerCubeCollision(position, playerRadius, cube)) {
            return true;
        }
    }
    return false;
}

// Check collision with all cubes and resolve (basic version)
glm::vec3 CheckAndResolveCollision(glm::vec3 newPosition, glm::vec3 oldPosition,
    float playerRadius, const std::vector<Cube>& cubes) {
    glm::vec3 resolvedPosition = newPosition;

    for (const auto& cube : cubes) {
        if (CheckPlayerCubeCollision(newPosition, playerRadius, cube)) {
            // Simple resolution: use old position
            resolvedPosition = oldPosition;

            // More sophisticated: sliding collision
            // Try moving only on X axis
            glm::vec3 xOnly = glm::vec3(newPosition.x, oldPosition.y, oldPosition.z);
            if (!CheckPlayerCubeCollision(xOnly, playerRadius, cube)) {
                resolvedPosition = xOnly;
            }
            // Try moving only on Z axis
            else {
                glm::vec3 zOnly = glm::vec3(oldPosition.x, oldPosition.y, newPosition.z);
                if (!CheckPlayerCubeCollision(zOnly, playerRadius, cube)) {
                    resolvedPosition = zOnly;
                }
            }

            break; // Found collision, stop checking
        }
    }

    return resolvedPosition;
}

// More accurate sliding collision
glm::vec3 CheckAndResolveCollisionSliding(glm::vec3 newPosition, glm::vec3 oldPosition,
    float playerRadius, const std::vector<Cube>& cubes) {
    // Try full movement first
    if (!CheckAnyCollision(newPosition, playerRadius, cubes)) {
        return newPosition;
    }

    // Try sliding along X axis
    glm::vec3 xSlide = glm::vec3(newPosition.x, oldPosition.y, oldPosition.z);
    if (!CheckAnyCollision(xSlide, playerRadius, cubes)) {
        return xSlide;
    }

    // Try sliding along Z axis
    glm::vec3 zSlide = glm::vec3(oldPosition.x, oldPosition.y, newPosition.z);
    if (!CheckAnyCollision(zSlide, playerRadius, cubes)) {
        return zSlide;
    }

    // Try sliding along Y axis (for stairs/slopes)
    glm::vec3 ySlide = glm::vec3(oldPosition.x, newPosition.y, oldPosition.z);
    if (!CheckAnyCollision(ySlide, playerRadius, cubes)) {
        return ySlide;
    }

    // No valid movement, return old position
    return oldPosition;
}