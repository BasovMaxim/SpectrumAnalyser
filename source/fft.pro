#-------------------------------------------------
#
# Project created by QtCreator 2015-01-18T12:06:42
#
#-------------------------------------------------

QT       += multimedia core gui opengl
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += C++11
QMAKE_CXXFLAGS += -std=c++0x

DEFINES += #DEBUGOUTPUT
DEFINES += #DESKTOP

TARGET = fft
TEMPLATE = app

SOURCES += \
    acustomslider.cpp \
    ainputdevice.cpp \
    asettingsdialog.cpp \
    main.cpp \
    mainwindow.cpp \
\#Graphics
    graphics/aview.cpp \
    graphics/aplot.cpp \
    graphics/aoscilloscope.cpp \
    graphics/aspectrograph.cpp \
    graphics/aspectrogram.cpp \
    affttransformer.cpp


HEADERS += \
    acustomslider.h \
    ainputdevice.h \
    asettingsdialog.h \
    mainwindow.h \
    fft.h \
\#Graphics
    graphics/aview.h \
    graphics/aplot.h \
    graphics/aoscilloscope.h \
    graphics/aspectrograph.h \
    graphics/aspectrogram.h \
    affttransformer.h

FORMS   += \
    mainwindow.ui \
    acustomslider.ui \
    asettingsdialog.ui

RESOURCES += \
    res.qrc

DISTFILES += \
    styleSheet.css
