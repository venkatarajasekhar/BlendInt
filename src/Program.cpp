/*
 * This file is part of BIL (Blender Interface Library).
 *
 * BIL (Blender Interface Library) is free software: you can
 * redistribute it and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * BIL (Blender Interface Library) is distributed in the hope that it
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with BIL.  If not, see
 * <http://www.gnu.org/licenses/>.
 *
 * Contributor(s): Freeman Zhang <zhanggyb@gmail.com>
 */

#include <GL/glew.h>

#include <BIL/Program.hpp>
#include <iostream>

namespace BIL {

	namespace GL {

		Program::Program ()
				: id_(0)
		{
			id_ = glCreateProgram();
		}

		Program::~Program ()
		{
			if (glIsProgram(id_)) {
				glDeleteProgram(id_);
			}

			std::vector<Shader*>::iterator it;
			for (it = shaders_.begin(); it != shaders_.end(); it++) {
				delete *it;
				*it = NULL;
			}
			shaders_.clear();
		}

		void Program::addShader (const std::string& filename, GLenum type)
		{
			if (glIsProgram(id_)) {
				Shader* shader = new Shader(filename, type);
				shaders_.push_back(shader);
				glAttachShader(id_, shader->id());
			}
		}

		void Program::addShader (const char* buf, GLenum type)
		{
			if (glIsProgram(id_)) {
				Shader* shader = new Shader(buf, type);
				shaders_.push_back(shader);
				glAttachShader(id_, shader->id());
			}
		}

		bool Program::isValid () const
		{
			return glIsProgram(id_);
		}

		bool Program::link ()
		{
			GLint link_ok = GL_FALSE;

			if (glIsProgram(id_)) {
				glLinkProgram(id_);
				glGetProgramiv(id_, GL_LINK_STATUS, &link_ok);
				if (!link_ok) {
					std::cerr << "Fail to glLinkProgram" << std::endl;
				}
			}

			return link_ok == GL_TRUE ? true : false;
		}

	}

}