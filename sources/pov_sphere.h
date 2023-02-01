// -*- coding: utf-8 -*-
/** @file pov_sphere.h
 ** Base object.
 **/
#ifndef POV_SPHERE_H
#define POV_SPHERE_H

#include <QDataStream>

#include "pov_baseobject.h"
#include "vec3d.h"

//~ class pov_baseobject;

class pov_Sphere : public pov_BaseObject {
private:
	vec3d m_center;
	double m_radius;
	//~ void fix_sphere();
	//~ void tesselate() const;
public:
	pov_Sphere(pov_Scene* s);
	~pov_Sphere();
	//~ int read(FILE *f);
	int read(QDataStream& ds);
	friend QDebug operator << (QDebug d, const pov_Sphere& obj);
};

#endif	// POV_SPHERE_H
