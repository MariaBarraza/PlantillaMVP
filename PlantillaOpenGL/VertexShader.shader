#version 330 core
//La primera linea SIEMPRE es la version

//Atributos de entrada (vienen desde c++)
in vec4 posicion;
in vec4 color;

//Uniforms
uniform mat4 modelo;
uniform mat4 vista;
uniform mat4 proyeccion;


//Atributos de salida (Van hacia el fragment shader)
out vec4 fragmentColor;

//Funcion main
void main() {

	//Crear la matriz MVP
	//tiene orden para tener el resultado deseado
	mat4 MVP = proyeccion * vista * modelo;

	//Es la posicion de salida del vertice
	//Es del tipo vec4
	gl_Position = MVP * posicion;

	//Establecer valores de atributos de salida
	fragmentColor = color;
}