// -*- coding: utf-8 -*-
/** @file pov_baseobject.h
 ** Base object.
 **/
#ifndef POV_BASEOBJECT_H
#define POV_BASEOBJECT_H

#include <QOpenGLVertexArrayObject>

#include "pov_scene.h"
#include "pov_transform.h"

class pov_Scene;

class pov_BaseObject {
protected:
	QString m_tag;
	pov_Scene* m_scene {nullptr};
	pov_Transform* m_transform {nullptr};
	QVector3D m_bbox[2];
	float m_color[5];

public:
	pov_BaseObject(pov_Scene* s);

	pov_BaseObject(const pov_BaseObject& pbo) {	// copy constructor
		qDebug() << ">pov_BaseObject::pov_BaseObject(" << pbo << ")";
		m_tag = pbo.m_tag;
		m_scene = pbo.m_scene;
		m_transform = new pov_Transform();
		m_transform = pbo.m_transform;
		memcpy(m_bbox, pbo.m_bbox, sizeof(m_bbox));
		memcpy(m_color, pbo.m_color, sizeof(m_color));
		qDebug() << "<pov_BaseObject::pov_BaseObject(" << pbo << ")";
	};
	pov_BaseObject& operator=(const pov_BaseObject& pbo)
	{
		qDebug() << ">pov_BaseObject& operator=(" << pbo << ")";
		if (&pbo == this)
			qWarning() << &pbo << "==" << this;
		m_tag = pbo.m_tag;
		m_scene = pbo.m_scene;
		m_transform = pbo.m_transform;
		memcpy(m_bbox, pbo.m_bbox, sizeof(m_bbox));
		memcpy(m_color, pbo.m_color, sizeof(m_color));
		qDebug() << "<pov_BaseObject& operator=(" << pbo << ")";
		return *this;
	};

	virtual ~pov_BaseObject();
	int read(QDataStream& ds);
	friend QDebug operator << (QDebug d, const pov_BaseObject& obj);
	QString tag() const;
	void getBBOX(QVector<QVector4D*>& m_mc
				 , QVector<QOpenGLVertexArrayObject*>& m_vaos
				 , QVector<QOpenGLBuffer*>& m_vbos
				 , QVector<QOpenGLBuffer*>& m_ibos);
	//~ bool is_sphere;
	//~ vec3d sphere_center;
	//~ unsigned int tag;
	//~ void set_scene(pov_scene* s);
	//~ void update() const;
	//~ void draw_bbox() const;
	//~ void begin_dots() const;
	//~ void end_dots() const;
	//~ int read(FILE *f);
	//~ vec3d get_scale_vector() const;
	//~ void generate_gllist(unsigned int mode);
	//~ GLOBJ_STATS get_stats() const;
	//~ void set_smoothed(const bool value);
	//~ pov_scene* get_scene();
	//~ vec3d get_center() const;
	//~ std::string get_type() const;
	//~ void get_lines(vector_line3d* lines) const;
	//~ void get_lines(set_line3d* lines) const;

private:
	//~ float m_bbox[2][3];
	//~ vec3d color;
	//~ vec3d inner_color;
	//~ SNGL colour[5];
	//~ DBL min_vc;
	//~ vec3d min_visual_color;
	//~ DBL point_size;
	//~ DBL point_scale;
	//~ gl_object *globj;
	//~ void fix_inner_color();
	//~ bool is_blob;
	//~ bool is_smoothed;

};
#endif	// POV_BASEOBJECT_H
