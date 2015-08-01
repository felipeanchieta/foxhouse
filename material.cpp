#include "material.h"
#include "openglwidget.h"

Material::Material()
{
	ambient = QVector4D(1, 1, 1, 1.0);
	diffuse = QVector4D(0.0, 0.0, 0.0, 1.0);
	specular = QVector4D(1, 1, 0.5, 1.0);
	shininess = 200.0;

}

