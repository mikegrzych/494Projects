//
//  bootstrap.cpp
//
//  Last edited by Mike Grzych on 10/9
//
//  Bootstrap file that contains play loop and instructions.
//  The code here is messier than Whitney Houston's flat and I apologize
//  in advance to anyone who dares to try and read it. You have been warned.
//
//  TRIGGER WARNING: Gratuitous "magic constants," nested for() loops,
//                    copious branch statements, and >80char lines
//

#include <zenilib.h>
#include "general_funcs.h"
#include "game_elements.h"
#include "collisions.h"
#include <iostream>
#include <algorithm>

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_Base {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);
  
public:
  Play_State()
  :m_time_passed(0.0f),
  m_up(false),
  m_down(false),
  m_left(false),
  m_right(false),
  m_fire(false),
  m_build(false),
  levelSize(1280.0f,960.0f),
  m_rng(Random(1696123206209))
  {
    cam_size = Vector2f(800.0f,600.0f);
    set_pausable(true);
    m_player = new Player(Point2f(288.0f, 148.0f), Vector2f(48.0f, 48.0f), PLAYER, Global::pi * 1.5f);
    
    EnemySpawner * spawnNW = new EnemySpawner(Point2f(-240.0f,-240.0f), Vector2f(480.0f,480.0f), Vector2f(70.0f,70.0f), 0.0f, 123456);
    EnemySpawner * spawnNE = new EnemySpawner(Point2f(levelSize.x-240.0f,-240.0f), Vector2f(480.0f,480.0f), Vector2f(-70.0f, 70.0f), -Global::pi_over_two, 234567);
    EnemySpawner * spawnSE = new EnemySpawner(Point2f(levelSize.x-240.0f,levelSize.y-240.0f), Vector2f(480.0f,480.0f), Vector2f(-70.0f,-70.0f), Global::pi, 345678);
    EnemySpawner * spawnSW = new EnemySpawner(Point2f(-240.0f,levelSize.y-240.0f), Vector2f(480.0f,480.0f), Vector2f(70.0f, -70.0f), Global::pi_over_two, 456789);
    
    Wall * wallN = new Wall(Point2f(0.0f,0.0f), Vector2f(levelSize.x, 32.0f), NEUTRAL, 0);
    Wall * wallE = new Wall(Point2f(levelSize.x - 32.0f, 0.0f), Vector2f(32.0f, levelSize.y), NEUTRAL, 1);
    Wall * wallW = new Wall(Point2f(0.0f,0.0f), Vector2f(32.0f, levelSize.y), NEUTRAL, 1);
    Wall * wallS = new Wall(Point2f(0.0f,levelSize.y-32.0f), Vector2f(levelSize.x, 32.0f), NEUTRAL, 0);
    
    m_players.push_back(m_player);
    
    m_environment.push_back(wallN);
    m_environment.push_back(wallE);
    m_environment.push_back(wallW);
    m_environment.push_back(wallS);
    
    m_spawners.push_back(spawnNW);
    m_spawners.push_back(spawnNE);
    m_spawners.push_back(spawnSE);
    m_spawners.push_back(spawnSW);
  }
                                              
                                              
  
  ~Play_State() {
    for(list<Game_Object *>::iterator obj = m_players.begin(); obj != m_players.end(); ++obj) {
      delete *obj;
    }
    for(list<Game_Object *>::iterator obj = m_environment.begin(); obj != m_environment.end(); ++obj) {
      delete *obj;
    }
    for(list<EnemySpawner *>::iterator obj = m_spawners.begin(); obj != m_spawners.end(); ++obj) {
      delete *obj;
    }
    for(list<EnemyWeak *>::iterator obj = m_enemies.begin(); obj != m_enemies.end(); ++obj) {
      delete *obj;
    }
    for(list<Bullet *>::iterator obj = m_bullets.begin(); obj != m_bullets.end(); ++obj) {
      delete *obj;
    }
  }
  
private:
  Chronometer<Time> m_chrono;
  Chronometer<Time> m_gameover;
  Vector2f cam_size;
  float m_time_passed;
  Projector2D m_projector;
  Point2f m_mousePos;
  Player * m_player;
  bool m_up;
  bool m_down;
  bool m_left;
  bool m_right;
  bool m_fire;
  bool m_build;
  Point2f levelSize;
  Random m_rng;
  list<Game_Object *> m_environment;
  list<Tower *> m_towers;
  list<EnemySpawner *> m_spawners;
  list<EnemyWeak *> m_enemies;
  list<Game_Object *> m_players;
  list<PowerUp *> m_powerups;
  list<Bullet *> m_bullets;
  
  
  void on_push() {
    get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
    m_chrono.start();
  }
  
  void on_pop() {
    get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
    m_chrono.stop();
  }
  
  void on_key(const SDL_KeyboardEvent &event) {
    if (event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_p) {
      get_Game().push_Popup_Pause_State();
    } else if (event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_ESCAPE) {
      get_Game().push_Popup_Menu_State();
    } else {
      switch(event.keysym.sym) {
        case SDLK_w:
          m_up = event.type == SDL_KEYDOWN;
          break;
        case SDLK_a:
          m_left = event.type == SDL_KEYDOWN;
          break;
        case SDLK_s:
          m_down = event.type == SDL_KEYDOWN;
          break;
        case SDLK_d:
          m_right = event.type == SDL_KEYDOWN;
          break;
      }
    }
  }
  
  void on_mouse_button(const SDL_MouseButtonEvent &event) {
    if (event.button == 1) {
      m_fire = event.type == SDL_MOUSEBUTTONDOWN;
    }
    if (event.button == 3) {
      m_build = event.type == SDL_MOUSEBUTTONDOWN;
    }
  }
  
  void on_mouse_motion(const SDL_MouseMotionEvent &event) {
    m_mousePos = Point2f(event.x, event.y);
  }
  
  void perform_logic() {
    if (m_player->get_health() <= 0) {
      if (m_gameover.is_running()) {
        if (m_gameover.seconds() > 10.0f)
          get_Game().pop_state();
      }
      else {
        m_chrono.stop();
        m_gameover.start();
      }
      return;
    }
    
    const float time_passed = m_chrono.seconds();
    const float time_step = time_passed - m_time_passed;
    m_time_passed = time_passed;

    Point2f get_cam = fix_camera(m_player->get_position() + 0.5f * (m_player->get_size() - cam_size),
                                 m_player->get_position() + 0.5f * (m_player->get_size() + cam_size),
                                 cam_size, levelSize);
    m_projector = Projector2D(make_pair(get_cam, get_cam + cam_size), get_Video().get_viewport());
    
    
    // Enemy Spawning
    
    for(list<EnemySpawner *>::iterator sp = m_spawners.begin(); sp != m_spawners.end(); ++sp) {
      (*sp)->update(time_step);
      if ((*sp)->will_spawn()) {
        EnemyWeak * newEnemy = (*sp)->createEnemy();
        m_enemies.push_back(newEnemy);        
      }
    }
    
    // Enemy Movement
    
    for(list<EnemyWeak *>::iterator e = m_enemies.begin(); e != m_enemies.end(); ++e) {
      Vector2f direction = Vector2f(m_player->get_position() + 0.5 * m_player->get_size() - ((*e)->get_position() + 0.5*(*e)->get_size()) ).normalized();
      (*e)->move(direction * time_step * (*e)->get_speed());
      direction.y *= -1;
      (*e)->set_theta(direction.theta());
    }
    
    // Player Bullet Firing
    
    if(m_fire) {
      m_fire = false;
      if (m_player->get_ammo() > 0){
        m_player->mod_ammo(-1);
        m_bullets.push_back(m_player->fire(0.0f));
        float theta_mod;
        for (int i = 0; i < m_player->get_multi_bonus(); ++i){
          theta_mod = 0.17f * (1 + i/2);
          if (i % 2)
            theta_mod *= -1.0f;
          m_bullets.push_back(m_player->fire(theta_mod));
        }
      }
    }
    m_player->restock_ammo(time_step);
    for(list<Bullet *>::iterator it = m_bullets.begin(); it != m_bullets.end(); ++it){
      (*it)->move_forward(time_step * (*it)->get_speed());
    }
    
    // Tower Building
    
    if (m_build) {
      m_build = false;
      bool m_build_blocked = false;
      BuildChecker * m_build_check = new BuildChecker(m_projector.unproject(m_mousePos)-Vector2f(16.0f,16.0f), Vector2f(32.0f,32.0f));
      
      for(list<Game_Object *>::iterator obj = m_players.begin(); obj != m_players.end() && !m_build_blocked ; ++obj) {
        MTV m_MTV = getMTV(*m_build_check, **obj);
        if (m_MTV.intersects)
          m_build_blocked = true;
      }
      for(list<Game_Object *>::iterator obj = m_environment.begin(); obj != m_environment.end() && !m_build_blocked ; ++obj) {
        MTV m_MTV = getMTV(*m_build_check, **obj);
        if (m_MTV.intersects)
          m_build_blocked = true;
      }
      for(list<EnemySpawner *>::iterator obj = m_spawners.begin(); obj != m_spawners.end() && !m_build_blocked ; ++obj) {
        MTV m_MTV = getMTV(*m_build_check, **obj);
        if (m_MTV.intersects)
          m_build_blocked = true;
      }
      for(list<EnemyWeak *>::iterator obj = m_enemies.begin(); obj != m_enemies.end() && !m_build_blocked ; ++obj) {
        MTV m_MTV = getMTV(*m_build_check, **obj);
        if (m_MTV.intersects)
          m_build_blocked = true;
      }
      
      delete m_build_check;
      m_build_check = NULL;
      
      if (!m_build_blocked && m_player->get_ammo() > 9){
        m_player->mod_ammo(-10);
        Tower * newTower = m_player->createTower(m_projector.unproject(m_mousePos)-Vector2f(16.0f,16.0f));
        m_players.push_back(newTower);
        m_towers.push_back(newTower);
      }
    }
    
    // Towers Firing
    
    for (list<Tower *>::iterator twr = m_towers.begin(); twr != m_towers.end(); ++twr){
      (*twr)->perform_logic(time_step);
      if (!(*twr)->can_fire())
        continue;
      Vector2f direction = Vector2f(0.0f,0.0f);
      bool target_found = false;
      float distance = 1000000.0f; //(generous) maximum range
      for (list<EnemyWeak *>::iterator nme = m_enemies.begin(); nme != m_enemies.end(); ++nme){
        Vector2f new_direc = (*nme)->get_position() - (*twr)->get_position() + 0.5f * ((*twr)->get_size() - (*nme)->get_size());
        float dist2 = new_direc * new_direc;
        if (dist2 < distance) {
          target_found = true;
          distance = dist2;
          direction = new_direc;
        }
      }
      if (target_found)
        m_bullets.push_back((*twr)->fire(direction));
      
    }
    
    
    // Player Movement
    
    Vector2f move_vector = Vector2f((m_right-m_left),(m_down-m_up));
    move_vector.normalize();
    move_vector *= (m_player->get_speed() * m_player->get_speed_bonus());
    m_player->move(move_vector * time_step);
    m_player->rotate(m_projector.unproject(m_mousePos));
    
    // Collision Checking
    
    for(list<Game_Object *>::iterator A = m_players.begin(); A != m_players.end();) {
      (*A)->update_invincible(time_step);
      for(list<PowerUp *>::iterator B = m_powerups.begin(); (*A) == m_player && B != m_powerups.end();) {
        MTV m_MTV = getMTV(**A,**B);
        if (m_MTV.intersects) {
          m_player->use_powerup((*B)->get_type());
          delete *B;
          B = m_powerups.erase(B);
        } else {
          ++B;
        }
      }
      
      
      for(list<EnemyWeak *>::iterator B = m_enemies.begin(); !((*A)->is_invincible() && (*A) == m_player) && B != m_enemies.end(); ++B) {
        MTV m_MTV = getMTV(**A,**B);
        if (m_MTV.intersects) {
          const int action = getAction(**A,**B);
          if (action == 3) {
            m_MTV.distance = 5.0f;
          } else
            m_MTV.distance += 0.1f;
          if (!(*A)->is_invincible()) {
            (*A)->take_damage((*B)->get_damage());
            (*A)->set_invincible(1.0f);
          }
          MTV m_MTV_A = m_MTV;
          if ((m_MTV_A.direction.x > 0.0f && ((*A)->get_position().x < (*B)->get_position().x)) ||
              (m_MTV_A.direction.y > 0.0f && ((*A)->get_position().y < (*B)->get_position().y)))
            m_MTV_A.direction = -m_MTV_A.direction;
          MTV m_MTV_B = m_MTV_A;
          m_MTV_B.direction = -m_MTV_B.direction;
          if (action == 1 || action == 3)
            (*A)->move(m_MTV_A.direction * m_MTV_A.distance);
          if (action == 2 || action == 3)
            (*B)->move(m_MTV_B.direction * m_MTV_B.distance);
        }
        //          }
      }
      for(list<Game_Object *>::iterator B = m_environment.begin(); B != m_environment.end(); ++B) {
        MTV m_MTV = getMTV(**A,**B);
        if (m_MTV.intersects) {
          const int action = getAction(**A,**B);
          if (action == 3)
            m_MTV.distance += 5.0f;
          else
            m_MTV.distance += 0.1f;
          MTV m_MTV_A = m_MTV;
          if ((m_MTV_A.direction.x > 0.0f && ((*A)->get_position().x < (*B)->get_position().x)) ||
              (m_MTV_A.direction.y > 0.0f && ((*A)->get_position().y < (*B)->get_position().y)))
            m_MTV_A.direction = -m_MTV_A.direction;
          MTV m_MTV_B = m_MTV_A;
          m_MTV_B.direction = -m_MTV_B.direction;
          if (action == 1 || action == 3)
            (*A)->move(m_MTV_A.direction * m_MTV_A.distance);
          if (action == 2 || action == 3)
            (*B)->move(m_MTV_B.direction * m_MTV_B.distance);
        }
        //          }
      }
      for(list<Game_Object *>::iterator B = m_players.begin(); B != m_players.end(); ++B) {
        if (A != B) {
          MTV m_MTV = getMTV(**A,**B);
          if (m_MTV.intersects) {
            const int action = getAction(**A,**B);
            if (action == 3)
              m_MTV.distance += 10.0f;
            else
              m_MTV.distance += 0.1f;
            MTV m_MTV_A = m_MTV;
            if ((m_MTV_A.direction.x > 0.0f && ((*A)->get_position().x < (*B)->get_position().x)) ||
                (m_MTV_A.direction.y > 0.0f && ((*A)->get_position().y < (*B)->get_position().y)))
              m_MTV_A.direction = -m_MTV_A.direction;
            MTV m_MTV_B = m_MTV_A;
            m_MTV_B.direction = -m_MTV_B.direction;
            if (action == 1 || action == 3)
              (*A)->move(m_MTV_A.direction * m_MTV_A.distance);
            if (action == 2 || action == 3)
              (*B)->move(m_MTV_B.direction * m_MTV_B.distance);
          }
        }
      }
      
      if ((*A) != m_player && (*A)->get_health() <= 0){
        m_towers.erase(std::find(m_towers.begin(),m_towers.end(), (*A)));
        delete *A;
        A = m_players.erase(A);
      } else
        ++A;

      
    }
    //    }
    for(list<EnemyWeak *>::iterator A = m_enemies.begin(); A != m_enemies.end();) {

      
      for(list<Game_Object *>::iterator B = m_environment.begin(); B != m_environment.end(); ++B) {
        MTV m_MTV = getMTV(**A,**B);
        if (m_MTV.intersects) {
          const int action = getAction(**A,**B);
          if (action == 3)
            m_MTV.distance += 10.0f;
          else
            m_MTV.distance += 0.1f;
          MTV m_MTV_A = m_MTV;
          if ((m_MTV_A.direction.x > 0.0f && ((*A)->get_position().x < (*B)->get_position().x)) ||
              (m_MTV_A.direction.y > 0.0f && ((*A)->get_position().y < (*B)->get_position().y)))
            m_MTV_A.direction = -m_MTV_A.direction;
          MTV m_MTV_B = m_MTV_A;
          m_MTV_B.direction = -m_MTV_B.direction;
          if (action == 1 || action == 3)
            (*A)->move(m_MTV_A.direction * m_MTV_A.distance);
          if (action == 2 || action == 3)
            (*B)->move(m_MTV_B.direction * m_MTV_B.distance);
        }
        //          }
      }
      
      for(list<Bullet *>::iterator B = m_bullets.begin(); B != m_bullets.end() && (*A)->get_health() > 0;) {
        MTV m_MTV = getMTV(**A,**B);
        if (m_MTV.intersects) {
          (*A)->mod_health(-(*B)->get_damage());
          (*B)->mod_pierce(-1);
        }
        if ((*B)->get_pierce() < 0) {
          delete *B;
          B = m_bullets.erase(B);
        } else
          ++B;
      }
      
      if ((*A)->get_health() <= 0){
        if (m_rng.frand_lt() < 0.12f)
          m_powerups.push_back(new PowerUp((*A)->get_position(), m_rng.rand_lt(100)));
        
        delete *A;
        A = m_enemies.erase(A);
      } else
        ++A;
    }
    
    
    for(list<Bullet *>::iterator it = m_bullets.begin(); it != m_bullets.end();) {
      const Point2f &p = (*it)->get_position();
      
      if(p.x < -10.0f || p.x > 10.0f + levelSize.x ||
         p.y < -10.0f || p.y > 10.0f + levelSize.y)
      {
        delete *it;
        it = m_bullets.erase(it);
      } else
        ++it;
    }
  }
  
  void render() {
    Video &vr = get_Video();
    
    // don't move camera if near the borders
    Point2f cam_NW(m_player->get_position() + 0.5f * (m_player->get_size() - cam_size)),
    cam_SE(cam_NW + cam_size);
    Point2f cam_final;
    
    cam_final = fix_camera(cam_NW, cam_SE, cam_size, levelSize);
    
    vr.set_2d(make_pair(cam_final, cam_final + cam_size), true);
    
    Quadrilateral<Vertex2f_Texture> background(
      Vertex2f_Texture(Point2f(0.0f, 0.0f), Point2f(-1.0f, 0.0f)),
      Vertex2f_Texture(Point2f(0.0f, levelSize.y), Point2f(-1.0f, 2.0f)),
      Vertex2f_Texture(Point2f(levelSize.x, levelSize.y), Point2f(3.0f, 2.0f)),
      Vertex2f_Texture(Point2f(levelSize.x, 0.0), Point2f(3.0f, 0.0f)));
    Material background_tex("background");
    background.lend_Material(&background_tex);
    vr.render(background);


    for(list<EnemySpawner *>::iterator obj = m_spawners.begin(); obj != m_spawners.end(); ++obj) {
      (*obj)->render();
    }
    for(list<Game_Object *>::iterator obj = m_environment.begin(); obj != m_environment.end(); ++obj) {
      (*obj)->render();
    }
    for(list<PowerUp *>::iterator obj = m_powerups.begin(); obj != m_powerups.end(); ++obj) {
      (*obj)->render();
    }
    for(list<Game_Object *>::iterator obj = m_players.begin(); obj != m_players.end(); ++obj) {
      (*obj)->render();
    }
    for(list<EnemyWeak *>::iterator obj = m_enemies.begin(); obj != m_enemies.end(); ++obj) {
      (*obj)->render();
    }
    for(list<Bullet *>::iterator obj = m_bullets.begin(); obj != m_bullets.end(); ++obj) {
      (*obj)->render();
    }

    vr.set_2d(make_pair(Point2f(), Point2f() + cam_size), true);
    Projector2D crossProject = Projector2D(make_pair(Point2f(0.0f,0.0f),Point2f(0.0f,0.0f)+cam_size), get_Video().get_viewport());
    render_image("crosshair", crossProject.unproject(m_mousePos)-Vector2f(16.0f,16.0f), crossProject.unproject(m_mousePos)+Vector2f(16.0f,16.0f));
    render_image("hud_NW", Point2f(0.0f,0.0f), Point2f(256.0f,128.0f));
    render_image("hud_center", Point2f(336.0f,0.0f), Point2f(464.0f,64.0f));
    get_Fonts()["info"].render_text("Health: " + itoa(m_player->get_health()) + "\nAmmo: " + itoa(m_player->get_ammo()),
                                    Point2f(0.0f,0.0f), get_Colors()["system_font"]);
    get_Fonts()["info"].render_text(ftoa(m_time_passed,4),
                                    Point2f(400.0f,5.0f), get_Colors()["system_font"], ZENI_CENTER);

    if (m_gameover.is_running()) {
      render_image("hud_gameover", Point2f(144.0f, 172.0f), Point2f(656.0f, 450.0f));
      get_Fonts()["title"].render_text("GAME OVER", Point2f(400.0f, 230.0f), get_Colors()["system_font"], ZENI_CENTER);
      get_Fonts()["info"].render_text("Your valiant effort to defeat process \ngarbage lasted " +
                                      ftoa(m_time_passed,4) +
                                      " seconds.\nRebooting in " +
                                      itoa(10 - m_gameover.seconds()) +
                                      " seconds...",
                                      Point2f(400.0f, 300.0f), get_Colors()["system_font"], ZENI_CENTER);
    }
  }
};

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
    render_image("player", Point2f(30.0f,100.0f), Point2f(94.0f,164.0f));
    info_text.render_text("This is YOU, the GARBAGE COLLECTOR! Use WASD to MOVE,\nuse your mouse to AIM, and left-click to SHOOT.",
                     Point2f(110.0f, 100.0f),
                     text_color,
                     ZENI_LEFT);
    render_image("tower", Point2f(48.0f,196.0f), Point2f(80.0f,228.0f));
    info_text.render_text("This is a TURRET! Right-click a clear area to BUILD one.\nThese automatically SHOOT at foes.",
                          Point2f(110.0f, 180.0f),
                          text_color,
                          ZENI_LEFT);
    render_image("powerup_health", Point2f(48.0f, 276.0f), Point2f(80.0f, 308.0f));
    info_text.render_text("This is one of many POWER-UPS! Move over one to boost\n HP, AMMO, ARMOR, DAMAGE, SPEED, or get new powers!",
                          Point2f(110.0f, 260.0f),
                          text_color,
                          ZENI_LEFT);
    render_image("enemy1", Point2f(48.0f, 356.0f), Point2f(80.0f, 388.0f));
    info_text.render_text("This LOOSE DATA, your ENEMY! Touching these will cause\nyou to lose HEALTH! SHOOT these to DESTROY them!",
                          Point2f(110.0f, 340.0f),
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

class Title_State_Custom : public Title_State<Play_State, Instructions_State> {
public:
  Title_State_Custom()
  : Title_State<Play_State, Instructions_State>("")
  {
    m_widgets.unlend_Widget(title);
  }
  
  void render() {
    Title_State<Play_State, Instructions_State>::render();
    
    render_image("logo", Point2f(200.0f, 25.0f), Point2f(600.0f, 225.0f));
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("Fytecode");
      
      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;
      
      return new Title_State_Custom;
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
