#~ CONFIG += c++11

message($$QMAKESPEC)

linux-g++ {
	QMAKE_CXXFLAGS += -std=c++11
}

CONFIG += console

CONFIG += debug

HEADERS       =	sources/myconfig.h \
				sources/povviewer.h \
				sources/pov_scene.h \
				sources/pov_transform.h \
				sources/pov_baseobject.h \
				sources/pov_sphere.h

SOURCES       =	sources/myconfig.cpp \
				sources/main.cpp \
				sources/pov_scene.cpp \
				sources/pov_transform.cpp \
				sources/pov_baseobject.cpp \
				sources/pov_sphere.cpp

QT           += widgets

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

