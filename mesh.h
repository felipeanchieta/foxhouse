#ifndef MESH_H
#define MESH_H

#include <QtOpenGL>
#include <QOpenGLFunctions>
#include <QOpenGLWidget>

#include <trackball.h>
#include <camera.h>
#include <material.h>
#include <light.h>

class Mesh : public QOpenGLWidget
{
public:
	Mesh();
	~Mesh();
	void drawMesh();

	QOpenGLBuffer *vboVertices;
	QOpenGLBuffer *vboIndices;
	QOpenGLBuffer *vboColors;
	QOpenGLBuffer *vboNormals;
	QOpenGLBuffer *vboCoordTex;
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
	double zoom, invDiag;
	QOpenGLShaderProgram *shaderProgram;

	TrackBall trackBall;

	QVector3D colorMaterial;
	QVector2D *texCoords;

	void createVBO();
	void destroyVBO();

	void setShaderProgram(QOpenGLShaderProgram *shaderProgram);
};

#endif // MESH_H
