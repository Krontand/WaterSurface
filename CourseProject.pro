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
    geometry.cpp \
    camera.cpp \
    cammod.cpp \
    mgraphicsview.cpp \
    poolmodel.cpp \
    texture.cpp \
    skybox.cpp \
    model.cpp \
    watermodel.cpp

HEADERS  += mainwindow.h \
    imageitem.h \
    painter.h \
    geometry.h \
    camera.h \
    cammod.h \
    mgraphicsview.h \
    poolmodel.h \
    texture.h \
    skybox.h \
    model.h \
    watermodel.h

FORMS    += mainwindow.ui

RESOURCES += \
    textures.qrc \
    modeldata.qrc
