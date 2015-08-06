#include "openglwidget.h"

#define PLANESCALE 0.1f

OpenGLWidget::OpenGLWidget(QWidget *parent) : QOpenGLWidget(parent)
{
	/* Alocação dos objetos na memória e definição das suas respectivas propriedades */

	casaFox = new Mesh(0.0f, 2);
	gramado = new Mesh(-0.21f, 2);
	ceu = new Mesh(0.0f, 2);

	casaFox->material.diffuse = QVector4D(0.8f, 0.2f, 0.2f, 0.8f);
	gramado->material.diffuse = QVector4D(0.2f, 1.0f, 0.2f, 1);
	ceu->material.diffuse = QVector4D(0.9f, 0.0f, 0.0f, 1.0f);
}

OpenGLWidget::~OpenGLWidget()
{

	if (casaFox) {
		delete casaFox;
		casaFox = NULL;
	}

	if (gramado) {
		delete gramado;
		gramado = NULL;
	}

	if (ceu) {
		delete ceu;
		ceu = NULL;
	}
}

void OpenGLWidget::initializeGL()
{
	initializeOpenGLFunctions();

	std::cout << "Versão do OpenGL: "	<< glGetString(GL_VERSION) << std::endl;
	std::cout << "Versão do GLSL: "		<< glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	/* Permite o eixo z, que dá a profunddade tridimesional */
	glEnable(GL_DEPTH_TEST);

	/* Carrega os objetos da cena */
	casaFox->newMesh("../foxhouse/fox.off");
	gramado->newMesh("../foxhouse/chao_.off");
	ceu->newMesh("../foxhouse/ceu.off");

	connect(&timer, SIGNAL(timeout()), this, SLOT(animate()));
	timer.start(5);
}

void OpenGLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);

	/* Ajuste da perspectiva dos objetos da cena dependendo do trackball e das dimensões do widget s*/

	casaFox->projectionMatrix.setToIdentity();
	casaFox->projectionMatrix.perspective(60.0, static_cast<qreal>(w)/static_cast<qreal>(h), 0.1, 20.0);

	gramado->projectionMatrix.setToIdentity();
	gramado->projectionMatrix.perspective(60.0, static_cast<qreal>(w)/static_cast<qreal>(h), 0.1, 20.0);

	gramado->trackBall.resizeViewport(w, h);
	casaFox->trackBall.resizeViewport(w, h);

	update();
}

void OpenGLWidget::paintGL()
{
	/* Limpa o buffer de pintura e profundidade e insere um plano de fundo azul */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.8f, 0.9f, 1.0f, 1.0f);

	/* Desenha os objetos de cena com seus respectivos métodos */
	casaFox->drawMesh(QVector3D(0, 0, 0));
	gramado->drawMesh(QVector3D(PLANESCALE, 0, PLANESCALE));
	ceu->drawMesh(QVector3D(5*PLANESCALE, 5*PLANESCALE, 5*PLANESCALE));

	update();
}


void OpenGLWidget::animate()
{
	update();
}

void OpenGLWidget::mouseMoveEvent(QMouseEvent *event)
{
	casaFox->trackBall.mouseMove(event->localPos());
	gramado->trackBall.mouseMove(event->localPos());
}

void OpenGLWidget::mousePressEvent(QMouseEvent *event)
{
	if (event->button() & Qt::LeftButton) {
		casaFox->trackBall.mousePress(event->localPos());
		gramado->trackBall.mousePress(event->localPos());
	}
}

void OpenGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton) {
		casaFox->trackBall.mouseRelease(event->localPos());
		gramado->trackBall.mouseRelease(event->localPos());
	}
}

void OpenGLWidget::wheelEvent(QWheelEvent *event)
{
	/* Dá o zoom nos eixos tridimensionais dos objetos da cena */

	casaFox->zoomEW += 0.001 * event->delta();
	casaFox->zoomNS += 0.001 * event->delta();
	gramado->zoomEW += 0.001 * event->delta();
	gramado->zoomNS += 0.001 * event->delta();
	ceu->zoomEW += 0.001 * event->delta();
	ceu->zoomNS += 0.001 * event->delta();
}


void OpenGLWidget::keyPressEvent(QKeyEvent *event)
{
	makeCurrent();

	switch (event->key()) {

	case Qt::Key_Escape:
		quick_exit(0);
		break;

	/* Movimentação da câmera */

	case Qt::Key_W:
	case Qt::Key_Up:
		/* north */
		casaFox->zoomNS += 0.1f;
		gramado->zoomNS += 0.1f;
		break;

	case Qt::Key_S:
	case Qt::Key_Down:
		/* south */
		casaFox->zoomNS -= 0.1f;
		gramado->zoomNS -= 0.1f;
		break;

	case Qt::Key_D:
	case Qt::Key_Right:
		/* east */
		casaFox->zoomEW -= 0.1f;
		gramado->zoomEW -= 0.1f;
		break;

	case Qt::Key_A:
	case Qt::Key_Left:
		/* west */
		casaFox->zoomEW += 0.1f;
		gramado->zoomEW += 0.1f;
		break;

	case Qt::Key_F11:
		/* TODO: Fullscreen */
		break;

	case Qt::Key_P:
		/* Ctrl-P: Screenshot */
		if (QApplication::keyboardModifiers() & Qt::ControlModifier)
			emit takeScreenshot();

	default:
		break;
	}

	update();
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


void OpenGLWidget::takeScreenshot()
{
	QImage screenshot = grabFramebuffer();

	QString fileName;
	fileName = QFileDialog::getSaveFileName(this, "Save File As",
											QDir::homePath(),
											QString("PNG Files (*.png)"));
	if (fileName.length()) {
		if (!fileName.contains(".png"))
			fileName += ".png";
		if (screenshot.save(fileName, "PNG")) {
			QMessageBox::information(this, "Screenshot",
									 "Screenshot taken!",
									 QMessageBox::Ok);
		}
	}
}
