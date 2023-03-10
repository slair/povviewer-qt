// -*- coding: utf-8 -*-
/** @file pov_scene.h
 ** pov_Scene and related.
 **/

#ifndef POV_SCENE_H
#define POV_SCENE_H

#include <QObject>
//~ #include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>

#include "myconfig.h"

#include "pov_baseobject.h"

class pov_BaseObject;

class pov_Scene : public QObject, protected QOpenGLFunctions {
	Q_OBJECT
	Q_PROPERTY(double clock MEMBER m_clockvalue)

public:
	pov_Scene(Config* cfg, const QString& filename);
	~pov_Scene();

	pov_Scene(const pov_Scene& scene) {
		qDebug() << ">pov_Scene::pov_Scene(" << scene << ")";
		qCritical() << "Never call me";
		qDebug() << "<pov_Scene::pov_Scene(" << scene << ")";
	};

	pov_Scene& operator=(const pov_Scene& scene) {
		qDebug() << ">pov_Scene::operator=(" << scene << ")";
		qCritical() << "Never call me";
		qDebug() << "<pov_Scene::operator=(" << scene << ")";
	};

	bool parse();
	friend QDebug operator << (QDebug d, const pov_Scene& pov_Scene);
	Config* cfg() const {
		return m_cfg;
	}
	QString filename() const {
		return m_scenefilename;
	}
	//~ void setup_perspective(float cam_ratio);
	//~ void setup_ortho();
	/*void getGeometry(QVector<QMatrix4x4*>& m_mm
					 , QVector<QVector4D*>& m_mc
					 , QVector<QOpenGLVertexArrayObject*>& m_vaos
					 , QVector<QOpenGLBuffer*>& m_vbos
					 , QVector<QOpenGLBuffer*>& m_ibos);*/
	void initializeGL();
	void draw(QMatrix4x4& m_proj, QMatrix4x4& m_view) const;

private:
	Config* m_cfg;
	QString m_scenefilename;
	//~ ViewCamera* m_camera;
	QVector<pov_BaseObject*> m_objects;
	double m_clockvalue;
	double m_parse_time;
	double m_load_time;

	QVector<QVector3D> m_vertices;
	uint m_vertices_reserve_count = 1000;
	QVector<QVector3D> m_normals;
	QVector<QVector4D> m_colors;
	QVector<GLuint> m_lines;
	uint m_lines_reserve_count = 1000;
	void clear_lists();

	void add_line(const QVector3D& v1, const QVector3D& v2
				  , const QVector4D& color);
	void add_color_axis(const QVector3D& pos, const GLfloat size);
};

#endif // POV_SCENE_H
