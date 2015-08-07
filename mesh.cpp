#include "mesh.h"

Mesh::Mesh(float zPosition, int _currentShader)
{
	vertices = NULL;
	colors = NULL;
	indices = NULL;
	normals = NULL;
	texture = NULL;
	colorTexture = NULL;
	colorTextureLayer = NULL;
	texCoords = NULL;
	vboVertices = NULL;
	vboColors = NULL;
	vboIndices = NULL;
	vboNormals = NULL;
	vboCoordTex = NULL;
	vaoObject = NULL;
	isUsingTextures = false;
	vertexShader = NULL;
	fragmentShader = NULL;
	shaderProgram = NULL;

	angle = .0f;
	currentShader = _currentShader;

	vPos = zPosition;
}

Mesh::~Mesh()
{
	destroyVAO();
	destroyShaders();
}

void Mesh::drawMesh(QVector3D scale)
{
	makeCurrent();

	/* Não desenha sem vértices */
	if (!vboVertices)
		return;

	modelView.setToIdentity();

	modelView.lookAt(camera.eye, camera.at, camera.up);
	modelView.translate(zoomEW, vPos, zoomNS);
	modelView.rotate(trackBall.getRotation());
	modelView.scale(invDiag + scale.x(), invDiag + scale.y(), invDiag + scale.z());
	modelView.translate(- midPoint);

	shaderProgram->bind();
	vaoObject->bind();

	QVector4D ambientProduct = light.ambient * material.ambient;
	QVector4D diffuseProduct = light.diffuse * material.diffuse;
	QVector4D specularProduct = light.specular * material.specular;

	vboCoordTex->bind();
	shaderProgram->enableAttributeArray("vcoordText") ;
	shaderProgram->setAttributeBuffer("vcoordText", GL_FLOAT, 0, 2, 0);

	texture->bind(0);
	shaderProgram->setUniformValue("colorTexture", 0);


//	if (currentShader == NORMAL) {
//		normalTexture->bind(1);
//		shaderProgram->setUniformValue("normalTexture", 1);
//	}

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

	vboTangents->bind();
	shaderProgram->enableAttributeArray("vTangent");
	shaderProgram->setAttributeArray("vTangent", GL_FLOAT, 0, 4, 0);

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

	if (normalTexture) {
		normalTexture->release(1);
		delete normalTexture;
		normalTexture = NULL;
	}

	vboCoordTex->release();
	vaoObject->release();
	shaderProgram->release();

	update();
}

void Mesh::destroyVAO()
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

	if (vboCoordTex) {
		vboCoordTex->release();
		delete vboCoordTex;
		vboCoordTex = NULL;
	}

	if (vboTangents) {
		vboTangents->release();
		delete vboTangents;
		vboTangents = NULL;
	}

	if (vaoObject) {
		vaoObject->release();
		delete vaoObject;
		vaoObject = NULL;
	}
}

void Mesh::createVAO()
{
	destroyVAO();

	vaoObject = new QOpenGLVertexArrayObject(this);
	vaoObject->create();
	vaoObject->bind();

	vboVertices = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboVertices->create();
	vboVertices->bind();
	vboVertices->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboVertices->allocate(vertices, numVertices * sizeof(QVector4D));
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
	vboNormals->allocate(normals, numFaces * sizeof(QVector3D));
	delete[] normals;
	normals = NULL;

	vboCoordTex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboCoordTex->create();
	vboCoordTex->bind();
	vboCoordTex->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboCoordTex->allocate(texCoords, numVertices * sizeof(QVector2D));
	delete[] texCoords;
	texCoords = NULL;

	vboTangents = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboTangents->create();
	vboTangents->bind();
	vboTangents->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboTangents->allocate(tangents, numFaces * sizeof(QVector4D));
	delete [] tangents;
	tangents = NULL;

	vboIndices = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	vboIndices ->create();
	vboIndices ->bind();
	vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboIndices ->allocate(indices, numFaces * 3 * sizeof (unsigned int));
	delete[] indices;
	indices = NULL;

	vaoObject->release();
}

/* Faz o parsing dos arquivos .OFF */
void Mesh::newMesh(QString fileName)
{
	 std::ifstream stream;
	stream.open(fileName.toUtf8(), std::ifstream::in);

	//std::cerr  << fileName.toStdString() << std::endl;

	if (!stream.is_open()) {
		qWarning("Cannot open file");
		return;
	}

	std::string line;


	stream >> line;
	stream >> numVertices >> numFaces >> line;

	delete[] vertices;
	vertices = new QVector4D[numVertices];

	delete[] colors;
	colors = new QVector4D[numVertices];

	delete[] indices;
	indices = new uint[numFaces * 3];

	if (numVertices > 0) {
		double minLim = std::numeric_limits <double>::min();
		double maxLim = std::numeric_limits <double>::max();

		QVector4D max(minLim , minLim , minLim , 1.0);
		QVector4D min(maxLim , maxLim , maxLim , 1.0);

		for (uint i = 0; i < numVertices; i++) {
			double x, y, z;

			stream >> x >> y >> z;
			max.setX(qMax <double>(max.x(), x));
			max.setY(qMax <double>(max.y(), y));
			max.setZ(qMax <double>(max.z(), z));

			min.setX(qMin <double>(min.x(), x));
			min.setY(qMin <double>(min.y(), y));
			min.setZ(qMin <double>(min.z(), z));

			vertices[i] = QVector4D(x, y, z, 1.0);

			float r1 = rand() / (float) (RAND_MAX);
			float r2 = rand() / (float) (RAND_MAX);
			float r3 = rand() / (float) (RAND_MAX);

			colors[i] = QVector4D( r1,
								   r2,
								   r3,
								   1.0);
		}

		midPoint = ((min + max) * 0.5).toVector3D();

		invDiag = 1 / (max - min).length();
	}

	for (uint i = 0; i < numFaces; i++) {

		uint a, b, c;
		stream >> line >> a >> b >> c;

		indices[i * 3 ] = a;
		indices[i * 3 + 1] = b;
		indices[i * 3 + 2] = c;
	}

	stream.close();
	calculateNormal();
	genTexCoordsCylinder();
	genTangents();

	createVAO();
	createShaders();
}

/* Calcula as normais de um objeto, feito em sala de aula */
void Mesh::calculateNormal()
{
	normals = new QVector3D[numVertices];
	QVector3D normalFace, vectorA, vectorB, vectorC;
	uint i;

	for (i = 0; i < numFaces; i++) {
		// Calcula a normal da face
		vectorA = QVector3D(vertices[indices[i * 3]]);
		vectorB = QVector3D(vertices[indices[i * 3 + 1]]);
		vectorC = QVector3D(vertices[indices[i * 3 + 2]]);

		normalFace = QVector3D
				::crossProduct( QVector3D(vectorB - vectorA),
								QVector3D(vectorC - vectorA)
								).normalized();

		// Acumula nos vertices
		normals[indices[i * 3]]		+= normalFace;
		normals[indices[i * 3 + 1]]	+= normalFace;
		normals[indices[i * 3 + 2]]	+= normalFace;
	}

	for (i = 0; i < numVertices; i++)
		normals[i].normalize();

}

void Mesh::destroyShaders()
{
	if (vertexShader) {
		delete vertexShader;
		vertexShader = NULL;
	}

	if (fragmentShader) {
		delete fragmentShader;
		fragmentShader = NULL;
	}

	if(shaderProgram) {
		shaderProgram->release();
		delete shaderProgram;
		shaderProgram = NULL;
	}
}

void Mesh::createShaders()
{
	destroyShaders();

	QString vertexShaderFile[] = {
		":/shaders/flat.vert",
		":/shaders/gouraud.vert",
		":/shaders/phong.vert",
		":/shaders/toon.vert",
		":/shaders/halfphong.vert",
		":/shaders/texture.vert",
		":/shaders/normal.vert"
	};

	QString fragmentShaderFile[] = {
		":/shaders/flat.frag",
		":/shaders/gouraud.frag",
		":/shaders/phong.frag",
		":/shaders/toon.frag",
		":/shaders/halfphong.frag",
		":/shaders/texture.frag",
		":/shaders/normal.frag"
	};

	vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
	if (!vertexShader->compileSourceFile(vertexShaderFile[currentShader]))
		qWarning() << vertexShader->log();

	fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
	if (!fragmentShader->compileSourceFile(fragmentShaderFile[currentShader]))
		qWarning() << fragmentShader->log();

	shaderProgram = new QOpenGLShaderProgram;
	shaderProgram->addShader(vertexShader);
	shaderProgram->addShader(fragmentShader);

	if (!shaderProgram->link())
		qWarning() << shaderProgram->log() << endl;
}

/* Texture cilíndrica */
void Mesh::genTexCoordsCylinder()
{


	if(texCoords)
		delete [] texCoords ;

	// Compute minimum and maximum values
	float fmax = std::numeric_limits<float>::max();
	float minz = fmax ;
	float maxz = - fmax ;

	for (int i = 0; i < numVertices; ++i) {
		if(vertices[i]. z()< minz)minz = vertices[i]. z();
		if(vertices[i]. z()> maxz)maxz = vertices[i]. z();
	}

	texCoords = new QVector2D[numVertices];

	for (int i = 0; i < numVertices; ++i) {
		float s =(atan2(vertices[i].y(), vertices[i].x())+M_PI)/(2*M_PI);
		float t = 1.0f -(vertices[i].z()- minz)/(maxz - minz);
		texCoords[i] = QVector2D(s,t);
	}
}

/* Normal mapping */
void Mesh::genTangents()
{
	uint i;

	if (tangents) {
		delete [] tangents;
		tangents = NULL;
	}

	tangents = new QVector4D[numVertices];
	QVector3D *bitangents = new QVector3D[numVertices];

	for (i = 0; i < numFaces; i++) {

		uint i1 = indices[i * 3];
		uint i2 = indices[i * 3 + 1];
		uint i3 = indices[i * 3 + 2];

		QVector3D E = vertices[i1].toVector3D();
		QVector3D F = vertices[i2].toVector3D();
		QVector3D G = vertices[i3].toVector3D();


		QVector2D stE = texCoords[i1];
		QVector2D stF = texCoords[i2];
		QVector2D stG = texCoords[i3];

		QVector3D P = F - E;
		QVector3D Q = G - E;

		QVector2D st1 = stF - stE;
		QVector2D st2 = stG - stE;

		QMatrix2x2 M ;
		M(0, 0) = st2.y();
		M(0, 1) = -st1.y();
		M(1, 0) = -st2.x();
		M(1, 1) = st1.x();
		M *= (1.0 / ( st1.x() * st2.y() - st2.x() * st1.y())) ;
		QVector4D T = QVector4D (M(0, 0) * P.x() + M(0, 1) * Q.x(),
		M (0, 0) * P.y() + M (0, 1) * Q.y(),
		M (0, 0) * P.z() + M (0, 1) * Q.z(), 0.0);

		QVector3D B = QVector3D (M (1, 0) * P.x() + M (1, 1) * Q.x(),
		M (1, 0) * P.y() + M (1, 1) * Q.y() ,
		M (1, 0) * P.z() + M (1, 1) * Q.z()) ;

		tangents[i1] += T;
		tangents[i2] += T;
		tangents[i3] += T;
		bitangents[i1] += B;
		bitangents[i2] += B;
		bitangents[i3] += B;
	}

	for (i = 0; i < numVertices ; i++) {
		const QVector3D &n = normals[i];
		const QVector4D &t = tangents[i];
		tangents [i] = (t - n * QVector3D::dotProduct(n, t.toVector3D())).normalized();
		QVector3D b = QVector3D::crossProduct(n, t.toVector3D());
		double hand = QVector3D::dotProduct(b, bitangents[i]);
		tangents[i].setW((hand < 0.0) ? -1.0 : 1.0) ;
	}

	delete [] bitangents;
}
