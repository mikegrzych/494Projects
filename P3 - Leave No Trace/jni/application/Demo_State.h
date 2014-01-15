//
//  Demo_State.h


#ifndef __game__Demo_State__
#define __game__Demo_State__

#include "game_elements.h"
#include "utility_funcs.h"
#include <zenilib.h>
#include <list>

class Demo_State : public Zeni::Gamestate_Base {
  struct Controls {
    Controls() : forward(false), left(false), back(false), right(false) {}
    
    bool forward;
    bool left;
    bool back;
    bool right;
  } m_controls;
  
public:
  Demo_State();
  void on_push();
  void on_pop();

void on_key(const SDL_KeyboardEvent &event);
void on_mouse_motion(const SDL_MouseMotionEvent &event);

  void perform_logic();

  void render();

private:
  void partial_step(const float &time_step, const Zeni::Vector3f &velocity);
//  Zeni::Time_HQ time_passed;
  Zeni::Chronometer<Zeni::Time_HQ> m_chrono;
  Zeni::Chronometer<Zeni::Time_HQ> m_gameover;
  Zeni::Light m_light_ambient;
  Zeni::Point3f m_next_room;
//  Environment_Static m_ground;
  Player * m_player;
  std::list<Object_Static *> m_statics;
  std::list<Object_Dynamic *> m_dynamics;

  bool m_moved;
  float m_time_passed;
  int m_levels_beat;
  bool m_game_over;
  
  void load_demo_level();
  void load_easy_0();
  void load_easy_1();
  void load_easy_2();
  void load_medium_0();
  void load_medium_1();
  void load_medium_2();
  void load_hard_0();
  void load_hard_1();
  void load_hard_2();


};

#endif /* defined(__game__Demo_State__) */
