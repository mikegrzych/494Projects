//
//  Demo_State.cpp


#include <zenilib.h>
#include "Demo_State.h"

#include <iostream>

using namespace Zeni;

Demo_State::Demo_State()
: m_light_ambient(Color(1.0f,0.75f,0.75f,0.75f),
                  Color(1.0f,0.75f,0.75f,0.75f),
                  Color(1.0f,0.5f,0.5f,0.5f),
                  Point3f(32.0f,64.0f,100.0f),
                  Vector3f(-1.0f,-1.0f,-1.0f)),
m_player(new Player(new Zeni::Model("models/orange_cube.3DS"),
                    Point3f(0.0f, 0.0f, 70.0f),
                    Vector3f(0.9f,0.9f,0.9f))),
m_next_room(Point3f(0.0f,0.0f,0.0f)),
m_time_passed(0.0f),
m_levels_beat(0),
m_game_over(0)
{
  set_pausable(true);
  m_light_ambient.set_light_type(LIGHT_DIRECTIONAL);
  m_dynamics.push_back(new Deathwall(Point3f(-300.0f,-2500.0f,-2500.0f),
                                     Vector3f(1.0f,5000.0f,5000.0f)));
  load_demo_level();
}

void Demo_State::on_push() {
  get_Window().set_mouse_state(Window::MOUSE_RELATIVE);
  m_chrono.start();
}

void Demo_State::on_pop() {
  get_Window().set_mouse_state(Window::MOUSE_NORMAL);
  m_chrono.stop();
}

void Demo_State::on_key(const SDL_KeyboardEvent &event) {
  if (event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_p) {
    get_Game().push_Popup_Pause_State();
  } else if (event.type == SDL_KEYDOWN && event.keysym.sym == SDLK_ESCAPE) {
    get_Game().push_Popup_Menu_State();
  } else
  {
    switch(event.keysym.sym) {
      case SDLK_w:
        m_controls.forward = event.type == SDL_KEYDOWN;
        break;
        
      case SDLK_a:
        m_controls.left = event.type == SDL_KEYDOWN;
        break;
        
      case SDLK_s:
        m_controls.back = event.type == SDL_KEYDOWN;
        break;
        
      case SDLK_d:
        m_controls.right = event.type == SDL_KEYDOWN;
        break;
        
      case SDLK_SPACE:
        if(event.type == SDL_KEYDOWN) {
          m_player->jump();
          m_moved = true;
        }
        break;
        
      default:
        Gamestate_Base::on_key(event);
        break;
    }
  }
}

void Demo_State::on_mouse_motion(const SDL_MouseMotionEvent &event) {
  m_player->turn_left_xy(-event.xrel / 500.0f);
}

void Demo_State::perform_logic()
{
  if (m_game_over) {
    if (m_gameover.is_running()) {
      if (m_gameover.seconds() > 10.0f)
        get_Game().pop_state();
    }
    else
    {
      m_chrono.stop();
      m_gameover.start();
    }
    return;
  }

  const float time_passed = m_chrono.seconds();
  float processing_time = time_passed - m_time_passed;
  m_time_passed = time_passed;
  
  // fwd & left vectors in xy-plane
  const Vector3f forward = (m_player->get_rotation() * Vector3f(1.0f,0.0f,0.0f)).get_ij().normalized();
  const Vector3f left = (m_player->get_rotation() * Vector3f(0.0f,1.0f,0.0f)).get_ij().normalized();
  
  // velocity vector into axes
  const Vector3f velocity = (m_controls.forward - m_controls.back) * m_player->get_speed() * forward
                          + (m_controls.left - m_controls.right) * m_player->get_speed() * left;
  const Vector3f x_vel = velocity.get_i();
  const Vector3f y_vel = velocity.get_j();
  Vector3f z_vel = m_player->get_velocity().get_k();
  
  //sound book keeping
  
  // process internal operations for dynamic objects
  for (std::list<Object_Dynamic *>::iterator obj = m_dynamics.begin();
       obj != m_dynamics.end();
       ++obj)
  {
    (*obj)->perform_logic(processing_time);
  }
  
  // keep delays under control in case program hangs
  if(processing_time > 0.1f)
    processing_time = 0.1f;
  
  // physics proc loop
  for(float time_step = 0.5f;
      processing_time > 0.0f;
      processing_time -= time_step)
  {
    if(time_step > processing_time)
      time_step = processing_time;
    
    // gravity affects player
    Vector3f gravity = Vector3f(0.0f, 0.0f, 175.0f);
    z_vel -= gravity * time_step;
    
    // try moving on each axis
    partial_step(time_step, x_vel);
    partial_step(time_step, y_vel);
    partial_step(time_step, z_vel);
    
    // detect if player falls off level
    const Point3f &position = m_player->get_position();
    if(position.z < -50.0f)
    {
      m_game_over = true;
    }
  }
  
  m_player->update_camera();
}

void Demo_State::render()
{
  Video &vr = get_Video();

  
  
  //skybox code
  Camera cam = m_player->get_camera();
  cam.position = Point3f(0.0f,0.0f,0.0f);
  vr.set_3d(cam);
//  m_player->set_camera();
//  vr.set_3d(m_player->get_camera());
  vr.set_zwrite(false);
  Material mat_sky("skybox");
  Quadrilateral<Vertex3f_Texture> sky_N(Vertex3f_Texture(Point3f(256.0f,-256.0f,256.0f), Point2f(0.0f,0.0f)),
                                       Vertex3f_Texture(Point3f(256.0f,256.0f,256.0f), Point2f(0.0f,1.0f)),
                                       Vertex3f_Texture(Point3f(256.0f,256.0f,-256.0f), Point2f(1.0f,1.0f)),
                                       Vertex3f_Texture(Point3f(256.0f,-256.0f,-256.0f), Point2f(1.0f,0.0f)));
  Quadrilateral<Vertex3f_Texture> sky_S(Vertex3f_Texture(Point3f(-256.0f,-256.0f,256.0f), Point2f(1.0f,0.0f)),
                                       Vertex3f_Texture(Point3f(-256.0f,256.0f,256.0f), Point2f(1.0f,1.0f)),
                                       Vertex3f_Texture(Point3f(-256.0f,256.0f,-256.0f), Point2f(0.0f,1.0f)),
                                       Vertex3f_Texture(Point3f(-256.0f,-256.0f,-256.0f), Point2f(0.0f,0.0f)));
  Quadrilateral<Vertex3f_Texture> sky_E(Vertex3f_Texture(Point3f(256.0f,-256.0f,256.0f), Point2f(0.0f,0.0f)),
                                        Vertex3f_Texture(Point3f(-256.0f,-256.0f,256.0f), Point2f(0.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(-256.0f,-256.0f,-256.0f), Point2f(1.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(256.0f,-256.0f,-256.0f), Point2f(1.0f,0.0f)));
  Quadrilateral<Vertex3f_Texture> sky_W(Vertex3f_Texture(Point3f(-256.0f,256.0f,256.0f), Point2f(0.0f,0.0f)),
                                        Vertex3f_Texture(Point3f(256.0f,256.0f,256.0f), Point2f(0.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(256.0f,256.0f,-256.0f), Point2f(1.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(-256.0f,256.0f,-256.0f), Point2f(1.0f,0.0f)));
  Quadrilateral<Vertex3f_Texture> sky_U(Vertex3f_Texture(Point3f(-256.0f,256.0f,256.0f), Point2f(0.0f,0.0f)),
                                        Vertex3f_Texture(Point3f(-256.0f,-256.0f,256.0f), Point2f(0.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(256.0f,-256.0f,256.0f), Point2f(1.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(256.0f,256.0f,256.0f), Point2f(1.0f,0.0f)));
  Quadrilateral<Vertex3f_Texture> sky_D(Vertex3f_Texture(Point3f(256.0f,256.0f,-256.0f), Point2f(0.0f,0.0f)),
                                        Vertex3f_Texture(Point3f(256.0f,-256.0f,-256.0f), Point2f(0.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(-256.0f,-256.0f,-256.0f), Point2f(1.0f,1.0f)),
                                        Vertex3f_Texture(Point3f(-256.0f,256.0f,-256.0f), Point2f(1.0f,0.0f)));
  sky_N.fax_Material(&mat_sky);
  sky_S.fax_Material(&mat_sky);
  sky_E.fax_Material(&mat_sky);
  sky_W.fax_Material(&mat_sky);
  sky_U.fax_Material(&mat_sky);
  sky_D.fax_Material(&mat_sky);
  vr.render(sky_N);
  vr.render(sky_S);
  vr.render(sky_E);
  vr.render(sky_W);
  vr.render(sky_U);
  vr.render(sky_D);
  
  vr.set_lighting(true);
  vr.set_ambient_lighting(Color(1.0f,0.0f,0.0f,0.0f));
  vr.set_Light(0, m_light_ambient);
  vr.set_3d(m_player->get_camera());
  vr.set_zwrite(true);
  
//  m_ground.render();
  m_player->render();
  for (std::list<Object_Static *>::iterator obj = m_statics.begin();
       obj != m_statics.end();
       ++obj)
  {
    (*obj)->render();
  }
  for (std::list<Object_Dynamic *>::iterator obj = m_dynamics.begin();
       obj != m_dynamics.end();
       ++obj)
  {
    (*obj)->render();
  }

  
  vr.unset_Light(0);
  vr.set_lighting(false);
  
  vr.set_2d(std::make_pair(Point2f(), Point2f(800.0f,600.0f)), true);
  render_image("hud_center", Point2f(336.0f,0.0f), Point2f(464.0f,64.0f));
  get_Fonts()["info"].render_text(ftoa(m_time_passed,4),
                                  Point2f(400.0f,5.0f), get_Colors()["system_font"], ZENI_CENTER);
  
  if (m_gameover.is_running()) {
    render_image("hud_gameover", Point2f(144.0f, 172.0f), Point2f(656.0f, 450.0f));
    get_Fonts()["title"].render_text("GAME OVER", Point2f(400.0f, 230.0f), get_Colors()["system_font"], ZENI_CENTER);
    get_Fonts()["info"].render_text("You succumbed to the nightmare after\na mere " +
                                    ftoa(m_time_passed,4) +
                                    " seconds.\nWaking up in " +
                                    itoa(10 - m_gameover.seconds()) +
                                    " seconds...",
                                    Point2f(400.0f, 300.0f), get_Colors()["system_font"], ZENI_CENTER);
  }
}

void Demo_State::partial_step(const float &time_step, const Zeni::Vector3f &velocity)
{
  m_player->set_velocity(velocity);
  const Point3f backup_position = m_player->get_position();
  bool intersect = false;
  
  m_player->execute_step(time_step);
  
  Collision::Infinite_Cylinder player_move_body = m_player->get_move_body();
  Collision::Parallelepiped player_box = m_player->get_body();
  
  // check collision, rollback if necessary
  for (std::list<Object_Dynamic *>::iterator obj = m_dynamics.begin();
       obj != m_dynamics.end();)
  {
    if (player_move_body.intersects((*obj)->get_body()) &&
        player_box.intersects((*obj)->get_body()))
    {
      std::cout << "Obj_Dyn collision" << std::endl;
      if ((*obj)->on_player() == rewardPlayer)
      {
        // first do actions to reward player, add rooms, etc
        Random _rng = Random(m_chrono.seconds());
        ++m_levels_beat;
        if (m_levels_beat < 5)
        {
          switch (_rng.rand_lt(3))
          {
            case 0:
              load_easy_0();
              break;
            case 1:
              load_easy_1();
              break;
            case 2:
              load_easy_2();
              break;
            default:
              load_easy_0();
              break;
          }
        } else if (m_levels_beat >= 5)
        {
          switch (_rng.rand_lt(6))
          {
            case 0:
              load_easy_0();
              break;
            case 1:
              load_easy_1();
              break;
            case 2:
              load_easy_2();
              break;
            case 3:
              load_medium_0();
              break;
            case 4:
              load_medium_1();
              break;
            case 5:
              load_medium_2();
              break;
            default:
              load_easy_0();
              break;
          }
        }
        // next handle object removal
        delete *obj;
        obj = m_dynamics.erase(obj);
      } else
      {
        if ((*obj)->on_player() == gameOver)
        {
          m_game_over = true;
        } else if ((*obj)->on_player() == doCollide)
        {
          intersect = true;
        }
        ++obj;
    
      }
    }
    else
      ++obj;
  }

  
  
  for (std::list<Object_Static *>::iterator obj = m_statics.begin();
       obj != m_statics.end();
       ++obj)
  {
    if (player_move_body.intersects((*obj)->get_body()) &&
          player_box.intersects((*obj)->get_body()))
        intersect = true;
  }
  
    
  if (intersect)
  {
    m_player->set_position(backup_position);
    if(velocity.k < 0.0f)
      m_player->set_on_ground(true);
    else if (velocity.k > 0.0f)
      m_player->set_velocity(-velocity);
  }
}
