// -*- coding: utf-8 -*-
/** @file glwidget.h
 ** glwidget and related
 **/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include "pov_scene.h"

//~ QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class PosCol {
public:
	PosCol() {
		position = QVector3D(0.0f, 0.0f, 0.0f);
		color = QVector4D(0.0f, 0.0f, 0.0f, 0.0f);
	};

	PosCol(const QVector3D& p, const QVector4D& c) {
		position = p;
		color = c;
	};

	PosCol(const PosCol& obj) {
		position = obj.position;
		color = obj.color;
	};

	~PosCol() {
		//~ qDebug() << "PosCol::~PosCol()";
	};

	friend bool operator==(const PosCol& lobj, const PosCol& robj) {
		if (lobj.position==robj.position && lobj.color==robj.color) {
			return true;
		}
		return false;
	};

	friend QDebug operator<<(QDebug d, const PosCol& obj) {
		d.nospace() << obj.position << "\t" << obj.color;
		return d;
	};
private:
	QVector3D position;
	QVector4D color;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
	Q_OBJECT

public:
	//~ GLWidget(QWidget* parent = nullptr, pov_Scene* scene);
	GLWidget(QWidget* parent, pov_Scene* scene);
	~GLWidget();

	//~ static bool isTransparent() { return m_transparent; }
	//~ static void setTransparent(bool t) { m_transparent = t; }

	QSize minimumSizeHint() const override;
	QSize sizeHint() const override;

public slots:
	//~ void setXRotation(int angle);
	//~ void setYRotation(int angle);
	//~ void setZRotation(int angle);
	void cleanup();

signals:
	//~ void xRotationChanged(int angle);
	//~ void yRotationChanged(int angle);
	//~ void zRotationChanged(int angle);

protected:
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int width, int height) override;
	void mousePressEvent(QMouseEvent *e) override;
	void mouseMoveEvent(QMouseEvent *e) override;
	void mouseReleaseEvent(QMouseEvent *e) override;
	void wheelEvent(QWheelEvent *e) override;

private:
	pov_Scene* m_scene = nullptr;
	QOpenGLFunctions_2_0* m_funcs = nullptr;
	QOpenGLShaderProgram* m_prg_a_pos_a_col = nullptr;
	QOpenGLShaderProgram* m_prg_u_col_a_pos = nullptr;

	void initializeShaders();
	void initializeTextures();
	void initializeMatrices();

	QVector<QOpenGLVertexArrayObject*> m_vaos;
	QVector<QMatrix4x4*> m_mm;		// model matrices
	QVector<QOpenGLBuffer*> m_vbos;	// VertexBuffers
	QVector<QOpenGLBuffer*> m_ibos;	// IndexBuffers
	QVector<QVector4D*> m_mc;		// models colors
	void getGeometry();		// fill m_vbos and m_ibos with data from m_scene
	void drawGeometry();

	QOpenGLBuffer m_axis_points {QOpenGLBuffer::VertexBuffer};
	QOpenGLBuffer m_axis_indices {QOpenGLBuffer::IndexBuffer};
	QOpenGLVertexArrayObject m_vao_axis;
	void getAxis();
	void drawAxis();

	void clearBuffers();

	QMatrix4x4 m_proj;
	QMatrix4x4 m_view;
	QMatrix4x4 m_mm1;
	QVector3D cam_pos;		// location
	QVector3D cam_lat;		// look_at
	QVector3D cam_up;		// up
	QVector3D cam_right;	// right
	QVector3D cam_dir;		// direction
	float cam_han;			// horizontal angle
	float cam_znear;
	float cam_zfar;
	float cam_ratio;

	void fix_right();
	void fix_up();
	void fix_dir();
	void generate_mv_mp();
	void show_cam() const;

	QPoint m_lastPos;
	bool m_lmb_pressed = false;
	bool m_mmb_pressed = false;
	bool m_rmb_pressed = false;
	float m_MouseSens = 0.1f;
};

#endif
