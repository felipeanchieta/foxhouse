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

	mesh[0] = new Mesh(0.0f);
	mesh[1] = new Mesh(-0.21f);

	for (int i = 2; i < 10; i++)
		mesh[i] = new Mesh(0.0f);

	mesh[0]->material.diffuse = QVector4D(0.8f, 0.2f, 0.2f, 0.8f);
	mesh[1]->material.diffuse = QVector4D(0.2f, 1.0f, 0.2f, 1);

	//	image = QImage(":/textures/texture_02.png");
	//	image = QImage(":/textures/texture_01.jpg", "JPG");

}

OpenGLWidget::~OpenGLWidget()
{

	for (int i = 0; i < 10; i++) {
		delete mesh[i];
		mesh[i] = NULL;
	}

}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	glEnable(GL_DEPTH_TEST);

	mesh[0]->newMesh("../foxhouse/fox.off");
	mesh[1]->newMesh("../foxhouse/chao.off");
	//ground->newmesh[0]("../foxhouse/sun.off");


	connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer.start(5);

	//loadNewObjects(0);
}

void OpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	mesh[0]->projectionMatrix.setToIdentity();
	mesh[0]->projectionMatrix.perspective(60.0, static_cast<qreal>(w)/
									   static_cast<qreal>(h), 0.1, 20.0);

	mesh[1]->projectionMatrix.setToIdentity();
	mesh[1]->projectionMatrix.perspective(60.0, static_cast<qreal>(w)/
									   static_cast<qreal>(h), 0.1, 20.0);
	mesh[1]->trackBall.resizeViewport(w, h);

	mesh[0]->trackBall.resizeViewport(w, h);
	update();
}

void OpenGLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.92f, 0.95f, 0.98f, 1.0f);

	viewMatrix.setToIdentity();
	//viewMatrix.rotate(trackBall.getRotation());

	mesh[0]->drawMesh(viewMatrix);
	mesh[1]->drawMesh(viewMatrix);
	//ground->drawMesh();
	update();
}

void OpenGLWidget::toggleBackgroundColor(bool changeBColor)
{
	blackBackground = changeBColor;

	update();
}

void OpenGLWidget::animate()
{
	update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	mesh[0]->trackBall.mouseMove(event->localPos());
	mesh[1]->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton) {
		mesh[0]->trackBall.mousePress(event->localPos());
		mesh[1]->trackBall.mousePress(event->localPos());
	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		mesh[0]->trackBall.mouseRelease(event->localPos());
		mesh[1]->trackBall.mouseRelease(event->localPos());

	}

}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
	mesh[0]->zoomEW += 0.001 * event->delta();
	mesh[0]->zoomNS += 0.001 * event->delta();
}


void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
	switch (event->key()) {
	/* Escape */
	case Qt::Key_Escape:
		quick_exit(0);
		break;

	/* Movimentação da câmera */

	case Qt::Key_W:
	case Qt::Key_Up:
		/* north */
		mesh[0]->zoomNS += 0.1f;
		mesh[0]->drawMesh(viewMatrix);
		mesh[1]->zoomNS += 0.1f;
		mesh[1]->drawMesh(viewMatrix);
		break;

	case Qt::Key_S:
	case Qt::Key_Down:
		/* south */
		mesh[0]->zoomNS -= 0.1f;
		mesh[0]->drawMesh(viewMatrix);
		mesh[1]->zoomNS -= 0.1f;
		mesh[1]->drawMesh(viewMatrix);
		break;

	case Qt::Key_D:
	case Qt::Key_Right:
		/* east */
		mesh[0]->zoomEW -= 0.1f;
		mesh[0]->drawMesh(viewMatrix);
		mesh[1]->zoomEW -= 0.1f;
		mesh[1]->drawMesh(viewMatrix);
		break;

	case Qt::Key_A:
	case Qt::Key_Left:
		/* west */
		mesh[0]->zoomEW += 0.1f;
		mesh[0]->drawMesh(viewMatrix);
		mesh[1]->zoomEW += 0.1f;
		mesh[1]->drawMesh(viewMatrix);
		break;

	default:
		break;
	}
}



//void OpenGLWidget::createTexture(const QString &imagePath)
//{
//	makeCurrent();
//	image.load(imagePath);
//	mesh->texture = new QOpenGLTexture(image);

//	mesh->texture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
//	mesh->texture->setMagnificationFilter(QOpenGLTexture::Linear);
//	mesh->texture->setWrapMode(QOpenGLTexture::Repeat);
//}



void OpenGLWidget::loadNewObjects(int i)
{
	switch (i) {
	case ARMADILLO:
		break;

	default:
		break;
	}

	mesh[0]->createVAO();
	mesh[0]->createShaders();

	mesh[1]->createVAO();
	mesh[1]->createShaders();

	update();
}
