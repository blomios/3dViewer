#pragma once
#include <GL/glew.h>
#include <string>

class CTextureUnit
{
public:
  CTextureUnit(CTextureUnit&& texunit);
  CTextureUnit& operator=(CTextureUnit&& texunit);
  CTextureUnit(GLuint nUnitId);
  ~CTextureUnit();
private:
  GLuint m_nTextureId;
  GLuint m_nUnitId;

  //Objet non copiable
  CTextureUnit(const CTextureUnit&) = delete;
  CTextureUnit& operator=(const CTextureUnit&) = delete;

public:

  // Charge la texture depuis un fichier image
  bool loadTexture(const std::string& strFileName);
  // définit un paramètre de texture
  void setParam(GLenum pname, GLfloat param);
  void setParam(GLenum pname, GLint param);
  void setParam(GLenum pname, const GLfloat * param);
  void setParam(GLenum pname, const GLint * param);

  bool associateToUniform(GLuint programId, const std::string& strUniform);

  void bind();
};

