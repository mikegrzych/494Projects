//
//  collisions.h
//
//  Created by Michael on 10/2/13.
//
//  Classes and functions used for collision detection and handling
//

#ifndef __game__collisions__
#define __game__collisions__

#include "game_elements.h"
#include <zenilib.h>
#include <algorithm>

using namespace Zeni;

class MTV {
public:
  bool intersects;
  Vector2f direction;
  float distance;
  MTV():intersects(false),direction(Vector2f(0.0f,0.0f)),distance(0.0f) {}
  MTV(bool _intersects, Vector2f &_direction, float &_distance)
  :intersects(_intersects),
  direction(_direction),
  distance(_distance)
  {}
};

class Projection2D {
public:
  Projection2D(const Game_Object &obj_, int axis);
  ~Projection2D() {};
  bool doesOverlap(const Projection2D &other);
  float getOverlap(const Projection2D &other);
private:
  float m_min;
  float m_max;
};

MTV getMTV(Game_Object &obj0, Game_Object &obj1);
MTV getMTV_box_box(Game_Object &box0, Game_Object &box1);
MTV getMTV_box_circ(Game_Object &box, Game_Object &circle);
MTV getMTV_circ_box(Game_Object &circle, Game_Object &box);
MTV getMTV_circ_circ(Game_Object &circle0, Game_Object &circle1);

const int getAction(const Game_Object &objA, const Game_Object &objB);

#endif /* defined(__game__collisions__) */
