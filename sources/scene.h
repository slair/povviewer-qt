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
	Scene(Config* cfg, const QString& filename);
	~Scene();
	bool parse();
	friend QDebug operator << (QDebug d, const Scene& scene);

private:
	Config* m_cfg;
	QString m_scenefilename;
	//~ ViewCamera* m_camera;
	QList<BaseObj*> m_objects;
	double m_clock;
	double m_parse_time;
	double m_load_time;
};

#endif // SCENE_H
