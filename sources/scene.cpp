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

bool Scene::parse()
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

	// todo:  11. read dump
	return false;
}
