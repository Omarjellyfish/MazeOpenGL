#include "CreateCube.h"
using namespace glm;

Cube CreateCube(glm::vec3 position, glm::vec3 size, glm::vec3 color, int facesToRemove) {
    Cube cube;
    cube.position = position;
    cube.scale = size;

    cube.min = position - (size * 0.5f);
    cube.max = position + (size * 0.5f);

    // Initialize main collider (full cube bounds)
    cube.collider.min = cube.min;
    cube.collider.max = cube.max;
    cube.hasCollision = true;

    // Wall thickness for individual face colliders
    float wallThickness = 0.1f;

    std::vector<VertexStruct> vertices;
    std::vector<unsigned int> indices;
    int vertexIndex = 0;

    float texTileX = size.x;
    float texTileY = size.y;
    float texTileZ = size.z;

    // Front Face (normal pointing -Z for interior view)
    cube.hasFront = !(facesToRemove & FACE_FRONT);
    if (cube.hasFront) {
        cube.frontFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, -1.0f);
        vertices.push_back({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(texTileX, texTileY), normal });

        unsigned int frontIndices[] = {
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 1,
            vertexIndex + 0, vertexIndex + 3, vertexIndex + 2
        };
        indices.insert(indices.end(), std::begin(frontIndices), std::end(frontIndices));
        vertexIndex += 4;
        cube.frontFace.count = 6;

        // Front face collider
        cube.frontCollider.min = glm::vec3(cube.min.x, cube.min.y, cube.max.z - wallThickness);
        cube.frontCollider.max = glm::vec3(cube.max.x, cube.max.y, cube.max.z);
    }
    else {
        cube.frontFace.start = 0;
        cube.frontFace.count = 0;
        cube.frontCollider.min = glm::vec3(0.0f);
        cube.frontCollider.max = glm::vec3(0.0f);
    }

    // Right Face (normal pointing -X for interior view)
    cube.hasRight = !(facesToRemove & FACE_RIGHT);
    if (cube.hasRight) {
        cube.rightFace.start = indices.size();
        glm::vec3 normal = glm::vec3(-1.0f, 0.0f, 0.0f);
        vertices.push_back({ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(texTileZ, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(texTileZ, texTileY), normal });

        unsigned int rightIndices[] = {
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 1,
            vertexIndex + 0, vertexIndex + 3, vertexIndex + 2
        };
        indices.insert(indices.end(), std::begin(rightIndices), std::end(rightIndices));
        vertexIndex += 4;
        cube.rightFace.count = 6;

        // Right face collider
        cube.rightCollider.min = glm::vec3(cube.max.x - wallThickness, cube.min.y, cube.min.z);
        cube.rightCollider.max = glm::vec3(cube.max.x, cube.max.y, cube.max.z);
    }
    else {
        cube.rightFace.start = 0;
        cube.rightFace.count = 0;
        cube.rightCollider.min = glm::vec3(0.0f);
        cube.rightCollider.max = glm::vec3(0.0f);
    }

    // Back Face (normal pointing +Z for interior view)
    cube.hasBack = !(facesToRemove & FACE_BACK);
    if (cube.hasBack) {
        cube.backFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
        vertices.push_back({ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(texTileX, texTileY), normal });

        unsigned int backIndices[] = {
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 1,
            vertexIndex + 0, vertexIndex + 3, vertexIndex + 2
        };
        indices.insert(indices.end(), std::begin(backIndices), std::end(backIndices));
        vertexIndex += 4;
        cube.backFace.count = 6;

        // Back face collider
        cube.backCollider.min = glm::vec3(cube.min.x, cube.min.y, cube.min.z);
        cube.backCollider.max = glm::vec3(cube.max.x, cube.max.y, cube.min.z + wallThickness);
    }
    else {
        cube.backFace.start = 0;
        cube.backFace.count = 0;
        cube.backCollider.min = glm::vec3(0.0f);
        cube.backCollider.max = glm::vec3(0.0f);
    }

    // Left Face (normal pointing +X for interior view)
    cube.hasLeft = !(facesToRemove & FACE_LEFT);
    if (cube.hasLeft) {
        cube.leftFace.start = indices.size();
        glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f);
        vertices.push_back({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(texTileZ, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(texTileZ, texTileY), normal });

        unsigned int leftIndices[] = {
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 1,
            vertexIndex + 0, vertexIndex + 3, vertexIndex + 2
        };
        indices.insert(indices.end(), std::begin(leftIndices), std::end(leftIndices));
        vertexIndex += 4;
        cube.leftFace.count = 6;

        // Left face collider
        cube.leftCollider.min = glm::vec3(cube.min.x, cube.min.y, cube.min.z);
        cube.leftCollider.max = glm::vec3(cube.min.x + wallThickness, cube.max.y, cube.max.z);
    }
    else {
        cube.leftFace.start = 0;
        cube.leftFace.count = 0;
        cube.leftCollider.min = glm::vec3(0.0f);
        cube.leftCollider.max = glm::vec3(0.0f);
    }

    // Top Face (normal pointing -Y for interior view)
    cube.hasTop = !(facesToRemove & FACE_TOP);
    if (cube.hasTop) {
        cube.topFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);
        vertices.push_back({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, texTileZ), normal });
        vertices.push_back({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(texTileX, texTileZ), normal });

        unsigned int topIndices[] = {
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 1,
            vertexIndex + 0, vertexIndex + 3, vertexIndex + 2
        };

        indices.insert(indices.end(), std::begin(topIndices), std::end(topIndices));
        vertexIndex += 4;
        cube.topFace.count = 6;

        // Top face collider
        cube.topCollider.min = glm::vec3(cube.min.x, cube.max.y - wallThickness, cube.min.z);
        cube.topCollider.max = glm::vec3(cube.max.x, cube.max.y, cube.max.z);
    }
    else {
        cube.topFace.start = 0;
        cube.topFace.count = 0;
        cube.topCollider.min = glm::vec3(0.0f);
        cube.topCollider.max = glm::vec3(0.0f);
    }

    // Bottom Face (normal pointing +Y for interior view)
    cube.hasBottom = !(facesToRemove & FACE_BOTTOM);
    if (cube.hasBottom) {
        cube.bottomFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
        vertices.push_back({ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, texTileZ), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(texTileX, texTileZ), normal });

        unsigned int bottomIndices[] = {
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 1,
            vertexIndex + 0, vertexIndex + 3, vertexIndex + 2
        };
        indices.insert(indices.end(), std::begin(bottomIndices), std::end(bottomIndices));
        vertexIndex += 4;
        cube.bottomFace.count = 6;

        // Bottom face collider
        cube.bottomCollider.min = glm::vec3(cube.min.x, cube.min.y, cube.min.z);
        cube.bottomCollider.max = glm::vec3(cube.max.x, cube.min.y + wallThickness, cube.max.z);
    }
    else {
        cube.bottomFace.start = 0;
        cube.bottomFace.count = 0;
        cube.bottomCollider.min = glm::vec3(0.0f);
        cube.bottomCollider.max = glm::vec3(0.0f);
    }

    cube.indexCount = (int)indices.size();

    // ... rest of OpenGL buffer setup remains the same ...
    glGenVertexArrays(1, &cube.VAO);
    glBindVertexArray(cube.VAO);

    glGenBuffers(1, &cube.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexStruct), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &cube.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStruct),
        (void*)offsetof(VertexStruct, texCoords));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct),
        (void*)offsetof(VertexStruct, Normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    cube.wallTexture = 0;
    cube.floorTexture = 0;
    cube.ceilingTexture = 0;

    cube.light.position = position;
    cube.light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    cube.light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    cube.light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    cube.light.constant = 1.0f;
    cube.light.linear = 0.09f;
    cube.light.quadratic = 0.032f;
    cube.hasLight = false;

    return cube;
}




// Add this new function to your CreateCube.h/cpp
Cube CreateCubeExterior(glm::vec3 position, glm::vec3 size, glm::vec3 color, int facesToRemove) {
    Cube cube;
    cube.position = position;
    cube.scale = size;

    cube.min = position - (size * 0.5f);
    cube.max = position + (size * 0.5f);

    std::vector<VertexStruct> vertices;
    std::vector<unsigned int> indices;
    int vertexIndex = 0;

    float texTileX = size.x;
    float texTileY = size.y;
    float texTileZ = size.z;

    // Create vertices with OUTWARD-FACING normals (standard cube)

    // Front Face (normal pointing +Z)
    cube.hasFront = !(facesToRemove & FACE_FRONT);
    if (cube.hasFront) {
        cube.frontFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, 1.0f);
        vertices.push_back({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(texTileX, texTileY), normal });

        unsigned int frontIndices[] = {
            vertexIndex + 0, vertexIndex + 1, vertexIndex + 2,
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 3
        };
        indices.insert(indices.end(), std::begin(frontIndices), std::end(frontIndices));
        vertexIndex += 4;
        cube.frontFace.count = 6;
    }
    else {
        cube.frontFace.start = 0;
        cube.frontFace.count = 0;
    }

    // Right Face (normal pointing +X)
    cube.hasRight = !(facesToRemove & FACE_RIGHT);
    if (cube.hasRight) {
        cube.rightFace.start = indices.size();
        glm::vec3 normal = glm::vec3(1.0f, 0.0f, 0.0f);
        vertices.push_back({ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(texTileZ, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(texTileZ, texTileY), normal });

        unsigned int rightIndices[] = {
            vertexIndex + 0, vertexIndex + 1, vertexIndex + 2,
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 3
        };
        indices.insert(indices.end(), std::begin(rightIndices), std::end(rightIndices));
        vertexIndex += 4;
        cube.rightFace.count = 6;
    }
    else {
        cube.rightFace.start = 0;
        cube.rightFace.count = 0;
    }

    // Back Face (normal pointing -Z)
    cube.hasBack = !(facesToRemove & FACE_BACK);
    if (cube.hasBack) {
        cube.backFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, 0.0f, -1.0f);
        vertices.push_back({ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(texTileX, texTileY), normal });

        unsigned int backIndices[] = {
            vertexIndex + 0, vertexIndex + 1, vertexIndex + 2,
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 3
        };
        indices.insert(indices.end(), std::begin(backIndices), std::end(backIndices));
        vertexIndex += 4;
        cube.backFace.count = 6;
    }
    else {
        cube.backFace.start = 0;
        cube.backFace.count = 0;
    }

    // Left Face (normal pointing -X)
    cube.hasLeft = !(facesToRemove & FACE_LEFT);
    if (cube.hasLeft) {
        cube.leftFace.start = indices.size();
        glm::vec3 normal = glm::vec3(-1.0f, 0.0f, 0.0f);
        vertices.push_back({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, texTileY), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(texTileZ, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(texTileZ, texTileY), normal });

        unsigned int leftIndices[] = {
            vertexIndex + 0, vertexIndex + 1, vertexIndex + 2,
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 3
        };
        indices.insert(indices.end(), std::begin(leftIndices), std::end(leftIndices));
        vertexIndex += 4;
        cube.leftFace.count = 6;
    }
    else {
        cube.leftFace.start = 0;
        cube.leftFace.count = 0;
    }

    // Top Face (normal pointing +Y)
    cube.hasTop = !(facesToRemove & FACE_TOP);
    if (cube.hasTop) {
        cube.topFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, 1.0f, 0.0f);
        vertices.push_back({ glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec2(0.0f, texTileZ), normal });
        vertices.push_back({ glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f,  0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(0.5f,  0.5f, -0.5f), glm::vec2(texTileX, texTileZ), normal });

        unsigned int topIndices[] = {
            vertexIndex + 0, vertexIndex + 1, vertexIndex + 2,
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 3
        };

        indices.insert(indices.end(), std::begin(topIndices), std::end(topIndices));
        vertexIndex += 4;
        cube.topFace.count = 6;
    }
    else {
        cube.topFace.start = 0;
        cube.topFace.count = 0;
    }

    // Bottom Face (normal pointing -Y)
    cube.hasBottom = !(facesToRemove & FACE_BOTTOM);
    if (cube.hasBottom) {
        cube.bottomFace.start = indices.size();
        glm::vec3 normal = glm::vec3(0.0f, -1.0f, 0.0f);
        vertices.push_back({ glm::vec3(0.5f, -0.5f,  0.5f), glm::vec2(texTileX, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec2(0.0f, 0.0f), normal });
        vertices.push_back({ glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec2(0.0f, texTileZ), normal });
        vertices.push_back({ glm::vec3(0.5f, -0.5f, -0.5f), glm::vec2(texTileX, texTileZ), normal });

        unsigned int bottomIndices[] = {
            vertexIndex + 0, vertexIndex + 1, vertexIndex + 2,
            vertexIndex + 0, vertexIndex + 2, vertexIndex + 3
        };
        indices.insert(indices.end(), std::begin(bottomIndices), std::end(bottomIndices));
        vertexIndex += 4;
        cube.bottomFace.count = 6;
    }
    else {
        cube.bottomFace.start = 0;
        cube.bottomFace.count = 0;
    }

    cube.indexCount = (int)indices.size();

    // OpenGL Buffers
    glGenVertexArrays(1, &cube.VAO);
    glBindVertexArray(cube.VAO);

    glGenBuffers(1, &cube.VBO);
    glBindBuffer(GL_ARRAY_BUFFER, cube.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexStruct), vertices.data(), GL_STATIC_DRAW);

    glGenBuffers(1, &cube.IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube.IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexStruct),
        (void*)offsetof(VertexStruct, texCoords));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(VertexStruct),
        (void*)offsetof(VertexStruct, Normal));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    cube.wallTexture = 0;
    cube.floorTexture = 0;
    cube.ceilingTexture = 0;

    cube.light.position = position + glm::vec3(0.0f, size.y + 1.0f, 0.0f);
    cube.light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    cube.light.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    cube.light.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    cube.light.constant = 1.0f;
    cube.light.linear = 0.09f;
    cube.light.quadratic = 0.032f;
    cube.hasLight = true;

    return cube;
}