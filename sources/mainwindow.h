// -*- coding: utf-8 -*-
/** @file mainwindow.h
 ** Main window
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>

#include "glwidget.h"
#include "pov_scene.h"

class MainWindow : public QMainWindow {
	Q_OBJECT

protected:
	void keyPressEvent(QKeyEvent *event) override;

public:
	MainWindow(pov_Scene* scene);
	~MainWindow();

private:
	pov_Scene* m_scene;
	GLWidget* glWidget;

private slots:
	//~ void onAddNew();
};

#endif
