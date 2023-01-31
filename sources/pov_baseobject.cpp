// -*- coding: utf-8 -*-
/** @file pov_baseobject.cpp
 ** pov_BaseObject and related.
 **/

#include "pov_baseobject.h"

#include "pov_transform.h"

pov_BaseObject::pov_BaseObject(pov_Scene* s)
{
	m_scene = s;
	m_tag = "BASE";
	m_transform = NULL;
	memset(m_bbox, 0, sizeof(m_bbox));
	memset(m_color, 0, sizeof(m_color));
}

pov_BaseObject::~pov_BaseObject()
{
}

QDebug operator << (QDebug d, const pov_BaseObject& obj)
{
	d << obj.m_tag;
	return d;
}

int pov_BaseObject::read(QDataStream& ds)
{
	char* tmp = new char[5];
	tmp[4] = 0;
	int freaded = 0;
	int readed = 0;

	readed = ds.readRawData(tmp, 4);
	if (readed == 4 && strncmp(tmp, "BBOX", 4) == 0) {
		freaded += ds.readRawData((char*)&m_bbox, sizeof(m_bbox)) + 4;
	} else {
		qDebug() << "BBOX absent";
	}

	readed = ds.readRawData(tmp, 4);
	if (readed == 4 && strncmp(tmp, "TRNS", 4) == 0) {
		//~ freaded += ds.readRawData((char*)&m_bbox, sizeof(m_bbox)) + 4;
	} else {
		qDebug() << "TRNS absent";
	}

	readed = ds.readRawData(tmp, 4);
	if (readed == 4 && strncmp(tmp, "COLR", 4) == 0) {
		// 5 floats rgbft
		freaded += ds.readRawData((char*)&m_color, sizeof(m_color)) + 4;
	} else {
		qDebug() << "COLR absent" << tmp;
	}

	return freaded;
}
