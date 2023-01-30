// -*- coding: utf-8 -*-
/** @file scene.cpp
 ** Scene and related.
 **/

#include <QProcess>
#include <qmath.h>

#include "scene.h"

Scene::Scene(Config* cfg, const QString& filename)
{
	m_cfg = cfg;
	m_scenefilename = filename;
	parse();
}

QDebug operator << (QDebug d, const Scene& scene)
{
	d << "\n+Scene" << endl;
	const QMetaObject* pmo = scene.metaObject();
	for (int i=0; i < pmo->propertyCount(); ++i) {
		const QMetaProperty mp = pmo->property(i);
		d << " Property# :" << i << endl;
		d << "      Type :" << mp.typeName() << endl;
		d << "      Name :" << mp. name() << endl;
		d << "     Value :" << scene.property(mp.name()) << endl;
	}
	d << "\n-Scene";
	return d;
}

Scene::~Scene()
{
	qDebug() << "Scene::~Scene()";
}

bool Scene::parse()
{
	// done:   3. check for local config
	m_cfg->load_from_dir(".");

	// done:   4. scan scene file and change config
	m_cfg->scan_scene_file(m_scenefilename);

	if (QFile::exists(m_cfg->path_to_dump())) {
		if (!QFile::remove(m_cfg->path_to_dump())) {
			qWarning() << "Cannot delete file" << m_cfg->path_to_dump();
		}
	}

	if (QFile::exists(m_cfg->path_to_cfg())) {
		if (!QFile::remove(m_cfg->path_to_cfg())) {
			qWarning() << "Cannot delete file" << m_cfg->path_to_cfg();
		}
	}

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
	// todo:  10. add streams +GDfile +GFfile +GRfile +GSfile +GWfile
	// debug, fatal, render, statistic, and warning
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

	// todo:   9. read dump
	return false;
}
