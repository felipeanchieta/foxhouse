#include "mesh.h"

Mesh::Mesh()
{
	vertices = NULL;
	colors = NULL;
	indices = NULL;
	normals = NULL;
	texture = NULL;
	colorTexture = NULL;
	colorTextureLayer = NULL;

	vboVertices = NULL;
	vboColors = NULL;
	vboIndices = NULL;
	vboNormals = NULL;
	vboCoordTex = NULL;

	colorMaterial = QVector3D(0,0,0);
}

Mesh::~Mesh()
{

}

void Mesh::drawMesh()
{
	makeCurrent();

	modelView.setToIdentity();
	modelView.lookAt(camera.eye, camera.at, camera.up);
	modelView.translate(0, 0, zoom);
	modelView.rotate(trackBall.getRotation());
	modelView.scale(invDiag, invDiag, invDiag);
	modelView.translate(- midPoint);

	if (!shaderProgram)
		return;

	shaderProgram->bind();

	QVector4D ambientProduct = light.ambient * material.ambient;
	QVector4D diffuseProduct = light.diffuse * material.diffuse;
	QVector4D specularProduct = light.specular * material.specular;

	vboCoordTex->bind();
	shaderProgram->enableAttributeArray("vcoordText") ;
	shaderProgram->setAttributeBuffer("vcoordText", GL_FLOAT, 0, 2, 0);

	colorTexture = new QOpenGLTexture(image);
	colorTexture->bind(0);
	shaderProgram->setUniformValue("colorTexture", 0);

	shaderProgram->setUniformValue("lightPosition", light.position);
	shaderProgram->setUniformValue("ambientProduct", ambientProduct);
	shaderProgram->setUniformValue("diffuseProduct", diffuseProduct);
	shaderProgram->setUniformValue("specularProduct", specularProduct);
	shaderProgram->setUniformValue("shininess", static_cast<GLfloat>(material.shininess));
	shaderProgram->setUniformValue("modelView", modelView);
	shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
	shaderProgram->setUniformValue("normalMatrix", modelView.normalMatrix());
	shaderProgram->setUniformValue("midPoint", midPoint);
	shaderProgram->setUniformValue("invDiag", (float) invDiag);

	vboVertices->bind();
	shaderProgram->enableAttributeArray("vPosition");
	shaderProgram->setAttributeArray("vPosition", GL_FLOAT, 0, 4, 0);

	vboNormals->bind();
	shaderProgram->enableAttributeArray("vNormal");
	shaderProgram->setAttributeArray("vNormal", GL_FLOAT, 0, 3, 0);

	vboIndices->bind();

	glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_INT, 0);

	vboIndices->release();
	vboColors->release();
	vboVertices->release();

	if(colorTexture) {
		colorTexture->release(0);
		delete colorTexture;
		colorTexture = NULL;
	}

	vboCoordTex->release();
	shaderProgram->release();

	update();
}

void Mesh::createVBO()
{
	destroyVBO();

	vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboVertices->create();
	vboVertices->bind();
	vboVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboVertices->allocate(vertices , numVertices * sizeof(QVector4D));
	delete[] vertices;
	vertices = NULL;

	vboColors = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboColors->create();
	vboColors->bind();
	vboColors->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboColors->allocate(colors, numVertices * sizeof(QVector4D));
	delete[] colors;
	colors = NULL;

	vboNormals = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboNormals->create();
	vboNormals->bind();
	vboNormals->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboNormals->allocate(normals, numVertices * sizeof(QVector3D));
	delete[] normals;
	normals = NULL;

	vboCoordTex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboCoordTex->create();
	vboCoordTex->bind();
	vboCoordTex->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboCoordTex->allocate(texCoords, numVertices * sizeof(QVector2D));
	delete[] texCoords;
	texCoords = NULL;

	vboIndices = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	vboIndices ->create();
	vboIndices ->bind();
	vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboIndices ->allocate(indices, numFaces * 3 * sizeof (unsigned int));
	delete[] indices;
	indices = NULL;
}

void Mesh::destroyVBO()
{
	if (vboVertices) {
		vboVertices->release();
		delete vboVertices;
		vboVertices = NULL;
	}

	if (vboColors) {
		vboColors->release();
		delete vboColors;
		vboColors = NULL;
	}

	if (vboNormals) {
		vboNormals->release();
		delete vboNormals;
		vboNormals = NULL;
	}

	if (vboIndices) {
		vboIndices->release();
		delete vboIndices;
		vboIndices = NULL;
	}

	//qWarning() << "vbos limpos\n";
}

void Mesh::setShaderProgram(QOpenGLShaderProgram *shaderProgram)
{
	this->shaderProgram = shaderProgram;

	update();
}
