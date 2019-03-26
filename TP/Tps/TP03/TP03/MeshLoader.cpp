#include "stdafx.h"
#include "MeshLoader.h"
#include <fstream>
#include <sstream>
#include <map>
#include <functional>
#include <iostream>

using namespace std;

CMeshLoader::CMeshLoader()
  : m_nNbPositionComponents(0)
  , m_nNbNormalComponents(0)
  , m_nNbColorComponents(0)
  , m_nNbTexCoordComponents(0)
  , m_nPositionOffset(0)
  , m_nNormalOffset(0)
  , m_nColorOffset(0)
  , m_nTexCoordOffset(0)
  , m_ePrimitives(GL_POINTS)
{
}


CMeshLoader::~CMeshLoader()
{
}


bool CMeshLoader::loadFromFile(const std::string& strFileName)
{
  ifstream file{ strFileName };

  while(!file.eof())
  {
    string line;
    getline(file, line);
    if(line[0] == '#')
    {
      stringstream ss{ line };
      string strCommand;
      ss >> strCommand;
      if(strCommand.compare("#VBO") == 0)
        readVBO(file, readVBOHeader(ss));
      else if(strCommand.compare("#EBO") == 0)
        readEBO(file, readEBOHeader(ss));
    }
  }
  return true;
}

int CMeshLoader::readVBOHeader(stringstream& header)
{
  string strCommand;
  GLuint nCurOffset = 0;
  int nNbVertex = 0;
  m_nNbPositionComponents = 0;
  m_nNbNormalComponents = 0;
  m_nNbColorComponents = 0;
  m_nNbTexCoordComponents = 0;
  m_nPositionOffset = 0;
  m_nNormalOffset = 0;
  m_nColorOffset = 0;
  m_nTexCoordOffset = 0;
  while(!header.eof())
  {
    header >> strCommand;
    if(strCommand.compare("count") == 0)
      header >> nNbVertex;
    else if(strCommand.compare("position") == 0)
    {
      header >> m_nNbPositionComponents;
      m_nPositionOffset = nCurOffset;
      nCurOffset += m_nNbPositionComponents;
    }
    else if(strCommand.compare("normal") == 0)
    {
      header >> m_nNbNormalComponents;
      m_nNormalOffset = nCurOffset;
      nCurOffset += m_nNbNormalComponents;
    }
    else if(strCommand.compare("texcoord") == 0)
    {
      header >> m_nNbTexCoordComponents;
      m_nTexCoordOffset = nCurOffset;
      nCurOffset += m_nNbTexCoordComponents;
    }
    else if(strCommand.compare("color") == 0)
    {
      header >> m_nNbColorComponents;
      m_nColorOffset = nCurOffset;
      nCurOffset += m_nNbColorComponents;
    }
  }
  return nNbVertex;
}

int CMeshLoader::readEBOHeader(stringstream& header)
{
  m_ePrimitives = GL_POINTS;
  int nNbFaces = 0;
  string strCommand;
  while(!header.eof())
  {
    header >> strCommand;
    if(strCommand.compare("count") == 0)
      header >> nNbFaces;
    else if(strCommand.compare("triangle") == 0)
      m_ePrimitives = GL_TRIANGLES;
  }
  return nNbFaces;
}

void CMeshLoader::readVBO(std::ifstream& file, size_t nNbVertices)
{
  m_vVBO.clear();
  m_vVBO.reserve(nNbVertices * getStride());

  map < GLuint, GLuint > readers;
  readers[m_nColorOffset] = m_nNbColorComponents;
  readers[m_nNormalOffset] = m_nNbNormalComponents;
  readers[m_nPositionOffset] = m_nNbPositionComponents;
  readers[m_nTexCoordOffset] = m_nNbTexCoordComponents;

  for(auto elem : readers)
  {
    for(GLuint i = 0; i < elem.second; ++i)
    {
      GLfloat value;
      file >> value;
      m_vVBO.push_back(value);
      if(elem.first == m_nPositionOffset)
        m_BoundingBox.m_fmax[i] = m_BoundingBox.m_fmin[i] = value;
    }
  }
  for(size_t i = 1; i < nNbVertices; ++i)
  {
    for(auto elem : readers)
    {
      for(GLuint i = 0; i < elem.second; ++i)
      {
        GLfloat value;
        file >> value;
        m_vVBO.push_back(value);
        if(elem.first == m_nPositionOffset)
        {
          if(value > m_BoundingBox.m_fmax[i]) m_BoundingBox.m_fmax[i] = value;
          if(value < m_BoundingBox.m_fmin[i]) m_BoundingBox.m_fmin[i] = value;
        }
      }
    }
  }
}

void CMeshLoader::readEBO(std::ifstream& file, size_t nNbFaces)
{
  size_t nNbValues = 0;
  switch(m_ePrimitives)
  {
  case GL_TRIANGLES:
    nNbValues = nNbFaces * 3;
    break;
  default:
    cerr << "Non implémenté ! " __FILE__ "(" << __LINE__ << ")" << endl;
    break;
  }

  m_vEBO.clear();
  m_vEBO.reserve(nNbValues);

  for(size_t i = 0; i < nNbValues; ++i)
  {
    GLuint value;
    file >> value;
    m_vEBO.push_back(value);
  }
}

void CMeshLoader::getBoundingBox(GLfloat& fXmin, GLfloat& fXmax, GLfloat& fYmin, GLfloat& fYmax, GLfloat& fZmin, GLfloat& fZmax) const
{
  fXmin = m_BoundingBox.m_fmin[0];
  fYmin = m_BoundingBox.m_fmin[1];
  fZmin = m_BoundingBox.m_fmin[2];
  fXmax = m_BoundingBox.m_fmax[0];
  fYmax = m_BoundingBox.m_fmax[1];
  fZmax = m_BoundingBox.m_fmax[2];
}
