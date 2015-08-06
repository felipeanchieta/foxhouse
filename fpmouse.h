#ifndef FPMOUSE_H
#define FPMOUSE_H

#include <QVector3D>
#include <QQuaternion>
#include <QTime>

#include <cmath>
#include <iostream>

class FPMouse
{
public:
	FPMouse();
	void mouseMove(const QPointF& p);
	void mousePress(const QPointF& p);
	void mouseRelease(const QPointF& p);
	void resizeViewport(int width, int height);
	QQuaternion getRotation();

private:
	QVector3D mousePosTo3D(const QPointF&);

	QQuaternion rotation;
	QVector3D axis;
	double velocity;
	QVector3D lastPos3D;
	QTime lastTime;
	bool trackingMouse;
	double viewportWidth;
	double viewportHeight;
	const double rad2deg;

};

#endif // FPMOUSE_H
