#include "mainwindow.h"
#include <QApplication>
#include <QSurfaceFormat>

int main(int argc, char *argv[])
{
	QSurfaceFormat format;
	format.setVersion(3, 3);
	format.setDepthBufferSize(24);
	format.setSamples(4);
	format.setProfile(QSurfaceFormat::CoreProfile);
	QSurfaceFormat::setDefaultFormat(format);

	QApplication a(argc, argv);
	MainWindow w;

	w.setGeometry(QStyle::alignedRect(Qt::LeftToRight,
									  Qt::AlignCenter,
									  w.size(),
									  a.desktop()->availableGeometry()));

	w.show();
	return a.exec();
}
