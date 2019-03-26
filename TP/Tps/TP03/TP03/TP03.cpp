#include "stdafx.h"
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include "Color.h"
#include "CBoolAndColor.h"
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <algorithm>
#include <ShaderProgram.h>
#include "HelperFunctions.h"
#include <FreeImagePlus.h>
#include "MeshLoader.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Texture.h"
using namespace std;

#ifdef WIN32
extern "C" _declspec(dllexport) unsigned long NvOptimusEnablement = 0x00000001;
#endif 
const GLfloat pi = 3.1415926535897932384626433832795f;


void sizefunct(GLFWwindow* pWnd, int width, int height);
void keyfunct(GLFWwindow* pWnd, int key, int scancode, int action, int mods);
void scrollfunct(GLFWwindow* pWnd, double xoffset, double yoffset);


struct WndData
{
	float m_fFovy = 45.0f;
	GLuint programId;
};

int _tmain(int argc, _TCHAR* argv[])
{

	const int nInitWndWidth = 800;
	const int nInitWndHeight = 600;
	const char* pText;
	//glfwInit(); //Fonction de la bibliothèque GLFW
	if (!glfwInit()) exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 16); //Multisample
	GLFWwindow* pWnd = glfwCreateWindow(800, 600, "Hello World", nullptr, nullptr);
	if (!pWnd) {
			cerr << "Impossible de créer la fenêtre !" << endl;
			glfwTerminate();
			exit(EXIT_FAILURE);
		}

	glfwMakeContextCurrent(pWnd);
	glEnable(GL_MULTISAMPLE);
	glewExperimental = GL_TRUE;
	glewInit(); //Fonction de la bibliothèque GLEW

	cout << glGetString(GL_VERSION); //Fonction de la bibliothèque OpenGL du système

	
	WndData wndData;
	glfwSetWindowUserPointer(pWnd, &wndData);

	glfwSetKeyCallback(pWnd, keyfunct);
	glfwSetScrollCallback(pWnd, scrollfunct);
	glfwSetWindowSizeCallback(pWnd, sizefunct);

	CMeshLoader mesh;
	mesh.loadFromFile("knot.mesh");

	

	CShaderProgram program{ CShader{ GL_VERTEX_SHADER, GetShaderSourceFromRessource(_T("vertex.vert")) },
							CShader{ GL_FRAGMENT_SHADER, GetShaderSourceFromRessource(_T("fragment.frag")) } };
	wndData.programId = program.GetProgramId();

	glm::vec3 meshPmin{ mesh.getXmin(), mesh.getYmin(), mesh.getZmin() };
	glm::vec3 meshPmax{ mesh.getXmax(), mesh.getYmax(), mesh.getZmax() };
	glm::vec3 meshCenter((meshPmax + meshPmin) / 2.0f);
	GLfloat meshSize = glm::distance(meshPmin, meshPmax);

	glm::mat4 ModelMatrix{ 1 };
	glm::mat4 ViewMatrix = glm::lookAt(meshCenter + glm::vec3{ 0.0f, 0.0f, 1 * meshSize }, meshCenter, glm::vec3{ 0.0f, 1.0f, 0.0f });
	sizefunct(pWnd, nInitWndWidth, nInitWndHeight);
	//récupère les positions des uniforms correspondant à ces matrices
	GLint nModelMatrix = glGetUniformLocation(program.GetProgramId(), "model");
	GLint nViewMatrix = glGetUniformLocation(program.GetProgramId(), "view");
	//transfère la matrice de visualisation (ne changera plus, la caméra est fixe)
	glUniformMatrix4fv(nViewMatrix, 1, GL_FALSE, glm::value_ptr(ViewMatrix));

	//Allocation des buffers
	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	GLuint VBO;
	glGenBuffers(1, &VBO);

	//Spécification des vertices pour le cercle
	// Le VAO
	glBindVertexArray(VAO);
	// Le VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, mesh.getVBOSize() * sizeof(GLfloat), mesh.getVBO(), GL_STATIC_DRAW);

	//spécifie l'organisation de l'entrée du vertex shader
	//Positions
	glVertexAttribPointer(0, mesh.getNbPositionComponents(), GL_FLOAT, GL_FALSE, mesh.getStride() * sizeof(GLfloat), reinterpret_cast<GLvoid*>(mesh.getPositionOffset() * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);
	//Coordonnées de textures
	glVertexAttribPointer(1, mesh.getNbTexCoordComponents(), GL_FLOAT, GL_FALSE, mesh.getStride() * sizeof(GLfloat), reinterpret_cast<GLvoid*>(mesh.getTexCoordOffset() * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Le EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.getEBOSize() * sizeof(GLuint), mesh.getEBO(), GL_STATIC_DRAW);
	
	CTexture texture;
	texture.loadTexture("MtlPlat2.jpg", 0, program.GetProgramId(),"material");
	texture.loadTexture("MtlPlat2-old.png", 1, program.GetProgramId(), "material_old");
	texture.setParam(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	texture.setParam(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	//GLuint nTexture;
	//glGenTextures(1, &nTexture);
	//{
	//	fipImage textureImg;
	//	textureImg.load("MtlPlat2.jpg");
	//	textureImg.convertTo32Bits();
	//	unsigned int widthPow2 = 1 << (unsigned int)ceil(log2((double)textureImg.getWidth()));
	//	unsigned int heightPow2 = 1 << (unsigned int)ceil(log2((double)textureImg.getHeight()));
	//	textureImg.rescale(widthPow2, heightPow2, FILTER_BICUBIC);

	//	glBindTexture(GL_TEXTURE_2D, nTexture);
	//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, textureImg.getWidth(), textureImg.getHeight(), 0, GL_BGRA, GL_UNSIGNED_BYTE, textureImg.accessPixels());
	//	glGenerateMipmap(GL_TEXTURE_2D);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//}

	glEnable(GL_DEPTH_TEST);

	glfwSetTime(0);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	while (!glfwWindowShouldClose(pWnd))
	{
		glfwPollEvents();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		program.Use();
		glm::mat4 rotation = glm::rotate((GLfloat)glfwGetTime(), glm::vec3{ 0.0f, 1.0f, 0.0f });
		glUniformMatrix4fv(nModelMatrix, 1, GL_FALSE, glm::value_ptr(ModelMatrix*rotation));
		glBindVertexArray(VAO);
		glDrawElements(mesh.getPrimitivesType(), mesh.getEBOSize(), GL_UNSIGNED_INT, reinterpret_cast<GLvoid*>(0));

		glfwSwapBuffers(pWnd);
	}
	glfwTerminate();

	return 0;
}

void keyfunct(GLFWwindow* pWnd, int key, int scancode, int action, int mods)
{
	if (action == GLFW_RELEASE)
		return;

	WndData& TheWndData = *reinterpret_cast<WndData*>(glfwGetWindowUserPointer(pWnd));

	switch (key)
	{
	case GLFW_KEY_W:
	{
		GLint modes[2];
		glGetIntegerv(GL_POLYGON_MODE, modes);
		if (modes[0] == GL_LINE)
			modes[0] = GL_FILL;
		else
			modes[0] = GL_LINE;
		glPolygonMode(GL_FRONT_AND_BACK, modes[0]);
		break;
	}
	case GLFW_KEY_T:
	{
		GLuint nUniform_bTexture = glGetUniformLocation(TheWndData.programId, "bTexture");
		GLuint bTexture;
		glGetUniformuiv(TheWndData.programId, nUniform_bTexture, &bTexture);
		glUseProgram(TheWndData.programId);
		glUniform1ui(nUniform_bTexture, !bTexture);
		break;
	}
	case GLFW_KEY_V:
	{
		GLuint nUniform_fAge = glGetUniformLocation(TheWndData.programId, "fAge");
		GLfloat fAge;
		glGetUniformfv(TheWndData.programId, nUniform_fAge, &fAge);
		if (mods & GLFW_MOD_SHIFT) //viellir
		{
			fAge += 0.1f;
			if (fAge > 1.0f)
				fAge = 1.0f;
		}
		else //rajeunir
		{
			fAge -= 0.1f;
			if (fAge < 0.0f)
				fAge = 0.0f;
		}
		glUseProgram(TheWndData.programId);
		glUniform1f(nUniform_fAge, fAge);
		break;
	}
	}
}

void scrollfunct(GLFWwindow* pWnd, double xoffset, double yoffset)
{
	WndData& TheWndData = *reinterpret_cast<WndData*>(glfwGetWindowUserPointer(pWnd));

	int width, height;
	glfwGetWindowSize(pWnd, &width, &height);
	TheWndData.m_fFovy += (float)yoffset;
	if (TheWndData.m_fFovy < 1.0f)TheWndData.m_fFovy = 1.0f;
	if (TheWndData.m_fFovy > 180.0f)TheWndData.m_fFovy = 180.0f;
	glm::mat4 projection = glm::perspective(glm::radians(TheWndData.m_fFovy), (float)width / (float)height, 1.0f, 1000.0f);

	GLint nProjectionMatrix = glGetUniformLocation(TheWndData.programId, "proj");
	glUseProgram(TheWndData.programId);
	glUniformMatrix4fv(nProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
}

void sizefunct(GLFWwindow* pWnd, int width, int height)
{
	if (width*height == 0)
		return;
	WndData &TheWndData = *(static_cast<WndData*>(glfwGetWindowUserPointer(pWnd)));

	glViewport(0, 0, width, height);

	glm::mat4 projection = glm::perspective(glm::radians(TheWndData.m_fFovy), (float)width / (float)height, 1.0f, 1000.0f);

	GLint nProjectionMatrix = glGetUniformLocation(TheWndData.programId, "proj");
	glUseProgram(TheWndData.programId);
	glUniformMatrix4fv(nProjectionMatrix, 1, GL_FALSE, glm::value_ptr(projection));
}

