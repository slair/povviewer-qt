// -*- coding: utf-8 -*-
/** @file pov_sphere.cpp
 ** pov_Sphere and related.
 **/

#include "pov_sphere.h"

pov_Sphere::pov_Sphere(pov_Scene* s) : pov_BaseObject(s)
{
	qDebug() << ">pov_Sphere::pov_Sphere(" << s << ")";
	m_tag = "SPHR";
	m_center = vec3d(1, 2, 3);
	m_radius = 0.5;
	qDebug() << "<pov_Sphere::pov_Sphere(" << s << ")";
}

pov_Sphere::~pov_Sphere()
{
	qDebug() << ">pov_Sphere::~pov_Sphere()";
	qDebug() << "<pov_Sphere::~pov_Sphere()";
}

QDebug operator<<(QDebug d, const pov_Sphere& obj)
{
	d << (pov_BaseObject&)obj;
	d << "\tcenter: " << obj.m_center << endl;
	d << "\tradius: " << obj.m_radius << endl;
	return d;
}


int pov_Sphere::read(QDataStream& ds)
{
	qDebug() << ">pov_Sphere::read(" << &ds << ")";
	int readed = pov_BaseObject::read(ds);
	readed += ds.readRawData((char*)&m_center, sizeof(m_center));
	readed += ds.readRawData((char*)&m_radius, sizeof(m_radius));
	qDebug() << "<pov_Sphere::read(" << &ds << ")";
	return readed;
}
