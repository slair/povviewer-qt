// -*- coding: utf-8 -*-
/** @file mainwindow.cpp
 ** Main window
 **/

//~ #include <QMenuBar>
//~ #include <QMenu>
//~ #include <QMessageBox>
#include <QVBoxLayout>

#include "mainwindow.h"

MainWindow::MainWindow(pov_Scene* scene)
{
	qDebug() << "MainWindow::MainWindow(" << scene << ")" << endl;
	m_scene = scene;
	glWidget = new GLWidget;
	setCentralWidget(glWidget);
	//~ QVBoxLayout* mainLayout = new QVBoxLayout;
	//~ mainLayout->addWidget(glWidget);
	//~ setLayout(mainLayout);
	//~ setWindowTitle(tr(scene->filename()));
	// todo:  12. set geometry
}
