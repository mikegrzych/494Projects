//
//  general_funcs.h
//
//  Created by Michael on 9/26/13.
//
//  This is meant as a home for future "general use" or misc functions who
//  don't belong in any other header or source files
//

#ifndef __game__general_funcs__
#define __game__general_funcs__

#include <zenilib.h>

using namespace Zeni;

Point2f fix_camera(const Point2f cam_NW, const Point2f cam_SE, const Vector2f cam_size, const Point2f level_size);

#endif /* defined(__game__general_funcs__) */
