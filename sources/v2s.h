// -*- coding: utf-8 -*-
/** @file v2s.h
 ** Vector String conversions.
 **/
#ifndef V2S_H
#define V2S_H

#include <QString>
#include <QVector2D>
#include <QVector3D>
#include <QVector4D>
#include "vec3d.h"

QString QVector2D_to_QString(QVector2D& v);
QVector2D QString_to_QVector2D(QString& s);

QString QVector3D_to_QString(QVector3D& v);
QVector3D QString_to_QVector3D(QString& s);

QString QVector4D_to_QString(QVector4D& v);
QVector4D QString_to_QVector4D(QString& s);

QString vec3d_to_QString(vec3d& v);
vec3d QString_to_vec3d(QString& s);

#endif	// V2S_H
