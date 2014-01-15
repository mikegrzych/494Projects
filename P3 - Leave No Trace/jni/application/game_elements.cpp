//
//  game_elements.cpp


#include "game_elements.h"
#include <iostream>
#include <math.h>

ButtonColor Button::m_active_color = noColor;

Game_Object::Game_Object(Zeni::Model * model_,
                         const Zeni::Point3f &position_,
                         const Zeni::Vector3f &scale_,
                         const Zeni::Quaternion &rotation_)
: m_model(model_),
m_position(position_),
m_scale(scale_),
m_rotation(rotation_)
{
  Zeni::Model_Extents extents = m_model->get_extents();
  std::cout << "object model size = {" <<
  extents.upper_bound.x - extents.lower_bound.x << "," <<
  extents.upper_bound.y - extents.lower_bound.y << "," <<
  extents.upper_bound.z - extents.lower_bound.z << "}" << std::endl;
  m_size = Zeni::Vector3f(extents.upper_bound.x - extents.lower_bound.x,
                          extents.upper_bound.y - extents.lower_bound.y,
                          extents.upper_bound.z - extents.lower_bound.z);
  m_size = m_size.multiply_by(m_scale);
}

Game_Object::~Game_Object()
{
  delete m_model;
}

void Game_Object::set_position(const Zeni::Point3f &position_)
{
  m_position = position_;
  create_body(get_corner());
}

void Game_Object::create_body(const Zeni::Point3f &corner_)
{
  m_body = Zeni::Collision::Parallelepiped(corner_,
                                           m_rotation * m_size.get_i(),
                                           m_rotation * m_size.get_j(),
                                           m_rotation * m_size.get_k());
}

void Game_Object::remove_body()
{
  m_body = Zeni::Collision::Parallelepiped(Zeni::Point3f(),
                                           Zeni::Vector3f(),
                                           Zeni::Vector3f(),
                                           Zeni::Vector3f());
}

void Game_Object::set_model(Zeni::Model * model_)
{
  Zeni::Model * _old = m_model;
  m_model = model_;
  delete _old;
}

void Game_Object::set_keyframe(const float &keyframe)
{
  m_model->set_keyframe(keyframe);
  std::cout << "keyframe # " << m_model->get_keyframe() << std::endl;
}

void Game_Object::render()
{
  const std::pair<Zeni::Vector3f, float> rotation = m_rotation.get_rotation();
  
  m_model->set_translate(m_position);
  m_model->set_scale(m_scale);
  m_model->set_rotate(rotation.second, rotation.first);
  
  m_model->render();
}

void Object_Dynamic::step(const float &time_step)
{
  Zeni::Point3f new_position = get_position() + time_step * m_velocity;
  set_position(new_position);
}

void Object_Dynamic::execute_step(const float &time_step)
{
  step(time_step);
  create_body(get_corner());
}

void Object_Dynamic::set_on_ground(const bool &grounded_)
{
  m_is_on_ground = grounded_;
  if (m_is_on_ground)
    m_velocity.k = 0.0f;
}

void Player::jump() {
  if (is_grounded())
  {
    mod_velocity(Zeni::Vector3f(0.0f,0.0f,120.0f));
    set_on_ground(false);
  }
}

void Player::set_position(const Zeni::Point3f &position_)
{
  Game_Object::set_position(position_);
  create_bodies();
}
      
void Player::turn_left_xy(const float &theta)
{
  Zeni::Quaternion new_rotation =
    Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), theta)
    * get_rotation();
  set_rotation(new_rotation);
  m_direction = new_rotation * Zeni::Vector3f(1.0f,0.0f,0.0f);
}

void Player::update_camera()
{
  m_camera.position = get_position() - (m_direction * 200.0f) + Zeni::Vector3f(0.0f,0.0f,80.0f);
  m_camera.look_at(get_position());
}

void Player::execute_step(const float &time_step)
{
  step(time_step);
  create_bodies();
}

const Zeni::Point3f Player::get_corner() const
{
  Zeni::Point3f corner = get_position();
  Zeni::Vector3f size = get_size();
  corner -= (size.multiply_by(get_rotation() * Zeni::Vector3f(0.5f,0.5f,0.0f)));
  return corner;
}

void Player::create_bodies()
{
  create_body(get_corner());
  m_move_body = Zeni::Collision::Infinite_Cylinder(get_position(),
                                                   get_position() + get_size().get_k(),
                                                   get_size().k * 0.5f);
  m_sphere = Zeni::Collision::Sphere(get_position() + get_size().get_k(),
                                    get_size().k);
}

void Player::set_camera(const Zeni::Point3f &newlocation)
{
  m_camera.position = newlocation;
}

ActionType Pickup::on_player()
{
  std::cout << "I am a pickup" << std::endl;
  return rewardPlayer;
}

ActionType Button::on_player()
{
  m_active_color = m_color;
  return doNothing;
}

void ColorBlock::perform_logic(const float &time_step)
{
  create_body(get_corner());
}

void ColorBlock::create_body(const Zeni::Point3f &corner_)
{
  if (get_color() == get_active_color())
    Game_Object::create_body(corner_);
  else
    remove_body();
}

void ColorBlock::render()
{
  if (get_color() == get_active_color())
    Game_Object::render();
}

ActionType Deathwall::on_player()
{
  return gameOver;
}

void Deathwall::perform_logic(const float &time_step)
{
  execute_step(time_step);
}

void Deathwall::render()
{
  Zeni::Material mat_trans("trans");
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> back(Zeni::Vertex3f_Texture(get_corner() + get_size(),
                                                                          Zeni::Point2f(0.0f,0.0f)),
                                                   Zeni::Vertex3f_Texture(get_corner() + get_size().get_ik(),
                                                                          Zeni::Point2f(0.0f,1.0f)),
                                                   Zeni::Vertex3f_Texture(get_corner() + get_size().get_i(),
                                                                          Zeni::Point2f(1.0f,1.0f)),
                                                   Zeni::Vertex3f_Texture(get_corner() + get_size().get_ij(),
                                                                          Zeni::Point2f(1.0f,0.0f)));
  Zeni::Quadrilateral<Zeni::Vertex3f_Texture> front(Zeni::Vertex3f_Texture(get_corner() + get_size().get_ik(),
                                                                          Zeni::Point2f(0.0f,0.0f)),
                                                   Zeni::Vertex3f_Texture(get_corner() + get_size(),
                                                                          Zeni::Point2f(0.0f,1.0f)),
                                                   Zeni::Vertex3f_Texture(get_corner() + get_size().get_ij(),
                                                                          Zeni::Point2f(1.0f,1.0f)),
                                                   Zeni::Vertex3f_Texture(get_corner() + get_size().get_i(),
                                                                          Zeni::Point2f(1.0f,0.0f)));
  back.fax_Material(&mat_trans);
  front.fax_Material(&mat_trans);
  
  Zeni::Video &vr = Zeni::get_Video();
  vr.render(back);
  vr.render(front);
//  vr.render(test);

}