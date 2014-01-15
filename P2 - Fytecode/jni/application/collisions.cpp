//
//  collisions.cpp
//
//  Created by Michael on 10/2/13.
//
//  Classes and functions used for collision detection and handling
//

#include "collisions.h"
#include "game_elements.h"
#include <zenilib.h>
#include <math.h>
#include <iostream>

using namespace Zeni;

//design consideration: All objects either are circles or are rectangles with edges aligned
//with the (x,y) axes --> collision should be easy to detect

Projection2D::Projection2D(const Game_Object &obj_, int axis){
  Point2f _min = obj_.get_position();
  Point2f _max = obj_.get_position() + obj_.get_size();
  
  if (axis == 0){
    m_min = _min.x;
    m_max = _max.x;
  } else {
    m_min = _min.y;
    m_max = _max.y;
  }
}

bool Projection2D::doesOverlap(const Projection2D &other){
  return !(m_min > other.m_max || other.m_min > m_max);
}

float Projection2D::getOverlap(const Projection2D &other){
  if(!doesOverlap(other)) {
    return 0.0f;
  }
  
  return std::min(abs(m_max-other.m_min),abs(m_min-other.m_max));
}

MTV getMTV(Game_Object &obj0, Game_Object &obj1){
  bool shape0(0), shape1(0);
  if (obj0.get_shape() == CIRCLE)
    shape0 = 1;
  if (obj1.get_shape() == CIRCLE)
    shape1 = 1;
  if (!shape0 && !shape1) {
    return getMTV_box_box(obj0, obj1);
  } else if (shape0 && !shape1) {
    return getMTV_circ_box(obj0, obj1);
  } else if (!shape0 && shape1) {
    return getMTV_box_circ(obj0, obj1);
  } else {
    return getMTV_circ_circ(obj0,obj1);
  }
}

MTV getMTV_box_box(Game_Object &box0, Game_Object &box1){
  MTV result;
  float overlap = 5000.0f;
  Vector2f smallest;
  for (int i = 0; i < 2; i++){
    Vector2f axis = Vector2f(float(1.0f-i),float(0.0f+i));
    Projection2D b0 = Projection2D(box0,i);
    Projection2D b1 = Projection2D(box1,i);
    
    if (!b0.doesOverlap(b1)){
      return result;
    } else {
      float o = b0.getOverlap(b1);
      if (o < overlap){
        overlap = o;
        smallest = axis;
      }
    }
  }
  result = MTV(true, smallest, overlap);
  return result;
}

MTV getMTV_circ_box(Game_Object &circle, Game_Object &box){
  return getMTV_box_circ(box, circle);
}

MTV getMTV_box_circ(Game_Object &box, Game_Object &circle){
  MTV result;
  float overlap = 5000.0f;
  Vector2f smallest;
  
  Point2f circle_center = circle.get_position() + 0.5f * circle.get_size() * -circle.get_theta();
  Vector2f possible_vecs[4] = {circle_center};
  possible_vecs[0] -= box.get_position();
  possible_vecs[1] -= (box.get_position() + Vector2f(0.0f,box.get_size().y));
  possible_vecs[2] -= (box.get_position() + box.get_size());
  possible_vecs[3] -= (box.get_position() + Vector2f(box.get_size().x,0.0f));
  Vector2f dist_vec = possible_vecs[0];
  
  for (int i = 1; i < 4; i++){
    if ((possible_vecs[i] * possible_vecs[i]) < (dist_vec * dist_vec))
      dist_vec = possible_vecs[i];
  }
  
  const float dist2 = dist_vec * dist_vec;
  const float radius_sum = circle.get_radius();
  
  if (dist2 < radius_sum * radius_sum) {
    float o = radius_sum - sqrt(dist2);
    if (o < overlap) {
      overlap = o;
      smallest = dist_vec.normalized();
    }
  } else {
    return result;
  }
  
  for (int i = 0; i < 2; i++){
    Vector2f axis = Vector2f(float(1-i),float(i));
    Projection2D b = Projection2D(box,i);
    Projection2D c = Projection2D(circle,i);
    
    if (!b.doesOverlap(c)){
      return result;
    } else {
      float o = b.getOverlap(c);
      if (o < overlap){
        overlap = o;
        smallest = axis;
      }
    }
  }
  result = MTV(true, smallest, overlap);
  return result;
}

MTV getMTV_circ_circ(Game_Object &circle0, Game_Object &circle1){
  MTV result;
  float overlap = 5000.0f;
  Vector2f smallest;
  const Vector2f dist_vec = (circle0.get_position() + 0.5f * circle0.get_size()) -
                            (circle1.get_position() + 0.5f * circle1.get_size());
  const float dist2 = dist_vec * dist_vec;
  const float radius_sum = circle0.get_radius() + circle1.get_radius();
  
  if (dist2 < radius_sum * radius_sum) {
    overlap = radius_sum - sqrt(dist2);
    smallest = dist_vec.normalized();
    result = MTV(true, smallest, overlap);
  }
  
  return result;
}

const int getAction(const Game_Object &objA, const Game_Object &objB) {
  if (objA.get_collider() == MOVABLE && objB.get_collider() == MOVABLE)
    if (objA.get_faction() + objB.get_faction() == 1)
      return 3;
  if (objA.get_collider() == MOVABLE && objB.get_collider() == IMMOVABLE)
    return 1;
  if (objA.get_collider() == IMMOVABLE && objB.get_collider() == MOVABLE)
    return 2;
  return 0;
}



