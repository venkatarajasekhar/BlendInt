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

#include <BlendInt/Gui/AbstractNode.hpp>
#include <BlendInt/Gui/Context.hpp>

namespace BlendInt {

	void AbstractNode::SetRoundRadius (float radius)
	{
		if(round_radius_ == radius) return;

		PerformRoundRadiusUpdate(radius);
	}

	void AbstractNode::SetRoundType (int type)
	{
		if((node_flag_ & 0x0F) == (type & 0x0F)) return;

		PerformRoundTypeUpdate(type & 0x0F);
	}

	void AbstractNode::GenerateRoundedVertices (std::vector<GLfloat>* inner,
	        std::vector<GLfloat>* outer)
	{
		GenerateVertices(size(),
				default_border_width() * Context::theme->pixel(),
				round_type(),
				round_radius_,
				inner,
				outer);
	}

	void AbstractNode::GenerateRoundedVertices (Orientation shadedir,
	        short shadetop, short shadedown, std::vector<GLfloat>* inner,
	        std::vector<GLfloat>* outer)
	{
		GenerateVertices(size(),
				default_border_width() * Context::theme->pixel(),
				round_type(),
				round_radius_,
				shadedir,
				shadetop,
				shadedown,
				inner,
				outer);
	}

	void AbstractNode::PerformRoundTypeUpdate (int round)
	{
		set_round_type(round);
	}

	void AbstractNode::PerformRoundRadiusUpdate (float radius)
	{
		round_radius_ = radius;
	}

}
