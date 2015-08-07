#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMessageBox>
#include <QFileDialog>
#include <QtGlobal>

#include <iostream>
#include <fstream>
#include <limits>
#include <cmath>

#include <QTime>
#include <QTimer>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>

#include "trackball.h"
#include "camera.h"
#include "material.h"
#include "light.h"
#include "mesh.h"

class OpenGLWidget : public QOpenGLWidget, private QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();

signals:
	void statusBarMessage(QString ns);

private slots:
	void takeScreenshot();
	void animate();

private:
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void initializeGL();
	void resizeGL(int, int);
	void paintGL();
	void createTexture(Mesh *mesh, const QString &imagePath);
	void createNormalTexture(Mesh *mesh, const QString &imagePath);


	Mesh *casaFox,*gramado, *ceu;
	QTimer timer;
	TrackBall trackBall;
	int currentShader;

	enum {
		FLAT, GOURAUD, PHONG, TOON, HALFPHONG, TEXTURE, NORMAL
	};
};

#endif // OPENGLWIDGET_H
