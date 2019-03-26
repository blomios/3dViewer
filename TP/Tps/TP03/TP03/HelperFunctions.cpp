#include "stdafx.h"
#include "helperFunctions.h"
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <iostream>
#ifndef WIN32
#include <fstream>
#include <sstream>
#endif

using namespace std;

void PrintRendererInfo()
{
	const char* pText;

	pText = (const char*)glGetString(GL_VENDOR);
	if (!pText) pText = GetGLErrorString();
	cout << "Company : " << pText << endl;

	pText = (const char*)glGetString(GL_RENDERER);
	if (!pText) pText = GetGLErrorString();
	cout << "Renderer : " << pText << endl;

	pText = (const char*)glGetString(GL_VERSION);
	if (!pText) pText = GetGLErrorString();
	cout << "Version : " << pText << endl;
}

const char* GetGLErrorString()
{
	switch (glGetError())
	{
	case GL_NO_ERROR:
		return "GL_NO_ERROR";
	case GL_INVALID_ENUM:
		return "GL_INVALID_ENUM";
	case GL_INVALID_VALUE:
		return "GL_INVALID_VALUE";
	case GL_INVALID_OPERATION:
		return "GL_INVALID_OPERATION";
	case GL_STACK_OVERFLOW:
		return "GL_STACK_OVERFLOW";
	case GL_STACK_UNDERFLOW:
		return "GL_STACK_UNDERFLOW";
	case GL_OUT_OF_MEMORY:
		return "GL_OUT_OF_MEMORY";
		//case GL_INVALID_FRAMEBUFFER_OPERATION?:
		//	return "GL_INVALID_FRAMEBUFFER_OPERATION???";
		//case GL_TABLE_TOO_LARGE?:
		//	return "GL_TABLE_TOO_LARGE???";
	}
	return "Erreur inconnue";
}

void terminate(const string& strErr)
{
	cerr << strErr << endl;
	glfwTerminate();
	exit(EXIT_FAILURE);
}

#ifdef WIN32
string GetShaderSourceFromRessource(const wstring& filename)
{
	HRSRC hrSrc = FindResource(NULL, filename.c_str(), _T("SHADERS"));
	HGLOBAL hRes = LoadResource(NULL, hrSrc);
	DWORD szRes = SizeofResource(NULL, hrSrc);
	return string{ (char*)LockResource(hRes), szRes };
}
#else
string GetShaderSourceFromRessource(const string& filename)
{
	ifstream shaderFile{ filename };
	if (!shaderFile.is_open())
		return string{};
	stringstream shaderStream;
	shaderStream << shaderFile.rdbuf();
	return shaderStream.str();
}
#endif