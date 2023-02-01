// -*- coding: utf-8 -*-
/** @file pov_sphere.cpp
 ** pov_Sphere and related.
 **/

#include "pov_sphere.h"

pov_Sphere::pov_Sphere(pov_Scene* s) : pov_BaseObject(s)
{
	m_tag = "SPHR";
	m_center = vec3d(1, 2, 3);
	m_radius = 0.5;
}

pov_Sphere::~pov_Sphere()
{
}

QDebug operator << (QDebug d, const pov_Sphere& obj)
{
	d << (pov_BaseObject)obj;
	d << "\tcenter: " << obj.m_center << endl;
	d << "\tradius: " << obj.m_radius << endl;
	return d;
}


int pov_Sphere::read(QDataStream& ds)
{
	int readed = pov_BaseObject::read(ds);
	readed += ds.readRawData((char*)&m_center, sizeof(m_center));
	readed += ds.readRawData((char*)&m_radius, sizeof(m_radius));
	return readed;
}
