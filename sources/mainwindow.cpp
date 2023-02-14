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
#include "poscol.h"

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

void MainWindow::show_cam() const
{
	qDebug() << "cam_pos:" << m_camera->position();
	qDebug() << "cam_dir:" << m_camera->front();
}

MainWindow::MainWindow(pov_Scene* scene)// : m_camera(this)
{
	qDebug() << "MainWindow::MainWindow(" << scene << ")";
	m_scene = scene;
	m_camera = new Camera(this);
	//~ qDebug() << "m_camera->setWindow(" << this << ")";
	m_camera->setWindow(this);
	//~ qDebug() << "setTitle(" << QCoreApplication::applicationName()
	//~ << " - " << m_scene->filename() << ")";
	setTitle(QCoreApplication::applicationName()
			 + " - " + m_scene->filename());
	// done:  12. set geometry
	//~ qDebug() << "setGeometry(" << m_scene->cfg()->window_pos_x()
	//~ << "," << m_scene->cfg()->window_pos_y()
	//~ << "," << m_scene->cfg()->window_width()
	//~ << "," << m_scene->cfg()->window_height() << ")";
	setGeometry(m_scene->cfg()->window_pos_x()
				, m_scene->cfg()->window_pos_y()
				, m_scene->cfg()->window_width()
				, m_scene->cfg()->window_height());
}

void MainWindow::initializeGL()
{
	qDebug() << "MainWindow::initializeGL()";
	if (!context()) {
		qCritical() << "Can't get OGL context";
		close();
		return;
	}

	m_funcs = context()->versionFunctions<QOpenGLFunctions_2_0>();
	if (!m_funcs) {
		qCritical() << "Can't get OGL 2.0";
		close();
		return;
	}

	m_funcs->initializeOpenGLFunctions();
	qInfo() << "OpenGL version:"
			<< reinterpret_cast<const char *>(
				m_funcs->glGetString(GL_VERSION));

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
	initializeTextures();
	initializeGeometry();
}

void MainWindow::initializeShaders()
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

void MainWindow::initializeTextures()
{
}

void MainWindow::initializeGeometry()
{
	m_mm1.setToIdentity();

	getAxis();
	getGeometry();
}

#define COLOR_BRIGHT_RED QVector4D(1, 0, 0, 0)
#define COLOR_RED QVector4D(0.5, 0, 0, 0)
#define COLOR_BRIGHT_GREEN QVector4D(0, 1, 0, 0)
#define COLOR_GREEN QVector4D(0, 0.5, 0, 0)
#define COLOR_BRIGHT_BLUE QVector4D(0, 0, 1, 0)
#define COLOR_BLUE QVector4D(0, 0, 0.5, 0)

void MainWindow::getAxis()
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

void MainWindow::getGeometry()
{
}

void MainWindow::resizeGL(int w, int h)
{
	if (!m_funcs) {
		return;
	}

	m_funcs->glViewport(0, 0, w, h);
}

void MainWindow::paintGL()
{
	qDebug() << "MainWindow::paintGL()";
	if (!m_funcs) {
		qDebug() << "m_funcs =" << m_funcs;
		return;
	}

	m_funcs->glClearColor(m_scene->cfg()->bg_color()[0]
						  , m_scene->cfg()->bg_color()[1]
						  , m_scene->cfg()->bg_color()[2]
						  , m_scene->cfg()->bg_color()[3]);
	m_funcs->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_camera->updateMatrixes();

	// draw axis
	m_prg_a_pos_a_col->bind();
	if (m_scene->cfg()->show_axis()) {
		drawAxis();
	}
	m_prg_a_pos_a_col->release();

	/*// draw scene
	m_prg_u_col_a_pos->bind();
	//~ generate_m_view();
	m_prg_u_col_a_pos->setUniformValue("u_mp", m_camera->projection());
	m_prg_u_col_a_pos->setUniformValue("u_mv", m_camera->view());
	drawGeometry();

	m_prg_u_col_a_pos->release();*/
	show_cam();
}

void MainWindow::drawAxis()
{
	qDebug() << "MainWindow::drawAxis()";
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

	m_prg_a_pos_a_col->setUniformValue("u_mp", m_camera->projection());
	m_prg_a_pos_a_col->setUniformValue("u_mv", m_camera->view());
	m_prg_a_pos_a_col->setUniformValue("u_mm", m_mm1);

	qDebug() << "m_axis_indices.size() =" << m_axis_indices.size() << "GLuint";
	qDebug() << "m_axis_indices.size() ="
			 << m_axis_indices.size() / sizeof(GLuint) / 2 << "lines";
	m_funcs->glDrawElements(GL_LINES
							, m_axis_indices.size() / sizeof(GLuint) / 2
							, GL_UNSIGNED_INT, nullptr);

	m_axis_indices.release();
	m_axis_points.release();
	m_vao_axis.release();
}

void MainWindow::drawGeometry()
{
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
	delete m_camera;
	qDebug() << "MainWindow deleted";
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
		QWindow::keyPressEvent(e);
	update();
}
