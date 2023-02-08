// -*- coding: utf-8 -*-
/** @file v2s.h
 ** Vector String conversions.
 **/
#ifndef V2S_H
#define V2S_H

#include <QString>
#include <QVector3D>
#include <QVector4D>
#include "vec3d.h"

QString QVector3D_to_QString(QVector3D& v);
QString QVector4D_to_QString(QVector4D& v);
QString vec3d_to_QString(vec3d& v);
QVector4D QString_to_QVector4D(QString& s);
QVector3D QString_to_QVector3D(QString& s);
vec3d QString_to_vec3d(QString& s);

#endif	// V2S_H
