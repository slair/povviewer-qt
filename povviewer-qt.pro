#!/usr/bin/qmake
# -*- coding: utf-8 -*-

# to show context
DEFINES += "DEBUG_CONTEXT=1"	# 1 - one line
#~ DEFINES += "DEBUG_CONTEXT=2"	# 2 - two lines(context then msg)

message($$QMAKESPEC)

linux-g++ {
	QMAKE_CXXFLAGS += -std=c++11
	QMAKE_CXXFLAGS += -Werror
}

win32-msvc* {
	QMAKE_CXXFLAGS += /WX
}

CONFIG += console

CONFIG += debug

HEADERS       =	sources/myconfig.h \
				sources/el_show.h \
				sources/glwidget.h \
				sources/mainwindow.h \
				sources/vec3d.h \
				sources/v2s.h \
				sources/povviewer.h \
				sources/pov_scene.h \
				sources/pov_transform.h \
				sources/pov_baseobject.h \
				sources/pov_sphere.h
				#~ sources/camera.h

SOURCES       =	sources/myconfig.cpp \
				#~ sources/el_show.cpp \
				sources/glwidget.cpp \
				sources/mainwindow.cpp \
				sources/vec3d.cpp \
				sources/v2s.cpp \
				sources/main.cpp \
				sources/pov_scene.cpp \
				sources/pov_transform.cpp \
				sources/pov_baseobject.cpp \
				sources/pov_sphere.cpp
				#~ sources/camera.cpp

QT           += widgets opengl

CONFIG(debug, debug|release) {
    DESTDIR = build/debug
}
CONFIG(release, debug|release) {
    DESTDIR = build/release
}

OBJECTS_DIR = $$DESTDIR/obj
MOC_DIR = $$DESTDIR/moc
RCC_DIR = $$DESTDIR/qrc
UI_DIR = $$DESTDIR/ui

PRECOMPILED_DIR = $$DESTDIR

#~ # install
#~ target.path = bin
#~ INSTALLS += target

