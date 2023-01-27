#~ CONFIG += c++11

linux {
	QMAKE_CXXFLAGS += -std=c++11
}

CONFIG += console

CONFIG += debug

HEADERS       =	sources/myconfig.h \
				sources/povviewer.h \
				sources/scene.h \
				sources/baseobj.h

SOURCES       =	sources/myconfig.cpp \
				sources/main.cpp \
				sources/scene.cpp \
				sources/baseobj.cpp

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

