#pragma once
#include <GL/glew.h>
#include <vector>
#include <string>

class CMeshLoader
{
public:
  CMeshLoader();
  ~CMeshLoader();
private:
  std::vector<GLfloat> m_vVBO;
  std::vector<GLuint> m_vEBO;
  int readVBOHeader(std::stringstream& header);
  int readEBOHeader(std::stringstream& header);
  void readVBO(std::ifstream& file, size_t nNbVertices);
  void readEBO(std::ifstream& file, size_t nNbFaces);
public:
  bool loadFromFile(const std::string& strFileName);
  const GLfloat* getVBO() const { return m_vVBO.data(); }
  const GLuint* getEBO() const { return m_vEBO.data(); }
  size_t getVBOSize() const { return m_vVBO.size(); }
  size_t getEBOSize() const { return m_vEBO.size(); }
private:
  GLuint m_nNbPositionComponents;
  GLuint m_nNbNormalComponents;
  GLuint m_nNbColorComponents;
  GLuint m_nNbTexCoordComponents;
  GLuint m_nPositionOffset;
  GLuint m_nNormalOffset;
  GLuint m_nColorOffset;
  GLuint m_nTexCoordOffset;
  GLenum m_ePrimitives;

  struct BoundingBox
  {
    GLfloat m_fmin[3];
    GLfloat m_fmax[3];
  } m_BoundingBox;

public:
  GLuint getNbPositionComponents() const { return m_nNbPositionComponents; }
  GLuint getNbNormalComponents() const { return m_nNbNormalComponents; }
  GLuint getNbColorComponents() const { return m_nNbColorComponents; }
  GLuint getNbTexCoordComponents() const { return m_nNbTexCoordComponents; }
  GLuint getPositionOffset() const { return m_nPositionOffset; }
  GLuint getNormalOffset() const { return m_nNormalOffset; }
  GLuint getColorOffset() const { return m_nColorOffset; }
  GLuint getTexCoordOffset() const { return m_nTexCoordOffset; }
  GLenum getPrimitivesType() const { return m_ePrimitives; }
  GLuint getStride() const { return m_nNbPositionComponents + m_nNbNormalComponents + m_nNbColorComponents + m_nNbTexCoordComponents; }
  void getBoundingBox(GLfloat& fXmin, GLfloat& fXmax, GLfloat& fYmin, GLfloat& fYmax, GLfloat& fZmin, GLfloat& fZmax) const;
  GLfloat getXmin() const { return m_BoundingBox.m_fmin[0]; }
  GLfloat getYmin() const { return m_BoundingBox.m_fmin[1]; }
  GLfloat getZmin() const { return m_BoundingBox.m_fmin[2]; }
  GLfloat getXmax() const { return m_BoundingBox.m_fmax[0]; }
  GLfloat getYmax() const { return m_BoundingBox.m_fmax[1]; }
  GLfloat getZmax() const { return m_BoundingBox.m_fmax[2]; }
};

