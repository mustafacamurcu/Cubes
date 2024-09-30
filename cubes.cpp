#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);


float yaw = -90.0f;
float pitch = 0.0f;

bool cameraLightOn = false;
bool specLightOn = true;
bool perspective = true;

const float cameraSpeed = 0.05f;
const float turnSpeed = 0.5f;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		perspective = !perspective;
	}
	if (key == GLFW_KEY_C && action == GLFW_PRESS)
	{
		cameraLightOn = !cameraLightOn;
	}
	if (key == GLFW_KEY_L && action == GLFW_PRESS)
	{
		specLightOn = !specLightOn;
	}
}

void processInput(GLFWwindow* window) {
	// Quit
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	// Arrow Keys to Look Around
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		pitch += turnSpeed;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		pitch -= turnSpeed;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		yaw -= turnSpeed;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		yaw += turnSpeed;

	glm::vec3 direction;
	direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	direction.y = sin(glm::radians(pitch));
	direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(direction);


	// WASD to move on X-Z plane
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		cameraPos += cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		cameraPos -= cameraSpeed * cameraFront;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	
	// Left-Shift / Tab to move vertically
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		cameraPos -= cameraUp * cameraSpeed;
	if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS)
		cameraPos += cameraUp * cameraSpeed;

}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
}

unsigned int create_triangles() {
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	/*
	// Triangle Strip
	float cube_vertices[24];
	for (int x = 0; x < 2; x++) {
		for (int y = 0; y < 2; y++) {
			for (int z = 0; z < 2; z++) {
				cube_vertices[(x * 4 + y * 2 + z * 1) * 3 + 0] = x - 0.5f;
				cube_vertices[(x * 4 + y * 2 + z * 1) * 3 + 1] = y - 0.5f;
				cube_vertices[(x * 4 + y * 2 + z * 1) * 3 + 2] = z - 0.5f;
			}
		}
	}

	unsigned int strip[] = { 3,2,7,6,4,2,0,3,1,7,5,4,1,0 };

	float triangle_strip_vertices[sizeof(strip) * 3];
	for (int i = 0; i < sizeof(strip) / sizeof(strip[0]); i++) {
		triangle_strip_vertices[i * 6 + 0] = cube_vertices[strip[i] * 6 + 0];
		triangle_strip_vertices[i * 6 + 1] = cube_vertices[strip[i] * 6 + 1];
		triangle_strip_vertices[i * 6 + 2] = cube_vertices[strip[i] * 6 + 2];
	}*/

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	return vbo;
}

unsigned int create_cube(unsigned int vbo) {
	unsigned int cube_vao;
	glGenVertexArrays(1, &cube_vao);
	glBindVertexArray(cube_vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal vector
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	return cube_vao;
}

unsigned int create_light(unsigned int vbo) {
	unsigned int light_vao;
	glGenVertexArrays(1, &light_vao);
	glBindVertexArray(light_vao);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	return light_vao;
}

int main() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetKeyCallback(window, key_callback);


	Shader lighting_shader("C:\\Users\\musta\\source\\repos\\OPENGL\\lighting_vshader.glsl",
		"C:\\Users\\musta\\source\\repos\\OPENGL\\lighting_fshader.glsl");
	Shader light_source_shader("C:\\Users\\musta\\source\\repos\\OPENGL\\light_source_vshader.glsl",
		"C:\\Users\\musta\\source\\repos\\OPENGL\\light_source_fshader.glsl");

	unsigned int vbo = create_triangles();
	unsigned int cube = create_cube(vbo);
	unsigned int light = create_light(vbo);

	glm::vec3 cubePositions[] = {
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	float cubeColors[10][3];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 3; j++) {
			cubeColors[i][j] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		}
	}

	glm::vec3 lightPos(1.2f, 3.0f, 0.0f);

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

		glm::mat4 projection;
		if (perspective) {
			projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.f, 0.1f, 100.0f);
		}
		else {
			projection = glm::ortho(-4.0f, 4.0f, -3.0f, 3.0f, 0.1f, 100.0f);
		}
		lighting_shader.use();
		lighting_shader.setMat4("view", view);
		lighting_shader.setMat4("projection", projection);
		lighting_shader.setFloat3("lightColor", 1.0f, 1.0f, 1.0f);
		lighting_shader.setFloat3("lightPos", lightPos.x, lightPos.y, lightPos.z);
		lighting_shader.setFloat3("cameraLightColor", 0.3f, 0.3f, 0.3f);
		lighting_shader.setFloat3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);
		lighting_shader.setBool("cameraLightOn", cameraLightOn);
		lighting_shader.setBool("specLightOn", specLightOn);
		
		glBindVertexArray(cube);

		for (int i = 0; i < sizeof(cubePositions) / sizeof(cubePositions[0]); i++) {
			lighting_shader.setFloat3("objectColor", cubeColors[i][0], cubeColors[i][1], cubeColors[i][2]);
			glm::mat4 model = glm::mat4(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angleDelta = 20.0f * i;
			model = glm::rotate(model, glm::radians(angleDelta), glm::vec3(1.0f, 0.3f, 0.5f));
			lighting_shader.setMat4("model", model);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		glBindVertexArray(light);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, lightPos);
		model = glm::scale(model, glm::vec3(0.2f));
		light_source_shader.use();
		light_source_shader.setMat4("view", view);
		light_source_shader.setMat4("projection", projection);
		light_source_shader.setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		processInput(window);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	return 0;
}

