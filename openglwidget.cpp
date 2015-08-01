#include "openglwidget.h"

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{


	vertexShader = NULL;
	fragmentShader = NULL;
	shaderProgram = NULL;

	_scaleObject = false;
	_centralizeObject = false;
	blackBackground = false;
	xAngle = yAngle = zAngle = 0;
	zoom = 0;

//	image = QImage(":/textures/texture_02.png");
//	image = QImage(":/textures/texture_01.jpg", "JPG");

}

OpenGLWidget::~OpenGLWidget()
{
	mesh->destroyVBO();
	destroyShaders();
}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();



	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer.start(5);

	glEnable(GL_DEPTH_TEST);

	//loadNewObjects(0);

//	this->createVBO();
//	this->createShaders();
}

void OpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	mesh->projectionMatrix.setToIdentity();
	mesh->projectionMatrix.perspective(60.0, static_cast<qreal>(w)/
					   static_cast<qreal>(h), 0.1, 20.0);
	trackBall.resizeViewport(w, h);
	update();
}

void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (!blackBackground)
		glClearColor(1, 1, 1, 1);
	else
		glClearColor(0, 0, 0, 1);

	mesh->setShaderProgram(shaderProgram);
	mesh->drawMesh();

	 /* if (!vboVertices)
		return;
	modelView.setToIdentity();
	modelView.lookAt(camera.eye, camera.at, camera.up);
	modelView.translate(0, 0, zoom);
	modelView.rotate(trackBall.getRotation());
	modelView.scale(invDiag, invDiag, invDiag);
	modelView.translate(- midPoint);

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

	*/
}

void OpenGLWidget::toggleBackgroundColor(bool changeBColor)
{
	blackBackground = changeBColor;

	update();
}

/* void OpenGLWidget::createVBO()
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

void OpenGLWidget::destroyVBO()
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
} */

void OpenGLWidget::destroyShaders()
{
	delete vertexShader;
	vertexShader = NULL;

	delete fragmentShader;
	fragmentShader = NULL;

	if(shaderProgram) {
		shaderProgram->release();
		delete shaderProgram;
		shaderProgram = NULL;
	}
}

void OpenGLWidget::createShaders()
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

	std::cerr	<< "Vertex: " << vertexShaderFile[currentShader].toStdString()
				<< std::endl
				<< "Fragment: " << fragmentShaderFile[currentShader].toStdString()
				<< std::endl;

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

void OpenGLWidget::showFileOpenDialog()
{
	QByteArray fileFormat = "off";
	QString fileName;

	fileName = QFileDialog::getOpenFileName(this, "Open File",
						QDir::homePath(),
						QString("%1 Files (*.%2) ")
						.arg(QString(fileFormat.
							     toUpper()))
						.arg(QString(fileFormat)));

	if (!fileName.isEmpty())
		readOFFFile(fileName);

	mesh->createVBO();
	createShaders();
	update();
}

void OpenGLWidget::readOFFFile(const QString &fileName)
{
	std::ifstream stream;
	stream.open(fileName.toUtf8(), std::ifstream::in);

	std::cerr  << fileName.toStdString() << std::endl;

	if (!stream.is_open()) {
		qWarning("Cannot open file");
		return;
	}

	std::string line;

	mesh = new Mesh();
	stream >> line;
	stream >> mesh->numVertices >> mesh->numFaces >> line;

	delete[] mesh->vertices;
	mesh->vertices = new QVector4D[mesh->numVertices];

	delete[] mesh->colors;
	mesh->colors = new QVector4D[mesh->numVertices];

	delete[] mesh->indices;
	mesh->indices = new uint[mesh->numFaces * 3];

	if (mesh->numVertices > 0) {
		double minLim = std::numeric_limits <double>::min();
		double maxLim = std::numeric_limits <double>::max();

		QVector4D max(minLim , minLim , minLim , 1.0);
		QVector4D min(maxLim , maxLim , maxLim , 1.0);

		for (uint i = 0; i < mesh->numVertices; i++) {
			double x, y, z;

			stream >> x >> y >> z;
			max.setX(qMax <double>(max.x(), x));
			max.setY(qMax <double>(max.y(), y));
			max.setZ(qMax <double>(max.z(), z));

			min.setX(qMin <double>(min.x(), x));
			min.setY(qMin <double>(min.y(), y));
			min.setZ(qMin <double>(min.z(), z));

			mesh->vertices[i] = QVector4D(x, y, z, 1.0);

			float r1 = rand() / (float) (RAND_MAX);
			float r2 = rand() / (float) (RAND_MAX);
			float r3 = rand() / (float) (RAND_MAX);

			mesh->colors[i] = QVector4D( r1,
					       r2,
					       r3,
					       1.0);
		}

		mesh->midPoint = ((min + max) * 0.5).toVector3D();

		invDiag = 1 / (max - min).length();
	}

	for (uint i = 0; i < mesh->numFaces; i++) {

		uint a, b, c;
		stream >> line >> a >> b >> c;

		mesh->indices[i * 3 ] = a;
		mesh->indices[i * 3 + 1] = b;
		mesh->indices[i * 3 + 2] = c;
	}

	emit statusBarMessage(QString("Samples %1, Faces %2").
				  arg(mesh->numVertices).arg(mesh->numFaces));

	stream.close();
	calculateNormal();
	genTexCoordsCylinder();
}

void OpenGLWidget::changeDiagonal(bool condition)
{
	mesh->vboIndices->bind();
	uint *pt = (uint *) mesh->vboIndices->map(QOpenGLBuffer::WriteOnly);

	if (condition) {
		pt[2] = 3;
		pt[5] = 1;
	} else {
		pt[2] = 2;
		pt[5] = 0;
	}

	mesh->vboIndices->unmap();
	update();
}

void OpenGLWidget::centralizeObject(bool t)
{
//	makeCurrent();

//	_centralizeObject = t;

//	update();
}

void OpenGLWidget::scaleObject(bool t)
{
//	makeCurrent();

//	_scaleObject = t;

//	update();
}

void OpenGLWidget::rotateX(int angle)
{
//	makeCurrent();

//	xAngle = angle;

//	update();
}

void OpenGLWidget::rotateY(int angle)
{
//	makeCurrent();

//	yAngle = angle;

//	update();
}

void OpenGLWidget::rotateZ(int angle)
{
//	makeCurrent();

//	zAngle = angle;

//	update();
}

void OpenGLWidget::animate()
{
	update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton)
		trackBall.mousePress(event->localPos());
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
		trackBall.mouseRelease(event->localPos());

}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
	zoom += 0.001 * event->delta();
}

void OpenGLWidget::calculateNormal()
{
	mesh->normals = new QVector3D[mesh->numVertices];
	QVector3D normalFace, vectorA, vectorB, vectorC;
	uint i;

	for (i = 0; i < mesh->numFaces; i++) {
		// Calcula a normal da face
		vectorA = QVector3D(mesh->vertices[mesh->indices[i * 3]]);
		vectorB = QVector3D(mesh->vertices[mesh->indices[i * 3 + 1]]);
		vectorC = QVector3D(mesh->vertices[mesh->indices[i * 3 + 2]]);

		normalFace = QVector3D
				::crossProduct( QVector3D(vectorB - vectorA),
						QVector3D(vectorC - vectorA)
					).normalized();

		// Acumula nos vertices
		mesh->normals[mesh->indices[i * 3]]		+= normalFace;
		mesh->normals[mesh->indices[i * 3 + 1]]	+= normalFace;
		mesh->normals[mesh->indices[i * 3 + 2]]	+= normalFace;
	}

	for (i = 0; i < mesh->numVertices; i++)
		mesh->normals[i].normalize();

}

void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	case Qt::Key_Escape:
		quick_exit(0);
		break;
	default:
		break;
	}
}

void OpenGLWidget::changeShader(int i)
{
	makeCurrent();
	//std::cout << i << std::endl;
	currentShader = i;
	createShaders();
}

void OpenGLWidget::Rchanged(int r)
{
	makeCurrent();
	float rF;

	rF = r/100.0;
	mesh->material.diffuse.setX(rF);

	createShaders();
}

void OpenGLWidget::Gchanged(int g)
{
	makeCurrent();
	float gF;

	gF = g/100.0;
	mesh->material.diffuse.setY(gF);

	createShaders();
}

void OpenGLWidget::Bchanged(int b)
{
	makeCurrent();
	float bF;

	bF = b/100.0;
	mesh->material.diffuse.setZ(bF);

	createShaders();
}

void OpenGLWidget::lightChanged(int l)
{
	mesh->light.ambient = QVector4D(l/100.0, l/100.0, l/100.0, mesh->light.ambient.w());
}

void OpenGLWidget::createTexture(const QString &imagePath)
{
	makeCurrent();
	image.load(imagePath);
	mesh->texture = new QOpenGLTexture(image);

	mesh->texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
	mesh->texture->setMagnificationFilter(QOpenGLTexture::Linear);
	mesh->texture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLWidget::genTexCoordsCylinder()
{
	if (mesh->texCoords)
		delete[] mesh->texCoords;

	// Compute minimum and maximum values
	float fmax = std::numeric_limits<float>::max();
	float minz = fmax;
	float maxz = - fmax;

	for (int i = 0; i < mesh->numVertices; ++i) {
		if(mesh->vertices[i].z() < minz)
			minz = mesh->vertices[i].z();

		if(mesh->vertices[i].z() > maxz)
			maxz = mesh->vertices[i].z();
	}

	mesh->texCoords = new QVector2D[mesh->numVertices];

	for (int i = 0; i < mesh->numVertices; ++i) {
		float s = (atan2(mesh->vertices[i].y(), mesh->vertices[i].x()) + M_PI) / (2 * M_PI);
		float t = 1.0f - (mesh->vertices[i].z() - minz) / (maxz - minz);

		mesh->texCoords[i] =  QVector2D(s, t);
	}
}

void OpenGLWidget::loadNewObjects(int i)
{
	switch (i) {
	case ARMADILLO:
		readOFFFile(QString("../aulaGL/armadillo.off"));
		break;

	case BEETLE:
		readOFFFile(QString("../aulaGL/beetle.off"));
		break;

	case BUNNY:
		readOFFFile(QString("../aulaGL/bunny.off"));
		break;

	case CAMEL:
		readOFFFile(QString("../aulaGL/camel.off"));
		break;

	case ELEPHANT:
		readOFFFile(QString("../aulaGL/elephant.off"));
		break;

	case ELK:
		readOFFFile(QString("../aulaGL/elk.off"));
		break;

	default:
		break;
	}

	mesh->createVBO();
	createShaders();
	update();
}

/* enum {
		ARMADILLO, BEETLE, BUNNY, CAMEL, ELEPHANT, ELK, MANNEQUIN, MUSHROOM,
		PEAR, PIG, SPHERE, TORUS
	};*/
