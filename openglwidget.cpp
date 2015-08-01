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
    mesh->destroyVAO();
    mesh->destroyShaders();
}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	std::cout << glGetString(GL_VERSION) << std::endl;
	std::cout << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    glEnable(GL_DEPTH_TEST);

	connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer.start(5);

    mesh = new Mesh();
    mesh->newMesh("../foxhouse/armadillo.off");


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

    mesh->createVAO();
    mesh->createShaders();
	update();
}

/* enum {
		ARMADILLO, BEETLE, BUNNY, CAMEL, ELEPHANT, ELK, MANNEQUIN, MUSHROOM,
		PEAR, PIG, SPHERE, TORUS
	};*/
