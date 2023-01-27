// -*- coding: utf-8 -*-
/** @file scene.cpp
 ** Scene and related.
 **/

#include <QSysInfo>
#include <QProcess>
#include <QDir>
#include <qmath.h>

#include "scene.h"

Scene::Scene(Config& cfg, const QString& filename)
{
	// done: 1. detect arch
	QString arch = QSysInfo::currentCpuArchitecture();
	qDebug() << "arch =" << arch;

	// done: 2. detect platform
	QString kernel = QSysInfo::kernelType();
	qDebug() << "kernel =" << kernel;

	QChar sep;
	if (kernel == "winnt") {
		sep = ';';
		if (arch == "x86_64") {
			m_povdumpbin = "povdump64.exe";
		} else {
			m_povdumpbin = "povdump.exe";
		}
	} else {
		sep = ':';
		if (arch == "x86_64") {
			m_povdumpbin = "povdump64";
		} else {
			m_povdumpbin = "povdump";
		}
	}
	qDebug() << "m_povdumpbin =" << m_povdumpbin;

	// done: 3. find povdump via system PATH
	QStringList env = QProcess::systemEnvironment();
	//~ qDebug() << "env =" << env;

	QStringList path;
	for(int i = 0; i < env.size(); ++i) {
		if (env.at(i).startsWith("PATH=", Qt::CaseInsensitive)) {
			path = env.at(i).right(env.at(i).size() - 5).split(sep);
			break;
		}
	}
	if (path.size() == 0) {
		qWarning() << "Empty PATH environment variable";
	}
	//~ qDebug() << "path =" << path;

	for(int i = 0; i < path.size(); ++i) {
		if (path.at(i) != "") {
			if (QFile::exists(path.at(i) + QDir::separator() + m_povdumpbin)) {
				m_fppovdumpbin = path.at(i) + QDir::separator() + m_povdumpbin;
				break;
			}
		}
	}
	qDebug() << "m_fppovdumpbin =" << m_fppovdumpbin;

	if (m_fppovdumpbin.isEmpty()) {
		qWarning() << m_povdumpbin << "not found via PATH variable";
	}

}

Scene::~Scene()
{
}

