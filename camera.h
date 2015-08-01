#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>

class Camera
{
public:
	Camera();

	QVector3D eye, at, up;
};

#endif // CAMERA_H
