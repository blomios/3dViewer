#pragma once
#ifdef WIN32
#include <Windows.h>
#include <tchar.h>
#else
#define _T(x) (x)
#endif
#include <string>

const char* GetGLErrorString();
void PrintRendererInfo();
void terminate(const std::string& strErr);

#ifdef WIN32
std::string GetShaderSourceFromRessource(const std::wstring& filename);
#else
std::string GetShaderSourceFromRessource(const std::string& filename);
#endif