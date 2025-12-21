#include "MazeGenerator.h"
#include <algorithm>
#include <iostream>

MazeGenerator::MazeGenerator(int width, int height, float cellSize)
    : width(width), height(height), cellSize(cellSize) {

    // Initialize random number generator
    std::random_device rd;
    rng.seed(rd());

    // Create grid - properly initialize each cell
    grid.resize(width);
    for (int x = 0; x < width; x++) {
        grid[x].resize(height);
        for (int z = 0; z < height; z++) {
            grid[x][z].x = x;  // Set the x position
            grid[x][z].z = z;  // Set the z position
            grid[x][z].visited = false;
            grid[x][z].walls[0] = true;
            grid[x][z].walls[1] = true;
            grid[x][z].walls[2] = true;
            grid[x][z].walls[3] = true;
        }
    }
}

bool MazeGenerator::IsValidCell(int x, int z) {
    return x >= 0 && x < width && z >= 0 && z < height;
}

std::vector<MazeCell*> MazeGenerator::GetUnvisitedNeighbors(MazeCell* cell) {
    std::vector<MazeCell*> neighbors;

    // North (+Z)
    if (IsValidCell(cell->x, cell->z + 1) && !grid[cell->x][cell->z + 1].visited) {
        neighbors.push_back(&grid[cell->x][cell->z + 1]);
    }

    // South (-Z)
    if (IsValidCell(cell->x, cell->z - 1) && !grid[cell->x][cell->z - 1].visited) {
        neighbors.push_back(&grid[cell->x][cell->z - 1]);
    }

    // East (+X)
    if (IsValidCell(cell->x + 1, cell->z) && !grid[cell->x + 1][cell->z].visited) {
        neighbors.push_back(&grid[cell->x + 1][cell->z]);
    }

    // West (-X)
    if (IsValidCell(cell->x - 1, cell->z) && !grid[cell->x - 1][cell->z].visited) {
        neighbors.push_back(&grid[cell->x - 1][cell->z]);
    }

    return neighbors;
}

void MazeGenerator::RemoveWallBetween(MazeCell* current, MazeCell* neighbor) {
    int dx = neighbor->x - current->x;
    int dz = neighbor->z - current->z;

    if (dx == 1) {  // Neighbor is EAST
        current->walls[(int)WallDirection::EAST] = false;
        neighbor->walls[(int)WallDirection::WEST] = false;
    }
    else if (dx == -1) {  // Neighbor is WEST
        current->walls[(int)WallDirection::WEST] = false;
        neighbor->walls[(int)WallDirection::EAST] = false;
    }
    else if (dz == 1) {  // Neighbor is NORTH
        current->walls[(int)WallDirection::NORTH] = false;
        neighbor->walls[(int)WallDirection::SOUTH] = false;
    }
    else if (dz == -1) {  // Neighbor is SOUTH
        current->walls[(int)WallDirection::SOUTH] = false;
        neighbor->walls[(int)WallDirection::NORTH] = false;
    }
}

void MazeGenerator::GenerateMazeRecursive(MazeCell* current) {
    current->visited = true;

    std::vector<MazeCell*> neighbors = GetUnvisitedNeighbors(current);

    while (!neighbors.empty()) {
        // Choose random neighbor
        std::uniform_int_distribution<int> dist(0, neighbors.size() - 1);
        int randomIndex = dist(rng);
        MazeCell* chosen = neighbors[randomIndex];

        // Remove wall between current and chosen
        RemoveWallBetween(current, chosen);

        // Recursively visit chosen cell
        GenerateMazeRecursive(chosen);

        // Get new unvisited neighbors
        neighbors = GetUnvisitedNeighbors(current);
    }
}

void MazeGenerator::Generate() {
    std::cout << "Generating maze of size " << width << "x" << height << "..." << std::endl;

    // Start from corner (0,0)
    int startX = 0;
    int startZ = 0;

    GenerateMazeRecursive(&grid[startX][startZ]);

    // Set start and end positions
    startPosition = glm::vec3(
        (startX - width / 2.0f) * cellSize,
        0.0f,
        (startZ - height / 2.0f) * cellSize
    );

    // End is at opposite corner
    int endX = width - 1;
    int endZ = height - 1;

    endPosition = glm::vec3(
        (endX - width / 2.0f) * cellSize,
        0.0f,
        (endZ - height / 2.0f) * cellSize
    );

    std::cout << "Maze generation complete!" << std::endl;
    std::cout << "Start: (" << startX << ", " << startZ << ")" << std::endl;
    std::cout << "End: (" << endX << ", " << endZ << ")" << std::endl;
}

std::vector<Cube> MazeGenerator::CreateMazeCubes(const std::string& wallPath,
    const std::string& floorPath,
    const std::string& ceilingPath) {
    std::vector<Cube> mazeCubes;

    std::cout << "Creating " << (width * height) << " maze cubes..." << std::endl;

    for (int x = 0; x < width; x++) {
        for (int z = 0; z < height; z++) {
            MazeCell& cell = grid[x][z];

            // Calculate world position (centered around origin)
            glm::vec3 position(
                (x - width / 2.0f) * cellSize,
                0.0f,
                (z - height / 2.0f) * cellSize
            );

            // Determine which faces to remove based on walls
            int facesToRemove = 0;

            if (!cell.walls[(int)WallDirection::NORTH]) {
                facesToRemove |= FACE_FRONT; // North is front (+Z)
            }
            if (!cell.walls[(int)WallDirection::SOUTH]) {
                facesToRemove |= FACE_BACK; // South is back (-Z)
            }
            if (!cell.walls[(int)WallDirection::EAST]) {
                facesToRemove |= FACE_RIGHT; // East is right (+X)
            }
            if (!cell.walls[(int)WallDirection::WEST]) {
                facesToRemove |= FACE_LEFT; // West is left (-X)
            }

            // Create cube - size should match cellSize exactly for no gaps
            glm::vec3 size(cellSize, cellSize, cellSize);
            Cube cube = CreateCube(position, size, glm::vec3(1.0f), facesToRemove);

            // Set textures
            SetTexture(cube, wallPath, floorPath, ceilingPath);

            // Add light to every cell for full brightness
            AddLightToCube(cube, glm::vec3(1.0f, 1.0f, 0.95f), 0.1f); // Bright warm white

            mazeCubes.push_back(cube);
        }
    }

    std::cout << "Maze cubes created!" << std::endl;
    return mazeCubes;
}

void MazeGenerator::AddLightsToMaze(std::vector<Cube>& cubes, int lightFrequency) {
    std::cout << "Adding lights to maze..." << std::endl;

    std::uniform_int_distribution<int> colorDist(0, 5);

    glm::vec3 lightColors[] = {
        glm::vec3(1.0f, 1.0f, 0.9f),  // Warm white
        glm::vec3(1.0f, 0.8f, 0.6f),  // Orange
        glm::vec3(0.6f, 0.8f, 1.0f),  // Cool blue
        glm::vec3(1.0f, 0.6f, 0.6f),  // Soft red
        glm::vec3(0.6f, 1.0f, 0.6f),  // Soft green
        glm::vec3(0.9f, 0.7f, 1.0f)   // Purple
    };

    int lightCount = 0;
    for (size_t i = 0; i < cubes.size(); i += lightFrequency) {
        int colorIndex = colorDist(rng);
        AddLightToCube(cubes[i], lightColors[colorIndex], 1.5f);
        lightCount++;
    }

    std::cout << "Added " << lightCount << " lights to maze" << std::endl;
}