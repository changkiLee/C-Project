#include <iostream>
#include <cstring>
#include "openGLHeader.h"
#include "advancePipelineProgram.h"
using namespace std;

int AdvancePipelineProgram::Init(const char * shaderBasePath)
{
	if (BuildShadersFromFiles(shaderBasePath, "advance.vertexShader.glsl", "advance.fragmentShader.glsl") != 0)
	{
		cout << "Failed to build the pipeline program." << endl;
		return 1;
	}

	cout << "Successfully built the pipeline program." << endl;
	return 0;
}

void AdvancePipelineProgram::SetModelViewMatrix(const float * m)
{
	// pass "m" to the pipeline program, as the modelview matrix
	const GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_modelViewMatrix, 1, isRowMajor, m);
}

void AdvancePipelineProgram::SetProjectionMatrix(const float * m)
{
	// pass "m" to the pipeline program, as the projection matrix
	const GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_projectionMatrix, 1, isRowMajor, m);
}

void AdvancePipelineProgram::SetNormalMatrix(const float *m)
{
	// upload n to the GPU
	GLboolean isRowMajor = GL_FALSE;
	glUniformMatrix4fv(h_normalMatrix, 1, isRowMajor, m);
}

void AdvancePipelineProgram::SetViewLightDirection(const float * view)
{
	glUniform3fv(h_viewLightDirection, 1, view);
}
void AdvancePipelineProgram::SetLightAmbient(const float * amb)
{
	glUniform4fv(h_lightAmbient, 1, amb);
}
void AdvancePipelineProgram::SetLightDiffuse(const float * diff)
{
	glUniform4fv(h_lightDiffuse, 1, diff);
}
void AdvancePipelineProgram::SetLightSpecular(const float * spec)
{
	glUniform4fv(h_lightSpecular, 1, spec);
}
void AdvancePipelineProgram::SetMaterialAmbient(const float * amb)
{
	glUniform4fv(h_matKa, 1, amb);
}
void AdvancePipelineProgram::SetMaterialDiffuse(const float * diff)
{
	glUniform4fv(h_matKd, 1, diff);
}
void AdvancePipelineProgram::SetMaterialSpecular(const float * spec)
{
	glUniform4fv(h_matKs, 1, spec);
}
void AdvancePipelineProgram::SetMaterialExponent(const float exp)
{
	glUniform1f(h_matKsExp, exp);
}

void AdvancePipelineProgram::SetTextureUnit(const GLint unit)
{
	// select the active texture unit
	glActiveTexture(unit);
	// deem the shader variable ¡°textureImage¡± to read from texture unit ¡°unit¡±
	glUniform1i(h_textureImage, unit - GL_TEXTURE0);
}

int AdvancePipelineProgram::SetShaderVariableHandles()
{
	// set h_modelViewMatrix and h_projectionMatrix
	h_modelViewMatrix = glGetUniformLocation(programHandle, "modelViewMatrix");
	h_projectionMatrix = glGetUniformLocation(programHandle, "projectionMatrix");
	// lighting
	h_normalMatrix = glGetUniformLocation(programHandle, "normalMatrix");
	h_lightAmbient = glGetUniformLocation(programHandle, "lightAmbient");
	h_lightDiffuse = glGetUniformLocation(programHandle, "lightDiffuse");
	h_lightSpecular = glGetUniformLocation(programHandle, "lightSpecular");
	h_viewLightDirection = glGetUniformLocation(programHandle, "viewLightDirection");
	h_matKa = glGetUniformLocation(programHandle, "matKa");
	h_matKd = glGetUniformLocation(programHandle, "matKd");
	h_matKs = glGetUniformLocation(programHandle, "matKs");
	h_matKsExp = glGetUniformLocation(programHandle, "matKsExp");

	// get a handle to the ¡°textureImage¡± shader variable
	h_textureImage = glGetUniformLocation(programHandle, "textureImage");

	return 0;
}

