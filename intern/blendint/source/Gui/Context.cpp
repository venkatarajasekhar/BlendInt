/*
 * This file is part of BlendInt (a Blender-like Interface Library in
 * OpenGL).
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is free
 * software: you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your
 * option) any later version.
 *
 * BlendInt (a Blender-like Interface Library in OpenGL) is
 * distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General
 * Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BlendInt.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <BlendInt/OpenGL/GLHeader.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/transform.hpp>

#include <BlendInt/Core/Time.hpp>

#include <BlendInt/Font/FcPattern.hpp>
#include <BlendInt/Font/FcConfig.hpp>

#include <BlendInt/OpenGL/GLFramebuffer.hpp>
#include <BlendInt/OpenGL/GLRenderbuffer.hpp>

#include <BlendInt/Gui/Context.hpp>
#include <BlendInt/Stock/Cursor.hpp>

namespace BlendInt
{
	Theme* Context::theme = 0;
	Icons* Context::icons = 0;
	Shaders* Context::shaders = 0;
	Cursor* Context::cursor = 0;

	glm::mat4 Context::default_view_matrix =
		glm::lookAt(glm::vec3(0.f, 0.f, 1.f),
					glm::vec3(0.f, 0.f, 0.f),
					glm::vec3(0.f, 1.f, 0.f));

	std::set<Context*> Context::context_set;

	bool Context::Initialize()
	{
		bool success = true;

#ifdef DEBUG
		int major, minor;
		GetGLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL version: %d.%d", major, minor);
		GetGLSLVersion(&major, &minor);
		DBG_PRINT_MSG("OpenGL shading language version: %d.%d", major, minor);
#endif

		if (success && Fc::Config::init()) {
			// do nothing
		} else {

			DBG_PRINT_MSG("%s", "Cannot initialize FontConfig");
			success = false;

		}

		if (success && InitializeTheme()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Themes");
			success = false;
		}

		if (success && InitializeShaders()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s",
						  "The Shader Manager is not initialized successfully!");
			success = false;
		}

		if (success && InitializeIcons()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize Stock Icons");
			success = false;
		}

		if (success && InitializeCursor()) {
			// do nothing;
		} else {
			DBG_PRINT_MSG ("%s", "Cannot initilize Cursor");
			success = false;
		}

		// Create Default font: must call this after theme initialized as it read the default_font
		if(success && InitializeFont()) {
			// do nothing
		} else {
			DBG_PRINT_MSG("%s", "Cannot initialize font");
			success = false;
		}

		if(success) {
			Time::SaveCurrent();
		}

		return success;
	}

	void Context::Release()
	{
		while(context_set.size()) {
			std::set<Context*>::iterator it = context_set.begin();

			if((*it)->managed() && ((*it)->reference_count() == 0)) {
				DBG_PRINT_MSG("%s", "Delete context");
				delete (*it);	// this will erase the context from the set
			} else {
				context_set.erase(it);
			}
		}

		ReleaseFont();
		ReleaseIcons();
		ReleaseShaders();
		ReleaseTheme();
		ReleaseCursor();
		Fc::Config::fini();
	}

	Context::Context ()
	: AbstractView(),
	  active_frame_(nullptr),
	  stencil_count_(0)
	{
		set_size(640, 480);
		set_refresh(true);

		events_.reset(new Cpp::ConnectionScope);

		InitializeContext();

		context_set.insert(this);
	}

	Context::~Context ()
	{
		if(superview() != 0) {
			DBG_PRINT_MSG("Error: %s",
						  "Context MUST NOT be in any other superview");
		}
		context_set.erase(this);
	}

	bool Context::AddFrame (AbstractFrame* frame, bool focus)
	{
		return false;
	}

	bool Context::InsertFrame(int index, AbstractFrame* frame, bool focus)
	{
		return false;
	}

	void Context::MoveFrameToTop(AbstractFrame* frame, bool focus)
	{
	}

	void Context::Draw()
	{

	}

	void Context::DispatchKeyEvent(KeyAction action,
								   int key,
								   int modifiers,
								   int scancode,
								   String text)
	{
		/*
		key_action_ = action;
		key_ = key;
		modifiers_ = modifiers;
		scancode_ = scancode;
		text_ = text;

		active_frame_ = nullptr;

		switch (key_action_) {

			case KeyPress: {
				PerformKeyPress(this);
				break;
			}

			case KeyRelease: {
				break;
			}

			case KeyRepeat: {
				break;
			}

			default:
				break;
		}
	*/
	}

	void Context::DispatchMouseEvent(int x,
									 int y,
									 MouseAction action,
									 MouseButton button,
									 int modifiers)
	{
		/*
		cursor_position_.reset(x, size().height() - y);

		mouse_action_ = action;
		mouse_button_ = button;
		modifiers_ = modifiers;

		active_frame_ = nullptr;

		switch (mouse_action_) {

			case MouseMove: {
				DispatchHoverEvent();
				PerformMouseMove(this);
				break;
			}

			case MousePress: {
				DispatchHoverEvent();
				PerformMousePress(this);
				break;
			}

			case MouseRelease: {
				PerformMouseRelease(this);
				DispatchHoverEvent();
				break;
			}

			default:
				break;
		}
	*/
	}

	bool Context::Contain (const Point& point) const
	{
		return true;
	}

	void Context::SynchronizeWindow()
	{
		// TODO: override this function
	}

	void Context::MakeGLContextCurrent()
	{
		// TODO: override this to support GL Context manipulation in thread
	}

	void Context::BeginPushStencil()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

		if(stencil_count_ == 0) {
			glEnable(GL_STENCIL_TEST);
			glStencilFunc(GL_NEVER, 1, 0xFF);	// GL_NEVER: always fails
			glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)
		} else {
			glStencilFunc(GL_LESS, stencil_count_, 0xFF);
			glStencilOp(GL_INCR, GL_KEEP, GL_KEEP); // increase 1s on test fail (always)
		}

		stencil_count_++;
	}

	void Context::EndPushStencil()
	{
		glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
		glStencilFunc(GL_EQUAL, stencil_count_, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	}

	void Context::BeginPopStencil()
	{
		glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
		glStencilFunc(GL_LESS, stencil_count_, 0xFF);
		glStencilOp(GL_DECR, GL_KEEP, GL_KEEP); // draw 1s on test fail (always)
	}

	void Context::EndPopStencil()
	{
		if(stencil_count_ > 0) {
			stencil_count_--;

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glStencilFunc(GL_EQUAL, stencil_count_, 0xFF);
			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

			if(stencil_count_ == 0) {
				glDisable(GL_STENCIL_TEST);
			}

		}
	}

	Context* Context::GetContext (AbstractView* widget)
	{
		AbstractView* superview = widget->superview();

		if(superview == 0) {
			return dynamic_cast<Context*>(widget);
		} else {

			while(superview->superview()) {
				superview = superview->superview();
			}

		}

		return dynamic_cast<Context*>(superview);
	}

	bool Context::SizeUpdateTest (const SizeUpdateRequest& request)
	{
		return true;
	}

	bool Context::PositionUpdateTest (const PositionUpdateRequest& request)
	{
		return true;
	}

	void Context::PerformPositionUpdate (const PositionUpdateRequest& request)
	{
	}

	void Context::PerformSizeUpdate (const SizeUpdateRequest& request)
	{
		if (request.target() == this) {
			set_size(*request.size());

			glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
			shaders->SetFrameProjectionMatrix(projection);

			set_refresh(true);

			resized_.fire(size());
		}
	}

	bool Context::PreDraw(const AbstractWindow* context)
	{
		return true;
	}

	ResponseType Context::Draw (const AbstractWindow* context)
	{
		for(AbstractView* p = first_subview(); p; p = p->next_view())
		{
			p->PreDraw(context);
			p->Draw(context);
			p->set_refresh(this->refresh());
			p->PostDraw(context);
		}

		return Finish;
	}

	void Context::PostDraw(const AbstractWindow* context)
	{
	}

	ResponseType Context::PerformKeyPress (const AbstractWindow* context)
	{
		ResponseType response = Ignore;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->PerformKeyPress(context);
			if(response == Finish) break;
		}

		return response;
	}

	ResponseType Context::PerformContextMenuPress (const AbstractWindow* context)
	{

		return Ignore;
	}

	ResponseType Context::PerformContextMenuRelease (
	        const AbstractWindow* context)
	{

		return Ignore;
	}

	ResponseType Context::PerformMousePress (const AbstractWindow* context)
	{
		ResponseType response = Ignore;
		//assert(context->leaf_frame() == 0);

		set_pressed(true);

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			response = p->PerformMousePress(context);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::PerformMouseRelease (const AbstractWindow* context)
	{
		ResponseType response = Ignore;
		set_pressed(false);

		for(AbstractView* p = last_subview(); p != nullptr; p = p->previous_view())
		{
			response = p->PerformMouseRelease(context);
			if(response == Finish) {
				break;
			}
		}

		return response;
	}

	ResponseType Context::PerformMouseMove (const AbstractWindow* context)
	{
		ResponseType response = Ignore;

		if(pressed_ext()) {

			for(AbstractView* p = last_subview(); p != nullptr; p = p->previous_view())
			{
				response = p->PerformMouseMove(context);
				if(response == Finish) {
					break;
				}
			}

		}

		return response;
	}

	bool Context::RemoveSubView(AbstractView* view)
	{
		AbstractFrame* new_last = nullptr;
		AbstractFrame* frame = dynamic_cast<AbstractFrame*>(view);

		if(view->next_view() == nullptr) {
			new_last = dynamic_cast<AbstractFrame*>(view->previous_view());

			if(frame != nullptr) {
				frame->set_focus(false);
			}
		}

		bool retval = AbstractView::RemoveSubView(view);

		if(new_last != nullptr) {
			DBG_PRINT_MSG("%s", "call focus event");
			new_last->set_focus(true);
			//new_last->PerformFocusOn(this);
		}

		return retval;
	}

	bool Context::InitializeTheme ()
	{
		bool result = false;

		if (!theme) {
			theme = new Theme;
		}

		if (theme) {
			theme->Reset();
			result = true;
		}

		return result;
	}

	bool Context::InitializeIcons ()
	{
		if (!icons)
			icons = new Icons;

		return true;
	}

	bool Context::InitializeShaders ()
	{
		bool ret = false;

		if (!shaders) {
			shaders = new Shaders;

			if (shaders) {
				ret = shaders->Setup();
			} else {
				ret = false;
			}
		}

		return ret;

	}

	bool Context::InitializeCursor ()
	{
		cursor = new Cursor;

		cursor->RegisterCursorType(new BlankCursorTheme);

		return true;
	}

	bool Context::InitializeFont()
	{
        Fc::Pattern p = Fc::Pattern::name_parse((const FcChar8*)theme->default_font());

		Fc::Config::substitute(0, p, FcMatchPattern);
		p.default_substitute();

		FcResult result;
		Fc::Pattern match = Fc::Config::match(0, p, &result);

		if(match) {
			FontCache::Create(match);
			FontCache::kDefaultFontHash = match.hash();
			return true;
		} else {
			return false;
		}
	}

	void Context::ReleaseTheme ()
	{
		if (theme) {
			delete theme;
			theme = 0;
		}
	}

	void Context::ReleaseIcons()
	{
		if (icons) {
			delete icons;
			icons = 0;
		}
	}

	void Context::ReleaseShaders ()
	{
		if (shaders) {
			delete shaders;
			shaders = 0;
		}
	}

	void Context::ReleaseCursor ()
	{
		if(cursor) {
			delete cursor;
			cursor = nullptr;
		}
	}

	void Context::ReleaseFont()
	{
		FontCache::ReleaseAll();
	}

	void Context::GetGLVersion (int *major, int *minor)
	{
		const char* verstr = (const char*) glGetString(GL_VERSION);
		if((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2)) {
			*major = *minor = 0;
			fprintf(stderr, "Invalid GL_VERSION format!!!\n");
		}
	}

	void Context::GetGLSLVersion (int *major, int *minor)
	{
		int gl_major, gl_minor;
		GetGLVersion(&gl_major, &gl_minor);

		*major = *minor = 0;
		if(gl_major == 1) {
			/* GL v1.x can provide GLSL v1.00 only as an extension */
			const char* extstr = (const char*)glGetString(GL_EXTENSIONS);
			if((extstr != NULL) && (strstr(extstr, "GL_ARB_shading_language_100") != NULL))
			{
				*major = 1;
				*minor = 0;
			}
		} else if (gl_major >= 2) {
			/* GL v2.0 and greater must parse the version string */
			const char* verstr = (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION);

			if((verstr == NULL) || (sscanf(verstr, "%d.%d", major, minor) != 2))
			{
				*major = *minor = 0;
				fprintf(stderr, "Invalid GL_SHADING_LANGUAGE_VERSION format!!!\n");
			}
		}
	}

	void Context::InitializeContext ()
	{
		glm::mat4 projection = glm::ortho(0.f, (float)size().width(), 0.f, (float)size().height(), 100.f, -100.f);
		shaders->SetFrameProjectionMatrix(projection);
		shaders->SetFrameViewMatrix(default_view_matrix);
		shaders->SetFrameModelMatrix(glm::mat3(1.f));

		shaders->SetWidgetViewMatrix(default_view_matrix);
		shaders->SetWidgetModelMatrix(glm::mat3(1.f));
	}

	void Context::DispatchHoverEvent()
	{
		ResponseType response = Ignore;
		//AbstractFrame* frame = 0;

		for(AbstractView* p = last_subview(); p; p = p->previous_view()) {
			//frame = dynamic_cast<AbstractFrame*>(p);
			//response = frame->DispatchHoverEvent(this);
			if(response == Finish) break;
		}
	}

	void Context::PerformFocusOn(const AbstractWindow* context)
	{
	}

	void Context::PerformFocusOff(const AbstractWindow* context)
	{
	}

	void Context::PerformHoverIn(const AbstractWindow* context)
	{
	}

	void Context::PerformHoverOut(const AbstractWindow* context)
	{
	}

}
