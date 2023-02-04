// -*- coding: utf-8 -*-
/** @file pov_scene.cpp
 ** pov_Scene and related.
 **/

#include <QProcess>
#include <QDataStream>
//~ #include <qmath.h>
#include <qopengl.h>

#include "pov_scene.h"

#include "pov_sphere.h"

pov_Scene::pov_Scene(Config* cfg, const QString& filename)
{
	m_objects.clear();
	m_cfg = cfg;
	m_scenefilename = filename;
	m_clockvalue = 123.456;
	parse();
}

pov_Scene::~pov_Scene()
{
	qDebug() << "pov_Scene::~pov_Scene()";
	qDebug() << m_objects.size() << "objects to delete";
	for(int i = 0; i < m_objects.size(); i++) {
		//~ qDebug().nospace() << "delete m_objects[" << i << "];";
		delete m_objects[i];
	}
	m_objects.clear();
}

static QOpenGLFunctions* pFunc = nullptr;

void pov_Scene::initGL()
{
/*	//~ initializeOpenGLFunctions();
	pFunc = QOpenGLContext::currentContext()->functions();

	// done:  13. set bg color
	pFunc->glClearColor(m_cfg->bg_color()[0]
						, m_cfg->bg_color()[1]
						, m_cfg->bg_color()[2]
						, m_cfg->bg_color()[3]);

	//~ pFunc->glShadeModel(GL_SMOOTH);
	pFunc->glCullFace(GL_BACK);
	pFunc->glFrontFace(GL_CCW);
	pFunc->glEnable(GL_CULL_FACE);
	pFunc->glEnable(GL_MAP2_VERTEX_3);
	pFunc->glEnable(GL_RESCALE_NORMAL);
	pFunc->glEnable(GL_NORMALIZE);
	pFunc->glEnable(GL_AUTO_NORMAL);
	pFunc->glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	pFunc->glEnable(GL_DEPTH_TEST);
	pFunc->glDepthFunc(GL_LEQUAL);*/
}

void pov_Scene::setup_perspective(float cam_ratio)
{
}

void pov_Scene::setup_ortho()
{
}

void pov_Scene::begin_frame()
{
/*	// todo:  18. void pov_Scene::begin_frame()
	pFunc->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glScalef(-1.0, 1.0, 1.0);

	// fixme: calc_cam_ratio
	float cam_ratio = float(512.0) / float(384.0);

	if (m_cfg->is_perspective())
		setup_perspective(cam_ratio);
	else
		setup_ortho();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glDisable(GL_LIGHTING);
	if (m_cfg->show_axis()) {
		//~ glBegin(GL_LINES);
		// X axis
		glColor3f(1, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(m_cfg->axis_size(), 0, 0);
		glColor3f(0.5, 0, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(-m_cfg->axis_size(), 0, 0);
		// Y axis
		glColor3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, m_cfg->axis_size(), 0);
		glColor3f(0, 0.5, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(0, -m_cfg->axis_size(), 0);
		// Z axis
		glColor3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, m_cfg->axis_size());
		glColor3f(0, 0, 0.5);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, -m_cfg->axis_size());
		//~ glEnd();
	}

	/*	if (show_navigator)
		{
			if (navigator_ontop)
			{
				//glDisable(GL_DEPTH_TEST);
				get_look_at().draw_navigator(navigator_size);
				//glEnable(GL_DEPTH_TEST);
			}
			else
			{
				get_look_at().draw_navigator(navigator_size);
			}
		}*/
}

void pov_Scene::end_frame()
{
	// todo:  21. void pov_Scene::end_frame()
}

void pov_Scene::setup_camera()
{
	// todo:  19. void pov_Scene::setup_camera()
}

void pov_Scene::drawGL()
{
	// todo:  20. void pov_Scene::drawGL()
}

QDebug operator << (QDebug d, const pov_Scene& scene)
{
	d << "\n+pov_Scene" << endl;
	const QMetaObject* pmo = scene.metaObject();
	for (int i=0; i < pmo->propertyCount(); ++i) {
		const QMetaProperty mp = pmo->property(i);
		d << " Property# :" << i << endl;
		d << "      Type :" << mp.typeName() << endl;
		d << "      Name :" << mp. name() << endl;
		d << "     Value :" << scene.property(mp.name()) << endl;
	}

	d << "scene.m_objects.size() =" << scene.m_objects.size() << endl;

	for(int i = 0; i < scene.m_objects.size(); i++) {
		if (scene.m_objects[i]->tag() == "SPHR") {
			d << *(pov_Sphere*)scene.m_objects[i];
		} else {
			d << *scene.m_objects[i];
		}
	}
	d << "\n-pov_Scene";
	return d;
}

bool delete_file(const QString& filepath)
{
	if (QFile::exists(filepath)) {
		if (!QFile::remove(filepath)) {
			qWarning() << "Cannot delete file" << filepath;
			return false;
		}
	}
	return true;
}

bool delete_zero_file(const QString& filepath)
{
	QFile f(filepath);
	if (f.exists()) {
		if (f.size() == 0) {
			if (!f.remove()) {
				qWarning() << "Cannot delete file" << filepath;
				return false;
			}
		}
	}
	return true;
}

bool pov_Scene::parse()
{
	// done:   3. check for local config
	m_cfg->load_from_dir(".");

	// done:   4. scan scene file and change config
	m_cfg->scan_scene_file(m_scenefilename);

	delete_file(m_cfg->path_to_dump());
	delete_file(m_cfg->path_to_cfg());

	delete_file(m_cfg->path_to_debuglog());
	delete_file(m_cfg->path_to_fatallog());
	delete_file(m_cfg->path_to_renderlog());
	delete_file(m_cfg->path_to_statisticlog());
	delete_file(m_cfg->path_to_warninglog());

	m_cfg->dump_cfg();	// create binary config

	// done:   8. run povdump to parse scene
	QProcess povdump;

	QStringList args;
	args << "/EXIT";
	args << "-D";
	args << "+NRE";
	args << QString("+I%1").arg(m_scenefilename);
	args << QString("+DF%1").arg(m_cfg->path_to_dump());
	args << QString("+TC%1").arg(m_cfg->path_to_cfg());

	// done:  10. add streams +GDfile +GFfile +GRfile +GSfile +GWfile
	// debug, fatal, render, statistic, and warning
	args << QString("+GD%1").arg(m_cfg->path_to_debuglog());
	args << QString("+GF%1").arg(m_cfg->path_to_fatallog());
	args << QString("+GR%1").arg(m_cfg->path_to_renderlog());
	args << QString("+GS%1").arg(m_cfg->path_to_statisticlog());
	args << QString("+GW%1").arg(m_cfg->path_to_warninglog());
	qDebug() << args;

	povdump.start(m_cfg->path_to_povdump(), args);
	if (!povdump.waitForStarted()) {
		qDebug() << "failed to start";
		return false;
	}

	if (!povdump.waitForFinished()) {
		qDebug() << "failed to finish";
		return false;
	}

	int exit_code = povdump.exitCode();
	QProcess::ExitStatus exit_status = povdump.exitStatus();
	qDebug() << "ExitStatus:" << exit_status;
	qDebug() << "ExitCode:" << exit_code;

	QByteArray result = povdump.readAll();
	if (!result.isEmpty()) {
		qDebug() << "result:" << result;
	}

	delete_zero_file(m_cfg->path_to_debuglog());
	delete_zero_file(m_cfg->path_to_fatallog());
	delete_zero_file(m_cfg->path_to_renderlog());
	delete_zero_file(m_cfg->path_to_statisticlog());
	delete_zero_file(m_cfg->path_to_warninglog());
	// todo:   9. check logs for errors

	// done:  11. read dump
	QFile dumpfile(m_cfg->path_to_dump());
	dumpfile.open(QFile::ReadOnly);
	QDataStream ds(&dumpfile);
	ds.setVersion(QDataStream::Qt_4_8);
	char* tmp = new char[5];
	tmp[4] = 0;
	uint readed;
	readed = ds.readRawData(tmp, 4);

	readed = ds.readRawData(tmp, 4);
	if (readed != 4 || strncmp(tmp, "SCNE", 4) != 0) {
		qDebug() << "SCNE is missing";
		delete [] tmp;
		dumpfile.close();
		return false;
	}
	readed = ds.readRawData(tmp, 4);
	if (readed != 4 || strncmp(tmp, "FRME", 4) != 0) {
		qDebug() << "FRME is missing";
		delete [] tmp;
		dumpfile.close();
		return false;
	}
	readed = ds.readRawData((char*)&m_clockvalue, sizeof(m_clockvalue));

	for(;;) {
		readed = ds.readRawData(tmp, 4);
		if (readed == 0) {
			break;	// eof
		}
		qDebug() << "readed:" << readed;
		qDebug() << "tmp:" << tmp;
		if (!strncmp(tmp, "SPHR", 4)) {
			qDebug() << "Sphere found";
			pov_Sphere* obj = new pov_Sphere(this);
			obj->read(ds);
			//~ qDebug() << *obj;
			m_objects.append(obj);
			qDebug() << m_objects.size();

		} else if (!strncmp(tmp, "BASE", 4)) {
			qDebug() << "BaseObject found";
			pov_BaseObject* obj = new pov_BaseObject(this);
			obj->read(ds);
			//~ qDebug() << *obj;
			m_objects.append(obj);
			qDebug() << m_objects.size();

		} else {
			qDebug() << "Unknown tag:" << tmp;
		}
	}
	delete [] tmp;
	dumpfile.close();
	return false;
}
