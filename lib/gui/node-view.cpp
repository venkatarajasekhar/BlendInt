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

#include <glm/gtx/matrix_transform_2d.hpp>

#include <blendint/gui/node-view.hpp>
#include <blendint/gui/abstract-window.hpp>

namespace BlendInt {

NodeView::NodeView ()
    : AbstractScrollable(),
      vao_(0),
      pressed_(false),
      focused_(false),
      hover_(false),
      mouse_tracking_record_(false)
{
  set_size(400, 300);

  InitializeNodeView();
  //		curve_ = new CubicBezierCurve;
  //		curve_->Unpack();

  guides_.reset(new GridGuides(size().width(), size().height()));
}

NodeView::NodeView (int width, int height)
    : AbstractScrollable(width, height), vao_(0), pressed_(false)
{

  InitializeNodeView();
  //		curve_ = new CubicBezierCurve;
  //		curve_->Unpack();
  guides_.reset(new GridGuides(width, height));
}

NodeView::~NodeView ()
{
  //		delete curve_;
  glDeleteVertexArrays(1, &vao_);
}

bool NodeView::AddNode (AbstractNode* node)
{
  AbstractWindow* win = AbstractWindow::GetWindow(this);

  if (PushBackSubView(node)) {

    if (win) {
      if (previous(node)) {
        dynamic_cast<AbstractNode*>(previous(node))->PerformFocusOff(
            win);
      }
      node->PerformFocusOn(win);
    }

    RequestRedraw();
    return true;
  }

  return false;
}

bool NodeView::InsertNode (int index, AbstractNode* node)
{
  AbstractWindow* win = AbstractWindow::GetWindow(this);

  if (InsertSubView(index, node)) {

    if (win) {

      if (next(node) == 0) { // push back
        if (previous(node)) {
          dynamic_cast<AbstractNode*>(previous(node))->PerformFocusOff(
              win);
        }
        node->PerformFocusOn(win);
      }

    }

    RequestRedraw();
    return true;
  }

  return false;
}

bool NodeView::IsExpandX () const
{
  return true;
}

bool NodeView::IsExpandY () const
{
  return true;
}

Size NodeView::GetPreferredSize () const
{
  return Size(500, 400);
}

void NodeView::SetFocusedNode (AbstractNode* node)
{
  if (node == 0) return;

  if (last() == node) return;

  if (node->super() == 0) {
    DBG_PRINT_MSG("%s", "the node is not in this view");
    return;
  }

  AbstractWindow* win = AbstractWindow::GetWindow(this);

  // if node is not the root node in this view, find and switch to it
  if (node->super() != this) {

    AbstractView* tmp = node;
    NodeView* node_view = 0;

    while (tmp->super()) {
      node_view = dynamic_cast<NodeView*>(tmp->super());
      if (node_view) break;
      tmp = tmp->super();
    }

    if (node_view == 0) {
      DBG_PRINT_MSG("%s", "the node is not in this view");
      return;
    }

    if (node_view != this) {
      DBG_PRINT_MSG("%s", "the node is not in this view");
      return;
    }

    node = dynamic_cast<AbstractNode*>(tmp);
    if (node == 0) return;
  }

  if (last()) {
    dynamic_cast<AbstractNode*>(last())->PerformFocusOff(win);
  }

  MoveToLast(node);
  node->PerformFocusOn(win);

  RequestRedraw();
}

NodeView* NodeView::GetNodeView (AbstractNode* node)
{
  AbstractView* parent = node->super();
  while (parent && is_node(parent)) {
    parent = parent->super();
  }

  return dynamic_cast<NodeView*>(parent);
}

bool NodeView::SizeUpdateTest (const AbstractView* source,
                               const AbstractView* target,
                               int width,
                               int height) const
{
  return true;
}

bool NodeView::PositionUpdateTest (const AbstractView* source,
                                   const AbstractView* target,
                                   int x,
                                   int y) const
{
  return true;
}

void NodeView::PerformSizeUpdate (const AbstractView* source,
                                  const AbstractView* target,
                                  int width,
                                  int height)
{
  if (target == this) {
    set_size(width, height);

    std::vector<GLfloat> inner_verts;
    GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                     0);

    vbo_.bind();
    vbo_.set_sub_data(0, sizeof(GLfloat) * inner_verts.size(),
                      &inner_verts[0]);
    vbo_.reset();

    guides_->Resize(width, height);
    RequestRedraw();
  }

  if (source == this) {
    report_size_update(source, target, width, height);
  }
}

void NodeView::PerformRoundTypeUpdate (int type)
{
  set_round_type(type);

  std::vector<GLfloat> inner_verts;
  GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                   0);

  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

void NodeView::PerformRoundRadiusUpdate (float radius)
{
  set_round_radius(radius);

  std::vector<GLfloat> inner_verts;
  GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                   0);

  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);
  vbo_.reset();

  RequestRedraw();
}

void NodeView::PerformFocusOn (AbstractWindow* context)
{
  DBG_PRINT_MSG("%s", "Focused");
  focused_ = true;

  if(hover_) {
    mouse_tracking_record_ = context->mouse_tracking();
    context->set_mouse_tracking(true);
  }
}

void NodeView::PerformFocusOff (AbstractWindow* context)
{
  DBG_PRINT_MSG("%s", "UnFocused");
  focused_ = false;

  if(hover_) {
    context->set_mouse_tracking(mouse_tracking_record_);
  }
}

void NodeView::PerformHoverIn (AbstractWindow* context)
{
  hover_ = true;

  if(focused_) {
    mouse_tracking_record_ = context->mouse_tracking();
    context->set_mouse_tracking(true);
  }
}

void NodeView::PerformHoverOut (AbstractWindow* context)
{
  hover_ = false;

  if(focused_ && context)
    context->set_mouse_tracking(mouse_tracking_record_);
}

Response NodeView::PerformKeyPress (AbstractWindow* context)
{
  Response response = Ignore;

  for (AbstractView* p = last(); p; p = previous(p)) {
    response = dynamic_cast<AbstractNode*>(p)->PerformKeyPress(context);
    if (response == Finish) break;
  }

  return response;
}

Response NodeView::PerformMousePress (AbstractWindow* context)
{
  pressed_ = true;

  PerformMouseHover(context);

  Response response = Ignore;
  Point local_position = context->local_cursor_position();
  for (AbstractView* p = last(); p; p = previous(p)) {
    response = dynamic_cast<AbstractNode*>(p)->PerformMousePress(context);
    context->set_local_cursor_position(local_position);
    if (response == Finish) break;
  }

  return Finish;
}

Response NodeView::PerformMouseRelease (AbstractWindow* context)
{
  pressed_ = false;

  PerformMouseHover(context);

  Response response = Ignore;
  Point local_position = context->local_cursor_position();
  for (AbstractView* p = last(); p; p = previous(p)) {
    response = dynamic_cast<AbstractNode*>(p)->PerformMouseRelease(context);
    context->set_local_cursor_position(local_position);
    if (response == Finish) {
      break;
    }
  }

  return Finish;
}

Response NodeView::PerformMouseMove (AbstractWindow* context)
{
  PerformMouseHover(context);

  if (pressed_) {

    Response response = Ignore;
    AbstractNode* node = 0;
    Point local_position = context->local_cursor_position();

    for (AbstractView* p = last(); p; p = previous(p)) {
      node = dynamic_cast<AbstractNode*>(p);
      response = node->PerformMouseMove(context);
      context->set_local_cursor_position(local_position);
      if (response == Finish) {
        break;
      }
    }

  }

  return Finish;
}

bool NodeView::PreDraw (AbstractWindow* context)
{
  glm::mat3 matrix = glm::translate(
      AbstractWindow::shaders()->widget_model_matrix(),
      glm::vec2(position().x(), position().y()));

  AbstractWindow::shaders()->PushWidgetModelMatrix();
  AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

  AbstractWindow::shaders()->widget_inner_program()->use();

  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_GAMMA), 0);
  glUniform1i(
      AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_SHADED),
      0);
  glUniform4f(
      AbstractWindow::shaders()->location(Shaders::WIDGET_INNER_COLOR),
      0.565f, 0.596f, 0.627f, 1.f);

  glBindVertexArray(vao_);
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

  context->BeginPushStencil();	// inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
  context->EndPushStencil();

  return true;
}

Response NodeView::Draw (AbstractWindow* context)
{
  //		curve_->Draw();
  guides_->Draw(0, 0);

  if (subview_count()) {

    Point offset = GetOffset();

    glm::mat3 matrix = glm::translate(
        AbstractWindow::shaders()->widget_model_matrix(),
        glm::vec2(offset.x(), offset.y()));

    AbstractWindow::shaders()->PushWidgetModelMatrix();
    AbstractWindow::shaders()->SetWidgetModelMatrix(matrix);

    return Ignore;

  } else {
    return Finish;
  }
}

void NodeView::PostDraw (AbstractWindow* context)
{
  if (subview_count()) {
    AbstractWindow::shaders()->PopWidgetModelMatrix();
  }

  // draw mask
  AbstractWindow::shaders()->widget_inner_program()->use();

  glBindVertexArray(vao_);

  context->BeginPopStencil();	// pop inner stencil
  glDrawArrays(GL_TRIANGLE_FAN, 0, 6);
  context->EndPopStencil();

  AbstractWindow::shaders()->PopWidgetModelMatrix();
}

void NodeView::PerformMouseHover (AbstractWindow* context)
{
  try {

    Response response = Ignore;
    Point local_position = context->local_cursor_position();
    for (AbstractView* p = last(); p; p = previous(p)) {
      response = dynamic_cast<AbstractNode*>(p)->PerformMouseHover(context);
      context->set_local_cursor_position(local_position);
      if (response == Finish) break;
    }

  } catch (std::bad_cast& e) {

    DBG_PRINT_MSG("Error: %s", "Only AbstractNode should be added in NodeView");
    exit(EXIT_FAILURE);

  }
}

void NodeView::InitializeNodeView ()
{
  std::vector<GLfloat> inner_verts;
  GenerateVertices(size(), 0.f, round_type(), round_radius(), &inner_verts,
                   0);

  vbo_.generate();
  glGenVertexArrays(1, &vao_);

  glBindVertexArray(vao_);

  vbo_.bind();
  vbo_.set_data(sizeof(GLfloat) * inner_verts.size(), &inner_verts[0]);

  glEnableVertexAttribArray(AttributeCoord);
  glVertexAttribPointer(AttributeCoord, 3,
                        GL_FLOAT,
                        GL_FALSE, 0, 0);

  glBindVertexArray(0);
  vbo_.reset();

}

}
