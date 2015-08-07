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
	/* void createTexture(const QString &imagePath); */


	/* Variáveis públicas */
	Camera camera;
	Material material;
	Light light;
	QImage image;
	TrackBall trackBall;
	double zoomNS, zoomEW, invDiag, angle;
	QMatrix4x4 modelView, projectionMatrix;

	float vPos; /* Posição vertical */
	//QString shader;
	QVector2D *texCoords;

	void newMesh(QString fileName);
	int currentShader;


	/* Texturas */
	QOpenGLTexture *texture, *colorTexture, *normalTexture, *colorTextureLayer;


private:

	/* VBOs e VAO */
	QOpenGLBuffer	*vboVertices, *vboIndices, *vboColors,
					*vboNormals, *vboCoordTex, *vboTangents;

	QOpenGLVertexArrayObject *vaoObject;

	/* Shaders */
	QOpenGLShader *vertexShader, *fragmentShader;
	QOpenGLShaderProgram *shaderProgram;

	enum {
		FLAT, GOURAUD, PHONG, TOON, HALFPHONG, TEXTURE, NORMAL
	};


	/* Varíaveis privadas */
	QVector4D *tangents, *vertices, *colors;
	QVector3D *normals, midPoint;
	uint *indices, numVertices, numFaces;
	bool isUsingTextures;

	/* Métodos */
	void createVAO();
	void destroyVAO();
	void createShaders();
	void destroyShaders();
	void calculateNormal();
	void genTexCoordsCylinder();
	void genTangents();
};

#endif // MESH_H
