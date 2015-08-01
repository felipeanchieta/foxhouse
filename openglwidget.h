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
#include "trackball.h"
#include <QMouseEvent>
#include <QWheelEvent>
#include <QKeyEvent>
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

	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);

signals:
	void statusBarMessage(QString ns);

private slots:
	void toggleBackgroundColor(bool);
	void showFileOpenDialog();
	void changeDiagonal(bool);
	void changeShader(int);
	void animate();
	void loadNewObjects(int);

	void centralizeObject(bool);
	void scaleObject(bool);
	void rotateX(int);
	void rotateY(int);
	void rotateZ(int);

	void Rchanged(int);
	void Gchanged(int);
	void Bchanged(int);

	void lightChanged(int);

private:

	enum {
		ARMADILLO, BEETLE, BUNNY, CAMEL, ELEPHANT, ELK, MANNEQUIN, MUSHROOM,
		PEAR, PIG, SPHERE, TORUS
	};

protected:

	Mesh *mesh;


public:

	/* QOpenGLBuffer *vboVertices;
	QOpenGLBuffer *vboIndices;
	QOpenGLBuffer *vboColors;
	QOpenGLBuffer *vboNormals;
	QOpenGLBuffer *vboCoordTex;
	QOpenGLTexture *texture;
	QOpenGLTexture *colorTexture;
	QOpenGLTexture *colorTextureLayer; */
	QImage image;

	bool blackBackground;

	/* QVector4D *vertices;
	QVector4D *colors;
	QVector3D *normals;
	unsigned int *indices; */

	QOpenGLShader *vertexShader;
	QOpenGLShader *fragmentShader;
	QOpenGLShaderProgram *shaderProgram;

	/* uint numVertices;
	uint numFaces;

	QMatrix4x4 modelView, projectionMatrix;
	QVector3D midPoint; */
	double invDiag, zoom;

	QTimer timer;
	TrackBall trackBall;

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

	 /*void createVBOs();
	void destroyVBOs(); */

	void createShaders();
	void destroyShaders();

	void createTexture(const QString &imagePath);
	void genTexCoordsCylinder();

	void readOFFFile(const QString &fileName);

	void calculateNormal();
};

#endif // OPENGLWIDGET_H
