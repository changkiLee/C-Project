#include <iostream>
#include <cstring>
#include "openGLHeader.h"
#include "lightPipelineProgram.h"
using namespace std;

int LightPipelineProgram::Init(const char * shaderBasePath)
{
	if (BuildShadersFromFiles(shaderBasePath, "phong.vertexShader.glsl", "phong.fragmentShader.glsl") != 0)
	{
		cout << "Failed to build the pipeline program." << endl;
		return 1;
	}

	cout << "Successfully built the pipeline program." << endl;
	return 0;
}

void LightPipelineProgram::SetModelViewMatrix(const float * m)
{
	// pass "m" to the pipeline program, as the modelview matrix
	const GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_modelViewMatrix, 1, isRowMajor, m);
}

void LightPipelineProgram::SetProjectionMatrix(const float * m)
{
	// pass "m" to the pipeline program, as the projection matrix
	const GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_projectionMatrix, 1, isRowMajor, m);
}

void LightPipelineProgram::SetNormalMatrix(const float *m)
{
	// upload n to the GPU
	GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_normalMatrix, 1, isRowMajor, m);
}

void LightPipelineProgram::SetViewLightDirection(const float * vec)
{
	glUniform3fv(h_viewLightDirection, 1, vec);
}


int LightPipelineProgram::SetShaderVariableHandles()
{
	// set h_modelViewMatrix and h_projectionMatrix
	h_modelViewMatrix = glGetUniformLocation(programHandle, "modelViewMatrix");
	h_projectionMatrix = glGetUniformLocation(programHandle, "projectionMatrix");
	h_viewLightDirection = glGetUniformLocation(programHandle, "viewLightDirection");
	h_normalMatrix = glGetUniformLocation(programHandle, "normalMatrix");

	return 0;
}

