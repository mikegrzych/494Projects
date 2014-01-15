//
//  Demo_Levels.cpp


#include <zenilib.h>
#include <Demo_State.h>

using namespace Zeni;

void Demo_State::load_demo_level()
{
  Point3f origin_ = m_next_room;
  Point3f room_door_ = Point3f(256.0f,0.0f,0.0f);
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(-128.0f,-64.0f,-1.0f),
                                              Vector3f(128.0f,128.0f,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f, -64.0f, -1.0f),
                                              Vector3f(256.0f, 128.0f, 1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(72.0f, -16.0f, 0.0f),
                                              Vector3f(32.0f, 32.0f, 32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(72.0f, -16.0f, 32.0f),
                                              Vector3f(32.0f, 32.0f, 32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(72.0f, -16.0f, 64.0f),
                                              Vector3f(32.0f, 32.0f, 32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(104.0f, -16.0f, 32.0f),
                                              Vector3f(32.0f, 32.0f, 32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(136.0f, -16.0f, 0.0f),
                                              Vector3f(32.0f, 32.0f, 32.0f)));
  m_dynamics.push_front(new Pickup(origin_ + Point3f(84.0f, -4.0f, 108.0f),
                                   Vector3f(8.0f, 8.0f, 8.0f)));
  
  m_next_room = origin_ + room_door_;
}

void Demo_State::load_easy_0()
{
  int mirror = 1;
  Random _rng = Random(m_chrono.seconds());
  if (_rng.frand_lt() >= 0.5f)
    mirror = -1;
  
  Point3f origin_ = m_next_room;
  Point3f room_door_ = Point3f(256.0f,160.0f * mirror,64.0f);
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,0.0f * mirror,-1.0f),
                                              Vector3f(256.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,-128.0f * mirror,-1.0f),
                                              Vector3f(256.0f,128.0f * mirror,1.0f)));
//  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
//                                              origin_ + Point3f(0.0f,-128.0f * mirror,-1.0f),
//                                              Vector3f(256.0f,256.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,-128.0f * mirror,-1.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(32.0f,-128.0f * mirror,32.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(64.0f,-128.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(64.0f,-32.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(64.0f,64.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(64.0f,160.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(160.0f,160.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(224.0f,160.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new Pickup(origin_ + Point3f(76.0f,76.0f * mirror,108.0f),
                                   Vector3f(8.0f,8.0f * mirror,8.0f)));
  m_next_room = origin_ + room_door_;
}

void Demo_State::load_easy_1()
{
  int mirror = 1;
  Random _rng = Random(m_chrono.seconds());
  if (_rng.frand_lt() >= 0.5f)
    mirror = -1;
  
  Point3f origin_ = m_next_room;
  Point3f room_door_ = Point3f(512.0f,-128.0f * mirror,0.0f);
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,-64.0f * mirror,-1.0f),
                                              Vector3f(256.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f,64.0f * mirror,-1.0f),
                                              Vector3f(128.0f,-128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f,-64.0f * mirror,-1.0f),
                                              Vector3f(256.0f,-128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(64.0f, 128.0f * mirror,0.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(64.0f, 196.0f * mirror,32.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(64.0f, 224.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(160.0f, 224.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f, 320.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new Pickup(origin_ + Point3f(268.0f,332.0f * mirror,108.0f),
                                   Vector3f(8.0f,8.0f * mirror,8.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f, 320.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  
  m_next_room = origin_ + room_door_;
}

void Demo_State::load_easy_2()
{
  int mirror = 1;
  Random _rng = Random(m_chrono.seconds());
  if (_rng.frand_lt() >= 0.5f)
    mirror = -1;
  mirror = 1;
  
  Point3f origin_ = m_next_room;
  Point3f room_door_ = Point3f(836.0f,-64.0f * mirror,96.0f);
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f, -256.0f * mirror,-1.0f),
                                              Vector3f(128.0f,256.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f, 0 * mirror,-1.0f),
                                              Vector3f(128.0f,256.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(196.0f, 16.0f * mirror,0.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(196.0f, 112.0f * mirror,0.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(196.0f, 208.0f * mirror,0.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(292.0f, -256.0f * mirror,63.0f),
                                              Vector3f(128.0f,256.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(292.0f, 0 * mirror,63.0f),
                                              Vector3f(128.0f,256.0f * mirror,1.0f)));
  m_dynamics.push_front(new Pickup(origin_ + Point3f(342.0f, 220.0f * mirror, 76.0f),
                                   Vector3f(8.0f,8.0f * mirror,8.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(484.0f, -272.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(484.0f, -176.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(484.0f, -80.0f * mirror,64.0f),
                                              Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(580.0f, -256.0f * mirror,95.0f),
                                              Vector3f(256.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(580.0f, -128.0f * mirror,95.0f),
                                              Vector3f(128.0f,256.0f * mirror,1.0f)));
  m_next_room = origin_ + room_door_;
}

void Demo_State::load_medium_0()
{
  int mirror = 1;
  Random _rng = Random(m_chrono.seconds());
  if (_rng.frand_lt() >= 0.5f)
    mirror = -1;
//  mirror = 1;
  
  Point3f origin_ = m_next_room;
  Point3f room_door_ = Point3f(384.0f,0.0f * mirror,128.0f);
  int reward_loc_ = _rng.rand_lt(11);
  
  m_dynamics.push_front(new ColorBlock(blueButton,
                                      origin_ + Point3f(0.0f, -336.0f * mirror,-1.0f),
                                      Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 0)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(44.0f, -292.0f * mirror, 8.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f, -144.0f * mirror,-1.0f),
                                              Vector3f(96.0f,96.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(redButton,
                                   origin_ + Point3f(32.0f,-112.0f * mirror, 0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  if (reward_loc_ == 1)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(44.0f, -100.0f * mirror, 8.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f, 48.0f * mirror,-1.0f),
                                              Vector3f(96.0f,96.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(blueButton,
                                   origin_ + Point3f(32.0f,80.0f * mirror, 0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  if (reward_loc_ == 2)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(44.0f, 92.0f * mirror, 8.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(redButton,
                                      origin_ + Point3f(0.0f, 240.0f * mirror,-1.0f),
                                      Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 3)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(44.0f, 284.0f * mirror, 8.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(redButton,
                                      origin_ + Point3f(96.0f, -240.0f * mirror,31.0f),
                                      Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 4)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(140.0f, -196.0f * mirror, 40.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(96.0f, -48.0f * mirror,31.0f),
                                       Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 5)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(140.0f, -4.0f * mirror, 40.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(96.0f, 144.0f * mirror,31.0f),
                                       Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 6)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(140.0f, 188.0f * mirror, 40.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(192.0f, -336.0f * mirror,63.0f),
                                       Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 7)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(236.0f, -292.0f * mirror, 72.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(192.0f, -144.0f * mirror,63.0f),
                                       Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 8)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(236.0f, -100.0f * mirror, 72.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(192.0f, 48.0f * mirror,63.0f),
                                       Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 9)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(236.0f, 92.0f * mirror, 72.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(192.0f, 240.0f * mirror,63.0f),
                                       Vector3f(96.0f,96.0f * mirror,1.0f)));
  if (reward_loc_ == 10)
    m_dynamics.push_front(new Pickup(origin_ + Point3f(236.0f, 284.0f * mirror, 72.0f),
                                     Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(288.0f, -48.0f * mirror,95.0f),
                                              Vector3f(96.0f,96.0f * mirror,1.0f)));
  m_next_room = origin_ + room_door_;
}

void Demo_State::load_medium_1()
{
  int mirror = 1;
  Random _rng = Random(m_chrono.seconds());
  if (_rng.frand_lt() >= 0.5f)
    mirror = -1;
  
  Point3f origin_ = m_next_room;
  Point3f room_door_ = Point3f(512.0f,0.0f * mirror,0.0f);

  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,-64.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(384.0f,-64.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(redButton,
                                   origin_ + Point3f(8.0f,-64.0f * mirror,0.0f),
                                   Vector3f(112.0f,128.0f * mirror,8.0f)));
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(128.0f,0.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f,0.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(128.0f,-128.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f,-128.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(128.0f,-256.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f,-256.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(128.0f, -128.0f * mirror, 0.0f),
                                       Vector3f(256.0f,4.0f * mirror,96.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(128.0f, -256.0f * mirror, 0.0f),
                                       Vector3f(4.0f,128.0f * mirror,96.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(384.0f, -256.0f * mirror, 0.0f),
                                       Vector3f(4.0f,128.0f * mirror,96.0f)));

  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(128.0f,416.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(256.0f,416.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(blueButton,
                                       origin_ + Point3f(300.0f,464.0f * mirror,0.0f),
                                       Vector3f(32.0f,32.0f * mirror,8.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(160.0f,160.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(160.0f,224.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(160.0f,288.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(160.0f,352.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(272.0f,160.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(336.0f,224.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(272.0f,288.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(336.0f,352.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(160.0f,-240.0f * mirror,0.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(224.0f,-240.0f * mirror,32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(224.0f,-176.0f * mirror,64.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(160.0f,-176.0f * mirror,96.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(160.0f,-112.0f * mirror,128.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(160.0f,-48.0f * mirror,128.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(128.0f,16.0f * mirror,127.0f),
                                              Vector3f(64.0f,64.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(196.0f,144.0f * mirror,127.0f),
                                              Vector3f(64.0f,64.0f * mirror,1.0f)));
  m_dynamics.push_front(new Pickup(origin_ + Point3f(224.0f,172.0f * mirror,136.0f),
                                   Vector3f(8.0f,8.0f * mirror,8.0f)));

  m_next_room = origin_ + room_door_;
  
}

void Demo_State::load_medium_2()
{
  int mirror = 1;
  Random _rng = Random(m_chrono.seconds());
  if (_rng.frand_lt() >= 0.5f)
    mirror = -1;
  
  Point3f origin_ = m_next_room;
  Point3f room_door_ = Point3f(608.0f,-128.0f * mirror,0.0f);
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,64.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(redButton,
                                   origin_ + Point3f(48.0f,112.0f * mirror,0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,-64.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(greenButton,
                                   origin_ + Point3f(48.0f,-16.0f * mirror,0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(0.0f,-192.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(blueButton,
                                   origin_ + Point3f(48.0f,-144.0f * mirror,0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  
//  m_dynamics.push_front(new ColorBlock(greenButton,
//                                       origin_ + Point3f(160.0f,80.0f,-32.0f),
//                                       Vector3f(32.0f,32.0f,32.0f)));
  m_dynamics.push_front(new ColorBlock(greenButton,
                                       origin_ + Point3f(288.0f,-176.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(160.0f,-176.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(288.0f,-48.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(160.0f,80.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(288.0f,80.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(352.0f,64.0f * mirror,-1.0f),
                                              Vector3f(256.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(352.0f,-64.0f * mirror,-1.0f),
                                              Vector3f(128.0f,128.0f * mirror,1.0f)));
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(352.0f,-192.0f * mirror,-1.0f),
                                              Vector3f(256.0f,128.0f * mirror,1.0f)));
  m_dynamics.push_front(new Button(noColor,
                                   origin_ + Point3f(352.0f,-64.0f * mirror,0.0f),
                                   Vector3f(128.0f,128.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new Button(redButton,
                                   origin_ + Point3f(400.0f,80.0f * mirror,0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  m_dynamics.push_front(new Button(greenButton,
                                   origin_ + Point3f(464.0f,80.0f * mirror,0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  m_dynamics.push_front(new Button(blueButton,
                                   origin_ + Point3f(528.0f,80.0f * mirror,0.0f),
                                   Vector3f(32.0f,32.0f * mirror,8.0f)));
  
  m_dynamics.push_front(new ColorBlock(redButton,
                                       origin_ + Point3f(464.0f,256.0f * mirror,-32.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(greenButton,
                                       origin_ + Point3f(464.0f,256.0f * mirror, 48.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  m_dynamics.push_front(new ColorBlock(blueButton,
                                       origin_ + Point3f(464.0f,256.0f * mirror,-64.0f),
                                       Vector3f(32.0f,32.0f * mirror,32.0f)));
  
  
  m_statics.push_front(new Environment_Static(new Model("models/crate.3ds"),
                                              origin_ + Point3f(416.0f,416.0f * mirror,-1.0f),
                                              Vector3f(64.0f,64.0f * mirror,1.0f)));
  m_dynamics.push_front(new Pickup(origin_ + Point3f(444.0f,444.0f * mirror,8.0f),
                                   Vector3f(8.0f,8.0f * mirror,8.0f)));
  
  m_next_room = origin_ + room_door_;
 
}