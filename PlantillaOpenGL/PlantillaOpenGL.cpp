// PlantillaOpenGL.cpp: define el punto de entrada de la aplicación de consola.

#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include <iostream>

#include "Shader.h"
#include "Vertice.h"

#include "Modelo.h"
#include "Carro.h"

#include "glm/glm.hpp"
#include "glm/gtx/transform.hpp"

using namespace std;

Shader *shader;
GLuint posicionID;
GLuint colorID;
GLuint modeloID;
GLuint vistaID;
GLuint proyeccionID;

mat4 vista;
mat4 proyeccion;

Carro *carro;

//Declaramos apuntador de ventana
GLFWwindow *window;

GLfloat ancho = 1024.0f;
GLfloat alto = 768.0f;

vec3 posicionCamara = vec3(3.0f, 1.5f, 7.0f);

void establecerVista() {
	//parametros: posicion camara, posicion objetivo,vector hacia arriba
	vista = lookAt(posicionCamara, vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
	if (carro != NULL) {
		carro->vista = vista;
	}
}

void establecerProyeccion() {
	//parametros: campo de vision(FoV,relacion de aspecto(Aspect Ratio),near clipping (desde donde renderea),far clipping (donde va a dejar de renderear)
	proyeccion = perspective(90.0f, ancho / alto, 0.1f, 150.0f);
}

void dibujar() {
	carro->dibujar(GL_QUADS);
}

void actualizar() {

	int estadoIzquierda = glfwGetKey(window, GLFW_KEY_LEFT);
	if (estadoIzquierda == GLFW_PRESS)
	{
		posicionCamara.x -= 0.01f;
		establecerVista();
	}

	int estadoDerecha = glfwGetKey(window, GLFW_KEY_RIGHT);
	if (estadoDerecha == GLFW_PRESS)
	{
		posicionCamara.x += 0.01f;
		establecerVista();
	}

	int estadoEspacio = glfwGetKey(window, GLFW_KEY_SPACE);
	if (estadoEspacio == GLFW_PRESS) {
		//carro->modelo = rotate(carro->modelo, 0.001f, vec3(0.0f, 1.0f, 0.0f));
		carro->avanzar();
	}
}

int main()
{
	
	
	//Si no se pudo iniciar glfw
	//terminamos ejcución
	if (!glfwInit()) {
		exit(EXIT_FAILURE);
	}
	//Si se pudo iniciar GLFW
	//entonces inicializamos la ventana
	window =
		glfwCreateWindow(1024, 768, "Ventana", 
			NULL, NULL);
	//Si no podemos iniciar la ventana
	//Entonces terminamos ejecucion
	if (!window) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	//Establecemos el contexto
	glfwMakeContextCurrent(window);

	//Una vez establecido  el contexto
	//Activamos funciones modernas
	glewExperimental = true;
	GLenum errorGlew = glewInit();
	if (errorGlew != GLEW_OK) {
		cout << 
			glewGetErrorString(errorGlew);
	}

	const GLubyte *versionGL =
		glGetString(GL_VERSION);
	cout << "Version OpenGL: "
		<< versionGL;

	const char *rutaVertex = 
		"VertexShader.shader";
	const char *rutaFragment = 
		"FragmentShader.shader";

	shader = new Shader(rutaVertex, rutaFragment);

	//Mapeo de atributos
	posicionID =
		glGetAttribLocation(shader->getID(), "posicion");
	colorID =
		glGetAttribLocation(shader->getID(), "color");
	modeloID =
		glGetUniformLocation(shader->getID(), "modelo");

	vistaID = glGetUniformLocation(shader->getID(), "vista");
	proyeccionID = glGetUniformLocation(shader->getID(), "proyeccion");

	//Desenlazar el shader
	shader->desenlazar();

	establecerVista();
	establecerProyeccion();

	//Inicializar modelo y Establecer shader al modelo
	carro = new Carro();
	carro->vista = vista;
	carro->proyeccion = proyeccion;
	carro->shader = shader;
	carro->inicializarVertexArray(posicionID, colorID, modeloID, vistaID, proyeccionID);

	//evaluar profundidad
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//Inicializar vertex array
	

	//Ciclo de dibujo (Draw loop)
	while (!glfwWindowShouldClose(window)) {
		//Esablecer region de dibujo
		glViewport(0, 0, 1024, 768);
		//Establece el color de borrado
		glClearColor(1, 0.2, 0.5, 1);
		//Borramos
		glClear(
			GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Rutina de dibujo
		dibujar();
		actualizar();

		//Cambiar los buffers
		glfwSwapBuffers(window);
		//Reconocer si hay entradas
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();

    return 0;
}

