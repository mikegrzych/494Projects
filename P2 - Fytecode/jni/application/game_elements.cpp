//
//  game_elements.cpp
//
//  Created by Michael on 9/22/13.
//
//  Game element classes, including players, towers, bullets, enemies, etc.
//

#include "game_elements.h"
#include "collisions.h"
#include <zenilib.h>

using namespace std;
using namespace Zeni;

Game_Object::Game_Object(const Point2f &position_,
                         const Vector2f &size_,
                         const Faction &faction_ = NEUTRAL,
                         const SHAPES &shape_ = BOX,
                         const COLLIDERTYPES &collider_ = MOVABLE,
                         const int &health_ = 0,
                         const float &theta_ = 0.0f)
: m_invincibility(0.0f),
m_position(position_),
m_size(size_),
m_theta(theta_),
m_faction(faction_),
m_shape(shape_),
m_collider(collider_),
m_speed(0.0f),
m_health(health_),
m_health_max(health_)
{
}

void Game_Object::render(const String &texture, const Color &filter = Color()) const
{
  render_image(
               texture,
               m_position,
               m_position + m_size,
               m_theta,
               1.0f,
               m_position + 0.5f * m_size,
               false,
               filter);
}

Game_Object::~Game_Object() {}

void Game_Object::move_vert(const float &move_) {
  m_position.y += move_;
}

void Game_Object::move_horiz(const float &move_) {
  m_position.x += move_;
}

void Game_Object::move_forward(const float &move_) {
  m_position.x += move_ * cos(m_theta);
  m_position.y += move_ * -sin(m_theta);
}

void Game_Object::move(const Zeni::Vector2f &move_) {
  m_position += move_;
}

PowerUp::PowerUp(const Point2f &position_,
                 const int &value_)
: Game_Object(position_, Vector2f(32.0f,32.0f), NEUTRAL, BOX, EFFECT)
{
  if (value_ < AMMO_INTER)
    type = AMMO;
  else if (value_ < ARMOR_INTER)
    type = ARMOR;
  else if (value_ < HEALTH_INTER)
    type = HEALTH;
  else if (value_ < DAMAGE_INTER)
    type = DAMAGE;
  else if (value_ < SPEED_INTER)
    type = SPEED;
  else if (value_ < MULTI_INTER)
    type = MULTI;
  else
    type = PIERCE;
}

void PowerUp::render() const{
  String sprite[] = {"powerup_ammo","powerup_armor","powerup_health","powerup_damage",
    "powerup_speed","powerup_multi","powerup_pierce"};
  Game_Object::render(sprite[type]);
}

Bullet::Bullet(const Point2f &position_,
               const Vector2f &size_,
               const float &speed_,
               const int &damage_,
               const Faction &faction_,
               const float &theta_,
               const int &pierce_)
: Game_Object(position_, size_, faction_, BOX, EFFECT, 0, theta_),
m_sprite_size(size_),
m_damage(damage_),
m_pierce(pierce_)
{
  modify_size(Vector2f(size_.x,size_.x));
  set_speed(speed_);
}

void Bullet::render() const {
  render_image(
               "player_beam",
               get_position() + 0.5f * (get_size() - m_sprite_size),
               get_position() + 0.5f * (get_size() + m_sprite_size),
               get_theta(),
               1.0f,
               get_position() + 0.5f * get_size(),
               false,
               Color());
}

Player::Player(const Point2f &position_,
               const Vector2f &size_,
               const Faction &faction_,
               const float &theta_)
: Game_Object(position_, size_, faction_, BOX, MOVABLE, 100, theta_),
m_sprite_size(Vector2f(64.0f,64.0f)),
m_ammo_time(0.0f),
m_restock_speed(0.4f),
//m_health_max(100),
m_armor(0),
m_ammo(50),
m_ammo_max(50),
m_damage(5),
m_speed_bonus(1.0f),
m_multi_bonus(0),
m_pierce_bonus(0)
{
  set_speed(300.0f);
}

void Player::render() const
{
  Color filter = Color(1.0f,1.0f,1.0f,1.0f);
  if (this->is_invincible())
    filter.a = 0.5f;
  render_image(
               "player",
               get_position() + 0.5f * (get_size() - m_sprite_size),
               get_position() + 0.5f * (get_size() + m_sprite_size),
               get_theta(),
               1.0f,
               get_position() + 0.5f * get_size(),
               false,
               filter);
}

void Player::rotate(const Point2f mouse) {
  const Point2f position(get_position() + 0.5f * get_size());
  this->set_theta(atan2(position.y - mouse.y, -(position.x - mouse.x)));
}

void Game_Object::mod_health(const int &health_) {
  m_health = min(m_health + health_, m_health_max);
  if (m_health < 0) m_health = 0;
}

void Player::set_armor(const int &amount_) {m_armor = amount_;}

void Player::mod_armor(const int &amount_) {m_armor = max(m_armor + amount_, 0);}

int Player::get_armor() {return m_armor;}

void Player::set_ammo_max(const int &amount_) {
  m_ammo_max = amount_;
  m_ammo = amount_;
  m_ammo_time = 0.0f;
}

void Player::mod_ammo(const int &amount_) {
  m_ammo = min(m_ammo + amount_, m_ammo_max);
  if (m_ammo == m_ammo_max)
    m_ammo_time = 0.0f;
}

int Player::get_ammo() {return m_ammo;}

int Player::get_ammo_max() {return m_ammo_max;}

void Player::restock_ammo(const float &time_) {
  if (m_ammo < m_ammo_max) {
    m_ammo_time += time_ * m_speed_bonus;
    if (m_ammo_time >= m_restock_speed) {
      m_ammo_time -= m_restock_speed;
      mod_ammo(1);
    }
  }
}

void Player::mod_damage(const int &amount_) {
  m_damage = max(m_damage + amount_, 2);
}

void Player::use_powerup(const POWERUPS &type_) {
  switch(type_){
    case AMMO:
      set_ammo_max(get_ammo_max() + 5);
      mod_ammo(5);
      break;
    case ARMOR:
      mod_armor(2);
      break;
    case HEALTH:
      set_health_max(get_health_max() + 5);
      mod_health(5);
      break;
    case DAMAGE:
      mod_damage(1);
      break;
    case SPEED:
      m_speed_bonus += 0.2f;
      break;
    case MULTI:
      m_multi_bonus += 1;
      break;
    case PIERCE:
      m_pierce_bonus += 1;
      break;
    default: break;
  }
}

Bullet * Player::fire(const float &theta_mod_) const {
  const float radius = 1.0f * get_radius();
  const Vector2f bullet_size(16.0f, 8.0f);
  const Point2f position(get_position() +
                         // Player's upper-left coordinate
                         0.5f * (get_size() - bullet_size) +
                         //shift to the center of the Player
                         radius * Vector2f(cos(get_theta()), -sin(get_theta())));
                        // then shift in front of the Player
  return new Bullet(position, bullet_size, 600.0f, m_damage, PLAYER, get_theta()+theta_mod_, m_pierce_bonus);
}

Tower * Player::createTower(const Point2f &loc_) const {
  const Vector2f tower_size(32.0f,32.0f);
  return new Tower(loc_, tower_size, m_damage / 2, get_health_max() * 0.5f);
}

BuildChecker::BuildChecker(const Point2f &position_, const Vector2f &size_)
: Game_Object (position_, size_, NEUTRAL, BOX, IMMOVABLE)
{
}

Tower::Tower(const Point2f &position_,
                     const Vector2f &size_,
                     const int &damage_,
             const int &health_)
: Game_Object (position_, size_, PLAYER, BOX, IMMOVABLE, health_),
m_range(1000000.0f),
m_till_next_fire(0.0f),
m_damage(damage_)
{
}

void Tower::perform_logic(const float &time_step_) {
  m_till_next_fire = std::max(0.0f, m_till_next_fire - time_step_);
}

Bullet * Tower::fire(const Vector2f &direction_) {
  const Vector2f bullet_size(16.0f, 8.0f);
  const Point2f position(get_position() +
                         // Tower's upper-left coordinate
                         0.5f * (get_size() - bullet_size));
                         //shift to the center of the Tower
  m_till_next_fire = 2.0f;
  return new Bullet(position, bullet_size, 600.0f, m_damage, PLAYER, -direction_.theta(), 0);
}

void Tower::render() const
{
  Color filter = Color();
  filter.b = float(get_health())/float(get_health_max());
  filter.g = filter.b;
  filter.r = 2.0f * filter.b;
  Game_Object::render("tower",filter);
}

Wall::Wall(const Point2f &position_,
           const Vector2f &size_,
           const Faction &faction_,
           const bool &orient_)
: Game_Object(position_, size_, faction_, BOX, IMMOVABLE)
{
  m_vert_orient = orient_;
}

void Wall::render() const {
  if (m_vert_orient)
    Game_Object::render("wall_vert");
  else
    Game_Object::render("wall_hori");
}

EnemySpawner::EnemySpawner(const Point2f &position_, const Vector2f &size_, const Vector2f &spawn_loc_, const float &theta_, const Uint32 &seed_)
: Game_Object(position_, size_, ENEMY, CIRCLE, EFFECT, 0, theta_)
{
  m_spawn_loc = spawn_loc_;
  m_rng = Random(seed_);
  m_can_spawn = false;
  m_spawn_delta = 0.0f;
  m_spawn_interval = 4.0f;
  m_spawn_power = 1.0f;
  m_spawned = 0;
}

void EnemySpawner::update(const float &time_step_) {
  m_spawn_delta += m_spawn_power * time_step_;

  if (m_spawned >= 3) {
    m_spawned = 0;
    m_spawn_interval -= 0.5f;
  }
  if (m_spawn_interval < 2.0f) {
    m_spawn_interval = 5.0f;
    m_spawn_power += 0.25f;
  }
    if (!m_can_spawn && m_spawn_delta > m_spawn_interval) {
    m_can_spawn = true;
  }
}

bool EnemySpawner::will_spawn() {
  return (m_can_spawn && 0.75f * m_spawn_power > m_rng.frand_lte());
}

EnemyWeak * EnemySpawner::createEnemy() {
  ++m_spawned;
  m_spawn_delta = 0.0f;
  m_can_spawn = false;
//  Vector2f location = get_position() + 0.5 * get_size() + m_spawn_loc * m_spawn_power;
  return new EnemyWeak(get_position() + 0.5f * get_size() + m_spawn_loc * m_spawn_power, m_spawn_power);
}



void EnemySpawner::render() const {
  render_image(
               "spawner",
               get_position() + 0.5f * get_size(),
               get_position() + 0.5f * get_size() + Vector2f(256.0f,256.0f),
               get_theta(),
               1.0f,
               get_position() + 0.5f * get_size(),
               false);
}

EnemyWeak::EnemyWeak(const Point2f &position_, const float &power_multi_)
: Game_Object(position_, Vector2f(32.0f,32.0f)*power_multi_, ENEMY, BOX, MOVABLE, std::pow(10,power_multi_))
{
  set_speed(80.0f * power_multi_);
  m_attack = std::pow(5,power_multi_);
}

void EnemyWeak::render() const {
  Game_Object::render("enemy1");
}