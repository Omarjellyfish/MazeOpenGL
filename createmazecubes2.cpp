/*
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

            // Create cube - NOTE: size should match cellSize exactly for no gaps
            glm::vec3 size(cellSize, cellSize, cellSize);
            Cube cube = CreateCube(position, size, glm::vec3(1.0f), facesToRemove);

            // Set textures
            SetTexture(cube, wallPath, floorPath, ceilingPath);

            // Add light to every cell for full brightness
            AddLightToCube(cube, glm::vec3(1.0f, 1.0f, 0.95f), 2.0f); // Bright warm white

            mazeCubes.push_back(cube);
        }
    }

    std::cout << "Maze cubes created!" << std::endl;
    return mazeCubes;
}*/


/*


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

            // Always remove top and bottom for open maze
            // Comment these out if you want ceiling and floor
            // facesToRemove |= FACE_TOP;
            // facesToRemove |= FACE_BOTTOM;

            // Create cube
            glm::vec3 size(cellSize, cellSize, cellSize);
            Cube cube = CreateCube(position, size, glm::vec3(1.0f), facesToRemove);

            // Set textures
            SetTexture(cube, wallPath, floorPath, ceilingPath);

            mazeCubes.push_back(cube);
        }
    }

    std::cout << "Maze cubes created!" << std::endl;
    return mazeCubes;
}
*/