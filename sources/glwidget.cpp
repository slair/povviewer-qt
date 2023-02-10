// -*- coding: utf-8 -*-
/** @file glwidget.cpp
 ** glwidget and related
 **/

#include "glwidget.h"
#include <QMouseEvent>
#include <QOpenGLShaderProgram>
#include <QCoreApplication>
#include <math.h>

const char* const VS_a_pos_a_col =
	"attribute vec3 a_pos;\n"
	"attribute vec4 a_col;\n"
	"varying vec3 v_pos;\n"
	"varying vec4 v_col;\n"
	"void main() {\n"
	"   v_pos = a_pos;\n"
	"   v_col = a_col;\n"
	"   gl_Position = vec4(a_pos, 1.0);\n"
	"}\n";

const char *const FS_a_pos_a_col =
	//~ "precision mediump float;\n"
	"varying vec4 v_col;\n"
	"varying vec3 v_pos;\n"
	"void main() {\n"
	"   gl_FragColor = v_col;\n"
	"}\n";

const char* const VS_u_col_a_pos =
	"attribute vec3 a_pos;\n"
	//~ "attribute vec4 a_col;\n"
	"varying vec3 v_pos;\n"
	//~ "varying vec4 v_col;\n"
	"void main() {\n"
	"   v_pos = a_pos;\n"
	//~ "   v_col = a_col;\n"
	"   gl_Position = vec4(a_pos, 1.0);\n"
	"}\n";

const char *const FS_u_col_a_pos =
	//~ "precision mediump float;\n"
	"uniform vec4 u_col;\n"
	"varying vec3 v_pos;\n"
	"void main() {\n"
	"   gl_FragColor = u_col;\n"
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
	if (m_prg_a_pos_a_col == nullptr)
		return;

	makeCurrent();

	m_axis_indices.destroy();
	m_axis_points.destroy();
	m_vao_axis.destroy();

	clearBuffers();

	delete m_prg_a_pos_a_col;
	m_prg_a_pos_a_col = nullptr;
	delete m_prg_u_col_a_pos;
	m_prg_u_col_a_pos = nullptr;

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
	getGeometry();

	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this
			, &GLWidget::cleanup);

	m_prg_a_pos_a_col->bind();
	m_prg_a_pos_a_col->release();
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
	if (!m_scene->cfg()->show_axis()) {
		return;
	}
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

void GLWidget::clearBuffers()
{
	qDebug() << "void GLWidget::clearBuffers()";

	qDebug() << "m_vbos.size() =" << m_vbos.size();
	for(int i = 0; i < m_vbos.size(); i++) {
		if (m_vbos[i] != nullptr) {
			//~ m_vbos[i]->release();
			m_vbos[i]->destroy();
			delete m_vbos[i];
			m_vbos[i] = nullptr;
		}
	}
	m_vbos.clear();
	qDebug() << "m_vbos.clear()";

	qDebug() << "m_ibos.size() =" << m_ibos.size();
	for(int i = 0; i < m_ibos.size(); i++) {
		if (m_ibos[i] != nullptr) {
			//~ m_ibos[i]->release();
			m_ibos[i]->destroy();
			delete m_ibos[i];
			m_ibos[i] = nullptr;
		}
	}
	m_ibos.clear();
	qDebug() << "m_ibos.clear()";

	qDebug() << "m_mv.size() =" << m_mv.size();
	for(int i = 0; i < m_mv.size(); i++) {
		if (m_mv[i] != nullptr) {
			qDebug() << m_mv[i];
			delete m_mv[i];
			m_mv[i] = nullptr;
		}
	}
	m_mv.clear();
	qDebug() << "m_mv.clear()";

	qDebug() << "m_mc.size() = " << m_mc.size();
	for(int i = 0; i < m_mc.size(); i++) {
		if (m_mc[i] != nullptr) {
			delete m_mc[i];
			m_mc[i] = nullptr;
		}
	}
	m_mc.clear();
	qDebug() << "m_mc.clear()";
}

void GLWidget::getGeometry()
{
	clearBuffers();
	// todo:  17. fill m_vbos and m_ibos with data from m_scene
	m_scene->getGeometry(m_mv, m_mc, m_vbos, m_ibos);
}

/*void GLWidget::initializeTriangle()
{
	// setup vertex data
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
}*/

void GLWidget::initializeShaders()
{
	//~ m_prg_a_pos_a_col = std::make_unique<QOpenGLShaderProgram>();
	m_prg_a_pos_a_col = new QOpenGLShaderProgram();
	m_prg_a_pos_a_col->addShaderFromSourceCode(QOpenGLShader::Vertex
			, VS_a_pos_a_col);
	m_prg_a_pos_a_col->addShaderFromSourceCode(QOpenGLShader::Fragment
			, FS_a_pos_a_col);
	m_prg_a_pos_a_col->link();

	m_prg_u_col_a_pos = new QOpenGLShaderProgram();
	m_prg_u_col_a_pos->addShaderFromSourceCode(QOpenGLShader::Vertex
			, VS_u_col_a_pos);
	m_prg_u_col_a_pos->addShaderFromSourceCode(QOpenGLShader::Fragment
			, FS_u_col_a_pos);
	m_prg_u_col_a_pos->link();
}

void GLWidget::drawAxis()
{
	m_prg_a_pos_a_col->bind();
	m_vao_axis.bind();
	m_axis_points.bind();
	m_axis_indices.bind();

	quintptr offset = 0;
	int pos = m_prg_a_pos_a_col->attributeLocation("a_pos");
	m_prg_a_pos_a_col->enableAttributeArray(pos);
	m_prg_a_pos_a_col->setAttributeBuffer(pos, GL_FLOAT, offset, 3
										  , sizeof(PosCol));
	offset += sizeof(QVector3D);
	int col = m_prg_a_pos_a_col->attributeLocation("a_col");
	m_prg_a_pos_a_col->enableAttributeArray(col);
	m_prg_a_pos_a_col->setAttributeBuffer(col, GL_FLOAT, offset, 4
										  ,sizeof(PosCol));
	m_funcs->glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, nullptr);

	m_axis_indices.release();
	m_axis_points.release();
	m_vao_axis.release();
	m_prg_a_pos_a_col->release();
}

void GLWidget::drawGeometry()
{
}

void GLWidget::paintGL()
{
	qDebug() << "GLWidget::paintGL()";

	if (!m_funcs)
		return;

	m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_funcs->glDisable(GL_LIGHTING);

	m_prg_a_pos_a_col->bind();

	// draw axis
	if (m_scene->cfg()->show_axis()) {
		drawAxis();
	}

	// draw scene
	drawGeometry();

	/*// draw triangle
	//~ qDebug() << "draw triangle";
	m_vao_triangle.bind();
	m_triangle_points.bind();
	m_triangle_indices.bind();
	offset = 0;
	pos = m_prg_a_pos_a_col->attributeLocation("a_vertex");
	m_prg_a_pos_a_col->enableAttributeArray(pos);
	m_prg_a_pos_a_col->setAttributeBuffer(pos, GL_FLOAT, offset, 3
										   , sizeof(PosCol));
	offset += sizeof(QVector3D);
	col = m_prg_a_pos_a_col->attributeLocation("a_color");
	m_prg_a_pos_a_col->enableAttributeArray(col);
	m_prg_a_pos_a_col->setAttributeBuffer(col, GL_FLOAT, offset, 4
										   ,sizeof(PosCol));
	m_funcs->glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);
	m_prg_a_pos_a_col->disableAttributeArray(col);
	m_prg_a_pos_a_col->disableAttributeArray(pos);
	m_prg_a_pos_a_col->release();*/
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
