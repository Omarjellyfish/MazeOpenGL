#include <iostream>
#include <gl/glew/glew.h>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>

#include "gl/glm/glm.hpp"
#include<gl\glm\gtc\type_ptr.hpp>
#include<gl\glm\gtx\transform.hpp>

#include <thread>
#include <chrono>

#include "CreateCube.h"
#include "EnumCubeFace.h"
#include "CubeStruct.h"
#include "MazeGenerator.h"
#include "CubeCollider.h"
using namespace std;
using namespace sf;
using namespace glm;



enum DrawingMode
{
	Points,
	Lines,
	Filled
};

GLuint InitShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name);

const GLint WIDTH = 600, HEIGHT = 600;
GLuint ProgramId_Base;
GLuint VBO_Cube, IBO_Cube;
DrawingMode Current_Drawing_Mode = DrawingMode::Filled;
GLuint ThetaLocation;
GLuint ModelMat_Location, ViewMat_Location, ProjMat_Location;
string wall_path = "Assets/wallsTex.png";
string floor_path = "Assets/medCeliing.jpeg";
string ceiling_path = "Assets/floorTex.png";
string door_path = "Assets/trapDoorTex.png";
bool moveForward = false;
bool moveBackward = false;
bool moveLeft = false;
bool moveRight = false;

void CompileShader(const char* vertex_shader_file_name, const char* fragment_shader_file_name, GLuint& ProgramId)
{
	ProgramId = InitShader(vertex_shader_file_name, fragment_shader_file_name);
	glUseProgram(ProgramId);
}

//end condition
Cube CreateEndIndicator(glm::vec3 position, float size) {
	// Create a glowing cube at the end position with EXTERIOR faces
	Cube endCube = CreateCubeExterior(  // Use the new function
		position + glm::vec3(0, size * 0.5f, 0),
		glm::vec3(size * 0.5f, size * 0.5f, size * 0.5f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		0  // No faces removed - full cube
	);

	// Add a bright  light to make it glow
	AddLightToCube(endCube, glm::vec3(0.0f, 1.0f, 0.0f), 1.f);

	return endCube;
}




void BindCube()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO_Cube);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO_Cube);

	glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(VertexStruct), 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(VertexStruct), (char*)(sizeof(vec3)));
	glEnableVertexAttribArray(1);
}
// assign proj/view matrices

bool CheckReachedEnd(glm::vec3 cameraPos, glm::vec3 endPos, float threshold = 2.0f) {
	// Check distance on X and Z axes (ignore Y)
	float dx = cameraPos.x - endPos.x;
	float dz = cameraPos.z - endPos.z;
	float distance = sqrt(dx * dx + dz * dz);

	return distance < threshold;
}

const vec3 WorldUp = vec3(0.0f, 1.0f, 0.0f);

vec3 p = vec3(0, 0, 3);
vec3 f = vec3(0, 0, -1);
vec3 CameraPos = p;
vec3 CameraTarget = p+ f;
vec3 CameraUp = vec3(0.0f, 1.0f, 0.0f);
std::vector<Cube> testCubes;

// Global variables at the top of your file
Cube endIndicator;
glm::vec3 mazeEndPosition;
bool gameWon = false;

int Init()
{
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		cout << "Error";
		getchar();
		return 1;
	}
	else
	{
		if (GLEW_VERSION_3_0)
			cout << "Driver support OpenGL 3.0\nDetails:\n";
	}
	cout << "\tUsing glew " << glewGetString(GLEW_VERSION) << endl;
	cout << "\tVendor: " << glGetString(GL_VENDOR) << endl;
	cout << "\tRenderer: " << glGetString(GL_RENDERER) << endl;
	cout << "\tVersion: " << glGetString(GL_VERSION) << endl;
	cout << "\tGLSL:" << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;

	CompileShader("VS.glsl", "FS.glsl", ProgramId_Base);


	//=== MAZE GENERATION ===
	cout << "\n=== GENERATING MAZE ===" << endl;

	MazeGenerator mazeGen(10, 10, 4.0f);
	mazeGen.Generate();
	testCubes = mazeGen.CreateMazeCubes(wall_path, floor_path, ceiling_path);

	// Get end position and create indicator
	mazeEndPosition = mazeGen.GetEndPosition();
	endIndicator = CreateEndIndicator(mazeEndPosition, 3.0f);

	// Load and apply trap door texture to all faces
	GLuint trapDoorTexture = LoadTexture(door_path.c_str());
	endIndicator.wallTexture = trapDoorTexture;
	endIndicator.floorTexture = trapDoorTexture;
	endIndicator.ceilingTexture = trapDoorTexture;

	std::cout << "\n=== END INDICATOR DEBUG ===" << std::endl;
	std::cout << "endIndicator.hasLight: " << endIndicator.hasLight << std::endl;

	cout << "=== MAZE COMPLETE ===" << endl << endl;

	ThetaLocation = glGetUniformLocation(ProgramId_Base, "theta");
	ModelMat_Location = glGetUniformLocation(ProgramId_Base, "model_mat");
	ViewMat_Location = glGetUniformLocation(ProgramId_Base, "view_mat");
	ProjMat_Location = glGetUniformLocation(ProgramId_Base, "proj_mat");

	CameraPos = glm::vec3(0.0f, 0.0f, 15.0f);
	CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);

	mat4 ViewMat = lookAt(CameraPos, CameraTarget, CameraUp);
	glUniformMatrix4fv(ViewMat_Location, 1, GL_FALSE, value_ptr(ViewMat));

	mat4 ProjMat_Prespective = perspectiveFov(60.0f, (float)WIDTH, (float)HEIGHT, 0.1f, 100.0f);
	glUniformMatrix4fv(ProjMat_Location, 1, GL_FALSE, value_ptr(ProjMat_Prespective));

	glClearColor(0, 0.1, 0.1, 1);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);

	return 0;
}

float theta = 0;
void UpdateCameraPos(vec3 newP, vec3 newF)
{
	CameraPos = newP;
	CameraPos.y = 0.0f; // Keep the camera at a fixed height (y=0)

	CameraTarget = CameraPos + newF;

	// Ensure we look at the target from the new position
	mat4 ViewMat = lookAt(CameraPos, CameraTarget, CameraUp);
	glUniformMatrix4fv(ViewMat_Location, 1, GL_FALSE, value_ptr(ViewMat));
}
void UpdateCameraOrientation(vec3 newF)
{
	// update camera orientation code 
	vec3 CameraRight = normalize(cross(WorldUp, newF));
	CameraUp = normalize(cross(CameraRight, newF));

	CameraTarget = CameraPos + newF;
	mat4 ViewMat = lookAt(CameraPos, CameraTarget, CameraUp);
	glUniformMatrix4fv(ViewMat_Location, 1, GL_FALSE, value_ptr(ViewMat));
}
// In your main update function
void Update(float deltaTime) {
	// Store old position
	glm::vec3 oldPlayerPosition = CameraPos;

	// Calculate new position based on input flags
	glm::vec3 newPlayerPosition = CameraPos;

	float moveSpeed = 10.0f * deltaTime;

	// Calculate camera front direction (on horizontal plane only)
	vec3 CameraFront = normalize(CameraTarget - CameraPos);
	vec3 CameraRight = normalize(cross(CameraFront, WorldUp));

	if (moveForward) {
		newPlayerPosition += CameraFront * moveSpeed;
	}
	if (moveBackward) {
		newPlayerPosition -= CameraFront * moveSpeed;
	}
	if (moveLeft) {
		newPlayerPosition += CameraRight * moveSpeed;
	}
	if (moveRight) {
		newPlayerPosition -= CameraRight * moveSpeed;
	}

	// Define player collision radius
	float playerRadius = 0.5f;

	// Check and resolve collisions
	CameraPos = CheckAndResolveCollisionSliding(
		newPlayerPosition,
		oldPlayerPosition,
		playerRadius,
		testCubes  // Your vector of cube walls
	);

	// Keep camera at fixed height
	CameraPos.y = 0.0f;

	// Update camera
	UpdateCameraPos(CameraPos, normalize(CameraTarget - CameraPos));
}


void Render(bool shouldClose)
{
	try {
		if (shouldClose) {
			std::this_thread::sleep_for(std::chrono::seconds(1));
			exit(0);
			return;
		}
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glUseProgram(ProgramId_Base);

		// Check if player reached the end
		if (!gameWon && CheckReachedEnd(CameraPos, mazeEndPosition)) {
			gameWon = true;
			std::cout << "\n================================" << std::endl;
			std::cout << "CONGRATULATIONS! YOU WON!" << std::endl;
			std::cout << "================================\n" << std::endl;

			// Wait 2 seconds then exit
			std::this_thread::sleep_for(std::chrono::seconds(1));
			exit(0); // Close the application
			return;
		}

		switch (Current_Drawing_Mode)
		{
		case DrawingMode::Points:
			glPointSize(10);
			glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
			break;
		case DrawingMode::Lines:
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		default:
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		}

		GLint viewPosLoc = glGetUniformLocation(ProgramId_Base, "viewPos");
		glUniform3fv(viewPosLoc, 1, glm::value_ptr(CameraPos));

		// Collect all lights (including end indicator)
		std::vector<PointLight> activeLights;
		for (const auto& cube : testCubes) {
			if (cube.hasLight) {
				activeLights.push_back(cube.light);
			}
		}

		// Add end indicator light
		if (endIndicator.hasLight) {
			activeLights.push_back(endIndicator.light);
		}

		GLint numLightsLoc = glGetUniformLocation(ProgramId_Base, "numLights");
		glUniform1i(numLightsLoc, activeLights.size());

		for (size_t i = 0; i < activeLights.size() && i < 100; i++) {
			std::string lightBase = "lights[" + std::to_string(i) + "]";

			GLint posLoc = glGetUniformLocation(ProgramId_Base, (lightBase + ".position").c_str());
			GLint ambLoc = glGetUniformLocation(ProgramId_Base, (lightBase + ".ambient").c_str());
			GLint diffLoc = glGetUniformLocation(ProgramId_Base, (lightBase + ".diffuse").c_str());
			GLint specLoc = glGetUniformLocation(ProgramId_Base, (lightBase + ".specular").c_str());
			GLint constLoc = glGetUniformLocation(ProgramId_Base, (lightBase + ".constant").c_str());
			GLint linLoc = glGetUniformLocation(ProgramId_Base, (lightBase + ".linear").c_str());
			GLint quadLoc = glGetUniformLocation(ProgramId_Base, (lightBase + ".quadratic").c_str());

			glUniform3fv(posLoc, 1, glm::value_ptr(activeLights[i].position));
			glUniform3fv(ambLoc, 1, glm::value_ptr(activeLights[i].ambient));
			glUniform3fv(diffLoc, 1, glm::value_ptr(activeLights[i].diffuse));
			glUniform3fv(specLoc, 1, glm::value_ptr(activeLights[i].specular));
			glUniform1f(constLoc, activeLights[i].constant);
			glUniform1f(linLoc, activeLights[i].linear);
			glUniform1f(quadLoc, activeLights[i].quadratic);
		}

		GLint useTextureLoc = glGetUniformLocation(ProgramId_Base, "useTexture");
		glUniform1i(useTextureLoc, 1);

		glActiveTexture(GL_TEXTURE0);
		GLint textureLoc = glGetUniformLocation(ProgramId_Base, "texture1");
		glUniform1i(textureLoc, 0);

		// Render maze cubes
		for (const auto& cube : testCubes) {
			glBindVertexArray(cube.VAO);

			mat4 ModelMat = translate(cube.position) * scale(cube.scale);
			glUniformMatrix4fv(ModelMat_Location, 1, GL_FALSE, value_ptr(ModelMat));

			if (cube.hasFront && cube.wallTexture != 0) {
				glBindTexture(GL_TEXTURE_2D, cube.wallTexture);
				glDrawElements(GL_TRIANGLES, cube.frontFace.count, GL_UNSIGNED_INT,
					(void*)(cube.frontFace.start * sizeof(unsigned int)));
			}

			if (cube.hasRight && cube.wallTexture != 0) {
				glBindTexture(GL_TEXTURE_2D, cube.wallTexture);
				glDrawElements(GL_TRIANGLES, cube.rightFace.count, GL_UNSIGNED_INT,
					(void*)(cube.rightFace.start * sizeof(unsigned int)));
			}

			if (cube.hasBack && cube.wallTexture != 0) {
				glBindTexture(GL_TEXTURE_2D, cube.wallTexture);
				glDrawElements(GL_TRIANGLES, cube.backFace.count, GL_UNSIGNED_INT,
					(void*)(cube.backFace.start * sizeof(unsigned int)));
			}

			if (cube.hasLeft && cube.wallTexture != 0) {
				glBindTexture(GL_TEXTURE_2D, cube.wallTexture);
				glDrawElements(GL_TRIANGLES, cube.leftFace.count, GL_UNSIGNED_INT,
					(void*)(cube.leftFace.start * sizeof(unsigned int)));
			}

			if (cube.hasTop && cube.ceilingTexture != 0) {
				glBindTexture(GL_TEXTURE_2D, cube.ceilingTexture);
				glDrawElements(GL_TRIANGLES, cube.topFace.count, GL_UNSIGNED_INT,
					(void*)(cube.topFace.start * sizeof(unsigned int)));
			}

			if (cube.hasBottom && cube.floorTexture != 0) {
				glBindTexture(GL_TEXTURE_2D, cube.floorTexture);
				glDrawElements(GL_TRIANGLES, cube.bottomFace.count, GL_UNSIGNED_INT,
					(void*)(cube.bottomFace.start * sizeof(unsigned int)));
			}
		}

		// Render end indicator cube (trap door with texture)
		glBindVertexArray(endIndicator.VAO);
		mat4 EndModelMat = translate(endIndicator.position) * scale(endIndicator.scale);
		glUniformMatrix4fv(ModelMat_Location, 1, GL_FALSE, value_ptr(EndModelMat));

		// Enable textures for the trap door
		glUniform1i(useTextureLoc, 1);  // Enable texture

		// Render all faces with trap door texture
		if (endIndicator.hasFront && endIndicator.wallTexture != 0) {
			glBindTexture(GL_TEXTURE_2D, endIndicator.wallTexture);
			glDrawElements(GL_TRIANGLES, endIndicator.frontFace.count, GL_UNSIGNED_INT,
				(void*)(endIndicator.frontFace.start * sizeof(unsigned int)));
		}

		if (endIndicator.hasRight && endIndicator.wallTexture != 0) {
			glBindTexture(GL_TEXTURE_2D, endIndicator.wallTexture);
			glDrawElements(GL_TRIANGLES, endIndicator.rightFace.count, GL_UNSIGNED_INT,
				(void*)(endIndicator.rightFace.start * sizeof(unsigned int)));
		}

		if (endIndicator.hasBack && endIndicator.wallTexture != 0) {
			glBindTexture(GL_TEXTURE_2D, endIndicator.wallTexture);
			glDrawElements(GL_TRIANGLES, endIndicator.backFace.count, GL_UNSIGNED_INT,
				(void*)(endIndicator.backFace.start * sizeof(unsigned int)));
		}

		if (endIndicator.hasLeft && endIndicator.wallTexture != 0) {
			glBindTexture(GL_TEXTURE_2D, endIndicator.wallTexture);
			glDrawElements(GL_TRIANGLES, endIndicator.leftFace.count, GL_UNSIGNED_INT,
				(void*)(endIndicator.leftFace.start * sizeof(unsigned int)));
		}

		if (endIndicator.hasTop && endIndicator.ceilingTexture != 0) {
			glBindTexture(GL_TEXTURE_2D, endIndicator.ceilingTexture);
			glDrawElements(GL_TRIANGLES, endIndicator.topFace.count, GL_UNSIGNED_INT,
				(void*)(endIndicator.topFace.start * sizeof(unsigned int)));
		}

		if (endIndicator.hasBottom && endIndicator.floorTexture != 0) {
			glBindTexture(GL_TEXTURE_2D, endIndicator.floorTexture);
			glDrawElements(GL_TRIANGLES, endIndicator.bottomFace.count, GL_UNSIGNED_INT,
				(void*)(endIndicator.bottomFace.start * sizeof(unsigned int)));
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	catch (const std::exception& e) {
		std::cerr << "Render error: " << e.what() << std::endl;
		return;
	}
}


float Yaw = -90.0f;
float Pitch = 0.0f;     
float lastMouseX = WIDTH / 2.0f; 
float lastMouseY = HEIGHT / 2.0f; 
const float MouseSensitivity = 0.1f;


void ChangeCam(sf::RenderWindow& window, float& Yaw, float& Pitch, float& lastMouseX, float& lastMouseY, const vec3& CameraPos, GLuint ViewMat_Location)
{
	if (!window.isOpen()) {
		return;
	}
	sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
	float mouseX = (float)pixelPos.x;
	float mouseY = (float)pixelPos.y;

	
	float xoffset = lastMouseX - mouseX;
	float yoffset = mouseY - lastMouseY;

	
	float sensitivity = 0.1f; 
	Yaw += xoffset * sensitivity;
	Pitch += yoffset * sensitivity;

	// 4. (vertical rotation limit)
	if (Pitch > 89.0f) {
		Pitch = 89.0f;
	}
	if (Pitch < -89.0f) {
		Pitch = -89.0f;
	}

	// 5. Recalculate Forward Vector
	float yawRad = radians(Yaw);
	float pitchRad = radians(Pitch);

	vec3 newF;
	newF.x = cos(yawRad) * cos(pitchRad);
	newF.y = sin(pitchRad);
	newF.z = sin(yawRad) * cos(pitchRad);
	vec3 Front = normalize(newF); // The final camera direction vector

	
	UpdateCameraOrientation(Front);

	
	sf::Mouse::setPosition(sf::Vector2i(WIDTH / 2, HEIGHT / 2), window);

	
	lastMouseX = (float)WIDTH / 2.0f;
	lastMouseY = (float)HEIGHT / 2.0f;
}

int main()
{
	ContextSettings context;
	context.depthBits = 24;
	sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "SFML works!", sf::Style::Close, context);
	if (Init()) return 1;

	// Clock for delta time
	sf::Clock clock;

	while (window.isOpen())
	{
		Event event;

		// Get delta time
		float deltaTime = clock.restart().asSeconds();

		bool shouldClose = false;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
			case Event::Closed:
			{
				shouldClose = true;
				break;
			}
			case Event::KeyPressed:
			{
				if (event.key.code == Keyboard::Num1)
				{
					Current_Drawing_Mode = DrawingMode::Points;
				}
				if (event.key.code == Keyboard::Num2)
				{
					Current_Drawing_Mode = DrawingMode::Lines;
				}
				if (event.key.code == Keyboard::Num3)
				{
					Current_Drawing_Mode = DrawingMode::Filled;
				}

				// Set movement flags on key press
				if (event.key.code == Keyboard::W) {
					moveForward = true;
				}
				if (event.key.code == Keyboard::S) {
					moveBackward = true;
				}
				if (event.key.code == Keyboard::A) {
					moveLeft = true;
				}
				if (event.key.code == Keyboard::D) {
					moveRight = true;
				}

				if (event.key.code == Keyboard::Escape) {
					shouldClose = true;
					break;
				}
			}
			break;

			case Event::KeyReleased:
			{
				// Clear movement flags on key release
				if (event.key.code == Keyboard::W) {
					moveForward = false;
				}
				if (event.key.code == Keyboard::S) {
					moveBackward = false;
				}
				if (event.key.code == Keyboard::A) {
					moveLeft = false;
				}
				if (event.key.code == Keyboard::D) {
					moveRight = false;
				}
			}
			break;
			}
		}

		if (shouldClose) {
			Render( shouldClose); 
			window.close();
			break; // EXIT THE LOOP IMMEDIATELY
		}

		if (window.isOpen()) {
			ChangeCam(window, Yaw, Pitch, lastMouseX, lastMouseY, CameraPos, ViewMat_Location);
			Update(deltaTime);
			Render(shouldClose);
			window.display();
		}
	}

	return 0;
}