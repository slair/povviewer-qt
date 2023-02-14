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
	"uniform mat4 u_mp;\n"
	"uniform mat4 u_mv;\n"
	"uniform mat4 u_mm;\n"
	"void main() {\n"
	"   v_pos = a_pos;\n"
	"   v_col = a_col;\n"
	"	vec4 vertex4 = u_mp * u_mv * u_mm * vec4(a_pos, 1.0);\n"
	"   gl_Position = vertex4;\n"
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
	"varying vec3 v_pos;\n"
	"uniform mat4 u_mp;\n"
	"uniform mat4 u_mv;\n"
	"uniform mat4 u_mm;\n"
	"void main() {\n"
	"   v_pos = a_pos;\n"
	"   gl_Position = u_mp * u_mv * u_mm * vec4(a_pos, 1.0);\n"
	"}\n";

const char *const FS_u_col_a_pos =
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
	qInfo() << "OpenGL version:"
			<< reinterpret_cast<const char *>(
				m_funcs->glGetString(GL_VERSION));
	//~ qDebug() << "GL_VERSION:" << *m_funcs->glGetString(GL_VERSION);

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

	initializeTextures();
	initializeMatrices();

	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this
			, &GLWidget::cleanup);
}

void GLWidget::initializeMatrices()
{
	m_mm1.setToIdentity();

	cam_han = m_scene->cfg()->cam_angle();	// horizontal angle
	cam_pos = m_scene->cfg()->cam_pos();	// location
	cam_lat = m_scene->cfg()->cam_tgt();	// look_at
	cam_up = QVector3D(0, 1, 0);			// up
	cam_dir = cam_lat - cam_pos;			// direction
	cam_dir.normalize();
	fix_right();
	fix_up();
	cam_han = 63.5f;
	cam_znear = 0.01f;
	cam_zfar = 2048.0f;
	generate_m_view();

	m_proj.setToIdentity();
	m_proj.perspective(cam_han / cam_ratio, cam_ratio, cam_znear, cam_zfar);
}

void GLWidget::initializeTextures() {}

void GLWidget::generate_m_view()
{
	m_view.setToIdentity();
	//~ m_view.scale(-1, 1, 1);
	//~ m_view.translate(cam_pos);
	//~ m_view.rotate(180, cam_up);
	m_view.lookAt(cam_pos, cam_lat, cam_up);
	show_cam();
}

void GLWidget::fix_right()
{
	cam_right = QVector3D::crossProduct(cam_dir, cam_up);
	cam_right.normalize();
}

void GLWidget::fix_dir()
{
	cam_dir = QVector3D::crossProduct(cam_up, cam_right);
	cam_dir.normalize();
}

void GLWidget::fix_up()
{
	cam_up = QVector3D::crossProduct(cam_right, cam_dir);
	cam_up.normalize();
}

void GLWidget::show_cam() const
{
	qDebug() << " cam position:" << cam_pos;
	qDebug() << "  cam look_at:" << cam_lat;
	qDebug() << "cam direction:" << cam_dir;
	qDebug() << "       cam up:" << cam_up;
	qDebug() << "    cam right:" << cam_right;
	qDebug() << "    cam angle:" << cam_han;
	qDebug() << "    m_view:" << m_view;
	qDebug() << "    m_proj:" << m_proj;
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

	qDebug() << "m_mm.size() =" << m_mm.size();
	for(int i = 0; i < m_mm.size(); i++) {
		if (m_mm[i] != nullptr) {
			qDebug() << "m_mm[" << i << "] = " << m_mm[i];
			delete m_mm[i];
			m_mm[i] = nullptr;
		}
	}
	m_mm.clear();
	qDebug() << "m_mm.clear()";

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
	// done:  17. fill m_vbos and m_ibos with data from m_scene
	m_vao_scene.create();
	m_vao_scene.bind();
	m_scene->getGeometry(m_mm, m_mc, m_vbos, m_ibos);
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
	m_prg_a_pos_a_col = new QOpenGLShaderProgram();
	bool ok;
	ok = m_prg_a_pos_a_col->addShaderFromSourceCode(QOpenGLShader::Vertex
			, VS_a_pos_a_col);
	if (!ok) qWarning() << "m_prg_a_pos_a_col";

	ok = m_prg_a_pos_a_col->addShaderFromSourceCode(QOpenGLShader::Fragment
			, FS_a_pos_a_col);
	if (!ok) qWarning() << "m_prg_a_pos_a_col";

	ok = m_prg_a_pos_a_col->link();
	if (!ok) qWarning() << "m_prg_a_pos_a_col";

	ok = m_prg_u_col_a_pos = new QOpenGLShaderProgram();
	if (!ok) qWarning() << "m_prg_u_col_a_pos";

	ok = m_prg_u_col_a_pos->addShaderFromSourceCode(QOpenGLShader::Vertex
			, VS_u_col_a_pos);
	if (!ok) qWarning() << "m_prg_u_col_a_pos";

	ok = m_prg_u_col_a_pos->addShaderFromSourceCode(QOpenGLShader::Fragment
			, FS_u_col_a_pos);
	if (!ok) qWarning() << "m_prg_u_col_a_pos";

	ok = m_prg_u_col_a_pos->link();
	if (!ok) qWarning() << "m_prg_u_col_a_pos";
}

void GLWidget::drawAxis()
{
	//~ m_prg_a_pos_a_col->bind();
	m_vao_axis.bind();
	m_axis_points.bind();
	m_axis_indices.bind();

	// todo: setup attributes
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

	// done: setup uniforms
	m_prg_a_pos_a_col->setUniformValue("u_mp", m_proj);
	m_prg_a_pos_a_col->setUniformValue("u_mv", m_view);
	m_prg_a_pos_a_col->setUniformValue("u_mm", m_mm1);
	m_funcs->glDrawElements(GL_LINES, 12, GL_UNSIGNED_INT, nullptr);

	m_axis_indices.release();
	m_axis_points.release();
	m_vao_axis.release();
	//~ m_prg_a_pos_a_col->release();
}

void GLWidget::drawGeometry()
{
	//~ makeCurrent();
	qDebug() << "GLWidget::drawGeometry()";
	if (!m_funcs) {
		qDebug() << "m_funcs =" << m_funcs;
		return;
	}
	quintptr offset = 0;
	//~ m_prg_u_col_a_pos->bind();
	m_vao_scene.bind();
	//~ qDebug() << "m_vao_scene.objectId() =" << m_vao_scene.objectId();

	for(int i=0; i < m_vbos.size(); i++) {
		m_vbos[i]->bind();
		m_ibos[i]->bind();

		// todo: setup attributes
		//~ int pos = m_prg_u_col_a_pos->attributeLocation("a_pos");
		//~ m_prg_u_col_a_pos->enableAttributeArray(pos);
		m_prg_u_col_a_pos->enableAttributeArray("a_pos");
		m_prg_u_col_a_pos->setAttributeBuffer("a_pos", GL_FLOAT, offset, 3
											  , 3 * sizeof(GLfloat));
		//~ qDebug() << "m_ibos[i]->size() = " << m_ibos[i]->size();
		//~ qDebug() << "m_ibos has"
		//~ << (m_ibos[i]->size() / sizeof(GLuint)) / 2 << "lines";

		// todo: setup uniforms
		m_prg_u_col_a_pos->setUniformValue("u_mm", *m_mm[i]);
		m_prg_u_col_a_pos->setUniformValue("u_mp", m_proj);
		generate_m_view();
		m_prg_u_col_a_pos->setUniformValue("u_mv", m_view);
		//~ int col = m_prg_u_col_a_pos->uniformLocation("u_col");
		// fixme: one color
		m_prg_u_col_a_pos->setUniformValue("u_col", *m_mc[i]);

		m_funcs->glDrawElements(GL_LINES
								, (m_ibos[i]->size() / sizeof(GLuint)) / 2
								, GL_UNSIGNED_INT, nullptr);
		qDebug() << "m_funcs->glGetError() =" << m_funcs->glGetError();

		m_ibos[i]->release();
		m_vbos[i]->release();
	}
	m_vao_scene.release();
	qDebug() << "*m_mm[" << 0 << "] =" << *m_mm[0];
	//~ m_prg_u_col_a_pos->release();
}

void GLWidget::paintGL()
{
	qDebug() << "GLWidget::paintGL()";

	if (!m_funcs) {
		qDebug() << "m_funcs =" << m_funcs;
		return;
	}

	m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_funcs->glDisable(GL_LIGHTING);

	m_prg_a_pos_a_col->bind();
	// draw axis
	if (m_scene->cfg()->show_axis()) {
		drawAxis();
	}
	m_prg_a_pos_a_col->release();

	// draw scene
	m_prg_u_col_a_pos->bind();
	drawGeometry();
	m_prg_u_col_a_pos->release();
}

void GLWidget::resizeGL(int w, int h)
{
	qDebug() << "GLWidget::resizeGL(int" << w << ", int" << h << ")";
	if (!m_funcs) {
		qDebug() << "m_funcs =" << m_funcs;
		return;
	}

	m_funcs->glViewport(0, 0, w, h);

	cam_ratio = GLfloat(w) / h;
	qDebug() << "cam_ratio =" << cam_ratio;
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
}*/

void GLWidget::mousePressEvent(QMouseEvent *e)
{
	m_lastPos = e->pos();
	//~ qDebug() << "e->button() =" << e->button();
	//~ qDebug() << "e->buttons() =" << e->buttons();
	//~ update();
	if (e->button() == Qt::LeftButton) {
		m_lmb_pressed = true;
	} else if (e->button() == Qt::MiddleButton) {
		m_mmb_pressed = true;
	} else if (e->button() == Qt::RightButton) {
		m_rmb_pressed = true;
	} else {
		qDebug() << "Strange mouse button pressed:" << e->button();
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *e)
{
	if (e->button() == Qt::LeftButton) {
		m_lmb_pressed = false;
	} else if (e->button() == Qt::MiddleButton) {
		m_mmb_pressed = false;
	} else if (e->button() == Qt::RightButton) {
		m_rmb_pressed = false;
	} else {
		qDebug() << "Strange mouse button released:" << e->button();
	}
	//~ // Mouse release position - mouse press position
	//~ QVector2D diff = QVector2D(e->localPos()) - mousePressPosition;

	//~ // Rotation axis is perpendicular to the mouse position difference
	//~ // vector
	//~ QVector3D n = QVector3D(diff.y(), diff.x(), 0.0).normalized();

	//~ // Accelerate angular speed relative to the length of the mouse sweep
	//~ qreal acc = diff.length() / 100.0;

	//~ // Calculate new rotation axis as weighted sum
	//~ rotationAxis = (rotationAxis * angularSpeed + n * acc).normalized();

	//~ // Increase angular speed
	//~ angularSpeed += acc;
}

void GLWidget::mouseMoveEvent(QMouseEvent *e)
{
	if (m_lmb_pressed) {
		int dx = e->x() - m_lastPos.x();
		int dy = e->y() - m_lastPos.y();
		//~ qDebug() << "m_lastPos =" << m_lastPos;
		qDebug() << "dx, dy =" << dx << "," << dy;

		// rotate around right
		QMatrix4x4 _tmp = QMatrix4x4();
		_tmp.setToIdentity();
		_tmp.rotate(m_MouseSens * dy, cam_right);
		QVector4D _dir = QVector4D(cam_dir, 1);
		_dir = _dir * _tmp;

		cam_dir = QVector3D(_dir);
		cam_dir.normalize();
		fix_up();

		cam_up = QVector3D(0, 1, 0);
		fix_right();
		fix_up();

		cam_lat = cam_pos + cam_dir;
		//~ cam_lat += QVector3D(m_MouseSens * dx, 0, 0);
		//~ cam_lat += QVector3D(0, m_MouseSens * dy, 0);

		m_lastPos = e->pos();
		update();
	} else {
	}
}

void GLWidget::wheelEvent(QWheelEvent *e)
{
	//~ e->delta() > 0 ? scale += scale*0.1f : scale -= scale*0.1f;
	//~ draw();
	//~ qDebug() << "e->delta() =" << e->delta();
	//~ qDebug() << "e->angleDelta() =" << e->angleDelta();
	if (e->delta() > 0) {
		cam_pos = cam_pos + cam_dir;
		cam_lat = cam_pos + cam_dir;
	} else {
		cam_pos = cam_pos - cam_dir;
		cam_lat = cam_pos + cam_dir;
	}
	update();
}
