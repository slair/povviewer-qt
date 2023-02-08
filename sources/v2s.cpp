// -*- coding: utf-8 -*-
/** @file v2s.cpp
 ** Vector String conversions.
 **/

#include <QDebug>
#include <QStringList>
#include "v2s.h"

QString QVector3D_to_QString(QVector3D& v)
{
	return QString("%1, %2, %3").arg(v[0]).arg(v[1]).arg(v[2]);
};

QString QVector4D_to_QString(QVector4D& v)
{
	return QString("%1, %2, %3, %4").arg(v[0]).arg(v[1]).arg(v[2]).arg(v[3]);
};

QString vec3d_to_QString(vec3d& v)
{
	return QString("%1, %2, %3").arg(v[0]).arg(v[1]).arg(v[2]);
};

QVector4D QString_to_QVector4D(QString& s)
{
	QVector4D res(0, 0, 0, 0);
	QStringList tmp = s.split(",");
	if (tmp.size() != 4) {
		qWarning() << "Wrong value in" << s;
		res = QVector4D(0, 0, 0, 0);
	} else {
		bool ok;
		float a, b, c, d;
		a = tmp[0].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value R in" << s;
			a = 0;
		}
		b = tmp[1].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value G in" << s;
			b = 0;
		}
		c = tmp[2].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value B in" << s;
			c = 0;
		}
		d = tmp[3].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value A in" << s;
			d = 0;
		}
		res = QVector4D(a, b, c, d);
	}
	return res;
};

QVector3D QString_to_QVector3D(QString& s)
{
	QVector3D res(0, 0, 0);
	QStringList tmp = s.split(",");
	if (tmp.size() != 3) {
		qWarning() << "Wrong value in" << s;
		res = QVector3D(0, 0, 0);
	} else {
		bool ok;
		float a, b, c;
		a = tmp[0].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value X in" << s;
			a = 0;
		}
		b = tmp[1].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value Y in" << s;
			b = 0;
		}
		c = tmp[2].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value Z in" << s;
			c = 0;
		}
		res = QVector3D(a, b, c);
	}
	return res;
};

vec3d QString_to_vec3d(QString& s)
{
	vec3d res(0, 0, 0);
	QStringList tmp = s.split(",");
	if (tmp.size() != 3) {
		qWarning() << "Wrong value in" << s;
		res = vec3d(0, 0, 0);
	} else {
		bool ok;
		double a, b, c;
		a = tmp[0].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value X in" << s;
			a = 0;
		}
		b = tmp[1].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value Y in" << s;
			b = 0;
		}
		c = tmp[2].toFloat(&ok);
		if (!ok) {
			qWarning() << "Wrong value Z in" << s;
			c = 0;
		}
		res = vec3d(a, b, c);
	}
	return res;
};
