/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
#include "Demo_State.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);
  
public:
  Instructions_State()
  : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }
  
private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }
  
  void render() {
    Widget_Gamestate::render();
    
    Zeni::Font &title = get_Fonts()["title"];
    Zeni::Font &info_text = get_Fonts()["info"];
    Zeni::Color &text_color = get_Colors()["system_font"];
    
    title.render_text("INSTRUCTIONS",
                      Point2f(400.0f, 16.0f),
                      text_color,
                      ZENI_CENTER);

    info_text.render_text("You are a chronic litterer! Your habits are so awful that\n"
                          "your own dreams have turns against you! Now you must try\n"
                          "to escape the nightmare! MOVE using W,A,S,D, JUMP with\n"
                          "SPACEBAR, and turn using your MOUSE!\n"
                          "Collect GREEN CUBES to move forward in the nightmare!\n"
                          "Step on RED, GREEN, and BLUE BUTTONS to reveal blocks!\n"
                          "Step on BLACK BUTTONS to hide all colored blocks!\n\n"
                          "Be fast, before the nightmare consumes you!",
                          Point2f(40.0f, 100.0f),
                          text_color,
                          ZENI_LEFT);
    info_text.render_text("Press ESC to PAUSE the game, and "
#if defined(_WINDOWS)
                          "ALT+F4"
#elif defined(_MACOSX)
                          "Apple+Q"
#else
                          "Ctrl+Q"
#endif
                          " to QUIT.\n Press ESC to exit this screen.",
                          Point2f(400.0f, 450.0f),
                          text_color,
                          ZENI_CENTER);
    
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("zenilib Application");

      get_Controllers();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();

      return new Title_State<Demo_State, Instructions_State>("Leave No Trace!");
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
