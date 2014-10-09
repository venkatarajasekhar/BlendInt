#ifdef __UNIX__
#ifdef __APPLE__
#include <gl3.h>
#include <glext.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <assert.h>
#include <algorithm>

#include <iostream>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>

#include <BlendInt/Gui/VertexTool.hpp>
#include <BlendInt/Stock/Theme.hpp>
#include <BlendInt/Stock/Shaders.hpp>

#include "StudioFrame.hpp"

using namespace BlendInt;
using Stock::Shaders;

StudioFrame::StudioFrame()
: BinLayout()
{
	set_size(400, 300);
}

StudioFrame::~StudioFrame ()
{
}

void StudioFrame::PerformSizeUpdate(const SizeUpdateRequest& request)
{
	if(request.target() == this) {

		set_size(*request.size());

		if (subs_count()) {
			assert(subs_count() == 1);
			FillSingleWidget(0, *request.size(), margin());
		}

		Refresh();
	}

	if(request.source() == this) {
		ReportSizeUpdate(request);
	}
}

ResponseType StudioFrame::Draw (Profile& profile)
{
	if(refresh()) {
		RenderToBuffer(profile);
	}

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	tex_buffer_.Draw(position().x(), position().y());
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return Accept;
}

void StudioFrame::RenderToFile (const std::string& filename)
{
	tex_buffer_.texture()->bind();
	tex_buffer_.texture()->WriteToFile(filename);
	tex_buffer_.texture()->reset();
}

void StudioFrame::RenderToBuffer (BI::Profile& profile)
{
	GLsizei width = size().width();
	GLsizei height = size().height();

	GLfloat left = position().x();
	GLfloat bottom = position().y();

	GLfloat right = left + width;
	GLfloat top = bottom + height;

	tex_buffer_.SetCoord(0.f, 0.f, size().width(), size().height());
	// Create and set texture to render to.
	GLTexture2D* tex = tex_buffer_.texture();
	if(tex->id() == 0)
		tex->generate();

	tex->bind();
	tex->SetWrapMode(GL_REPEAT, GL_REPEAT);
	tex->SetMinFilter(GL_NEAREST);
	tex->SetMagFilter(GL_NEAREST);
	tex->SetImage(0, GL_RGBA, size().width(), size().height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	GLFramebuffer* fb = new GLFramebuffer;
	fb->generate();
	fb->bind();

	// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
			GL_TEXTURE_2D, tex->id(), 0);
	//fb->Attach(*tex, GL_COLOR_ATTACHMENT0);

	// Critical: Create a Depth_STENCIL renderbuffer for this off-screen rendering
	GLuint rb;
	glGenRenderbuffers(1, &rb);

	glBindRenderbuffer(GL_RENDERBUFFER, rb);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_STENCIL,
			size().width(), size().height());
	//Attach depth buffer to FBO
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
			GL_RENDERBUFFER, rb);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT,
			GL_RENDERBUFFER, rb);

	if(GLFramebuffer::CheckStatus()) {

		fb->bind();

		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClearDepth(1.0);
		glClearStencil(0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);

		glm::mat4 origin;

		Shaders::instance->GetUIProjectionMatrix(origin);

		glm::mat4 projection = glm::ortho(left, right, bottom, top, 100.f,
		        -100.f);

		Shaders::instance->SetUIProjectionMatrix(projection);

        GLint vp[4];
        glGetIntegerv(GL_VIEWPORT, vp);
		glViewport(0, 0, size().width(), size().height());

		// Draw frame panel

		Profile off_screen_profile(profile, position());

		if(first_child()) {
			DispatchDrawEvent(first_child(), off_screen_profile);
		}

		// Restore the viewport setting and projection matrix
		glViewport(vp[0], vp[1], vp[2], vp[3]);

		Shaders::instance->SetUIProjectionMatrix(origin);

		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	}

	fb->reset();
	tex->reset();

	//delete tex; tex = 0;

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glDeleteRenderbuffers(1, &rb);

	fb->reset();
	delete fb; fb = 0;

}
