#ifndef DEF_H
#define DEF_H

#include <QDebug>

#define PI 3.141592653589793238463
#define ANG2RAD( a ) ( ( (a) * PI ) / 180.0f )
#define RAD2ANG( a ) ( ( (a) * 180.0f ) / PI )

#define WORLD_SCALE_FACTOR 0.1f

#define WSCALE(x)      ( x * WORLD_SCALE_FACTOR )
#define WSCALE2(x,y)   ( WSCALE(x) ),( WSCALE(y) )
#define WSCALE3(x,y,z) ( WSCALE(x) ),( WSCALE(y) ),( WSCALE(z) )

#endif // DEF_H
