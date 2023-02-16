// -*- coding: utf-8 -*-
/** @file pov_baseobject.cpp
 ** pov_BaseObject and related.
 **/

#include "pov_baseobject.h"
#include "pov_transform.h"
#include "el_show.h"

/*pov_BaseObject::pov_BaseObject()
{
	qDebug() << ">pov_BaseObject::pov_BaseObject()";
	m_scene = nullptr;
	m_tag = "BASE";
	//~ m_transform = nullptr;
	m_transform = new pov_Transform();
	memset(m_bbox, 0, sizeof(m_bbox));
	memset(m_color, 0, sizeof(m_color));
}*/

pov_BaseObject::pov_BaseObject(pov_Scene* s=nullptr)
{
	qDebug() << ">pov_BaseObject::pov_BaseObject(" << s << ")";
	m_scene = s;
	m_tag = "BASE";
	//~ m_transform = nullptr;
	m_transform = new pov_Transform();
	qDebug() << "m_transform =" << m_transform;
	memset(m_bbox, 0, sizeof(m_bbox));
	memset(m_color, 0, sizeof(m_color));
	qDebug() << "<pov_BaseObject::pov_BaseObject(" << s << ")";
}

pov_BaseObject::~pov_BaseObject()
{
	qDebug() << ">pov_BaseObject::~pov_BaseObject()";
	qDebug() << "m_transform =" << m_transform;
	if (m_transform != nullptr) {
		qDebug() << "m_transform =" << endl << *m_transform;
		delete m_transform;
		m_transform = nullptr;
	}
	qDebug() << "<pov_BaseObject::~pov_BaseObject()";
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
							 , QVector<QOpenGLVertexArrayObject*>& m_vaos
							 , QVector<QOpenGLBuffer*>& m_vbos
							 , QVector<QOpenGLBuffer*>& m_ibos)
{
	qDebug() << ">pov_BaseObject::getBBOX("
			 << &m_mc << "," << &m_vbos << "," << &m_ibos << ")";

#define A m_bbox[0]
#define B m_bbox[1]
	QVector3D _vertices[] = {
		A,									// 0
		QVector3D(A.x(), A.y(), B.z()),		// 1
		QVector3D(B.x(), A.y(), B.z()),		// 2
		QVector3D(B.x(), A.y(), A.z()),		// 3

		QVector3D(B.x(), B.y(), A.z()),		// 4
		B,									// 5
		QVector3D(A.x(), B.y(), B.z()),		// 6
		QVector3D(A.x(), B.y(), A.z())		// 7
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

	QVector4D* _color = new QVector4D(m_color[0], m_color[1], m_color[2], 1);
	QOpenGLVertexArrayObject* vao = new QOpenGLVertexArrayObject();
	QOpenGLBuffer* vbo = new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer);
	QOpenGLBuffer* ibo = new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer);

	vao->create();
	//~ vao->bind();
	QOpenGLVertexArrayObject::Binder vaoBinder(vao);

	qDebug() << "sizeof(_vertices) =" << sizeof(_vertices) << "bytes"
			 << sizeof(_vertices) / sizeof(QVector3D) << "vertices";
	vbo->create();
	vbo->bind();
	vbo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	vbo->allocate(_vertices, sizeof(_vertices));

	qDebug() << "sizeof(_indices) =" << sizeof(_indices) << "bytes"
			 << sizeof(_indices) / sizeof(GLuint) / 2 << "lines";
	ibo->create();
	ibo->bind();
	ibo->setUsagePattern(QOpenGLBuffer::StaticDraw);
	ibo->allocate(_indices, sizeof(_indices));

	m_mc << _color;
	qDebug() << "_color:" << *_color;

	m_vaos << vao;
	qDebug() << "vao:" << vbo << "vao->objectId() =" << vao->objectId();
	m_vbos << vbo;
	qDebug() << "vbo:" << vbo << "vbo->bufferId() =" << vbo->bufferId();
	m_ibos << ibo;
	qDebug() << "ibo:" << ibo << "ibo->bufferId() =" << ibo->bufferId();

	//~ ibo->release();
	//~ vbo->release();
	//~ vao->release();

	show_ibo(GL_LINES, ibo, vbo, vao, QVector3D());
}
int pov_BaseObject::read(QDataStream& ds)
{
	qDebug() << ">pov_BaseObject::read(" << &ds << ")";
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
