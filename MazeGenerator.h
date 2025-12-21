#pragma once
#include <vector>
#include <stack>
#include <random>
#include "CreateCube.h" // Your cube creation header

enum class WallDirection {
    NORTH = 0,  // +Z
    SOUTH = 1,  // -Z
    EAST = 2,   // +X
    WEST = 3    // -X
};

struct MazeCell {
    int x, z;  // Grid position
    bool visited;
    bool walls[4]; // North, South, East, West

    // Add default constructor
    MazeCell() : x(0), z(0), visited(false) {
        walls[0] = walls[1] = walls[2] = walls[3] = true;
    }

    MazeCell(int x, int z) : x(x), z(z), visited(false) {
        walls[0] = walls[1] = walls[2] = walls[3] = true; // All walls initially
    }
};
class MazeGenerator {
private:
    int width;
    int height;
    float cellSize;
    std::vector<std::vector<MazeCell>> grid;
    std::mt19937 rng;

    glm::vec3 startPosition;  // Add this
    glm::vec3 endPosition;    // Add this

    // Helper functions
    bool IsValidCell(int x, int z);
    std::vector<MazeCell*> GetUnvisitedNeighbors(MazeCell* cell);
    void RemoveWallBetween(MazeCell* current, MazeCell* neighbor);
    void GenerateMazeRecursive(MazeCell* current);

public:
    MazeGenerator(int width, int height, float cellSize);
    void Generate();
    std::vector<Cube> CreateMazeCubes(const std::string& wallPath,
        const std::string& floorPath,
        const std::string& ceilingPath);
    void AddLightsToMaze(std::vector<Cube>& cubes, int lightFrequency = 3);

    // Add these getters
    glm::vec3 GetStartPosition() const { return startPosition; }
    glm::vec3 GetEndPosition() const { return endPosition; }
};