// -*- coding: utf-8 -*-
/** @file pov_transform.h
 ** Matrix of transformation object.
 **/
#ifndef POV_TRANSFORM_H
#define POV_TRANSFORM_H

#include <QDataStream>
#include <QDebug>

class pov_Transform {
private:
	double* m_matrix;
public:
	pov_Transform();
	~pov_Transform();
	void setToIdentity();
	double operator[](const int idx) const;
	//~ void apply();
	//~ vec3d transform_vec3d(const vec3d v);
	//~ int read(FILE *f);
	int read(QDataStream& ds);
	friend QDebug operator<<(QDebug d, const pov_Transform& t);
	//~ void remove_translation();
	//~ void set_translation(const vec3d v);
	//~ void clear();
	//~ vec3d get_scale() const;
	//~ void print(std::ostream& output) const;
};

#endif	// POV_TRANSFORM_H
