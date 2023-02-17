// -*- coding: utf-8 -*-
/** @file el_show.h
 ** Show elements functions.
 **/
#ifndef EL_SHOW_H
#define EL_SHOW_H

#include <QOpenGLBuffer>

template <typename _T>
void show_vbo(const QOpenGLBuffer* vbo, const _T i)
{
	qDebug() << ">show_vbo(" << vbo << "," << i << ")";
	qDebug() << "vbo->size() =" << vbo->size() << "bytes";
	qDebug() << "sizeof(i) =" << sizeof(i) << "bytes";
	qDebug() << "vbo->size() / sizeof(i) =" << vbo->size() / sizeof(i)
			 << "items";
	_T* items = new _T[vbo->size() / sizeof(i)];
	if (vbo->read(0, items, vbo->size())) {
		for(int _i=0; _i < vbo->size() / sizeof(i); _i++) {
			qDebug() << _i << items[_i];
		}
	} else {
		qDebug() << "vbo->read( 0 ," << items << "," << vbo->size()
				 << ") failed";
	};
	delete[] items;
	qDebug() << "<show_vbo(" << vbo << "," << i << ")";
}

template <typename _T>
void show_ibo(const GLenum type, QOpenGLBuffer* ibo
			  , QOpenGLBuffer* vbo, QOpenGLVertexArrayObject* vao, const _T i)
{
	qDebug() << ">show_ibo(" << type << "," << ibo << "," << vbo << "," << vao
			 << "," << i << ")";
	qDebug() << "vao->objectId() =" << vao->objectId();
	qDebug() << "ibo->bufferId() =" << ibo->bufferId();
	qDebug() << "vbo->bufferId() =" << vbo->bufferId();

	qDebug() << "vbo->size() =" << vbo->size() << "bytes";
	qDebug() << "sizeof(_T) =" << sizeof(_T) << "bytes";
	qDebug() << "vbo->size() / sizeof(i) =" << vbo->size() / sizeof(i)
			 << "items";
	qDebug() << " ";

	//~ vao->bind();
	QOpenGLVertexArrayObject::Binder vaoBinder(vao);

	_T* items = new _T[vbo->size() / sizeof(i)];
	if (vbo->read(0, items, vbo->size())) {
		//~ for(int _i=0; _i < vbo->size() / sizeof(i); _i++) {
		//~ qDebug() << _i << items[_i];
		//~ }
	} else {
		qDebug() << "vbo->read( 0 ," << items << "," << vbo->size()
				 << ") failed";
	};


	int el_size = 0;
	if (type == GL_LINES) {
		el_size = 2;
	} else {
		qWarning() << "unknown type =" << type;
		el_size = 1;
	}
	qDebug() << "ibo->size() =" << ibo->size() << "bytes";
	qDebug() << "sizeof(GLuint) =" << sizeof(GLuint) << "bytes";
	qDebug() << "ibo->size() / sizeof(GLuint) ="
			 << ibo->size() / sizeof(GLuint) << "items";

	GLuint* indices = new GLuint[ibo->size() / sizeof(GLuint)];

	if (ibo->read(0, indices, ibo->size())) {
		int i_count = 0;
		for(int _i=0; _i < ibo->size() / sizeof(GLuint); _i++) {
			qDebug() << _i << indices[_i] << items[indices[_i]];
			i_count++;
			if (i_count == el_size) {
				qDebug() << " ";
				i_count = 0;
			}
		}
	} else {
		qDebug() << "ibo->read( 0 ," << indices << "," << ibo->size()
				 << ") failed";
	}
	delete[] indices;
	delete[] items;

	vao->release();
	qDebug() << "<show_ibo(" << type << "," << ibo << "," << vbo << "," << vao
			 << "," << i << ")";
}

#endif // EL_SHOW_H
