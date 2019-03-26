#pragma once
#pragma once
#include <GL/glew.h>
#include <string>
#include <map>
#include "TextureUnit.h"

class CTexture
{
private:
  //Objet non copiable
  CTexture(const CTexture&) = delete;
  CTexture& operator=(const CTexture&) = delete;

  std::map<GLuint, CTextureUnit> m_vpTextureUnits;

  void deleteAllUnits();

  template<typename T>
  void setParamHelper(GLenum pname, T param);

  template<typename T>
  void setParamHelper(GLuint nTextureUnit, GLenum pname, T param);

public:
  //Objet déplacable
  CTexture(CTexture&& tex);
  CTexture& operator=(CTexture&& tex);
  CTexture();
  CTexture(const std::string& strFileName);
  ~CTexture();

  //Charge une simple texture (avec comme seule unité 0)
  bool loadTexture(const std::string& strFileName);
  //Charge une texture multiple (pour une unité particulière)
  bool loadTexture(const std::string& strFileName, GLuint nTextureUnit, GLuint programId = 0, const std::string& strUniform = "");
  //Associe une unité à un uniform pour un programme
  bool associateToUniform(GLuint nTextureUnit, GLuint programId, const std::string& strUniform);

  //Définit les paramètres de texture pour toutes les unités
  void setParam(GLenum pname, GLfloat param) { setParamHelper(pname, param); }
  void setParam(GLenum pname, GLint param) { setParamHelper(pname, param); }
  void setParam(GLenum pname, const GLfloat * param) { setParamHelper(pname, param); }
  void setParam(GLenum pname, const GLint * param) { setParamHelper(pname, param); }
  //Définit les paramètres de texture pour une unité en particulier
  void setParam(GLuint nTextureUnit, GLenum pname, GLfloat param) { setParamHelper(nTextureUnit, pname, param); }
  void setParam(GLuint nTextureUnit, GLenum pname, GLint param) { setParamHelper(nTextureUnit, pname, param); }
  void setParam(GLuint nTextureUnit, GLenum pname, const GLfloat * param) { setParamHelper(nTextureUnit, pname, param); }
  void setParam(GLuint nTextureUnit, GLenum pname, const GLint * param) { setParamHelper(nTextureUnit, pname, param); }

  void bind();
};

template<typename T>
void CTexture::setParamHelper(GLenum pname, T param)
{
  for(auto& pairK_TexUnit : m_vpTextureUnits)
    pairK_TexUnit.second.setParam(pname, param);
}

template<typename T>
void CTexture::setParamHelper(GLuint nTextureUnit, GLenum pname, T param)
{
  auto it = m_vpTextureUnits.find(nTextureUnit);
  if(it == m_vpTextureUnits.end())
    return;

  it->second.setParam(pname, param);
}
