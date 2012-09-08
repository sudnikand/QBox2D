#ifndef DEF_H
#define DEF_H

#include <QDebug>

#define PI 3.141592653589793238463
#define ANG2RAD( a ) ( ( (a) * PI ) / 180.0f )
#define RAD2ANG( a ) ( ( (a) * 180.0f ) / PI )

#define WORLD_SCALE_VALUE 50.0f

#define W2Q(x,y)    ( x * WORLD_SCALE_VALUE ),( y * WORLD_SCALE_VALUE )
#define W2Q_(x)     ( x * WORLD_SCALE_VALUE )

#define Q2W(x,y)    ( x / WORLD_SCALE_VALUE ),( y / WORLD_SCALE_VALUE )
#define Q2W_(x)     ( x / WORLD_SCALE_VALUE )

#endif // DEF_H
