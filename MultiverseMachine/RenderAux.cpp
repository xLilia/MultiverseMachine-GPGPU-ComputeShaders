#include "RenderAux.h"

RenderAux::RenderAux::RenderAux()
{
}

void RenderAux::RenderAux::RenderQuad(GLfloat x, GLfloat y, GLfloat w, GLfloat h, bool removeProgramAferUse, GLuint Shader) {
	if (Shader != 0)
		glUseProgram(Shader);
	glViewport(x, y, w, h);
	//REPLACE WITH VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	GLuint aScreenQuadTexCoords = 0;
	glEnableVertexAttribArray(aScreenQuadTexCoords);
	glVertexAttribPointer(aScreenQuadTexCoords, 2, GL_FLOAT, GL_FALSE, 0, (GLvoid*)ScreenQuad().fullquad_v);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, (GLvoid*)ScreenQuad().fullquad_i);
	glDisableVertexAttribArray(aScreenQuadTexCoords);

	if (removeProgramAferUse)
		glUseProgram(0);
	check_gl_error();
}

GLuint RenderAux::CreateTexture2D(GLfloat w, GLfloat h)
{
	GLuint TexID;
	glCreateTextures(GL_TEXTURE_2D, 1, &TexID);
	glBindTexture(GL_TEXTURE_2D, TexID);

	//2D TEX PROPERTIES
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(
		GL_TEXTURE_2D, 
		0, 
		GL_RGBA32F,
		w, 
		h, 
		0, 
		GL_RGBA,
		GL_FLOAT, 
		NULL
	);
	check_gl_error();
	return TexID;
}

void RenderAux::BindFramebuffer(GLuint Framebuffer)
{
	glBindFramebuffer(GL_FRAMEBUFFER, Framebuffer);
	check_gl_error();
}

GLuint RenderAux::CreateFramebuffer()
{

	GLuint FramebufferID;

	glCreateFramebuffers(1, &FramebufferID);
	BindFramebuffer(FramebufferID);
	check_gl_error();

	return FramebufferID;
}

RenderAux::Texture* RenderAux::GenerateTexture(std::string TextureOutputName, GLfloat Xpxu, GLfloat Ypxu)
{
	RenderAux::Texture* TexObj = new RenderAux::Texture;
	TexObj->ID = RenderAux::CreateTexture2D(Xpxu, Ypxu);
	TexObj->w = Xpxu;
	TexObj->h = Ypxu;
	TexObj->Name = TextureOutputName;
	return TexObj;
}

bool RenderAux::CheckFramebufferStatus(GLuint Framebuffer) {
	BindFramebuffer(Framebuffer);
	GLuint fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE) {
		check_gl_error();
		std::cout << "Framebuffer not complete. Status: " << fboStatus << std::endl;
		return false;
	}
	else {
		check_gl_error();
		std::cout << "Framebuffer complete! Status:" << fboStatus << std::endl;
		return true;
	}
}

void RenderAux::BindRenderTargetToFramebuffer(GLuint Framebuffer, GLuint TextureAtachment, GLuint DepthStencilAtachment)
{
	BindFramebuffer(Framebuffer);
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
		GL_TEXTURE_2D,
		TextureAtachment,
		0);
	check_gl_error();
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D,
		DepthStencilAtachment,
		0);
	check_gl_error();
}

void RenderAux::BindMultipleRenderTargetsToFramebuffer(GLuint Framebuffer, GLuint* ColorAtachments, GLuint nRenderTargets, GLuint DepthStencilAtachment)
{

	PrepareFramebufferForMultipleRenderTargets(Framebuffer, nRenderTargets);

	for (GLuint i = 0; i < nRenderTargets; i++) {
		glFramebufferTexture2D(
			GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + i,
			GL_TEXTURE_2D,
			ColorAtachments[i],
			0);
		check_gl_error();
	}
	glFramebufferTexture2D(
		GL_FRAMEBUFFER,
		GL_DEPTH_STENCIL_ATTACHMENT,
		GL_TEXTURE_2D,
		DepthStencilAtachment,
		0);
	check_gl_error();
}

void RenderAux::PrepareFramebufferForMultipleRenderTargets(GLuint Framebuffer, GLuint nRenderTargets)
{

	BindFramebuffer(Framebuffer);
	if (nRenderTargets > 0) {
		GLuint* atachments = new GLuint[nRenderTargets];
		for (GLuint i = 0; i < nRenderTargets; i++) {
			atachments[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glDrawBuffers(nRenderTargets, atachments);
		delete[] atachments;
	}
	check_gl_error();
}

void RenderAux::BindImageFromTextureToUnit(GLuint TexID, GLuint UnitIndex, GLenum Access, GLenum Format, GLuint LODlvl, bool layered, GLuint Layer)
{
	glBindImageTexture(UnitIndex, TexID, LODlvl, layered, Layer, Access, Format);
	check_gl_error();
}

void RenderAux::BindTextureUnitToUniformID(GLuint TexID, GLuint TexUnit)
{
	glActiveTexture(GL_TEXTURE0 + TexUnit);
	glBindTexture(GL_TEXTURE_2D, TexID);
	glUniform1i(TexUnit, TexUnit);
	check_gl_error();
}

void RenderAux::BindTextureUnitToUniformName(GLuint TexID, GLuint TexUnit, GLuint ProgramID, std::string UniformName)
{
	bool c2 = false;
	glActiveTexture(GL_TEXTURE0 + TexUnit);
	glBindTexture(GL_TEXTURE_2D, TexID);
	glUniform1i(glGetUniformLocation(ProgramID, UniformName.c_str()), TexUnit);
	
}

void RenderAux::BindAtributeName(GLuint AtribID, GLuint ProgramID, std::string AtributeName)
{
	glBindAttribLocation(ProgramID, AtribID, AtributeName.c_str());
	check_gl_error();
}

void RenderAux::CleanTextureUnit(GLuint TexUnit = 0)
{
	glActiveTexture(GL_TEXTURE0 + TexUnit);
	glBindTexture(GL_TEXTURE_2D, 0);
	check_gl_error();
}

void RenderAux::UseRenderProgram(GLuint ShaderID)
{
	glUseProgram(ShaderID);
	check_gl_error();
}

void RenderAux::PrintComputeWorkGroupStats() {
	
	int work_grp_cnt[3];
	
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 0, &work_grp_cnt[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 1, &work_grp_cnt[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, 2, &work_grp_cnt[2]);
	
	printf("max global (total) work group size x:%i y:%i z:%i\n",
		work_grp_cnt[0], work_grp_cnt[1], work_grp_cnt[2]);
	
	int work_grp_size[3];
	
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 0, &work_grp_size[0]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 1, &work_grp_size[1]);
	glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, 2, &work_grp_size[2]);
	
	printf("max local (in one shader) work group sizes x:%i y:%i z:%i\n",
		work_grp_size[0], work_grp_size[1], work_grp_size[2]);
	
	int work_grp_inv;
	
	glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &work_grp_inv);
	
	printf("max local work group invocations %i\n", work_grp_inv);

	check_gl_error();
	
}

void RenderAux::UseComputeProgram(GLuint ComputeShaderID, GLuint num_groups_x, GLuint work_group_size_x, GLuint num_groups_y, GLuint work_group_size_y, GLuint num_groups_z, GLuint work_group_size_z)
{

	//GLuint NUMGROUPSx = ( num_groups_x / work_group_size_x );
	//GLuint NUMGROUPSy = ( num_groups_y / work_group_size_y );
	//GLuint NUMGROUPSz = ( num_groups_z / work_group_size_z );

	glUseProgram(ComputeShaderID);
	check_gl_error();
	glDispatchComputeGroupSizeARB(num_groups_x, num_groups_y, num_groups_z, work_group_size_x, work_group_size_y, work_group_size_z);
	check_gl_error();
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	check_gl_error();
}

void RenderAux::SwapTexIDs(GLuint & Tex_A, GLuint & Tex_B)
{
	GLuint aux = Tex_A;
	Tex_A = Tex_B;
	Tex_B = aux;
}

GLdouble RenderAux::MapValueOfAB(GLdouble A, GLdouble B)
{
	GLdouble r = A / B;
	return r;
}

void RenderAux::BindShaderStorageBufferObject(GLuint SSboID, GLuint N_Units, GLuint UnitSize, GLuint index)
{
	glBindBufferRange(GL_SHADER_STORAGE_BUFFER, index, SSboID, 0, (GLsizeiptr)(UnitSize) * (GLsizeiptr)(N_Units));
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, index, SSboID);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	check_gl_error();
}

double t;
int frameCount;
int FPS = 1;
double deltaTime;
double RenderAux::FPS_counter(double Time)
{

	frameCount += 1;
	double t_minus_one = t;
	t = Time;

	deltaTime += t - t_minus_one;

	if (deltaTime > 1.0) {
		FPS = frameCount;
		deltaTime = 0.0;
		frameCount = 0;
	}

	return FPS;
}

void RenderAux::FPS_lock(int FPSlock)
{
	//int err = abs(FPS - FPSlock);
	//
	//std::cout << "err :" << err << std::endl;
	//
	//std::this_thread::sleep_for(std::chrono::nanoseconds(1000000000 * err));
	
	//std::cout << FPScount;
}



RenderAux::~RenderAux()
{
}