#ifndef OPENGLWIDGET_H
#define OPENGLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QMessageBox>
#include <QFileDialog>
#include <iostream>
#include <fstream>
#include <limits>
#include <QtGlobal>
#include <cmath>
#include <QTime>
#include <QTimer>
#include <QtOpenGL>
/* #include "trackball.h" */
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
#include "camera.h"
#include "material.h"
#include "light.h"
#include "mesh.h"
#include "fpmouse.h"

class OpenGLWidget : public QOpenGLWidget, private QOpenGLFunctions
{
	Q_OBJECT
public:
	explicit OpenGLWidget(QWidget *parent = 0);
	~OpenGLWidget();

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);

signals:
	void statusBarMessage(QString ns);

private slots:
	void toggleBackgroundColor(bool);
	void animate(); void loadNewObjects(int i);


private:

	enum {
		ARMADILLO, BEETLE, BUNNY, CAMEL, ELEPHANT, ELK, MANNEQUIN, MUSHROOM,
		PEAR, PIG, SPHERE, TORUS
	};

protected:

	Mesh *mesh[10];


public:

	QImage image;

	bool blackBackground;


	QOpenGLShader *vertexShader;
	QOpenGLShader *fragmentShader;
	QOpenGLShaderProgram *shaderProgram;

	QMatrix4x4 viewMatrix;

	QTimer timer;
	FPMouse trackBall;

	bool _scaleObject;
	bool _centralizeObject;

	int xAngle, yAngle, zAngle;
	int currentShader;

	/* Camera camera;
	Material material;
	Light light;

	QVector3D colorMaterial;
	QVector2D *texCoords; */


	void initializeGL();
	void resizeGL(int, int);
	void paintGL();

};

#endif // OPENGLWIDGET_H
