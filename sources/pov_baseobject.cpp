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
	//~ m_transform = nullptr;
	m_transform = new pov_Transform();
	memset(m_bbox, 0, sizeof(m_bbox));
	memset(m_color, 0, sizeof(m_color));
}

pov_BaseObject::~pov_BaseObject()
{
	qDebug() << "pov_BaseObject::~pov_BaseObject()";
	qDebug() << m_transform;
	if (m_transform != nullptr) {
		delete m_transform;
		m_transform = nullptr;
	}
	qDebug() << "pov_BaseObject deleted";
}

QString pov_BaseObject::tag() const
{
	return m_tag;
}

QDebug operator<<(QDebug d, const pov_BaseObject& obj)
{
	d << endl << obj.m_tag << endl;

	d.nospace() << "\tbbox: <" << obj.m_bbox[0][0] << ", "
				<< obj.m_bbox[0][1] << ", " << obj.m_bbox[0][2]
				<< ">, <" << obj.m_bbox[1][0] << ", " << obj.m_bbox[1][1]
				<< ", " << obj.m_bbox[1][2] << ">" << endl;

	if (obj.m_transform != nullptr) {
		d << "\ttransform: " << endl;
		d << *(obj.m_transform) << endl;
	}

	d << "\tcolor: <" << obj.m_color[0] << ", " << obj.m_color[1] << ", "
	  << obj.m_color[2] << ", " << obj.m_color[3] << ", " << obj.m_color[4]
	  << ">" << endl;

	return d;
}

void pov_BaseObject::getBBOX(QVector<QVector4D*>& m_mc
							 , QVector<QOpenGLBuffer*>& m_vbos
							 , QVector<QOpenGLBuffer*>& m_ibos)
{
	QVector4D* _color = new QVector4D(m_color[0], m_color[1], m_color[2], 1);
	m_mc << _color;
	qDebug() << "_color:" << _color;
	QOpenGLBuffer* vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	m_vbos << vbo;
	qDebug() << "vbo:" << vbo;
	QOpenGLBuffer* ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);
	m_ibos << ibo;
	qDebug() << "ibo:" << ibo;
#define A m_bbox[0]
#define B m_bbox[1]
	QVector3D _vertices[] = {
		A,
		QVector3D(A.x(), A.y(), B.z()),
		QVector3D(B.x(), A.y(), B.z()),
		QVector3D(B.x(), A.y(), A.z()),
		QVector3D(B.x(), B.y(), A.z()),
		B,
		QVector3D(A.x(), B.y(), B.z()),
		QVector3D(A.x(), B.y(), A.z())
	};
#undef B
#undef A
	GLuint _indices[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0,
		3, 4,
		4, 5,
		5, 6,
		6, 7,
		7, 4,
		0, 7,
		1, 6,
		2, 5
	};

	qDebug() << "sizeof(_vertices) =" << sizeof(_vertices);
	vbo->create();
	vbo->bind();
	vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo->allocate(_vertices, sizeof(_vertices));

	qDebug() << "sizeof(_indices) =" << sizeof(_indices);
	ibo->create();
	ibo->bind();
	ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo->allocate(_indices, sizeof(_indices));

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
		m_transform->read(ds);
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

	delete[] tmp;
	return freaded;
}
