#include "Framebuffer.h"
#include "glew/include/GL/glew.h"

#include "Globals.h"

Framebuffer::Framebuffer(int w, int h) : width(w), height(h)
{
	framebuffer = 0;
	textureColorbuffer = 0;
	rboDepthStencil = 0;
	SetFramebuffer();
}

Framebuffer::~Framebuffer()
{
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &textureColorbuffer);
	glDeleteTextures(1, &rboDepthStencil);
}

void Framebuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0, 0, width, height);
}

void Framebuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Framebuffer::SetFramebuffer()
{
	if (framebuffer > 0)
	{
		glDeleteFramebuffers(1, &framebuffer);
		glDeleteTextures(1, &textureColorbuffer);
		glDeleteTextures(1, &rboDepthStencil);
	}

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glGenTextures(1, &textureColorbuffer);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenTextures(1, &rboDepthStencil);
	glBindTexture(GL_TEXTURE_2D, rboDepthStencil);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, width, height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, rboDepthStencil, 0);

	glBindTexture(GL_TEXTURE_2D, 0);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (err != GL_FRAMEBUFFER_COMPLETE)
	{
		DEBUG_LOG("Framebuffer is Incomplete. Error %s", glGetString(err));
	}
	else DEBUG_LOG("Framebuffer is Complete");
}

void Framebuffer::ResizeFramebuffer(int w, int h)
{
	width = w;
	height = h;
	SetFramebuffer();
}