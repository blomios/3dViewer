#pragma once
#include <stdexcept>
#include <string>


#ifndef _NOEXCEPT
#define _NOEXCEPT _GLIBCXX_USE_NOEXCEPT
#endif

class CShaderException :
  public std::exception
{
public:
  enum ExceptionType
  {
    TypeBadSourceStream,
    TypeCompilationError,
    TypePrepareLinkError,
    TypeLinkError
  };
private:
  ExceptionType m_eType;
  std::string m_strWhat;

public:
  CShaderException(const std::string& strWhat, ExceptionType eType) : m_strWhat(strWhat), m_eType(eType) {};
  const char* what() const _NOEXCEPT { return m_strWhat.c_str(); }
  ExceptionType type() const { return m_eType; }
};

