#ifndef __DEF_H__
#define __DEF_H__

#include <QDebug>

#define DATA_DIR "../data/"
#define SHADER_DIR   ( DATA_DIR "shaders/"  )
#define TEXTURE_DIR  ( DATA_DIR "textures/" )
#define LEVELS_DIR   ( DATA_DIR "levels/"   )
#define RESOURCE_DIR ( DATA_DIR "res/"      )

#define PI 3.141592653589793238463
#define ANG2RAD( a ) ( ( (a) * PI ) / 180.0f )
#define RAD2ANG( a ) ( ( (a) * 180.0f ) / PI )

#define WORLD_SCALE_FACTOR 1.0f

#define WSCALE(x) (x) * WORLD_SCALE_FACTOR

#define WSCALE2(x,y)   ( WSCALE(x) ),( WSCALE(y) )
#define WSCALE3(x,y,z) ( WSCALE(x) ),( WSCALE(y) ),( WSCALE(z) )

#endif // DEF_H
