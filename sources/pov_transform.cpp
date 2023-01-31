// -*- coding: utf-8 -*-
/** @file pov_transform.cpp
 ** pov_Transform and related.
 **/

#include "pov_transform.h"

#include <string.h>
//~ #include <GL/gl.h>

pov_Transform::pov_Transform()
{
	m_matrix = new double[16];
	memset(m_matrix, 0, 16*sizeof(double));
	/*
	for (int i; i<4; i++)
	{
		matrix[i][0] = 0;
		matrix[i][1] = 0;
		matrix[i][2] = 0;
		matrix[i][3] = 0;
	}
	*/
}

pov_Transform::~pov_Transform()
{
	delete [] m_matrix;
}

//~ void pov_Transform::apply()
//~ {
	//~ //glPushMatrix();
	//~ glMultMatrixd(matrix);
//~ }

//~ int pov_Transform::read(FILE *f)
//~ {
	//~ fread(matrix, sizeof(DBL)*16, 1, f);
	//~ int readden = sizeof(DBL)*16;
	//~ return readden;
//~ }

int pov_Transform::read(QDataStream& ds)
{
	int readed = ds.readRawData((char*)&m_matrix, 16*sizeof(double));
	return readed;
}

//~ vec3d pov_Transform::get_scale() const
//~ {
	//~ return vec3d(matrix[0], matrix[5], matrix[10]);
//~ }

//~ void pov_Transform::set_translation(const vec3d v)
//~ {
	//~ matrix[12] = v[0];
	//~ matrix[13] = v[1];
	//~ matrix[14] = v[2];
//~ }

//~ void pov_Transform::remove_translation()
//~ {
	//~ matrix[12] = 0;
	//~ matrix[13] = 0;
	//~ matrix[14] = 0;
//~ }

//~ void pov_Transform::clear()
//~ {
	//~ memset (matrix, 0, 16*sizeof(DBL));
	//~ matrix[0] = 1;
	//~ matrix[5] = 1;
	//~ matrix[10] = 1;
	//~ matrix[15] = 1;
//~ }

//~ vec3d pov_Transform::transform_vec3d(const vec3d v)
//~ {
	//~ DBL a,b,c;
//~ //	a = v[0]*matrix[0] + v[1]*matrix[1] + v[2]*matrix[2] + 1*matrix[3];
//~ //	b = v[0]*matrix[4] + v[1]*matrix[5] + v[2]*matrix[6] + 1*matrix[7];
//~ //	c = v[0]*matrix[8] + v[1]*matrix[9] + v[2]*matrix[10] + 1*matrix[11];
	//~ a = v[0]*matrix[0] + v[1]*matrix[4] + v[2]*matrix[8] + 1*matrix[12];
	//~ b = v[0]*matrix[1] + v[1]*matrix[5] + v[2]*matrix[9] + 1*matrix[13];
	//~ c = v[0]*matrix[2] + v[1]*matrix[6] + v[2]*matrix[10] + 1*matrix[14];
	//~ return vec3d(a, b, c);
//~ }

//~ void pov_Transform::print(std::ostream& output) const
//~ {
	//~ output << "\t" << matrix[0] << ",\t"<< matrix[1] << ",\t"<< matrix[2] << ",\t"<< matrix[3] << ",\t" << std::endl;
	//~ output << "\t" << matrix[4] << ",\t"<< matrix[5] << ",\t"<< matrix[6] << ",\t"<< matrix[7] << ",\t" << std::endl;
	//~ output << "\t" << matrix[8] << ",\t"<< matrix[9] << ",\t"<< matrix[10] << ",\t"<< matrix[11] << ",\t" << std::endl;
	//~ output << "\t" << matrix[12] << ",\t"<< matrix[13] << ",\t"<< matrix[14] << ",\t"<< matrix[15] << ",\t" << std::endl;
//~ }

//~ std::ostream& operator<<( std::ostream& os, const pov_Transform& t )
//~ {
	//~ // out to stream
	//~ t.print(os);
//~ //	os << t.data[0] << " " << t.data[1] << " " << t.data[2];
	//~ return os;
//~ }

