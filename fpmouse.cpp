#include "fpmouse.h"

FPMouse::FPMouse() : rad2deg(180.0 /M_PI)
{
	velocity = 0.0;
	trackingMouse = false;
	lastTime = QTime::currentTime();

	std::cerr << "Mouse criado\n";
}

void FPMouse::mouseMove(const QPointF &p)
{

	if (!trackingMouse)
		return;

	std::cerr << "Movendo mouse \n";

	QTime currentTime = QTime::currentTime();
	int msecs = lastTime.msecsTo(currentTime);
	if (msecs) {
		QVector3D vp = mousePosTo3D(p);
		QVector3D currentPos3D = QVector3D(vp.x(), vp.y(), 0);
		double lenSqr = currentPos3D.lengthSquared();

		if (lenSqr >= 1.0)
			currentPos3D.normalize();
		else
			currentPos3D.setZ(sqrt(1.0 - lenSqr));

/*      (lenSqr >= 1.0) ? currentPos3D.normalize() :
						  currentPos3D.setZ(sqrt(1.0 - lenSqr)); */

		axis = - QVector3D::crossProduct(lastPos3D, currentPos3D);
		double angle = rad2deg * axis.length();
		velocity = angle / msecs;
		axis.normalize();
		rotation = QQuaternion::fromAxisAndAngle(axis, angle) * rotation;

		lastPos3D = currentPos3D;
		lastTime = currentTime;
	}
}

void FPMouse::mousePress(const QPointF &p)
{
	rotation = getRotation();
	trackingMouse = true;
	lastTime = QTime::currentTime();

	std::cerr << "Pressionando mouse \n";

	lastPos3D = mousePosTo3D(p);
	double lenSqr = lastPos3D.lengthSquared();
	(lenSqr >= 1.0) ? lastPos3D.normalize() :
					  lastPos3D.setZ(sqrt(lenSqr));

	velocity = 0.0;
}

void FPMouse::mouseRelease(const QPointF &p)
{
	//mouseMove(p);
	//trackingMouse = false;
}

void FPMouse::resizeViewport(int width, int height)
{
	viewportWidth = static_cast<double>(width);
	viewportHeight = static_cast<double>(height);
}

QQuaternion FPMouse::getRotation()
{
	if (trackingMouse)
		return rotation;

	QTime currentTime = QTime::currentTime();
	double angle = velocity * lastTime.msecsTo(currentTime);
	return QQuaternion::fromAxisAndAngle(axis, angle) * rotation;
}

QVector3D FPMouse::mousePosTo3D(const QPointF &p)
{

	return QVector3D(1.0 * p.x() / viewportWidth - 1.0,
					 1.0 - 1.0 * p.y() / viewportHeight,
					 0.0);
}
