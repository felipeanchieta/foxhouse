#include "mesh.h"

Mesh::Mesh(float zPosition)
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
	vaoObject = NULL;

	vertexShader = NULL;
	fragmentShader = NULL;
	shaderProgram = NULL;

	colorMaterial = QVector3D(0,0,0);
	angle = .0f;
	currentShader = 0;

	vPos = zPosition;
}

Mesh::~Mesh()
{
	destroyVAO();
	destroyShaders();
}

void Mesh::drawMesh(QMatrix4x4 &viewMatrix)
{

	/* glClear(GL_COLOR_BUFFER_BIT); */

	if (!vboVertices)
		return;


	modelView.setToIdentity();

//	modelView *= viewMatrix;

	modelView.lookAt(camera.eye, camera.at, camera.up);
	modelView.translate(zoomEW, vPos, zoomNS);
	modelView.rotate(trackBall.getRotation());
//	modelView *= viewMatrix;
	modelView.scale(invDiag, invDiag, invDiag);
	modelView.translate(- midPoint);



	shaderProgram->bind();
	vaoObject->bind();


	QVector4D ambientProduct = light.ambient * material.ambient;
	QVector4D diffuseProduct = light.diffuse * material.diffuse;
	QVector4D specularProduct = light.specular * material.specular;

	/* vboCoordTex->bind();
	shaderProgram->enableAttributeArray("vcoordText") ;
	shaderProgram->setAttributeBuffer("vcoordText", GL_FLOAT, 0, 2, 0);

	colorTexture = new QOpenGLTexture(image);
	colorTexture->bind(0);
	shaderProgram->setUniformValue("colorTexture", 0); */

	shaderProgram->setUniformValue("lightPosition", light.position);
	shaderProgram->setUniformValue("ambientProduct", ambientProduct);
	shaderProgram->setUniformValue("diffuseProduct", diffuseProduct);
	shaderProgram->setUniformValue("specularProduct", specularProduct);
	shaderProgram->setUniformValue("shininess", static_cast<GLfloat>(material.shininess));
	shaderProgram->setUniformValue("modelView", modelView);
	//shaderProgram->setUniformValue();
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
	//vboColors->release();
	//vboVertices->release();


	/* if(colorTexture) {
		colorTexture->release(0);
		delete colorTexture;
		colorTexture = NULL;
	}
 view *
	vboCoordTex->release();  */
	vaoObject->release();
	shaderProgram->release();

	update();
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
	// 4 = numVertices
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

	/* vboCoordTex = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	vboCoordTex->create();
	vboCoordTex->bind();
	vboCoordTex->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vboCoordTex->allocate(texCoords, numVertices * sizeof(QVector2D));
	delete[] texCoords;
	texCoords = NULL; */

	vboIndices = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	vboIndices ->create();
	vboIndices ->bind();
	vboIndices ->setUsagePattern(QOpenGLBuffer::StaticDraw);
	// 2 = numFaces
	vboIndices ->allocate(indices, numFaces * 3 * sizeof (unsigned int));
	delete[] indices;
	indices = NULL;

	vaoObject->release();
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

	if (vaoObject) {
		vaoObject->release();
		delete vaoObject;
		vaoObject = NULL;
	}
}

void Mesh::setShaderProgram(QOpenGLShaderProgram *shaderProgram)
{
	this->shaderProgram = shaderProgram;

	update();
}

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

//	emit statusBarMessage(QString("Samples %1, Faces %2").arg(numVertices).arg(numFaces));

	stream.close();
	calculateNormal();
	genTexCoordsCylinder();

	createVAO();
	createShaders();
}

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

void Mesh::genTexCoordsCylinder()
{
	if(texCoords)
		delete [] texCoords ;
	// Compute minimum and maximum values
	float fmax = std::numeric_limits<float>::max();
	float minz = fmax ;
	float maxz = - fmax ;
	for(int i =0; i < numVertices ; ++ i) {
		if(vertices[i]. z()< minz)minz = vertices[i]. z();
		if(vertices[i]. z()> maxz)maxz = vertices[i]. z();
	}
	texCoords = new QVector2D[numVertices];
	for(int i =0; i < numVertices ; ++ i) { // https :// en . wikipedia . org / wiki / Atan2
		float s =(atan2(vertices[i].y(), vertices[i].x())+M_PI)/(2*M_PI);
		float t = 1.0f -(vertices[i].z()- minz)/(maxz - minz);
		texCoords[i] = QVector2D(s,t);
	}
}
