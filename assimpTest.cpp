#include <files.hpp>
#include <model.hpp>
#include <cam.hpp>

glm::vec3 lightPos2(0.0f, 0.0f, 0.0f);
const u32 SCR_WIDTH  = 1280;
const u32 SCR_HEIGHT = 720;
const f32 ASPECT     = (f32)SCR_WIDTH / (f32)SCR_HEIGHT;

Cam* cam;

f32  deltaTime  = 100.0f;
f32  lastFrame  = 0.0f;
bool wireframe  = false;

void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		cam->processKeyboard(FORWARD, 0.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		cam->processKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		cam->processKeyboard(BACKWARD, 0.1f);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		cam->processKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		lightPos2.z += 0.2f;
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		lightPos2.z -= 0.2f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		lightPos2.x += 0.09f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		lightPos2.x -= 0.09f;
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		lightPos2.y += 0.09f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		lightPos2.y -= 0.09f;
	}
}
void key_callback(GLFWwindow*, int key, int, int act, int) {
	wireframe ^= key == GLFW_KEY_E && act == GLFW_PRESS;
}
void mouse_callback(GLFWwindow* window, f64 xpos, f64 ypos) {
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
		cam->movePov(xpos, ypos);
	} else {
		cam->stopPov();
	}
}
void scroll_callback(GLFWwindow*, f64, f64 yoffset) {
	cam->processScroll((f32)yoffset);
}

i32 main() {
	GLFWwindow* window = glutilInit(3, 3, SCR_WIDTH, SCR_HEIGHT, "Macaco");
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	cam = new Cam();

	Files* files = new Files("bin", "resources/textures", "resources/objects");

	Shader* shader = new Shader(files, "shader.vert", "shader.frag");
	Model*  prota = new Model(files, "prota/Sun.obj");
	Model* creeper = new Model(files, "creeper/creeper.obj");
	u32 texture0 = TextureFromFile("resources/textures/rubik.jpg");
	u32 texture1 = TextureFromFile("resources/textures/AA1.jpg");
	glm::vec3 lightPos   = glm::vec3(1.0f);
	glm::vec3 lightColor = glm::vec3(1.0f);
	glEnable(GL_DEPTH_TEST);
	i32 n = 5;
	std::vector<glm::vec3> posiciones(n * n * n);
	for (i32 i = 0; i < n; i++)
	{
		for (i32 j = 0; j < n; j++)
		{
			for (i32 k = 0; k < n; k++)
			{
				f32 x = (rand() % 100 / 100.0f) * 75.0f - 50.0f;
				f32 y = (rand() % 100 / 100.0f) * 75.0f - 50.0f;
				f32 z = (rand() % 100 / 100.0f) * 75.0f - 50.0f;
				posiciones[i * n * n + j * n + k] = glm::vec3(x, y, z);
			}
		}

	}
	while (!glfwWindowShouldClose(window)) {
		f32 currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glPolygonMode(GL_FRONT_AND_BACK, wireframe? GL_LINE: GL_FILL);

		shader->use();
		lightPos.x = 2.0f*(cos(currentFrame) - sin(currentFrame));
		lightPos.z = 2.0f*(cos(currentFrame) + sin(currentFrame));
		shader->setVec3("xyz", lightPos);
		shader->setVec3("xyzColor", lightColor);
		shader->setVec3("xyzView", cam->pos);
		glm::mat4 proj = glm::perspective(cam->zoom, ASPECT, 0.1f, 100.0f);
		shader->setMat4("proj", proj);
		shader->setMat4("view", cam->getViewM4());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture0);
		glm::mat4 model = glm::mat4(1.0f);
		model = translate(model, lightPos2);
		model = glm::scale(model, glm::vec3(0.5f));
		shader->setMat4("model", model);
		prota->Draw(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glm::mat4 model1 = glm::mat4(1.0f);
		for (i32 i = 0; i < posiciones.size() / 2; i++) {
			model1 = glm::mat4(1.0f);
			model1 = translate(model1,posiciones[i]);
			model1 = glm::scale(model1, glm::vec3(0.5f));
			shader->setMat4("model", model1);
			creeper->Draw(shader);
		}
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	delete cam;
	delete shader;
	delete prota;

	return 0;
}


