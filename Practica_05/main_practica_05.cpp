/*---------------------------------------------------------*/
/* ----------------   Práctica 5 --------------------------*/
/*-----------------    2019-2   ---------------------------*/
/*------------- Computación gráfica e interacción humano computadora ---------------*/

#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void display(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
movY = 0.0f,
movZ = -5.0f,
rotX = 0.0f,
rotY = 0.0f,
rotZ = 0.0f;
//Brazos
float
angHom = 0.0f,
angCod = 0.0f,
angMun = 0.0f,
angPul = 0.0f,
angInd = 0.0f,
angMed = 0.0f,
angAnu = 0.0f,
angMen = 0.0f;

float
angHom_R = 0.0f,
angCod_R = 0.0f,
angMun_R = 0.0f,
angPul_R = 0.0f,
angInd_R = 0.0f,
angMed_R = 0.0f,
angAnu_R = 0.0f,
angMen_R = 0.0f;


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat vertices[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V0 - Frontal
		0.5f, -0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V1
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V5
		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 0.0f,	//V4

		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 0.0f,	//V3
		-0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V7
		0.5f, 0.5f, -0.5f,		1.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V4 - Izq
		-0.5f, 0.5f, -0.5f,		0.0f, 0.0f, 1.0f,	//V7
		-0.5f, -0.5f, -0.5f,	0.0f, 0.0f, 1.0f,	//V3
		-0.5f, -0.5f, 0.5f,		0.0f, 0.0f, 1.0f,	//V0

		0.5f, 0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V5 - Der
		0.5f, -0.5f, 0.5f,		0.0f, 1.0f, 0.0f,	//V1
		0.5f, -0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V2
		0.5f, 0.5f, -0.5f,		0.0f, 1.0f, 0.0f,	//V6

		-0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V4 - Sup
		0.5f, 0.5f, 0.5f,		1.0f, 0.0f, 1.0f,	//V5
		0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V6
		-0.5f, 0.5f, -0.5f,		1.0f, 0.0f, 1.0f,	//V7

		-0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V0 - Inf
		-0.5f, -0.5f, -0.5f,	1.0f, 1.0f, 1.0f,	//V3
		0.5f, -0.5f, -0.5f,		1.0f, 1.0f, 1.0f,	//V2
		0.5f, -0.5f, 0.5f,		1.0f, 1.0f, 1.0f,	//V1
	};

	unsigned int indices[] =	//I am not using index for this session
	{
		0
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	//glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void display(void)
{
	//Shader myShader("shaders/shader.vs", "shaders/shader.fs");
	Shader projectionShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");

	projectionShader.use();

	// create transformations and Projection
	glm::mat4 modelTemp;
	modelTemp = glm::mat4(1.0f); //Temp
	glm::mat4 modelTemp2;
	modelTemp2 = glm::mat4(1.0f); //Temp 
	glm::mat4 model = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
	glm::mat4 view = glm::mat4(1.0f);		//Use this matrix for ALL models
	glm::mat4 projection = glm::mat4(1.0f);	//This matrix is for Projection


											//Use "projection" in order to change how we see the information
	projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

	//Use "view" in order to affect all models
	view = glm::translate(view, glm::vec3(movX, movY, movZ));
	view = glm::rotate(view, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
	view = glm::rotate(view, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
	// pass them to the shaders
	projectionShader.setMat4("model", model);
	projectionShader.setMat4("view", view);
	// note: currently we set the projection matrix each frame, but since the projection matrix rarely changes it's often best practice to set it outside the main loop only once.
	projectionShader.setMat4("projection", projection);


	glBindVertexArray(VAO);

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.5, 0));
	model = glm::scale(model, glm::vec3(1.5, 2.5, 1));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
	projectionShader.setMat4("model", model);
	glDrawArrays(GL_QUADS, 0, 24); //Head

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -1, 0));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 1));
	projectionShader.setVec3("aColor", glm::vec3(0.9f, 0.5f, 0.5f));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24); //Neacle

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -3.75, 0));
	model = glm::scale(model, glm::vec3(4, 5, 1));
	projectionShader.setVec3("aColor", glm::vec3(0.0f, 0.0f, 1.0f));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24); //Body


	//Brazo Izquierdo

	//Hombro
	model = modelTemp;	//Resetear la posicion y escala

	model = glm::translate(model, glm::vec3(2.0, -1.75, 0));
	model = glm::rotate(model, glm::radians(angHom), glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(0.5, 0.0, 0));
	modelTemp = model;
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	//Bicep
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(2, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;	//Resetear la posicion y escala

	//Antebrazo
	model = glm::translate(model, glm::vec3(1, 0, 0));
	model = glm::rotate(model, glm::radians(angCod), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;	//Resetear la posicion y escala

	//Palma
	model = glm::translate(model, glm::vec3(1.5, 0, 0));
	model = glm::rotate(model, glm::radians(angMun), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.5, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(1, 1, 0.5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;
	modelTemp2 = model;

	//Pulgar
	model = glm::translate(model, glm::vec3(-0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(angPul), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(angPul*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Indice

	model = glm::translate(model, glm::vec3(0.5, 0.375, 0));
	model = glm::rotate(model, glm::radians(angInd), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angInd*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angInd*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Medio

	model = glm::translate(model, glm::vec3(0.5, 0.125, 0));
	model = glm::rotate(model, glm::radians(angMed), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.175, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.175, 0, 0));
	model = glm::rotate(model, glm::radians(angMed*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.175, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.175, 0, 0));
	model = glm::rotate(model, glm::radians(angMed*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Anular

	model = glm::translate(model, glm::vec3(0.5, -0.125, 0));
	model = glm::rotate(model, glm::radians(angAnu), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angAnu*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.175, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.175, 0, 0));
	model = glm::rotate(model, glm::radians(angAnu*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Meñique

	model = glm::translate(model, glm::vec3(0.5, -0.375, 0));
	model = glm::rotate(model, glm::radians(angMen), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.125, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.125, 0, 0));
	model = glm::rotate(model, glm::radians(angMen*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.125, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0.125, 0, 0));
	model = glm::rotate(model, glm::radians(angMen*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(0.125, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


//Brazo Izquierdo

	//Hombro
	modelTemp = glm::mat4(1.0f); //Temp
	model = modelTemp;	//Resetear la posicion y escala

	model = glm::translate(model, glm::vec3(-2.0, -1.75, 0));
	model = glm::rotate(model, glm::radians(angHom_R), glm::vec3(0, 0, 1));
	model = glm::translate(model, glm::vec3(-0.5, 0.0, 0));
	modelTemp = model;
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	//Bicep
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(2, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 0, 1));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;	//Resetear la posicion y escala

	//Antebrazo
	model = glm::translate(model, glm::vec3(-1, 0, 0));
	model = glm::rotate(model, glm::radians(angCod_R), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(3, 1, 1));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;	//Resetear la posicion y escala

	//Palma
	model = glm::translate(model, glm::vec3(-1.5, 0, 0));
	model = glm::rotate(model, glm::radians(angMun_R), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.5, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(1, 1, 0.5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(0, 1, 0));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp;
	modelTemp2 = model;

	//Pulgar
	model = glm::translate(model, glm::vec3(0.35, 0.5, 0));
	model = glm::rotate(model, glm::radians(angPul_R), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	model = glm::rotate(model, glm::radians(angPul_R*0.7f), glm::vec3(1, 0, 0));
	model = glm::translate(model, glm::vec3(0, 0.15, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.3, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Indice

	model = glm::translate(model, glm::vec3(-0.5, 0.375, 0));
	model = glm::rotate(model, glm::radians(angInd_R), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angInd_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angInd_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Medio

	model = glm::translate(model, glm::vec3(-0.5, 0.125, 0));
	model = glm::rotate(model, glm::radians(angMed_R), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.175, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.175, 0, 0));
	model = glm::rotate(model, glm::radians(angMed_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.175, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.175, 0, 0));
	model = glm::rotate(model, glm::radians(angMed_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Anular

	model = glm::translate(model, glm::vec3(-0.5, -0.125, 0));
	model = glm::rotate(model, glm::radians(angAnu_R), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	model = glm::rotate(model, glm::radians(angAnu_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.175, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.35, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.175, 0, 0));
	model = glm::rotate(model, glm::radians(angAnu_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.15, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.3, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.8, 0.8));
	glDrawArrays(GL_QUADS, 0, 24);

	model = modelTemp2;

	//Meñique

	model = glm::translate(model, glm::vec3(-0.5, -0.375, 0));
	model = glm::rotate(model, glm::radians(angMen_R), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.125, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.125, 0, 0));
	model = glm::rotate(model, glm::radians(angMen_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.125, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


	model = modelTemp;

	model = glm::translate(model, glm::vec3(-0.125, 0, 0));
	model = glm::rotate(model, glm::radians(angMen_R*0.7f), glm::vec3(0, 1, 0));
	model = glm::translate(model, glm::vec3(-0.125, 0, 0));
	modelTemp = model;
	model = glm::scale(model, glm::vec3(0.25, 0.25, .5));
	projectionShader.setMat4("model", model);
	projectionShader.setVec3("aColor", glm::vec3(1, 0.9, 0.9));
	glDrawArrays(GL_QUADS, 0, 24);


//Parte Baja

	model = glm::translate(glm::mat4(1.0f), glm::vec3(0, -7.25, 0));
	model = glm::scale(model, glm::vec3(4, 2, 1));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24); //Cint


	model = glm::translate(glm::mat4(1.0f), glm::vec3(1.5, -9.75, 0));
	model = glm::scale(model, glm::vec3(1, 3, 1));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24); //Right leg


	model = glm::translate(glm::mat4(1.0f), glm::vec3(-1.5, -9.75, 0));
	model = glm::scale(model, glm::vec3(1, 3, 1));
	projectionShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24); //Left leg

	model = glm::translate(glm::mat4(1.0f), glm::vec3(3, -11.75, 0));
	model = glm::scale(model, glm::vec3(4, 1, 1));
	projectionShader.setVec3("aColor", glm::vec3(0.7f, 0.4f, 0.3f));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24); //Right leg


	model = glm::translate(glm::mat4(1.0f), glm::vec3(-3, -11.75, 0));
	model = glm::scale(model, glm::vec3(4, 1, 1));
	projectionShader.setVec3("aColor", glm::vec3(0.7f, 0.4f, 0.3f));
	projectionShader.setMat4("model", model);

	glDrawArrays(GL_QUADS, 0, 24); //Left leg


	


	glBindVertexArray(0);

}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

														 // glfw window creation
														 // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 5", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwSetWindowPos(window, 0, 30);
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	// render loop
	// While the windows is not closed
	while (!glfwWindowShouldClose(window))
	{
		// input
		// -----
		my_input(window);

		// render
		// Backgound color
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi función de dibujo
		display();

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;

	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;

	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 0.18f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 0.18f;

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 0.2f;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 0.2f;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		rotZ -= 0.2f;

	if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		rotZ += 0.2f;
/*-----------------------Movimiento del brazo Izquierdo---------------------*/

	//Hombro
	if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angHom < 90 ? angHom += 0.8f : NULL;
		else angHom > -90 ? angHom -= 0.8f : NULL;

	//Codo
	if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angCod < 0 ? angCod += 0.8f : NULL;
		else angCod > -140 ? angCod -= 0.8f : NULL;

	//Muñeca
	if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angMun < 20 ? angMun += 0.8f : NULL;
		else angMun > -40 ? angMun -= 0.8f : NULL;

	//Pulgar
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angPul > 0 ? angPul -= 0.8f : NULL; 
		else angPul < 70 ? angPul += 0.8f : NULL;

	//Indice
	if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angInd < 0 ? angInd += 0.8f : NULL;
		else angInd > -70 ? angInd -= 0.8f : NULL;

	//Medio
	if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angMed < 0 ? angMed += 0.8f : NULL;
		else angMed > -70 ? angMed -= 0.8f : NULL;

	//Anular
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angAnu < 0 ? angAnu += 0.8f : NULL;
		else angAnu > -70 ? angAnu -= 0.8f : NULL;

	//Meñique
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angMen < 0 ? angMen += 0.8f : NULL;
		else angMen > -70 ? angMen -= 0.8f : NULL;


	/*-----------------------Movimiento del brazo Derecho---------------------*/

		//Hombro
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angHom_R > -90 ? angHom_R -= 0.8f : NULL;
		else angHom_R < 90 ? angHom_R += 0.8f : NULL;

	//Codo
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angCod_R > 0 ? angCod_R -= 0.8f : NULL;
		else angCod_R < 140 ? angCod_R += 0.8f : NULL;

	//Muñeca
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angMun_R > -20 ? angMun_R -= 0.8f : NULL;
		else angMun_R < 40 ? angMun_R += 0.8f : NULL;

	//Pulgar
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angPul_R > 0 ? angPul_R -= 0.8f : NULL;
		else angPul_R < 70 ? angPul_R += 0.8f : NULL;

	//Indice
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angInd_R > 0 ? angInd_R -= 0.8f : NULL;
		else angInd_R < 70 ? angInd_R += 0.8f : NULL;

	//Medio
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angMed_R > 0 ? angMed_R -= 0.8f : NULL;
		else angMed_R < 70 ? angMed_R += 0.8f : NULL;

	//Anular
	if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angAnu_R > 0 ? angAnu_R -= 0.8f : NULL;
		else angAnu_R < 70 ? angAnu_R += 0.8f : NULL;

	//Meñique
	if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)

		if (glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS)
			angMen_R > 0 ? angMen_R -= 0.8f : NULL;
		else angMen_R < 70 ? angMen_R += 0.8f : NULL;

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
	// Set the Viewport to the size of the created window
	glViewport(0, 0, width, height);
}
