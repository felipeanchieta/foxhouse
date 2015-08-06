#-------------------------------------------------
#
# Project created by QtCreator 2015-06-10T14:22:16
#
#-------------------------------------------------

QT       += core gui opengl openglextensions

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = aulaGL
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    openglwidget.cpp \
    trackball.cpp \
    camera.cpp \
    material.cpp \
    light.cpp \
    mesh.cpp \
    fpmouse.cpp

HEADERS  += mainwindow.h \
    openglwidget.h \
    trackball.h \
    camera.h \
    material.h \
    light.h \
    mesh.h \
    fpmouse.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc

DISTFILES += \
    fshader1.glsl \
    vshader1.glsl \
    fshader.glsl \
    vshader.glsl \
    gouraud.vert \
    gouraud.frag \
    phong.vert \
    phong.frag \
    flat.vert \
    flat.frag \
    toon.frag \
    toon.vert \
    halfphong.frag \
    halfphong.vert \
    texture.frag \
    texture.vert
