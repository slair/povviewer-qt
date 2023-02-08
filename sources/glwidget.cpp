// -*- coding: utf-8 -*-
/** @file glwidget.cpp
 ** glwidget and related
 **/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

const char* const VS_color_vertex =
	"attribute vec4 a_color;\n"
	"attribute vec3 a_vertex;\n"
	"varying vec4 v_color;\n"
	"varying vec3 v_vertex;\n"
	"void main() {\n"
	"   v_color = a_color;\n"
	"   v_vertex = a_vertex;\n"
	"   gl_Position = vec4(a_vertex, 1.0);\n"
	"}\n";

const char *const FS_color_vertex =
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
	qDebug() << "fmt.alphaBufferSize() =" << fmt.alphaBufferSize();
	setFormat(fmt);
}

void GLWidget::cleanup()
{
	qDebug() << "GLWidget::cleanup()";
	if (m_prg_color_vertex == nullptr)
		return;
	makeCurrent();
	m_axis_indices.destroy();
	m_axis_points.destroy();
	m_triangle_indices.destroy();
	m_triangle_points.destroy();
	m_vao_axis.destroy();
	m_vao_triangle.destroy();
	delete m_prg_color_vertex;
	m_prg_color_vertex = nullptr;
	doneCurrent();
}

GLWidget::~GLWidget()
{
	qDebug() << "GLWidget::~GLWidget()";
	cleanup();
	qDebug() << "GLWidget deleted";
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(51, 38);
}

QSize GLWidget::sizeHint() const
{
	return QSize(512, 384);
}

void GLWidget::initializeGL()
{
	if (!context()) {
		qCritical() << "Can't get OpenGL context";
		close();
		return;
	}

	makeCurrent();

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

	initializeShaders();
	initializeAxis();
	initializeTriangle();

	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this
			, &GLWidget::cleanup);

	m_prg_color_vertex->bind();
	m_prg_color_vertex->release();
}

template <class _T>
GLuint add_item(QVector<_T>& v, const _T i)
{
	// if i in v return index or add i to v and return index
	GLuint res;
	if (!v.contains(i)) {
		res = v.count();
		v << i;
	} else {
		res = v.indexOf(i);
	}
	return res;
}

#define COLOR_BRIGHT_RED QVector4D(1, 0, 0, 0)
#define COLOR_RED QVector4D(0.5, 0, 0, 0)
#define COLOR_BRIGHT_GREEN QVector4D(0, 1, 0, 0)
#define COLOR_GREEN QVector4D(0, 0.5, 0, 0)
#define COLOR_BRIGHT_BLUE QVector4D(0, 0, 1, 0)
#define COLOR_BLUE QVector4D(0, 0, 0.5, 0)

void GLWidget::initializeAxis()
{
	QVector<PosCol> points;
	QVector<GLuint> indices;

	float _as = m_scene->cfg()->axis_size() / 2.0;

	// X axis
	indices << add_item(points, {QVector3D(0, 0, 0), COLOR_BRIGHT_RED});
	indices << add_item(points, {QVector3D(_as, 0, 0), COLOR_BRIGHT_RED});
	indices << add_item(points, {QVector3D(0, 0, 0), COLOR_RED});
	indices << add_item(points, {QVector3D(-_as, 0, 0), COLOR_RED});
	// Y axis
	indices << add_item(points, {QVector3D(0, 0, 0), COLOR_BRIGHT_GREEN});
	indices << add_item(points, {QVector3D(0, _as, 0), COLOR_BRIGHT_GREEN});
	indices << add_item(points, {QVector3D(0, 0, 0), COLOR_GREEN});
	indices << add_item(points, {QVector3D(0, -_as, 0), COLOR_GREEN});
	// Z axis
	indices << add_item(points, {QVector3D(0, 0, 0), COLOR_BRIGHT_BLUE});
	indices << add_item(points, {QVector3D(0, 0, _as), COLOR_BRIGHT_BLUE});
	indices << add_item(points, {QVector3D(0, 0, 0), COLOR_BLUE});
	indices << add_item(points, {QVector3D(0, 0, -_as), COLOR_BLUE});

	qDebug() << "points.size() =" << points.size() << "PosCol";
	qDebug() << "points.size() =" << points.size() * sizeof(PosCol)
			 << "bytes";
	qDebug() << "points:";
	for(int i = 0; i < points.size(); i++) qDebug() << i << points[i];
	qDebug() << "indices.size() =" << indices.size() << "GLuint";
	qDebug() << "iindices.size() =" << indices.size() * sizeof(GLuint)
			 << "bytes";
	qDebug() << "indices:";
	for(int i = 0; i < indices.size(); i++) qDebug() << i << indices[i];

	m_vao_axis.create();
	m_vao_axis.bind();

	m_axis_points.create();
	m_axis_points.bind();
	m_axis_points.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_axis_points.allocate(points.constData()
						   , points.size() * sizeof(PosCol));

	m_axis_indices.create();
	m_axis_indices.bind();
	m_axis_indices.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_axis_indices.allocate(indices.constData()
							, indices.size() * sizeof(GLuint));

	indices.clear();
	points.clear();
}

void GLWidget::initializeTriangle()
{
	// setup vertex data
	/*GLfloat vertices3[] = {
		// Positions
		0.0f, 1.0f, 0.0f, 0.0f,		// color Bottom right corner
		0.5f, -0.5f, 0.0f,			// Bottom right corner
		1.0f,  0.0f, 0.0f, 0.0f,	// color Top corner
		0.0f,  0.5f, 0.0f,			// Top corner
		0.0f, 0.0f, 1.0f, 0.0f,		// Bottom left corner
		-0.5f, -0.5f, 0.0f,			// color Bottom left corner
	};*/
	QVector<PosCol> points;
	QVector<GLuint> indices;

	float _ts = 0.5;

	indices << add_item(points, {QVector3D(_ts, -_ts, 0), COLOR_BRIGHT_GREEN});
	indices << add_item(points, {QVector3D(0, _ts, 0), COLOR_BRIGHT_RED});
	indices << add_item(points, {QVector3D(-_ts, -_ts, 0), COLOR_BRIGHT_BLUE});

	qDebug() << "points.size() =" << points.size() << "PosCol";
	qDebug() << "points.size() =" << points.size() * sizeof(PosCol)
			 << "bytes";
	qDebug() << "points:";
	for(int i = 0; i < points.size(); i++) qDebug() << i << points[i];
	qDebug() << "indices.size() =" << indices.size() << "GLuint";
	qDebug() << "iindices.size() =" << indices.size() * sizeof(GLuint)
			 << "bytes";
	qDebug() << "indices:";
	for(int i = 0; i < indices.size(); i++) qDebug() << i << indices[i];

	m_vao_triangle.create();
	m_vao_triangle.bind();

	m_triangle_points.create();
	m_triangle_points.bind();
	m_triangle_points.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_triangle_points.allocate(points.constData()
						   , points.size() * sizeof(PosCol));

	m_triangle_indices.create();
	m_triangle_indices.bind();
	m_triangle_indices.setUsagePattern(QOpenGLBuffer::StaticDraw);
	m_triangle_indices.allocate(indices.constData()
							, indices.size() * sizeof(GLuint));

	indices.clear();
	points.clear();
}

void GLWidget::initializeShaders()
{
	//~ m_prg_color_vertex = std::make_unique<QOpenGLShaderProgram>();
	m_prg_color_vertex = new QOpenGLShaderProgram();
	m_prg_color_vertex->addShaderFromSourceCode(QOpenGLShader::Vertex
			, VS_color_vertex);
	m_prg_color_vertex->addShaderFromSourceCode(QOpenGLShader::Fragment
			, FS_color_vertex);
	m_prg_color_vertex->link();
}

void GLWidget::paintGL()
{
	qDebug() << "GLWidget::paintGL()";

	if (!m_funcs)
		return;

	m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_funcs->glDisable(GL_LIGHTING);

	m_prg_color_vertex->bind();

	// draw axis
	m_vao_axis.bind();
	m_axis_points.bind();
	m_axis_indices.bind();
	quintptr offset = 0;
	int pos = m_prg_color_vertex->attributeLocation("a_vertex");
	m_prg_color_vertex->enableAttributeArray(pos);
	m_prg_color_vertex->setAttributeBuffer(pos, GL_FLOAT, offset, 3
										   , sizeof(PosCol));
	offset += sizeof(QVector3D);
	int col = m_prg_color_vertex->attributeLocation("a_color");
	m_prg_color_vertex->enableAttributeArray(col);
	m_prg_color_vertex->setAttributeBuffer(col, GL_FLOAT, offset, 4
										   ,sizeof(PosCol));
	m_funcs->glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, nullptr);
	//~ m_prg_color_vertex->disableAttributeArray(col);
	//~ m_prg_color_vertex->disableAttributeArray(pos);

	// draw triangle
	//~ qDebug() << "draw triangle";
	m_vao_triangle.bind();
	m_triangle_points.bind();
	m_triangle_indices.bind();
	offset = 0;
	pos = m_prg_color_vertex->attributeLocation("a_vertex");
	m_prg_color_vertex->enableAttributeArray(pos);
	m_prg_color_vertex->setAttributeBuffer(pos, GL_FLOAT, offset, 3
										   , sizeof(PosCol));
	offset += sizeof(QVector3D);
	col = m_prg_color_vertex->attributeLocation("a_color");
	m_prg_color_vertex->enableAttributeArray(col);
	m_prg_color_vertex->setAttributeBuffer(col, GL_FLOAT, offset, 4
										   ,sizeof(PosCol));
	m_funcs->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	m_prg_color_vertex->disableAttributeArray(col);
	m_prg_color_vertex->disableAttributeArray(pos);
	m_prg_color_vertex->release();
}

void GLWidget::resizeGL(int w, int h)
{
	qDebug() << "GLWidget::resizeGL(int" << w << ", int" << h << ")";
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
