//
//  game_elements.h
//
//  Created by Michael on 9/22/13.
//
//  Game element classes, including players, towers, bullets, enemies, etc.
//

#ifndef __game__game_elements__
#define __game__game_elements__

#include <zenilib.h>
#include <algorithm>

using namespace Zeni;

enum Faction {PLAYER, ENEMY, NEUTRAL};
enum SHAPES {BOX, CIRCLE};
enum COLLIDERTYPES {MOVABLE, IMMOVABLE, EFFECT};
enum POWERUPS {AMMO, ARMOR, HEALTH, DAMAGE, SPEED, MULTI, PIERCE};
enum POWERUP_INTERVALS {AMMO_INTER = 20, ARMOR_INTER = 32, HEALTH_INTER = 48,
  DAMAGE_INTER = 70, SPEED_INTER = 82, MULTI_INTER = 91, PIERCE_INTER = 100};

class Game_Object {
public:
  Game_Object(const Point2f &position_,
              const Vector2f &size_,
              const Faction &faction_,
              const SHAPES &shape_,
              const COLLIDERTYPES &collider_,
              const int &health_,
              const float &theta_);
  virtual void render() const = 0; // pure virtual function call
  virtual ~Game_Object();
  virtual void move_vert(const float &move_);
  virtual void move_horiz(const float &move_);
  virtual void move_forward(const float &move_);
  virtual void move(const Vector2f &move_);
  virtual void modify_size(const Vector2f &new_size_) {m_size = new_size_;}
  virtual void set_health_max(const int &health_) {m_health_max = health_;}
  virtual void mod_health(const int &health_);
  virtual const int & get_health() const {return m_health;}
  virtual const int & get_health_max() const {return m_health_max;}
  virtual void take_damage(const int &amount_) {mod_health(-amount_);}
  virtual void set_theta(const float &theta_) {m_theta = theta_;}
  virtual const Point2f & get_position() const {return m_position;}
  virtual const Vector2f & get_size() const {return m_size;}
  virtual const float & get_theta() const {return m_theta;}
  virtual const float get_radius() const {return 0.5f * m_size.x;}
  virtual void set_speed(const float &speed_) {m_speed = speed_;}
  virtual const float & get_speed() const {return m_speed;}
  virtual const Faction & get_faction() const {return m_faction;}
  virtual const SHAPES & get_shape() const {return m_shape;}
  virtual const COLLIDERTYPES & get_collider() const {return m_collider;}
  virtual void set_invincible(const float &time_) {m_invincibility = time_;}
  virtual void update_invincible(const float &time_step_) {m_invincibility = std::max(0.0f, m_invincibility - time_step_);}
  virtual const bool is_invincible() const {return (m_invincibility > 0.0f);}
  
protected:
  void render(const String &texture, const Color &filter) const;
  
private:
  float m_invincibility;
  Point2f m_position; // Upper left corner
  Vector2f m_size; // (width, height)
  float m_theta;
  Faction m_faction;
  SHAPES m_shape;
  COLLIDERTYPES m_collider;
  float m_speed;
  Chronometer<Time> m_chrono;
  float m_time_passed;
  int m_health;
  int m_health_max;
  void perform_logic();
};

class PowerUp : public Game_Object {
public:
  PowerUp(const Point2f &position_,
          const int &value_);
  const POWERUPS & get_type() {return type;}
  void render() const;
private:
  POWERUPS type;
};

class Bullet : public Game_Object {
public:
  Bullet(const Point2f &position_,
         const Vector2f &size_,
         const float &speed_,
         const int &damage_,
         const Faction &faction_,
         const float &theta_,
         const int &pierce_);
  void render() const;
  void set_cast_size(const Vector2f &new_size_) {modify_size(new_size_);}
  const Point2f & get_position() const {return this->Game_Object::get_position();}
  const Vector2f & get_size() const {return this->Game_Object::get_size();}
  const Vector2f & get_sprite_size() {return m_sprite_size;}
  const float get_radius() const {return this->Game_Object::get_radius();}
  const int & get_damage() {return m_damage;}
  void mod_pierce(const int &amount_) {m_pierce += amount_;}
  const int & get_pierce() {return m_pierce;}
private:
  Vector2f m_sprite_size;
  int m_damage;
  int m_pierce;
};

class Tower : public Game_Object {
public:
  Tower(const Point2f &position_,
        const Vector2f &size_,
        const int &damage_,
        const int &health_);
  void perform_logic(const float &time_step_);
  const bool can_fire() {return (m_till_next_fire <= 0.0f);}
  Bullet * fire(const Vector2f &direction_);
  void render() const;
  
private:
  int m_damage;
  float m_range;
  float m_till_next_fire;
  Game_Object * m_target;
  Game_Object * find_target(const std::list<Game_Object *> &_objects);
};

class Player : public Game_Object {
public:
  Player(const Point2f &position_,
         const Vector2f &size_,
         const Faction &faction_,
         const float &theta_);
  const Vector2f & get_sprite_size() const {return m_sprite_size;}
  const Point2f & get_position() const {return this->Game_Object::get_position();}
  const Vector2f & get_size() const {return this->Game_Object::get_size();}
  const float get_radius() const {return this->Game_Object::get_radius();}
  void rotate(const Point2f mouse);
  void render() const;
  void set_armor(const int &amount_);
  void mod_armor(const int &amount_);
  int get_armor();
  void set_ammo_max(const int &amount_);
  void mod_ammo(const int &amount_);
  int get_ammo();
  int get_ammo_max();
  void restock_ammo(const float &time_);
  void take_damage(const int &amount_) {mod_health(-std::max(1,amount_ - m_armor));}
  void mod_damage(const int &amount_);
  void use_powerup(const POWERUPS &type_);
  const int & get_multi_bonus() {return m_multi_bonus;}
  const float & get_speed_bonus() {return m_speed_bonus;}
  Bullet * fire(const float &theta_mod_) const;
  Tower * createTower(const Point2f &loc_) const;
private:
  Vector2f m_sprite_size;
  float m_ammo_time;
  float m_restock_speed;
  int m_armor;
  int m_ammo;
  int m_ammo_max;
  int m_damage;
  float m_speed_bonus;
  int m_multi_bonus;
  int m_pierce_bonus;
};

class BuildChecker : public Game_Object {
public:
  BuildChecker(const Point2f &position_,
               const Vector2f &size_);
  void render() const {};
};

class Wall : public Game_Object {
public:
  Wall(const Point2f &position_,
       const Vector2f &size_,
       const Faction &faction_,
       const bool &orient_);
  const Point2f & get_position() const {return this->Game_Object::get_position();}
  const Vector2f & get_size() const {return this->Game_Object::get_size();}
  const float get_radius() const {return this->Game_Object::get_radius();}
  void render() const;
private:
  bool m_vert_orient;
};

class EnemyWeak : public Game_Object {
public:
  EnemyWeak(const Point2f &position_,
            const float &power_multi_);
  void render() const;
  int get_damage() {return m_attack;}
private:
  int m_attack;
};

class EnemySpawner : public Game_Object {
public:
  EnemySpawner(const Point2f &position_,
               const Vector2f &size_,
               const Vector2f &spawn_loc_,
               const float &theta_,
               const Uint32 &seed_);
  void update(const float &time_step_);
  bool will_spawn();
  EnemyWeak * createEnemy();
  void render() const;
private:
  Vector2f m_spawn_loc;   //spawn location relative to center
  Random m_rng;
  bool m_can_spawn;
  float m_spawn_interval;
  float m_spawn_delta;
  float m_spawn_power;
  int m_spawned;
};


#endif /* defined(__game__game_elements__) */
