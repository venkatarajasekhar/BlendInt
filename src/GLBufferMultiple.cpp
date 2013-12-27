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

#ifdef __UNIX__
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#include <GL/glext.h>
#endif
#endif  // __UNIX__

#include <iterator>

#include <BlendInt/GLBufferMultiple.hpp>

namespace BlendInt {

	GLBufferMultiple::GLBufferMultiple ()
	: AbstractGLBuffer()
	{
	}

	GLBufferMultiple::~GLBufferMultiple ()
	{
		m_properties.clear();
	}

	void GLBufferMultiple::Generate (size_t num)
	{
		if(ids().size()) Clear();

		ids().resize(num);
		glGenBuffers((GLsizei)num, &(ids()[0]));

		m_properties.resize(num);
	}

	void GLBufferMultiple::Destroy ()
	{
		size_t index_last = ids().size() - 1;

		glDeleteBuffers(1, &(ids()[index()]));

		std::vector<GLuint>::iterator it_id = ids().begin();
		std::vector<GLBufferProperty>::iterator it_pro = m_properties.begin();

		std::advance(it_id, index());
		std::advance(it_pro, index());

		ids().erase(it_id);
		m_properties.erase(it_pro);

		if(index() == index_last)
			set_index(index() - 1);
	}

	void GLBufferMultiple::Destroy (size_t idx)
	{
		size_t index_last = ids().size() - 1;

		glDeleteBuffers(1, &(ids()[idx]));

		if(index() > idx) {
			set_index (index() - 1);
		} else if(index() == index_last && idx == index_last) {
			set_index (idx - 1);
		}

		std::vector<GLuint>::iterator it_id = ids().begin();
		std::vector<GLBufferProperty>::iterator it_pro = m_properties.begin();

		std::advance(it_id, idx);
		std::advance(it_pro, idx);

		ids().erase(it_id);
		m_properties.erase(it_pro);
	}

	void GLBufferMultiple::Clear ()
	{
		GLBufferWrapper::Clear();

		m_properties.clear();
	}

	void GLBufferMultiple::SetProperty (int vertices, int unit_size,
        GLenum target, GLenum usage)
	{
		m_properties[index()].vertices = vertices;
		m_properties[index()].unit_size = unit_size;
		m_properties[index()].target = target;
		m_properties[index()].usage = usage;
	}

	void GLBufferMultiple::SetProperty (size_t index, int vertices,
        int unit_size, GLenum target, GLenum usage)
	{
		// TODO: check index is in scope
		m_properties[index].vertices = vertices;
		m_properties[index].unit_size = unit_size;
		m_properties[index].target = target;
		m_properties[index].usage = usage;
	}

	void GLBufferMultiple::Bind ()
	{
		glBindBuffer(m_properties[index()].target, ids()[index()]);
	}

	void GLBufferMultiple::Bind (size_t index)
	{
		// TODO: check index is in scope
		glBindBuffer(m_properties[index].target, ids()[index]);
	}

	void GLBufferMultiple::Unbind ()
	{
		glBindBuffer(m_properties[index()].target, 0);
	}

	void GLBufferMultiple::Unbind (size_t index)
	{
		// TODO: check index is in scope
		glBindBuffer(m_properties[index].target, 0);
	}

	void GLBufferMultiple::Upload (const GLvoid* data)
	{
		glBufferData (m_properties[index()].target,
				m_properties[index()].unit_size * m_properties[index()].vertices,
				data,
				m_properties[index()].usage);
	}

	void GLBufferMultiple::Upload (size_t index, const GLvoid* data)
	{
		glBufferData (m_properties[index].target,
				m_properties[index].unit_size * m_properties[index].vertices,
				data,
				m_properties[index].usage);
	}

	int GLBufferMultiple::Vertices ()
	{
		return m_properties[index()].vertices;
	}

	int GLBufferMultiple::UnitSize ()
	{
		return m_properties[index()].unit_size;
	}

	GLenum GLBufferMultiple::Target ()
	{
		return m_properties[index()].target;
	}

	GLenum GLBufferMultiple::Usage ()
	{
		return m_properties[index()].usage;
	}

}
