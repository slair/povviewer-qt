// -*- coding: utf-8 -*-
/** @file glwidget.cpp
 ** glwidget and related
 **/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

const char* const vertexShaderSource =
	"attribute vec4 a_color;\n"
	"attribute vec3 a_vertex;\n"
	"varying vec4 v_color;\n"
	"varying vec3 v_vertex;\n"
	"void main() {\n"
	"   v_color = a_color;\n"
	"   v_vertex = a_vertex;\n"
	"   gl_Position = vec4(a_vertex, 1.0);\n"
	"}\n";

const char *const fragmentShaderSource =
	//~ "precision mediump float;\n"
	"varying vec4 v_color;\n"
	"varying vec3 v_vertex;\n"
	"void main() {\n"
	"   gl_FragColor = v_color;\n"
	"}\n";

GLWidget::GLWidget(QWidget *parent, pov_Scene* scene)
	: QOpenGLWidget(parent)
{
	qDebug() << "GLWidget::GLWidget(" << parent << "," << scene << ")";
	m_scene = scene;
	QSurfaceFormat fmt = format();
	fmt.setAlphaBufferSize(8);
	setFormat(fmt);
}

GLWidget::~GLWidget()
{
	qDebug() << "GLWidget::~GLWidget()";
	cleanup();
	delete m_scene;
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(51, 38);
}

QSize GLWidget::sizeHint() const
{
	return QSize(512, 384);
}

void GLWidget::cleanup()
{
	if (m_program == nullptr)
		return;
	makeCurrent();
	//~ m_logoVbo.destroy();
	delete m_program;
	m_program = nullptr;
	doneCurrent();
}

void GLWidget::initializeGL()
{
	if (!context()) {
		qCritical() << "Can't get OpenGL context";
		close();
		return;
	}

	qDebug() << context();
	if (context()->isValid()) {
		qDebug() << "Valid";
	} else {
		qDebug() << "Invalid";
	}
	if (context()->isOpenGLES()) {
		qDebug() << "OpenGLES";
	} else {
		qDebug() << "OpenGL";
	}

	m_funcs = context()->versionFunctions<QOpenGLFunctions_2_0>();
	if (!m_funcs) {
		qCritical() << "Can't get OpenGL 2.0";
		close();
		return;
	}
	m_funcs->initializeOpenGLFunctions();
	qDebug() << "GL_VERSION:" << *m_funcs->glGetString(GL_VERSION);

	m_funcs->glClearColor(m_scene->cfg()->bg_color()[0]
						  , m_scene->cfg()->bg_color()[1]
						  , m_scene->cfg()->bg_color()[2]
						  , m_scene->cfg()->bg_color()[3]);
	m_funcs->glShadeModel(GL_SMOOTH);
	m_funcs->glCullFace(GL_BACK);
	m_funcs->glFrontFace(GL_CCW);
	m_funcs->glEnable(GL_CULL_FACE);
	m_funcs->glEnable(GL_MAP2_VERTEX_3);
	m_funcs->glEnable(GL_RESCALE_NORMAL);
	m_funcs->glEnable(GL_NORMALIZE);
	m_funcs->glEnable(GL_AUTO_NORMAL);
	m_funcs->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	m_funcs->glEnable(GL_DEPTH_TEST);
	m_funcs->glDepthFunc(GL_LEQUAL);

	initializeGeometry();
	initializeShaders();

	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this
			, &GLWidget::cleanup);

	m_program->bind();
	m_program->release();
}

void GLWidget::initializeGeometry()
{
	// setup vertex data
	GLfloat vertices[] = {
		// Positions
		0.0f, 1.0f, 0.0f, 0.0f,  // Bottom right corner
		0.5f, -0.5f, 0.0f,  // Bottom right corner
		1.0f,  0.0f, 0.0f, 0.0f,  // Top corner
		0.0f,  0.5f, 0.0f,  // Top corner
		0.0f, 0.0f, 1.0f, 0.0f, // Bottom left corner
		-0.5f, -0.5f, 0.0f, // Bottom left corner
	};

	m_vao.create();
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);

	m_vbo.create();
	m_vbo.bind();
	m_vbo.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_vbo.allocate(vertices, sizeof(vertices));

	m_funcs->glEnableVertexAttribArray(0);
	m_funcs->glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE
								   , 7 * sizeof(GLfloat), nullptr);

	m_funcs->glEnableVertexAttribArray(1);
	m_funcs->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE
								   , 7 * sizeof(GLfloat)
								   , reinterpret_cast<void *>
								   (4 * sizeof(GLfloat)));
	m_vbo.release();
}

void GLWidget::initializeShaders()
{
	//~ m_program = std::make_unique<QOpenGLShaderProgram>();
	m_program = new QOpenGLShaderProgram();
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex
									   , vertexShaderSource);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment
									   , fragmentShaderSource);
	m_program->bindAttributeLocation("a_color", 0);
	m_program->bindAttributeLocation("a_vertex", 1);
	m_program->link();
}

void GLWidget::paintGL()
{
	// todo:  17. draw scene
	//~ m_scene->begin_frame();
	//~ m_scene->setup_camera();
	//~ m_scene->drawGL();
	//~ m_scene->end_frame();
	if (!m_funcs)
		return;

	m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_program->bind();
	m_funcs->glDisable(GL_LIGHTING);
	QOpenGLVertexArrayObject::Binder vaoBinder(&m_vao);
	m_funcs->glDrawArrays(GL_TRIANGLES, 0, 3);
	m_program->release();
}

void GLWidget::resizeGL(int w, int h)
{
	if (!m_funcs)
		return;

	m_funcs->glViewport(0, 0, w, h);
}

/*static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360 * 16)
		angle -= 360 * 16;
}

void GLWidget::setXRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_xRot) {
		m_xRot = angle;
		emit xRotationChanged(angle);
		update();
	}
}

void GLWidget::setYRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_yRot) {
		m_yRot = angle;
		emit yRotationChanged(angle);
		update();
	}
}

void GLWidget::setZRotation(int angle)
{
	qNormalizeAngle(angle);
	if (angle != m_zRot) {
		m_zRot = angle;
		emit zRotationChanged(angle);
		update();
	}
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	m_lastPos = event->pos();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - m_lastPos.x();
	int dy = event->y() - m_lastPos.y();

	if (event->buttons() & Qt::LeftButton) {
		setXRotation(m_xRot + 8 * dy);
		setYRotation(m_yRot + 8 * dx);
	} else if (event->buttons() & Qt::RightButton) {
		setXRotation(m_xRot + 8 * dy);
		setZRotation(m_zRot + 8 * dx);
	}
	m_lastPos = event->pos();
}*/
