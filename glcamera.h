#ifndef GLCAMERA_H
#define GLCAMERA_H

#include <QMatrix4x4>

class GLCamera
{
public:
    void setPosition(const QVector3D &position);
    void setUpDirection(const QVector3D &upDirection);
    void lookAt(const QVector3D &center);
    QMatrix4x4& projMatrix();
    QMatrix4x4& viewMatrix();
    QMatrix4x4  projViewProduct() const;

private:
    QMatrix4x4 _projMatrix;
    QMatrix4x4 _viewMatrix;
    QMatrix4x4 _transformMatrix;
    QVector3D  _position;
    QVector3D  _upDirection;
};

#endif // GLCAMERA_H
