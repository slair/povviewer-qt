// -*- coding: utf-8 -*-
/** @file mainwindow.h
 ** Main window
 **/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QKeyEvent>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLWindow>

#include "pov_scene.h"
#include "camera.h"

class MainWindow : public QOpenGLWindow {
public:
	MainWindow(pov_Scene* scene);
	~MainWindow();

protected:
    void initializeGL() override;
    void resizeGL(int w, int h) override;
    void paintGL() override;
	void keyPressEvent(QKeyEvent *event) override;

private:
	pov_Scene* m_scene {nullptr};
	Camera* m_camera;

	QOpenGLFunctions_2_0* m_funcs {nullptr};
	QOpenGLShaderProgram* m_prg_a_pos_a_col {nullptr};
	QOpenGLShaderProgram* m_prg_u_col_a_pos {nullptr};
	// axis
	QOpenGLVertexArrayObject m_vao_axis;	// vao for axis
	QOpenGLBuffer m_axis_points {QOpenGLBuffer::VertexBuffer};	// for axis
	QOpenGLBuffer m_axis_indices {QOpenGLBuffer::IndexBuffer};	// for axis
	QMatrix4x4 m_mm1;	// for axis
	// scene
	QOpenGLVertexArrayObject m_vao_scene;	// vao for scene
	QVector<QMatrix4x4*> m_mm;		// model matrices for scene
	QVector<QOpenGLBuffer*> m_vbos;	// VertexBuffers for scene
	QVector<QOpenGLBuffer*> m_ibos;	// IndexBuffers for scene
	QVector<QVector4D*> m_mc;		// models colors for scene

	void initializeGeometry();
	void initializeShaders();
	void initializeTextures();

	void getGeometry();		// fill m_vbos and m_ibos with data from m_scene
	void getAxis();			// fill m_vbos and m_ibos with data from m_scene

	void clearBuffers();	// clear buffers for scene

	void drawAxis();		// draw axis
	void drawGeometry();	// draw scene

	void show_cam() const;

//~ private slots:
	//~ void onAddNew();
};

#endif
