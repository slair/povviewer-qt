// -*- coding: utf-8 -*-
/** @file glwidget.h
 ** glwidget and related
 **/

#ifndef POSCOL_H
#define POSCOL_H

#include <QDebug>
#include <QVector3D>
#include <QVector4D>

class PosCol {
public:
	PosCol();

	PosCol(const QVector3D& p, const QVector4D& c) {
		position = p;
		color = c;
	};

	PosCol(const PosCol& obj) {
		position = obj.position;
		color = obj.color;
	};

	~PosCol() {
		qDebug() << "PosCol::~PosCol()";
	};

	friend bool operator==(const PosCol& lobj, const PosCol& robj) {
		if (lobj.position==robj.position && lobj.color==robj.color) {
			return true;
		}
		return false;
	};

	friend QDebug operator<<(QDebug d, const PosCol& obj) {
		d << obj.position << "," << obj.color;
		return d;
	};
private:
	QVector3D position;
	QVector4D color;
};

template <class _T>
GLuint add_item(QVector<_T>& v, const _T i)
{
	// if i in v return index or add i to v and return index
	GLuint res;
	if (!v.contains(i)) {
		res = v.count();
		v << i;
	} else {
		res = v.indexOf(i);
	}
	return res;
}
;

#endif	// POSCOL_H
