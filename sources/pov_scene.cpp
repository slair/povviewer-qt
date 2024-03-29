// -*- coding: utf-8 -*-
/** @file pov_scene.cpp
 ** pov_Scene and related.
 **/

#include <QProcess>
#include <QDataStream>
#include <QMatrix4x4>
//~ #include <qmath.h>
//~ #include <qopengl.h>

#include "pov_scene.h"

#include "pov_sphere.h"

pov_Scene::pov_Scene(Config* cfg, const QString& filename)
{
	qDebug() << ">pov_Scene::pov_Scene(" << cfg << "," << filename << ")";
	m_objects.clear();
	m_cfg = cfg;
	m_scenefilename = filename;
	m_clockvalue = 123.456;
	parse();
	qDebug() << "<pov_Scene::pov_Scene(" << cfg << "," << filename << ")";
}

pov_Scene::~pov_Scene()
{
	qDebug() << ">pov_Scene::~pov_Scene()";
	qDebug() << m_objects.size() << "objects to delete";
	for(int i = 0; i < m_objects.size(); i++) {
		qDebug().nospace() << "delete m_objects[" << i << "];";
		delete m_objects[i];
	}
	m_objects.clear();
	//~ delete m_cfg;
	qDebug() << "<pov_Scene::~pov_Scene()";
}

QDebug operator << (QDebug d, const pov_Scene& scene)
{
	d << "+pov_Scene" << endl;
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
			d << *((pov_Sphere*)scene.m_objects[i]);
		} else {
			d << *scene.m_objects[i];
		}
	}
	d << "-pov_Scene";
	return d;
}

void pov_Scene::clear_lists()
{
	m_vertices.clear();
	m_vertices.reserve(m_vertices_reserve_count);
	m_normals.clear();
	m_colors.clear();
	m_lines.clear();
}

void pov_Scene::add_line(const QVector3D& v1, const QVector3D& v2
						 , const QVector4D& color)
{
	GLuint v1i, v2i, ci;
	if (!m_vertices.contains(v1)) {
		v1i = m_vertices.count();
		m_vertices << v1;
	} else {
		v1i = m_vertices.indexOf(v1);
	}
	if (!m_vertices.contains(v2)) {
		v2i = m_vertices.count();
		m_vertices << v2;
	} else {
		v2i = m_vertices.indexOf(v2);
	}
	if (!m_colors.contains(color)) {
		ci = m_colors.count();
		m_colors << color;
	} else {
		ci = m_colors.indexOf(v2);
	}
	m_lines << v1i << ci << v2i << ci;
}

#define COLOR_BRIGHT_RED QVector4D(1, 0, 0, 0)
#define COLOR_RED QVector4D(0.5, 0, 0, 0)
#define COLOR_BRIGHT_GREEN QVector4D(0, 1, 0, 0)
#define COLOR_GREEN QVector4D(0, 0.5, 0, 0)
#define COLOR_BRIGHT_BLUE QVector4D(0, 0, 1, 0)
#define COLOR_BLUE QVector4D(0, 0, 0.5, 0)

void pov_Scene::add_color_axis(const QVector3D& pos, const GLfloat size)
{
	// X axis
	add_line(pos, pos + QVector3D(size, 0, 0), COLOR_BRIGHT_RED);
	add_line(pos, pos + QVector3D(-size, 0, 0), COLOR_RED);
	// Y axis
	add_line(pos, pos + QVector3D(0, size, 0), COLOR_BRIGHT_GREEN);
	add_line(pos, pos + QVector3D(0, -size, 0), COLOR_GREEN);
	// Z axis
	add_line(pos, pos + QVector3D(0, 0, size), COLOR_BRIGHT_BLUE);
	add_line(pos, pos + QVector3D(0, 0, -size), COLOR_BLUE);
}

void pov_Scene::initializeGL()
{
	clear_lists();

	if (m_cfg->show_axis()) {
		// todo: add axis
		float _as = m_cfg->axis_size() / 2.0;
		add_color_axis(QVector3D(0, 0, 0), _as);
	} else {
		qDebug() << "m_cfg->show_axis() =" << m_cfg->show_axis();
	}

	// todo: add m_objects
}

void pov_Scene::draw(QMatrix4x4& m_proj, QMatrix4x4& m_view) const
{
	qDebug() << m_proj << m_view;
}

/*void pov_Scene::getGeometry(QVector<QMatrix4x4*>& m_mm
							, QVector<QVector4D*>& m_mc
							, QVector<QOpenGLVertexArrayObject*>& m_vaos
							, QVector<QOpenGLBuffer*>& m_vbos
							, QVector<QOpenGLBuffer*>& m_ibos)
{
	qDebug() << ">pov_Scene::getGeometry(" << &m_mm
			 << "," << &m_mc << "," << &m_vbos << "," << &m_ibos << ")";

	if (m_cfg->show_bbox()) {
		for(int i = 0; i < m_objects.size(); i++) {
			QMatrix4x4* mm = new QMatrix4x4();
			mm->setToIdentity();	// fixme: one matrix for all bboxes
			qDebug() << "mm =" << mm;
			m_mm << mm;
			m_objects[i]->getBBOX(m_mc, m_vaos, m_vbos, m_ibos);
		}
	}
	qDebug() << "<pov_Scene::getGeometry(" << &m_mm
			 << "," << &m_mc << "," << &m_vbos << "," << &m_ibos << ")";
}*/

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
	qDebug() << ">pov_Scene::parse()";
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
	// todo: check logs for errors

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
			qDebug() << "will read from" << &ds;
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
	qDebug() << "<pov_Scene::parse()";
	return false;
}
