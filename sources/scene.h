// -*- coding: utf-8 -*-
/** @file scene.h
 ** Scene and related.
 **/

#ifndef SCENE_H
#define SCENE_H

#include <QObject>
#include <qopengl.h>
#include <QVector>
#include <QVector3D>

#include "myconfig.h"
#include "baseobj.h"

class Scene : public QObject {
	Q_OBJECT

public:
	Scene(Config& cfg, const QString& filename);
	~Scene();
	bool parse(Config& cfg
			   , const QString& scene_file_name		// scene.pov
			   , const QString& dump_file_name		// %TEMP\\scene.dump
			   , const QString& config_file_name);	// %TEMP%\\scene.cfg

private:
	QString m_povdumpbin;	// fullpath to povdump or povdump64
	//~ ViewCamera* m_camera;
	QList<BaseObj*> m_objects;
	double m_clock;
	double m_parse_time;
	double m_load_time;
};

#endif // SCENE_H
