//
//  game_elements.h


#ifndef __game__game_elements__
#define __game__game_elements__

#include <zenilib.h>
#include <iostream>

enum ActionType {doNothing, doCollide, rewardPlayer, gameOver};
enum ButtonColor {noColor, redButton, blueButton, greenButton};

class Game_Object {
public:
  Game_Object(Zeni::Model * model_,
              const Zeni::Point3f &position_,
              const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
              const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f));
  virtual ~Game_Object();
  virtual void render();
  void set_keyframe(const float & keyframe);
  virtual void execute_step(const float &time_step) {}
  const Zeni::Point3f & get_position() const {return m_position;}
  virtual void set_position(const Zeni::Point3f &position_);
  const Zeni::Vector3f & get_scale() const {return m_scale;}
  const Zeni::Vector3f & get_size() const {return m_size;}
  void set_scale(const Zeni::Vector3f &scale_) {m_scale = scale_;}
  const Zeni::Quaternion & get_rotation() const {return m_rotation;}
  void set_rotation(const Zeni::Quaternion &rotation_) {m_rotation = rotation_;}
  const Zeni::Collision::Parallelepiped & get_body() const {return m_body;}
  virtual void create_body(const Zeni::Point3f &corner_);
  void remove_body();
  virtual const Zeni::Point3f get_corner() const {return m_position;}
//  virtual void collide() = 0;
protected:
  void set_model(Zeni::Model * model_);
private:
  Zeni::Collision::Parallelepiped m_body;
  Zeni::Model * m_model;
  Zeni::Point3f m_position;
  Zeni::Vector3f m_scale;
  Zeni::Vector3f m_size;
  Zeni::Quaternion m_rotation;
  
};

class Object_Static: public Game_Object {
public:
  Object_Static(Zeni::Model * model_,
                const Zeni::Point3f &position_,
                const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
                const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Game_Object(model_, position_, scale_, rotation_)
  {
    create_body(get_corner());
  }
  
};

class Environment_Static: public Object_Static {
public:
  Environment_Static(Zeni::Model * model_,
                const Zeni::Point3f &position_,
                const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
                const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Object_Static(model_, position_, scale_, rotation_)
  {}
  
};

class Goal_Static: public Object_Static {
public:
  Goal_Static(Zeni::Model * model_,
              const Zeni::Point3f &position_,
              const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
              const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Object_Static(model_, position_, scale_, rotation_)
  {}
  
  void modify_goal(int &goal_);
};

class Object_Dynamic: public Game_Object {
public:
  Object_Dynamic(Zeni::Model * model_,
                 const Zeni::Point3f &position_,
                 const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
                 const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Game_Object(model_, position_, scale_, rotation_),
  m_velocity(Zeni::Vector3f(0.0f,0.0f,0.0f)),
  m_health(1),
  m_speed(100.0f),
  m_is_on_ground(false)
  {}
  virtual ActionType on_player() {std::cout << "I am an object" << std::endl; return doNothing;};
  virtual void perform_logic(const float &time_step) {};
  const Zeni::Vector3f & get_velocity() const {return m_velocity;}
  void set_velocity(const Zeni::Vector3f &velocity_) {m_velocity = velocity_;}
  void mod_velocity(const Zeni::Vector3f &velocity_) {m_velocity += velocity_;}
  const int & get_health() const {return m_health;}
  void modify_health(const int &health_) {m_health += health_;}
  void set_health(const int &health_) {m_health = health_;}
  void set_speed(const int &speed_) {m_speed = speed_;}
  const float & get_speed() const {return m_speed;}
  virtual void step(const float &time_step);
  virtual void execute_step(const float &time_step);
  const bool & is_grounded() {return m_is_on_ground;}
  void set_on_ground(const bool &grounded_);
  
private:
  Zeni::Vector3f m_velocity;
  int m_health;
  float m_speed;
  bool m_is_on_ground;
  
};

class Player: public Object_Dynamic {
public:
  Player(Zeni::Model * model_,
         const Zeni::Point3f &position_,
         const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
         const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Object_Dynamic(model_, position_, scale_, rotation_),
  m_direction(Zeni::Vector3f(1.0f,0.0f,0.0f)),
  m_camera(Zeni::Camera(Zeni::Point3f(),
                        Zeni::Quaternion(),
                        1.0f, 10000.0f))
  {
    m_camera.fov_rad = Zeni::Global::pi / 3.0f;

    update_camera();
  }
  
  void set_position(const Zeni::Point3f &position_);
  void turn_left_xy(const float &theta);
  void update_camera();
  void execute_step(const float &time_step);
  
  const Zeni::Collision::Infinite_Cylinder & get_move_body() const {return m_move_body;}
  const Zeni::Collision::Sphere & get_sphere_body() const {return m_sphere;}
  const Zeni::Vector3f & get_direction() const {return m_direction;}
  const Zeni::Camera & get_camera() const {return m_camera;}
  const Zeni::Point3f get_corner() const;
  void create_bodies();
  void jump();
  void set_camera(const Zeni::Point3f &newlocation = Zeni::Point3f(0.0f,0.0f,0.0f));
private:
  Zeni::Vector3f m_direction;
  Zeni::Camera m_camera;
  Zeni::Collision::Infinite_Cylinder m_move_body;
  Zeni::Collision::Sphere m_sphere;

  
};

class Enemy: public Object_Dynamic {
public:
  Enemy(Zeni::Model * model_,
        const Zeni::Point3f &position_,
        const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
        const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Object_Dynamic(model_, position_, scale_, rotation_)
  {}

};

class Environment_Dynamic: public Object_Dynamic {
public:
  Environment_Dynamic(Zeni::Model * model_,
                      const Zeni::Point3f &position_,
                      const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
                      const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Object_Dynamic(model_, position_, scale_, rotation_)
  {}

};

class Goal_Dynamic: public Object_Dynamic {
public:
  Goal_Dynamic(Zeni::Model * model_,
               const Zeni::Point3f &position_,
               const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
               const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Object_Dynamic(model_, position_, scale_, rotation_)
  {
    create_body(get_corner());
  }
  
};

class Pickup: public Goal_Dynamic {
public:
  Pickup(const Zeni::Point3f &position_,
         const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
         const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Goal_Dynamic(new Zeni::Model("models/pickup.3ds"), position_, scale_, rotation_)
  {}
  ActionType on_player();
};

class Button: public Object_Dynamic {
public:
  Button(const ButtonColor &color_,
         const Zeni::Point3f &position_,
         const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
         const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Object_Dynamic(new Zeni::Model("models/button_nocolor.3ds"), position_, scale_, rotation_),
  m_color(color_)
  {
    if (color_ == redButton)
      set_model(new Zeni::Model("models/button_red.3ds"));
    else if (color_ == blueButton)
      set_model(new Zeni::Model("models/button_blue.3ds"));
    else if (color_ == greenButton)
      set_model(new Zeni::Model("models/pickup.3ds"));
    create_body(get_corner());
  }
  ActionType on_player();
  ButtonColor get_active_color() {return m_active_color;}
  ButtonColor get_color() {return m_color;}

private:
  static ButtonColor m_active_color;
  ButtonColor m_color;
};

class ColorBlock: public Button {
public:
  ColorBlock(const ButtonColor &color_,
             const Zeni::Point3f &position_,
             const Zeni::Vector3f &scale_,
             const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Button(color_, position_, scale_, rotation_)
  {
    create_body(get_corner());
  }
  
  void perform_logic(const float &time_step);
  ActionType on_player() {return doCollide;}
  void create_body(const Zeni::Point3f &corner_);
  void render();
};


class Deathwall: public Environment_Dynamic {
public:
  Deathwall(const Zeni::Point3f &position_,
            const Zeni::Vector3f &scale_ = Zeni::Vector3f(1.0f,1.0f,1.0f),
            const Zeni::Quaternion &rotation_ = Zeni::Quaternion::Axis_Angle(Zeni::Vector3f(0.0f,0.0f,1.0f), 0.0f))
  : Environment_Dynamic(new Zeni::Model("models/button_nocolor.3ds"), position_, scale_, rotation_)
  {
    set_speed(30.0f);
    set_velocity(Zeni::Vector3f(get_speed(),0.0f,0.0f));
  }
  
  ActionType on_player();
  void perform_logic(const float &time_step);
  void render();
};

#endif /* defined(__game__game_elements__) */
