#ifndef MESH_H
#define MESH_H

#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <iostream>
#include <fstream>

#include <trackball.h>
#include <camera.h>
#include <material.h>
#include <light.h>


class Mesh : public QOpenGLWidget
{

public:
	Mesh(float, int);
	~Mesh();
	void drawMesh(QVector3D scale);

	QOpenGLShader *vertexShader;
	QOpenGLShader *fragmentShader;

	QOpenGLBuffer *vboVertices;
	QOpenGLBuffer *vboIndices;
	QOpenGLBuffer *vboColors;
	QOpenGLBuffer *vboNormals;
	QOpenGLBuffer *vboCoordTex;
	QOpenGLVertexArrayObject *vaoObject;

	QOpenGLTexture *texture;
	QOpenGLTexture *colorTexture;
	QOpenGLTexture *colorTextureLayer;
	QImage image;
	QVector4D *vertices;
	QVector4D *colors;
	QVector3D *normals;
	unsigned int *indices;
	uint numVertices;
	uint numFaces;
	QMatrix4x4 modelView, projectionMatrix;
	QVector3D midPoint;

	Camera camera;
	Material material;
	Light light;
	double zoomNS, zoomEW, invDiag, angle;
	QOpenGLShaderProgram *shaderProgram;

	float vPos;
	QString shader;

	TrackBall trackBall;

	QVector3D colorMaterial;
	QVector2D *texCoords;

	void createVAO();
	void destroyVAO();
	void newMesh(QString fileName);
	void createShaders();
	void destroyShaders();
	void calculateNormal();
	void genTexCoordsCylinder();

	int currentShader;
	void createTexture(const QString &imagePath);


private:

	bool isUsingTextures;
};

#endif // MESH_H
