#include <iostream>
#include <cstring>
#include "openGLHeader.h"
#include "texturePipelineProgram.h"
using namespace std;

int TexturePipelineProgram::Init(const char * shaderBasePath)
{
	if (BuildShadersFromFiles(shaderBasePath, "texture.vertexShader.glsl", "texture.fragmentShader.glsl") != 0)
	{
		cout << "Failed to build the pipeline program." << endl;
		return 1;
	}

	cout << "Successfully built the pipeline program." << endl;
	return 0;
}

void TexturePipelineProgram::SetModelViewMatrix(const float * m)
{
	// pass "m" to the pipeline program, as the modelview matrix
	const GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_modelViewMatrix, 1, isRowMajor, m);
}

void TexturePipelineProgram::SetProjectionMatrix(const float * m)
{
	// pass "m" to the pipeline program, as the projection matrix
	const GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_projectionMatrix, 1, isRowMajor, m);
}

void TexturePipelineProgram::SetTextureUnit(const GLint unit)
{
	glActiveTexture(unit);
	glUniform1i(h_textureImage, unit - GL_TEXTURE0);
}

int TexturePipelineProgram::SetShaderVariableHandles()
{
	// set h_modelViewMatrix and h_projectionMatrix
	h_modelViewMatrix = glGetUniformLocation(programHandle, "modelViewMatrix");
	h_projectionMatrix = glGetUniformLocation(programHandle, "projectionMatrix");
	h_textureImage = glGetUniformLocation(programHandle, "textureImage");
	return 0;
}

