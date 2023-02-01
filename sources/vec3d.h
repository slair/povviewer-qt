// -*- coding: utf-8 -*-
/** @file vec3d.h
 ** vec3d and related
 **/
#ifndef VEC3D_H
#define VEC3D_H

#include <QString>
//~ #include <iostream>
//~ #include <vector>
#include <math.h>

typedef short vec3s[3];
#define EPSILON 1.0e-3

class vec3d
{
	protected:
		double data[3];
	public:
		~vec3d();
		vec3d();
		vec3d(const double a);
		vec3d(const double a, const double b, const double c);
		//~ vec3d(const std::string val);
		vec3d(const QString val);
		vec3d(const vec3d& src);	// copy constructor
		const vec3d &operator=(const vec3d& src);	//
		double operator[](const int idx) const;
		// math operations
		vec3d operator-(const vec3d& a) const;	// this - a
		vec3d operator-(const double& a) const;	// this - (a, a, a)
		vec3d operator+(const vec3d& a) const;	// this + a
		vec3d operator+(const double& a) const;	// this + (a, a, a)
		vec3d operator*(const vec3d& a) const;	// this * a
		vec3d operator*(const double& a) const; // this * (a, a, a)
		vec3d operator/(const vec3d& a) const; // this / a
		vec3d operator/(const double& a) const; // this / (a, a, a)
		// vector operations
		double length() const;
		vec3d normalize() const;
		vec3d operator^(const vec3d& tgt);	// cross product: this x tgt
		double vdot(const vec3d& tgt); 	// dot product: this (dot) tgt
		// compare operations
		bool operator==(const vec3d& a) const;
		bool operator<(const vec3d& a) const;
		// io subsystem
		void print(std::ostream& output) const;
		void input(std::istream& is);
		unsigned int read(FILE* f);
		void gl_normal() const;
		void gl_vertex() const;
		void gl_color() const;
		void gl_clear_color() const;
		void draw_star(double star_size) const;
		void draw_vector(vec3d loc, vec3d color) const;
		void draw_vector_a(vec3d loc, vec3d color) const;
		void to4f(float tgt[4]) const;
		void draw_navigator(double size) const;
		std::string print(const char* fmt) const;
		void clamp_max(const double max_value);
		void clamp_min(const double min_value);
		void build_matrix(double m[3][3], const double angle) const;
		void mult_matrix(const double m[3][3]);
		void gl_rasterpos() const;
		friend QDebug operator << (QDebug d, const vec3d& v);
};

bool compare_ex(const vec3d a, const vec3d b);

typedef vec3d triangle[3];

double cos_alpha(const vec3d a, const vec3d b);

vec3d vec3d_from_vec3s(const vec3s src);

std::ostream& operator<<( std::ostream& os, const vec3d& t );

#endif	// VEC3D_H
