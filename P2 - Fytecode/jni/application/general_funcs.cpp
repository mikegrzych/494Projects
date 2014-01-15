//
//  general_funcs.cpp
//
//  Created by Michael on 9/26/13.
//
//  This is meant as a home for future "general use" or misc functions who
//  don't belong in any other header or source files
//

#include "general_funcs.h"
#include <zenilib.h>

using namespace Zeni;

Point2f fix_camera(const Point2f cam_NW, const Point2f cam_SE, const Vector2f cam_size, const Point2f level_size) {
  Point2f cam_final;
  if (cam_NW.x < 0.0f) {
    cam_final.x = 0.0f;
  } else if (cam_SE.x > level_size.x) {
    cam_final.x = level_size.x - cam_size.x;
  } else {
    cam_final.x = cam_NW.x;
  }
  if (cam_NW.y < 0.0f) {
    cam_final.y = 0.0f;
  } else if (cam_SE.y > level_size.y) {
    cam_final.y = level_size.y - cam_size.y;
  } else {
    cam_final.y = cam_NW.y;
  }
  return cam_final;
}