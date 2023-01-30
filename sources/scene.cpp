// -*- coding: utf-8 -*-
/** @file scene.cpp
 ** Scene and related.
 **/

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
	// done: 3. check for local config
	m_cfg->load_from_dir(".");

	// done: 4. scan scene file and change config
	m_cfg->scan_scene_file(m_scenefilename);

	// todo: 8. run povdump to parse scene
	// povdump64.exe /EXIT +NRE +I%POVDUMP_TEST_FILE%
	QString _cmd = QString("%1 /EXIT -D +NRE +I%2 +DF%3 +TC%4").arg(m_cfg->path_to_povdump)
	qDebug() << _cmd;
	return false;
}
