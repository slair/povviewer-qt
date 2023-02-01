// -*- coding: utf-8 -*-
/** @file pov_scene.h
 ** pov_Scene and related.
 **/

#ifndef POV_SCENE_H
#define POV_SCENE_H

#include <QObject>
#include <qopengl.h>
#include <QVector>
#include <QVector3D>

#include "myconfig.h"

#include "pov_baseobject.h"

class pov_BaseObject;

class pov_Scene : public QObject {
	Q_OBJECT
	Q_PROPERTY(double clock MEMBER m_clockvalue)

public:
	pov_Scene(Config* cfg, const QString& filename);
	~pov_Scene();
	bool parse();
	friend QDebug operator << (QDebug d, const pov_Scene& pov_Scene);
	Config* cfg() const { return m_cfg; }

private:
	Config* m_cfg;
	QString m_scenefilename;
	//~ ViewCamera* m_camera;
	QVector<pov_BaseObject*> m_objects;
	double m_clockvalue;
	double m_parse_time;
	double m_load_time;
};

#endif // POV_SCENE_H
