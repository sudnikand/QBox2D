#include "glcamera.h"

void  GLCamera::setPosition(const QVector3D &position){
    position_ = transformMatrix_ * position;
}

void  GLCamera::setUpDirection(const QVector3D &upDirection){
    upDirection_ = transformMatrix_ * upDirection;
}

void  GLCamera::lookAt(const QVector3D &center){
    viewMatrix_.lookAt(position_, center, upDirection_);
}
