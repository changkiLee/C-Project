#include "pipelineProgram.h"

class AdvancePipelineProgram : public PipelineProgram
{
public:
	int Init(const char * shaderBasePath); // init the program, "shaderBasePath" is the path to the folder containing the shaders
	void SetModelViewMatrix(const float * m); // m is column-major
	void SetProjectionMatrix(const float * m); // m is column-major
	void SetNormalMatrix(const float *m);
	void SetLightAmbient(const float * amb);
	void SetLightDiffuse(const float * diff);
	void SetLightSpecular(const float * spec);
	void SetViewLightDirection(const float * view);
	void SetMaterialAmbient(const float * amb);
	void SetMaterialDiffuse(const float * diff);
	void SetMaterialSpecular(const float * spec);
	void SetMaterialExponent(const float exp);
	void SetTextureUnit(const GLint unit);

protected:
	virtual int SetShaderVariableHandles();

	GLint h_projectionMatrix; // handle to the projectionMatrix variable in the shader
	GLint h_modelViewMatrix; // handle to the modelViewMatrix variable in the shader
							 // note: we can use the general syntax: h_name is a handle to the shader variable "name"
	GLint h_normalMatrix;
	GLint h_lightAmbient;
	GLint h_lightDiffuse;
	GLint h_lightSpecular;
	GLint h_viewLightDirection;
	GLint 	h_matKa;
	GLint h_matKd;
	GLint h_matKs;
	GLint h_matKsExp;
	GLint h_textureImage;
};