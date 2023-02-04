// -*- coding: utf-8 -*-
/** @file mainwindow.cpp
 ** Main window
 **/

//~ #include <QMenuBar>
//~ #include <QMenu>
//~ #include <QMessageBox>
#include <QApplication>
#include <QVBoxLayout>

#include "mainwindow.h"

MainWindow::MainWindow(pov_Scene* scene)
{
	qDebug() << "MainWindow::MainWindow(" << scene << ")";
	m_scene = scene;
	glWidget = new GLWidget(nullptr, m_scene);
	setCentralWidget(glWidget);
	setWindowTitle(QCoreApplication::applicationName()
				   + " - " + m_scene->filename());
	// done:  12. set geometry
	setGeometry(m_scene->cfg()->window_pos_x()
				, m_scene->cfg()->window_pos_y()
				, m_scene->cfg()->window_width()
				, m_scene->cfg()->window_height());
}

MainWindow::~MainWindow()
{
	qDebug() << "MainWindow::~MainWindow()";
	// done:  16. save position and size mainwindow
	qDebug() << geometry();
	m_scene->cfg()->set_window_pos_x(geometry().x());
	m_scene->cfg()->set_window_pos_y(geometry().y());
	m_scene->cfg()->set_window_width(geometry().width());
	m_scene->cfg()->set_window_height(geometry().height());
}
