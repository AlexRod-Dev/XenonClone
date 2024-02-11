﻿#include <iostream>
#include <glad/glad.h>
#include <SDL.h>
#include "stb_image.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>



glm::vec3 cameraPos = glm::vec3(0.0f, 1.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.5f, 0.0f);

float deltaTime = 0.0f;	// Time between current frame and last frame
float lastFrameTime = SDL_GetTicks(); // Time of last frame

float lastX = 400, lastY = 300;

float pitch = 0.f;
float yaw = -90.f;

float fov = 45.0f;

float screenWidth = 800;
float screenHeight = 600;


SDL_Window* window;

void processKeyboard(float deltaTime)
{

	float cameraSpeed = 5.0f * deltaTime; // adjust accordingly

	// Only update the X and Z components of cameraPos
	glm::vec3 movement(0.0f);

	const Uint8* keyState;
	keyState = SDL_GetKeyboardState(NULL);
	if (keyState[SDL_SCANCODE_W] || keyState[SDL_SCANCODE_UP])
		movement += cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_S] || keyState[SDL_SCANCODE_DOWN])
		movement -= cameraSpeed * cameraFront;
	if (keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_LEFT])
		movement -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_RIGHT])
		movement += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (keyState[SDL_SCANCODE_E])
		cameraPos += cameraSpeed * cameraUp;
	if (keyState[SDL_SCANCODE_Q])
		cameraPos -= cameraSpeed * cameraUp;

	// Exclude the Y component
	movement.y = 0.0f;

	// Apply smoothing/interpolation to camera position
	cameraPos += movement;


}

void processMouse(SDL_Event ev, float deltaTime)
{

	if (ev.type == SDL_MOUSEMOTION)
	{
		float xpos = ev.button.x;
		float ypos = ev.button.y;

		static bool firstMouse = true;
		if (firstMouse)
		{
			firstMouse = false;
			lastX = xpos;
			lastY = ypos;
		}

		float xoffset = xpos - lastX;
		float yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
		SDL_WarpMouseInWindow(window, 400, 300);
		lastX = 400; //xpos;
		lastY = 300; //ypos;

		float sensitivity = 0.05f;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;

		glm::vec3 front;
		front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
		front.y = sin(glm::radians(pitch));
		front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
		cameraFront = glm::normalize(front);
	}

	if (ev.type == SDL_MOUSEWHEEL)
	{
		if (fov >= 1.0f && fov <= 45.0f)
			fov -= ev.wheel.y;
		if (fov <= 1.0f)
			fov = 1.0f;
		if (fov >= 45.0f)
			fov = 45.0f;
	}
}


void load_obj(const char* filename, std::vector<glm::vec4>& vertices, std::vector<glm::vec3>& normals, std::vector<GLushort>& elements)
{
	std::ifstream in(filename, std::ios::in);
	if (!in.is_open())
	{
		std::cerr << "Error Opening file " << filename << std::endl;
		exit(1);
	}

	std::string line;
	while (getline(in, line))
	{
		if (line.substr(0, 2) == "v ")
		{
			std::istringstream s(line.substr(2));
			glm::vec4 v;
			s >> v.x;
			s >> v.y;
			s >> v.z;
			v.w = 1.0f;
			vertices.push_back(v);
		}
		else if (line.substr(0, 2) == "f ")
		{
			std::istringstream s(line.substr(2));
			GLushort a, b, c, ign;
			s >> a;
			s.ignore();
			s >> ign;
			s.ignore();
			s >> ign;

			s >> b;
			s.ignore();
			s >> ign;
			s.ignore();
			s >> ign;

			s >> c;
			a--;
			b--;
			c--;
			elements.push_back(a);
			elements.push_back(b);
			elements.push_back(c);
		}
		/* anything else is ignored */
	}

	normals.resize(vertices.size(), glm::vec3(0.0, 0.0, 0.0));
	for (int i = 0; i < elements.size(); i += 3)
	{
		GLushort ia = elements[i];
		GLushort ib = elements[i + 1];
		GLushort ic = elements[i + 2];
		if (ia <= vertices.size() && ib <= vertices.size() && ic <= vertices.size())
		{
			glm::vec3 normal = glm::normalize(glm::cross(glm::vec3(vertices[ib]) - glm::vec3(vertices[ia]), glm::vec3(vertices[ic]) - glm::vec3(vertices[ia])));
			normals[ia] = normals[ib] = normals[ic] = normal;
		}
		else
		{
			break;
		}
	}
}





int main(int argc, char **argv)
{

	int startFrameTime = SDL_GetTicks();

	SDL_Init(SDL_INIT_VIDEO);

	

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

	window = SDL_CreateWindow("Computer Graphics Project 1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);
	if (window == nullptr)
	{
		std::cout << "Failed to create SDL Window" << std::endl;
		SDL_Quit();
		return -1;
	}

	SDL_GLContext context = SDL_GL_CreateContext(window);


	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
		return -2;
	}

	SDL_SetRelativeMouseMode(SDL_TRUE);

	// Load the .obj file
	std::vector<glm::vec4> vertices;
	std::vector<glm::vec3> normals;
	std::vector<GLushort> elements;

	load_obj("suzanne.obj", vertices, normals, elements);



	GLuint vbo; // vertex buffer object
	glGenBuffers(1, &vbo); // Generate 1 buffer

	GLuint ebo;
	glGenBuffers(1, &ebo);	

	GLuint vao;
	glGenVertexArrays(1, &vao);

	// Set up the vertex array, vertex buffer, and element buffer
	glBindVertexArray(vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec4), vertices.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements.size() * sizeof(GLushort), elements.data(), GL_STATIC_DRAW);

	const char* vertexShaderSource = R"glsl(
		#version 330 core

		in vec3 position;
		in vec3 normal;
		in vec2 texCoord;

		out vec3 FragPos;      // FragPos in world coordinates
		out vec3 Normal;       // Normal in world coordinates
		out vec2 TexCoord;     // Output texture coordinates

		uniform mat4 model;
		uniform mat4 view;
		uniform mat4 projection;

		void main()
		{
			gl_Position = projection * view * model * vec4(position, 1.0);
			FragPos = vec3(model * vec4(position, 1.0));
			Normal = mat3(transpose(inverse(model))) * normal;  
			TexCoord = texCoord; // Pass texture coordinates to fragment shader
		}
		)glsl";


	// Vertex Shader

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	GLint  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Fragment Shader

	const char* fragmentShaderSource = R"glsl(
		#version 330 core

		in vec3 Color;
		in vec2 TexCoord;

		out vec4 outColor;

		uniform sampler2D ourTexture;
		uniform sampler2D ourTexture2;

		void main()
		{
			vec4 colTex1 = texture(ourTexture, TexCoord);
			vec4 colTex2 = texture(ourTexture2, TexCoord);
			outColor = mix(colTex1, colTex2, 0.5);
		})glsl";
	
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	GLuint shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (void*)0);
	glEnableVertexAttribArray(posAttrib);

	GLint normalAttrib = glGetAttribLocation(shaderProgram, "normal");
	glVertexAttribPointer(normalAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(normalAttrib);


	GLint texCoordAttrib = glGetAttribLocation(shaderProgram, "texCoord");
	glEnableVertexAttribArray(texCoordAttrib);
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));


	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


	// the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);

	int width, height, nrChannels;
	unsigned char* data = stbi_load("bricks.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);



	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);


	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	
	glUseProgram(shaderProgram);
	
	GLuint textureLocation;
	GLuint textureLocation2;

	textureLocation = glGetUniformLocation(shaderProgram, "ourTexture");
	textureLocation2 = glGetUniformLocation(shaderProgram, "ourTexture2");
	
	glUniform1i(textureLocation, 0);
	glUniform1i(textureLocation2, 1);


	glm::mat4 model(1.0f);
	model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0, 0.0, 0.0));

	glm::mat4 projection;
	projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);

	
	GLuint modelLocation = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

	GLuint viewLocation = glGetUniformLocation(shaderProgram, "view");
	
	GLuint projectionLocation = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

	glClearColor(0.2f, 0.5f, 0.3f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	


	int start = SDL_GetTicks();
	bool gameIsRunning = true;

	SDL_Event windowEvent;
	while (gameIsRunning)
	{

		int now = SDL_GetTicks();
		float time = (now - start) / 1000.0f;
		

		float deltaTime = (now - lastFrameTime) / 1000.0f;
		lastFrameTime = now;


		while (SDL_PollEvent(&windowEvent) != 0)
		{
			if (windowEvent.type == SDL_QUIT)
				gameIsRunning = false;

			
			processMouse(windowEvent, deltaTime);

			
		}
		processKeyboard(deltaTime);

		//glm::mat4 view;
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, glm::value_ptr(view));

		projection = glm::perspective(glm::radians(fov), screenWidth / screenHeight, 0.1f, 100.0f);
		glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, glm::value_ptr(projection));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(vao);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		glDrawElements(GL_TRIANGLES, elements.size(), GL_UNSIGNED_SHORT, 0);


		SDL_GL_SwapWindow(window);

		}

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}