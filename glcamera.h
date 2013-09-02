#ifndef GLCAMERA_H
#define GLCAMERA_H

#include <QMatrix4x4>

class GLCamera
{
public:
    void setPosition(const QVector3D &position);
    void setUpDirection(const QVector3D &upDirection);
    void lookAt(const QVector3D &center);

    QMatrix4x4 projMatrix_;
    QMatrix4x4 viewMatrix_;
    QMatrix4x4 transformMatrix_;
    QVector3D  position_;
    QVector3D  upDirection_;
};

#endif // GLCAMERA_H
