// -*- coding: utf-8 -*-
/** @file glwidget.h
 ** glwidget and related
 **/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLFunctions_2_0>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>

#include "pov_scene.h"

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
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
    //~ void mousePressEvent(QMouseEvent *event) override;
    //~ void mouseMoveEvent(QMouseEvent *event) override;

private:
	pov_Scene* m_scene;
	QOpenGLFunctions_2_0 *m_funcs {nullptr};
    QOpenGLShaderProgram *m_program = nullptr;
    void initializeGeometry();
    void initializeShaders();

    QOpenGLBuffer m_vbo {QOpenGLBuffer::VertexBuffer};
    QOpenGLVertexArrayObject m_vao;

    //~ void setupVertexAttribs();
    //~ bool m_core;
    //~ int m_xRot = 0;
    //~ int m_yRot = 0;
    //~ int m_zRot = 0;
    //~ QPoint m_lastPos;
    //~ BaseObj m_logo;
    //~ QOpenGLVertexArrayObject m_vao;
    //~ QOpenGLBuffer m_logoVbo;
	//~ std::unique_ptr<QOpenGLShaderProgram> m_program;
    //~ int m_projMatrixLoc = 0;
    //~ int m_mvMatrixLoc = 0;
    //~ int m_normalMatrixLoc = 0;
    //~ int m_lightPosLoc = 0;
    //~ QMatrix4x4 m_proj;
    //~ QMatrix4x4 m_camera;
    //~ QMatrix4x4 m_world;
    //~ static bool m_transparent;
};

#endif
