#-------------------------------------------------
#
# Project created by QtCreator 2016-06-27T16:21:26
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CourseProject
TEMPLATE = app
QMAKE_CXXFLAGS += -std=c++11

#QMAKE_CXXFLAGS_DEBUG += -pg
#QMAKE_LFLAGS_DEBUG += -pg


SOURCES += main.cpp\
        mainwindow.cpp \
    imageitem.cpp \
    painter.cpp \
    camera.cpp \
    cammod.cpp \
    mgraphicsview.cpp \
    texture.cpp \
    geometry/geometry.cpp \
    geometry/matrix.cc \
    model/poolmodel.cpp \
    model/skybox.cpp \
    model/watermodel.cpp \
    vertex.cpp \
    model/model.cpp \
    render.cpp

HEADERS  += mainwindow.h \
    imageitem.h \
    painter.h \
    camera.h \
    cammod.h \
    mgraphicsview.h \
    texture.h \
    geometry/geometry.h \
    geometry/matrix2.hxx \
    model/poolmodel.h \
    model/skybox.h \
    model/watermodel.h \
    vertex.h \
    model/model.h \
    geometry/cmatrix \
    render.h

FORMS    += mainwindow.ui

RESOURCES += \
    textures.qrc \
    modeldata.qrc
