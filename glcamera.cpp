#include "glcamera.h"

void  GLCamera::setPosition(const QVector3D &position){
    _position = _transformMatrix * position;
}

void  GLCamera::setUpDirection(const QVector3D &upDirection){
    _upDirection = _transformMatrix * upDirection;
}

void  GLCamera::lookAt(const QVector3D &center){
    _viewMatrix.lookAt(_position, center, _upDirection);
}

QMatrix4x4& GLCamera::projMatrix() {
    return _projMatrix;
}

QMatrix4x4& GLCamera::viewMatrix() {
    return _viewMatrix;
}

QMatrix4x4 GLCamera::projViewProduct() const {
    return _projMatrix * _viewMatrix;
}
