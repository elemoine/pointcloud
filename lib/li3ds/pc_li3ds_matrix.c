/***********************************************************************
* pc_matrix.c
*
*  Simple matrix library.
*  Allow this library to be used both inside and outside a
*  PgSQL backend.
*
*  Copyright (c) 2017 Oslandia
*  Copyright (c) 2017 IGN
*
***********************************************************************/

#include "pc_li3ds_api_internal.h"
#include "../pc_api_internal.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>


void pc_matrix_44_warn(const char *func, int line, const char *name, const PCMAT44 m)
{
	pcwarn("%s:%d %s=\n%lf %lf %lf %lf\n%lf %lf %lf %lf\n%lf %lf %lf %lf\n%lf %lf %lf %lf\n",
		func, line, name,
		m[ 0], m[ 1], m[ 2], m[ 3],
		m[ 4], m[ 5], m[ 6], m[ 7],
		m[ 8], m[ 9], m[10], m[11],
		m[12], m[13], m[14], m[15]);
}

void pc_vector_3_warn(const char *func, int line, const char *name, const PCVEC4 v)
{
	pcwarn("%s:%d %s=%lf %lf %lf", func, line, name, v[0], v[1], v[2]);
}

void pc_vector_4_warn(const char *func, int line, const char *name, const PCVEC4 v)
{
	pcwarn("%s:%d %s=%lf %lf %lf %lf", func, line, name, v[0], v[1], v[2], v[3]);
}

/**
* Set a 4x3 matrix (4 columns, 3 lines)
*/
void
pc_matrix_43_set(PCMAT43 mat,
		double a, double b, double c, double d,
		double e, double f, double g, double h,
		double i, double j, double k, double l)
{
	mat[0] = a; mat[1] = b; mat[ 2] = c; mat[ 3] = d;
	mat[4] = e; mat[5] = f; mat[ 6] = g; mat[ 7] = h;
	mat[8] = i; mat[9] = j; mat[10] = k; mat[11] = l;
}



/**
* Set a 4x4 matrix (4 columns, 4 lines)
*/
void
pc_matrix_44_copy(PCMAT44 res, const PCMAT44 mat)
{
	memcpy(res, mat, 16*sizeof(double));
}

/**
* Set a 3 vector
*/
void
pc_vector_3_copy(PCVEC3 res, const PCVEC3 vec)
{
	memcpy(res, vec, 3*sizeof(double));
}

void
pc_vector_3_cross(PCVEC3 res, const PCVEC3 v0, const PCVEC3 v1)
{
	res[0] = v0[1]*v1[2] - v0[2]*v1[1];
	res[1] = v0[2]*v1[0] - v0[0]*v1[2];
	res[2] = v0[0]*v1[1] - v0[1]*v1[0];
}

double
pc_vector_3_dot(const PCVEC3 v0, const PCVEC3 v1)
{
	return v0[0]*v1[0] + v0[1]*v1[1] + v0[2]*v1[2];
}

void
pc_vector_3_abs(PCVEC3 res, const PCVEC3 vec)
{
	res[0] = fabs(vec[0]);
	res[1] = fabs(vec[1]);
	res[2] = fabs(vec[2]);
}

/**
* Set a 4x4 matrix.
*/
void
pc_matrix_44_set(PCMAT44 mat,
		double a, double b, double c, double d,
		double e, double f, double g, double h,
		double i, double j, double k, double l,
		double m, double n, double o, double p)
{
	mat[ 0] = a; mat[ 1] = b; mat[ 2] = c; mat[ 3] = d;
	mat[ 4] = e; mat[ 5] = f; mat[ 6] = g; mat[ 7] = h;
	mat[ 8] = i; mat[ 9] = j; mat[10] = k; mat[11] = l;
	mat[12] = m; mat[13] = n; mat[14] = o; mat[15] = p;
}


/**
* Sets the 3x3 matrix mat associated to the (qw, qx, qy, qz) quaternion. Assume
* that the quaternion is a unit quaternion.
*/
void
pc_matrix_33_set_from_quaternion(PCMAT33 mat,
		double qw, double qx, double qy, double qz)
{
	double x = qx+qx;
	double y = qy+qy;
	double z = qz+qz;

	double xx = qx * x;
	double xy = qx * y;
	double xz = qx * z;
	double yy = qy * y;
	double yz = qy * z;
	double zz = qz * z;
	double wx = qw * x;
	double wy = qw * y;
	double wz = qw * z;

	mat[0] = 1 - ( yy + zz );
	mat[1] = xy - wz;
	mat[2] = xz + wy;

	mat[3] = xy + wz;
	mat[4] = 1 - ( xx + zz );
	mat[5] = yz - wx;

	mat[6] = xz - wy;
	mat[7] = yz + wx;
	mat[8] = 1 - ( xx + yy );
}

/**
* Multiply mat by vec and store the resuting vector in res.
*/
void
pc_matrix_33_multiply_vector_3(PCVEC3 res, const PCMAT33 mat, const PCVEC3 vec)
{
	res[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[2] * vec[2];
	res[1] = mat[3] * vec[0] + mat[4] * vec[1] + mat[5] * vec[2];
	res[2] = mat[6] * vec[0] + mat[7] * vec[1] + mat[8] * vec[2];
}

/**
* Multiply mat by vec and store the resuting vector in res.
*/
void
pc_matrix_44_multiply_vector_3(PCVEC4 res, const PCMAT44 mat, const PCVEC3 vec)
{
	res[0] = mat[ 0] * vec[0] + mat[ 1] * vec[1] + mat[ 2] * vec[2] + mat[ 3];
	res[1] = mat[ 4] * vec[0] + mat[ 5] * vec[1] + mat[ 6] * vec[2] + mat[ 7];
	res[2] = mat[ 8] * vec[0] + mat[ 9] * vec[1] + mat[10] * vec[2] + mat[11];
	res[3] = mat[12] * vec[0] + mat[13] * vec[1] + mat[14] * vec[2] + mat[15];
}

/**
* Multiply mat by vec and store the resuting vector in res.
*/
void
pc_matrix_44_multiply_vector_4(PCVEC4 res, const PCMAT44 mat, const PCVEC4 vec)
{
	res[0] = mat[ 0] * vec[0] + mat[ 1] * vec[1] + mat[ 2] * vec[2] + mat[ 3] * vec[3];
	res[1] = mat[ 4] * vec[0] + mat[ 5] * vec[1] + mat[ 6] * vec[2] + mat[ 7] * vec[3];
	res[2] = mat[ 8] * vec[0] + mat[ 9] * vec[1] + mat[10] * vec[2] + mat[11] * vec[3];
	res[3] = mat[12] * vec[0] + mat[13] * vec[1] + mat[14] * vec[2] + mat[15] * vec[3];
}

/**
* Apply an affine transformation to the vector vec and store the resulting
* vector in res.
*/
void
pc_matrix_43_transform_affine(PCVEC3 res, const PCMAT43 mat, const PCVEC3 vec)
{
	res[0] = mat[0] * vec[0] + mat[1] * vec[1] + mat[ 2] * vec[2] + mat[ 3];
	res[1] = mat[4] * vec[0] + mat[5] * vec[1] + mat[ 6] * vec[2] + mat[ 7];
	res[2] = mat[8] * vec[0] + mat[9] * vec[1] + mat[10] * vec[2] + mat[11];
}

/**
* Multiply mat by vec and store the resuting vector in res.
*/
void
pc_matrix_44_multiply_matrix_44(PCMAT44 res, const PCMAT44 mat1, const PCMAT44 mat2)
{
	PCMAT44 t;
	pc_matrix_44_transpose(t,mat2);
	pc_matrix_44_multiply_vector_4(res   ,t,mat1   );
	pc_matrix_44_multiply_vector_4(res+ 4,t,mat1+ 4);
	pc_matrix_44_multiply_vector_4(res+ 8,t,mat1+ 8);
	pc_matrix_44_multiply_vector_4(res+12,t,mat1+12);
}

/**
* Multiply mat by vec and store the resuting vector in res.
*/
void
pc_matrix_44_transpose(PCMAT44 res, const PCMAT44 mat)
{
	res[ 0] = mat[0]; res[ 1] = mat[4]; res[ 2] = mat[ 8]; res[ 3] = mat[12];
	res[ 4] = mat[1]; res[ 5] = mat[5]; res[ 6] = mat[ 9]; res[ 7] = mat[13];
	res[ 8] = mat[2]; res[ 9] = mat[6]; res[10] = mat[10]; res[11] = mat[14];
	res[12] = mat[3]; res[13] = mat[7]; res[14] = mat[11]; res[15] = mat[15];
}

void
pc_matrix_44_multiply(PCMAT44 res, const PCMAT44 mat, double v)
{
	int i;
	for( i = 0; i < 16; ++i)
		res[i] = v*mat[i];
}

void
pc_matrix_44_plucker(double v[6], const PCMAT44 m)
{
	double m00 = m[ 0], m10 = m[ 1];
	double m01 = m[ 4], m11 = m[ 5];
	double m02 = m[ 8], m12 = m[ 9];
	double m03 = m[12], m13 = m[13];

	v[0] = m01*m12-m02*m11;
	v[1] = m00*m12-m02*m10;
	v[2] = m00*m11-m01*m10;
	v[3] = m00*m13-m03*m10;
	v[4] = m01*m13-m03*m11;
	v[5] = m02*m13-m03*m12;
}

double
pc_matrix_44_determinant(const PCMAT44 m)
{
	double p[6];
	double q[6];
	pc_matrix_44_plucker(p,m  ); // 2 left  columns
	pc_matrix_44_plucker(q,m+2); // 2 right columns
	return p[0]*q[3] - p[1]*q[4] + p[2]*q[5] + p[3]*q[0] - p[4]*q[1] + p[5]*q[2];
}

/**
* adjugate is the transposed of the comatrix.
* This is equal to the inverse, up to the scaling by 1/det
* and may be used in place of the inverse in homogeneous contexts.
*/
void
pc_matrix_44_adjugate(PCMAT44 res, const PCMAT44 m, double *determinant)
{
	double p[6];
	double q[6];
	pc_matrix_44_plucker(p,m  ); // 2 left  columns
	pc_matrix_44_plucker(q,m+2); // 2 right columns

	res[ 0] = m[5]*q[5]-m[ 9]*q[4]+m[13]*q[0];
	res[ 4] =-m[4]*q[5]+m[ 8]*q[4]-m[12]*q[0];
	res[ 8] = m[7]*p[5]-m[11]*p[4]+m[15]*p[0];
	res[12] =-m[6]*p[5]+m[10]*p[4]-m[14]*p[0];

	res[ 1] =-m[1]*q[5]+m[ 9]*q[3]-m[13]*q[1];
	res[ 5] = m[0]*q[5]-m[ 8]*q[3]+m[12]*q[1];
	res[ 9] =-m[3]*p[5]+m[11]*p[3]-m[15]*p[1];
	res[13] = m[2]*p[5]-m[10]*p[3]+m[14]*p[1];

	res[ 2] = m[1]*q[4]-m[ 5]*q[3]+m[13]*q[2];
	res[ 6] =-m[0]*q[4]+m[ 4]*q[3]-m[12]*q[2];
	res[10] = m[3]*p[4]-m[ 7]*p[3]+m[15]*p[2];
	res[14] =-m[2]*p[4]+m[ 6]*p[3]-m[14]*p[2];

	res[ 3] =-m[1]*q[0]+m[ 5]*q[1]-m[ 9]*q[2];
	res[ 7] = m[0]*q[0]-m[ 4]*q[1]+m[ 8]*q[2];
	res[11] =-m[3]*p[0]+m[ 7]*p[1]-m[11]*p[2];
	res[15] = m[2]*p[0]-m[ 6]*p[1]+m[10]*p[2];

	if(determinant)
		*determinant = p[0]*q[3] - p[1]*q[4] + p[2]*q[5] + p[3]*q[0] - p[4]*q[1] + p[5]*q[2];
}

/**
* inverse matrix (could be optimized)
*/
int
pc_matrix_44_inverse(PCMAT44 res, const PCMAT44 mat)
{
	double det;
	pc_matrix_44_adjugate(res,mat,&det);
	if(det == 0)
		return PC_FAILURE;
	pc_matrix_44_multiply(res,res,1./det);
	return PC_SUCCESS;
}

void
pc_vector_3_add(PCVEC3 res, const PCVEC3 v1, const PCVEC3 v2)
{
	res[0] = v1[0] + v2[0];
	res[1] = v1[1] + v2[1];
	res[2] = v1[2] + v2[2];
}

void
pc_vector_3_sub(PCVEC3 res, const PCVEC3 v1, const PCVEC3 v2)
{
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
	res[2] = v1[2] - v2[2];
}

void
pc_vector_3_min(PCVEC3 res, const PCVEC3 v1, const PCVEC3 v2)
{
	res[0] = fmin(v1[0],v2[0]);
	res[1] = fmin(v1[1],v2[1]);
	res[2] = fmin(v1[2],v2[2]);
}

void
pc_vector_3_max(PCVEC3 res, const PCVEC3 v1, const PCVEC3 v2)
{
	res[0] = fmax(v1[0],v2[0]);
	res[1] = fmax(v1[1],v2[1]);
	res[2] = fmax(v1[2],v2[2]);
}

void
pc_vector_4_add(PCVEC4 res, const PCVEC4 v1, const PCVEC4 v2)
{
	res[0] = v1[0] + v2[0];
	res[1] = v1[1] + v2[1];
	res[2] = v1[2] + v2[2];
	res[3] = v1[3] + v2[3];
}

void
pc_vector_4_sub(PCVEC4 res, const PCVEC4 v1, const PCVEC4 v2)
{
	res[0] = v1[0] - v2[0];
	res[1] = v1[1] - v2[1];
	res[2] = v1[2] - v2[2];
	res[3] = v1[3] - v2[3];
}

int
pc_vector_3_from_homogeneous(PCVEC3 res, const PCVEC4 v)
{
	if(v[3]==0) return PC_FAILURE;
	res[0] = v[0] / v[3];
	res[1] = v[1] / v[3];
	res[2] = v[2] / v[3];
	return PC_SUCCESS;
}

#define PCVEC2MID(x,y) 0.5*((x)[0]+(y)[0]), 0.5*((x)[1]+(y)[1])
#define PCVEC3SUB(x,y) (x)[0]-(y)[0], (x)[1]-(y)[1], (x)[2]-(y)[2]
#define PCVEC3ADD(x,y) (x)[0]+(y)[0], (x)[1]+(y)[1], (x)[2]+(y)[2]
#define PCVEC4SUB(x,y) (x)[0]-(y)[0], (x)[1]-(y)[1], (x)[2]-(y)[2], (x)[3]-(y)[3]
#define PCVEC4ADD(x,y) (x)[0]+(y)[0], (x)[1]+(y)[1], (x)[2]+(y)[2], (x)[3]+(y)[3]
#define PCBOX3CORNERS(b) \
	{ b[0][0], b[0][1], b[0][2] }, \
	{ b[1][0], b[0][1], b[0][2] }, \
	{ b[0][0], b[1][1], b[0][2] }, \
	{ b[1][0], b[1][1], b[0][2] }, \
	{ b[0][0], b[0][1], b[1][2] }, \
	{ b[1][0], b[0][1], b[1][2] }, \
	{ b[0][0], b[1][1], b[1][2] }, \
	{ b[1][0], b[1][1], b[1][2] }

/*
* res[i] = transform_projective(f->fwd, corner[i])
* corner[i] = {
*     [ -1 , -1, -1 ]
*     [  1 , -1, -1 ]
*     [ -1 ,  1, -1 ]
*     [  1 ,  1, -1 ]
*     [ -1 , -1,  1 ]
*     [  1 , -1,  1 ]
*     [ -1 ,  1,  1 ]
*     [  1 ,  1,  1 ]
* }
*/
int pc_frustum_get_points(PCVEC3 res[8], const PCMAT44 fwd)
{
	PCMAT44 m;
	PCVEC4 v[8];
	int i;
	pc_matrix_44_transpose(m,fwd);
	pc_vector_4_add(v[1], m+12, m);
	pc_vector_4_sub(v[0], m+12, m);

	pc_vector_4_add(v[2], v[0], m+4);
	pc_vector_4_add(v[3], v[1], m+4);

	pc_vector_4_sub(v[0], v[0], m+4);
	pc_vector_4_sub(v[1], v[1], m+4);

	pc_vector_4_add(v[4], v[0], m+8);
	pc_vector_4_add(v[5], v[1], m+8);
	pc_vector_4_add(v[6], v[2], m+8);
	pc_vector_4_add(v[7], v[3], m+8);

	pc_vector_4_sub(v[0], v[0], m+8);
	pc_vector_4_sub(v[1], v[1], m+8);
	pc_vector_4_sub(v[2], v[2], m+8);
	pc_vector_4_sub(v[3], v[3], m+8);

	for( i = 0; i < 8; ++i )
	{
		if(!pc_vector_3_from_homogeneous(res[i],v[i]))
			return PC_FAILURE;
	}
	return PC_SUCCESS;
}

int
pc_box_from_vector_3_array(PCBOX3 res,  PCVEC3 * const p, size_t n)
{
	int i;
	if (n==0)
		return PC_FAILURE;
	pc_vector_3_copy(res[0],p[0]);
	pc_vector_3_copy(res[1],p[0]);
	for( i = 1; i < n; ++i )
	{
		pc_vector_3_min(res[0],res[0],p[i]);
		pc_vector_3_max(res[1],res[1],p[i]);
	}
	return PC_SUCCESS;
}

/*
* PCFRUSTUM <-> PCBOX3 conversions
*/

/** convert a box to a frustum (lossless) */
int
pc_frustum_from_box(PCFRUSTUM *res, const PCBOX3 b)
{
	PCVEC3 s = { PCVEC3SUB(b[1],b[0]) };
	PCVEC3 c = { PCVEC3ADD(b[0],b[1]) };
	PCVEC3 v = { s[1]*s[2], s[0]*s[2], s[0]*s[1] };
	double vol = s[0]*v[0]; // s0*s1*s2

	double fwd[] = {
		s[0], 0,  0,  c[0],
		0,  s[1], 0,  c[1],
		0,  0,  s[2], c[2],
		0,  0,  0,  2
	};
	pc_matrix_44_copy(res->fwd, fwd );

	double bwd[] = {
		2*v[0], 0, 0, -v[0]*c[0],
		0, 2*v[1], 0, -v[1]*c[1],
		0, 0, 2*v[2], -v[2]*c[2],
		0, 0, 0, vol
	};
	res->bwd = pcalloc(16*sizeof(double));
	pc_matrix_44_copy(res->bwd, bwd );

	res->det = 2*vol;
	return PC_SUCCESS;
}

/** convert a frustum to a box (lossy if not axis-aligned) */
int
pc_box_from_frustum(PCBOX3 res, const PCFRUSTUM *f)
{
	PCVEC3 p[8];
	return pc_frustum_get_points(p,f->fwd) && pc_box_from_vector_3_array(res,p,8);
}

/**
* Apply a projective transformation to the vector vec and store the resulting
* vector in res.
*/
int
pc_matrix_44_transform_projective_vector_3(PCVEC3 res, const PCMAT44 mat, const PCVEC3 vec)
{
	PCVEC4 hres;
	pc_matrix_44_multiply_vector_3(hres,mat,vec);
	return pc_vector_3_from_homogeneous(res,hres);
}

int
pc_matrix_44_transform_projective_vector_4(PCVEC3 res, const PCMAT44 mat, const PCVEC4 vec)
{
	PCVEC4 hres;
	pc_matrix_44_multiply_vector_4(hres,mat,vec);
	return pc_vector_3_from_homogeneous(res,hres);
}

int
pc_box_projective(PCBOX3 res, const PCMAT44 mat, PCBOX3 b)
{
	PCVEC3 p[] = { PCBOX3CORNERS(b) };
	int i;
	for( i = 0; i < 8; ++i )
	{
		if(!pc_matrix_44_transform_projective_vector_3(p[i],mat,p[i]))
			return PC_FAILURE;
	}
	return pc_box_from_vector_3_array(res,p,8);
}

int
pc_frustum_projective(PCFRUSTUM *res, const PCFRUSTUM *f,
	const PCMAT44 fwd)
{
	pc_matrix_44_multiply_matrix_44(res->fwd, fwd, f->fwd);
	res->bwd = NULL;
	return PC_SUCCESS;
}

/**
* vector3 normalization
*/

int
pc_vector_3_normalize(PCVEC3 res, const PCVEC3 vec)
{
	double r2 = pc_vector_3_dot(vec,vec);
	if(r2<=0)
		return PC_FAILURE;
	double inv = 1/sqrt(r2);
	res[0] = vec[0] * inv;
	res[1] = vec[1] * inv;
	res[2] = vec[2] * inv;
	return PC_SUCCESS;
}

int
pc_box_normalize(PCBOX3 res, const PCBOX3 b)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}

int
pc_frustum_normalize(PCFRUSTUM *res, const PCFRUSTUM *f)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}

/**
* Cartesian <- Spherical
*/

int
pc_vector_3_cartesian_from_spherical(PCVEC3 res, const PCVEC3 vec)
{
	double rc2 = vec[0]*cos(vec[2]);
	double rs2 = vec[0]*sin(vec[2]);
	res[0] = rc2*cos(vec[1]);
	res[1] = rc2*sin(vec[1]);
	res[2] = rs2;
	return PC_SUCCESS;
}

int
pc_box_cartesian_from_spherical(PCBOX3 res, const PCBOX3 b)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}

int
pc_frustum_cartesian_from_spherical(PCFRUSTUM *res, const PCFRUSTUM *f)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}

/**
* Spherical <- Cartesian
*/

int
pc_vector_3_spherical_from_cartesian(PCVEC3 res, const PCVEC3 vec)
{
	res[0] = sqrt(pc_vector_3_dot(vec,vec));
	res[1] = atan2(vec[1],vec[0]);
	res[2] = asin (vec[2]/res[0]);
	return PC_SUCCESS;
}

int
pc_box_spherical_from_cartesian(PCBOX3 res, const PCBOX3 b)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}

int
pc_frustum_spherical_from_cartesian(PCFRUSTUM *res, const PCFRUSTUM *f)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}


/**
* Distorsion polynom : r3,r5,r7
* https://github.com/IGNF/libOri/blob/master/src/DistortionPolynom.cpp
*/

int
pc_vector_3_distorsion(PCVEC3 res, const PCDISTORSION *d, const PCVEC3 vec)
{
	double v[] = { res[0]-d->pps[0], res[0]-d->pps[1] };
	double r2 = v[0]*v[0]+v[1]*v[1];
	double dr;
	if(r2>d->r2max)
		return PC_FAILURE;

	dr = r2*(d->c[0] + r2*(d->c[1] + r2*d->c[2]));
	res[0] = vec[0] + dr*v[0];
	res[1] = vec[1] + dr*v[1];
	res[2] = vec[2];
	return PC_SUCCESS;
}

int
pc_box_transform_distorsion(PCBOX3 res, const PCDISTORSION *d, const PCBOX3 b)
{
	double c0 = d->c[0], c1 = d->c[1], c2 = d->c[2];
	// roots (xp,xm) of P' = c0 + 2 c1 X + 3 c2 X^2
	double delta = c1*c1-3*c0*c2;
	double xp = 0, xm = 0, Pp = 0, Pm = 0;
	if ( delta > 0 )
	{
		double sqrtdelta = sqrt(delta);
		xp = (-c1+sqrtdelta)/(3*c2);
		xm = (-c1-sqrtdelta)/(3*c2);
		// values of extrema of P = c0 X + c1 X^2 + c2 X^3
		Pp = xp * ( c0 + xp * ( c1 + xp * c2 ) );
		Pm = xm * ( c0 + xm * ( c1 + xm * c2 ) );
	}

	// distances to PPS along axes
	double r0[] = {
		b[1][0] - d->pps[0],
		b[1][1] - d->pps[1],
		b[0][0] - d->pps[0],
		b[0][1] - d->pps[1]
	};
	// < 0 if pps is inside box horizontaly and verticaly
	double inside[] = { r0[0]*r0[2], r0[1]*r0[3] };
	// squared distances to PPS along axes
	double r20[4];
	// squared distances to PPS at corners
	double r2c[4];
	// values of P at corners
	double Pc[4];
	// max values of P(corner) for each edge
	double Pe[4];
	int i;
	for( i = 0; i < 4; ++i )
	{
		r20[i] = r0[i] * r0[i];
		r2c[i] = r20[i] + r20[(i+3)%4];
		Pc[i] = r2c[i] * ( c0 + r2c[i] * ( c1 + r2c[i] * c2 ) );
	}
	for( i = 0; i < 4; ++i )
	{
		if(r2c[i] > d->r2max)
			return PC_FAILURE;

		Pe[i] = fmax(Pc[i],Pc[(i+1)%4]);
		if(inside[i&1] < 0)
		{
			double P0 = r20[i] * ( c0 + r20[i] * ( c1 + r20[i] * c2 ) );
			Pe[i] = fmax(Pe[i], P0);
		}
		if(delta>0)
		{
			double r2min = (inside[i&1] < 0) ? r20[i] : fmin(r2c[i], r2c[(i+1)%4]);
			double r2max = fmax(r2c[i], r2c[(i+1)%4]);
			if( r2min < xp && xp < r2max )
				Pe[i] = fmax(Pe[i], Pp);
			if( r2min < xm && xm < r2max )
				Pe[i] = fmax(Pe[i], Pm);
		}
	}
	res[1][0] = b[1][0] + Pe[0] * r0[0];
	res[1][1] = b[1][1] + Pe[1] * r0[1];
	res[0][0] = b[0][0] + Pe[2] * r0[2];
	res[0][1] = b[0][1] + Pe[3] * r0[3];
	res[0][2] = b[0][2];
	res[1][2] = b[1][2];

	return PC_SUCCESS;
}

int
pc_frustum_transform_distorsion(PCFRUSTUM *res, const PCDISTORSION *d, const PCFRUSTUM *f)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}

/**
* Undistorsion : polynom r3,r5,r7
* https://github.com/IGNF/libOri/blob/master/src/DistortionPolynom.cpp
*/

#define PCUNDISTORSION_ERR2MAX (1e-15)

int
pc_vector_3_undistorsion(PCVEC3 res, const PCDISTORSION *d, const PCVEC3 vec)
{

	double v[] = { res[0]-d->pps[0], res[0]-d->pps[1] };
	double r2 = v[0]*v[0]+v[1]*v[1];
	double r4 = r2*r2;
	double r6 = r4*r2;

	double c3 = d->c[0]*r2, d2 = 3*c3;
	double c5 = d->c[1]*r4, d4 = 5*c5;
	double c7 = d->c[2]*r6, d6 = 7*c7;

	double t = 1; // Newton's method initialisation
	int i, i_max=50; // max num of iterations
	for( i = 0; i < i_max; ++i )
	{
		double t2 = t *t;
		double R_, R  = -1+t+t*t2*(c3+t2*(c5+t2*c7));

		if(R*R*r2<PCUNDISTORSION_ERR2MAX && t2*r2<d->r2max)
		{
			res[0] = d->pps[0] + t * v[0];
			res[1] = d->pps[1] + t * v[1];
			res[2] = vec[2];
			return PC_SUCCESS;
		}
		R_ = 1+  t2*(d2+t2*(d4+t2*d6));
		t -= R/R_; // Newton's method iteration
		if(t<0) t=0;
	}
	return PC_FAILURE;
}

int
pc_box_transform_undistorsion(PCBOX3 res, const PCDISTORSION *d, const PCBOX3 b)
{
	pcwarn("%s: current implementation is not conservative (bbox of 8points)...", __func__);
	double c[2] = { PCVEC2MID(b[0],b[1]) };
	PCVEC3 q[8];
	PCVEC3 p[8] = {
		{ b[0][0], b[0][1], b[0][2]},
		{ b[1][0], b[0][1], b[0][2]},
		{ b[0][0], b[1][1], b[1][2]},
		{ b[1][0], b[1][1], b[1][2]},
		{ c[0],    b[0][1], b[0][2]},
		{ c[0],    b[1][1], b[0][2]},
		{ b[0][0], c[1],    b[0][2]},
		{ b[1][0], c[1],    b[0][2]}
	};
	int i;
	for( i = 0; i < 8; ++i )
		if(!pc_vector_3_undistorsion(q[i],d,p[i]))
			return PC_FAILURE;

	return pc_box_from_vector_3_array(res,q,8);
}

int
pc_frustum_transform_undistorsion(PCFRUSTUM *res, const PCDISTORSION *d, const PCFRUSTUM *f)
{
	pcwarn("%s: not implemented yet...", __func__);
	return PC_FAILURE;
}

/**
* EWKB export
*/

uint8_t *
pc_frustum_corners_to_geometry_wkb( const PCVEC3 p[8], uint32_t srid, size_t *wkbsize)
{
	static uint32_t srid_mask = 0x20000000;
	static uint32_t z_mask = 0x80000000;
	uint32_t wkbtype = 15 | z_mask; /* WKB POLYHEDRALSURFACETYPE Z */
	uint32_t wkbpolyztype = 3 | z_mask; /* WKB POLYGONTYPE Z */
	int32_t nrings = 1;
	int32_t npoints = 5;
	int32_t ngeoms = 6;
	size_t pointsize = 3*sizeof(double);
	size_t polysize = 1 + 4 + 4 + 4 + npoints * pointsize;
	size_t size = 1 + 4 + 4 + ngeoms*polysize; /* endian + type + 6 quads */
	uint8_t *wkb, *ptr;
	uint32_t i,j;

	// should we care about face orientation ?
	int face[6][5] = {
		{0,2,4,6,0}, // -X
		{1,3,5,7,1}, // +X
		{0,1,4,5,0}, // -Y
		{2,3,6,7,2}, // +Y
		{0,1,2,3,0}, // -Z
		{4,5,6,7,4}  // +Z
	};

	if ( srid != 0 )
	{
		wkbtype |= srid_mask;
		size += 4;
	}

	wkb = pcalloc(size);
	ptr = wkb;

	ptr[0] = machine_endian(); /* Endian flag */
	ptr += 1;

	memcpy(ptr, &wkbtype, 4); /* WKB type */
	ptr += 4;

	if ( srid != 0 )
	{
		memcpy(ptr, &srid, 4); /* SRID */
		ptr += 4;
	}

	memcpy(ptr, &ngeoms, 4); /* 6 */
	ptr += 4;

	for( i = 0; i < ngeoms; ++i )
	{
		ptr[0] = wkb[0]; /* Endian flag */
		ptr += 1;

		memcpy(ptr, &wkbpolyztype, 4); /* WKB POLYGONTYPE type */
		ptr += 4;

		memcpy(ptr, &nrings, 4); /* 1 */
		ptr += 4;

		memcpy(ptr, &npoints, 4); /* 4 */
		ptr += 4;

		for( j = 0; j < npoints; ++j )
		{
			memcpy(ptr, p[face[i][j]], pointsize);
			ptr += pointsize;
		}
	}

	if ( wkbsize ) *wkbsize = size;
	return wkb;
}

//should we add a srid to PCBOX3 ?
uint8_t *
pc_box_to_geometry_wkb(const PCBOX3 b, uint32_t srid, size_t *wkbsize)
{
	const PCVEC3 p[] = { PCBOX3CORNERS(b) };
	return pc_frustum_corners_to_geometry_wkb(p,srid,wkbsize);
}

//should we add a srid to PCFRUSTUM ?
uint8_t *
pc_frustum_to_geometry_wkb(const PCFRUSTUM *f, uint32_t srid, size_t *wkbsize)
{
	const PCVEC3 p[8];
	if( ! pc_frustum_get_points((PCVEC3 *)p,f->fwd) )
		return NULL;
	return pc_frustum_corners_to_geometry_wkb(p,srid,wkbsize);
}

/*
* mode specifies the tradeoff between efficiency and exactness of the test
* - 1 : separating axes defined by face normals of f1
* - 2 : separating axes defined by face normals of f2
* - 3 : separating axes defined by face normals of f1 and f2
* - 4 : separating axes defined cross products of face edges + mode 3
*
* The test is always conservative (no intersections missed) but may
* report false positives if mode is not 4.
*  https://en.wikipedia.org/wiki/Hyperplane_separation_theorem
* separating planes along the face normals of a face are checked by
* transforming the problem with the bwd transform of the other face
* so that the transformed frustum is checked against vertices of the
* unit cube [-1,1]^3 of canonical coordinates +-1.
*
* in practice mode=3 is a good trade off between efficiency and false positives
*/
int
pc_frustum_intersects(const PCFRUSTUM *f1, const PCFRUSTUM *f2, uint8_t mode)
{
	PCMAT44 m12, m21;
	PCVEC4 plane[6];
	PCVEC4 *p;
	double *m, *mw;

	assert(pc_frustum_is_valid(f1) && pc_frustum_is_valid(f2));

	if( mode<1 || mode>4 )
	{
		pcwarn("%s : invalid mode %d (valid modes are 1,2,3 and 4)", __func__, mode);
		return PC_TRUE;
	}

	if(mode & 5) // mode is 1, 3 or 4
	{
		// checking the transformed frustum planes against the +-1 vertices
		pc_matrix_44_multiply_matrix_44(m12,f2->bwd,f1->fwd);

		p = plane;
		mw = m12 + 12;
		for( m = m12; m != mw; m+=4 )
		{
			pc_vector_4_add(*p, mw, m);
			if ( fabs((*p)[0]) + fabs((*p)[1]) + fabs((*p)[2]) + (*p)[3] < 0 )
				return PC_FALSE;
			++p;
			pc_vector_4_sub(*p, mw, m);
			if ( fabs((*p)[0]) + fabs((*p)[1]) + fabs((*p)[2]) + (*p)[3] < 0 )
				return PC_FALSE;
			++p;
		}
	}

	if(mode & 6) // mode is 2, 3 or 4
	{
		// checking the transformed frustum planes against the +-1 vertices
		pc_matrix_44_multiply_matrix_44(m21,f1->bwd,f2->fwd);

		p = plane;
		mw = m21 + 12;
		for( m = m21; m != mw; m += 4 )
		{
			pc_vector_4_add(*p, mw, m);
			if ( fabs((*p)[0]) + fabs((*p)[1]) + fabs((*p)[2]) + (*p)[3] < 0 )
				return PC_FALSE;
			++p;
			pc_vector_4_sub(*p, mw, m);
			if ( fabs((*p)[0]) + fabs((*p)[1]) + fabs((*p)[2]) + (*p)[3] < 0 )
				return PC_FALSE;
			++p;
		}
	}

	if(mode == 4)
	{
		// checking the axis separation on the cross product of edges
		int i;
		// 12 edges defined by adjacent faces
		int i0[] = { 0,0, 1,1, 2,2, 3,3, 4,4, 5,5 };
		int i1[] = { 2,3, 2,3, 4,5, 4,5, 0,1, 0,1 };
/*
		// 12 edges defined by adjacent vertices
		int pedge0[] = { 0,2, 1,3, 0,4, 2,6, 0,1, 4,5 };
		int pedge1[] = { 4,6, 5,7, 1,5, 3,7, 2,3, 6,7 };
*/
		// select a single vertex representant adjacent each edge (within 0,3,5,6)
		int pedge[] = { 0,6, 5,3, 0,5, 3,6, 0,3, 6,5 };
		// select a vertex from each of the 2 neighboring facets, but away from the edge
		int paway1[] = { 5,0, 0,5, 3,0, 0,3, 3,0, 0,3 };
		int paway2[] = { 6,3, 3,6, 5,6, 6,5, 6,5, 5,6 };

		PCVEC3 corner[8];
		if( ! pc_frustum_get_points(corner,m12) )
		{
			pcerror("%s : failed to get finite frustum corners", __func__);
			return PC_TRUE;
		}

		for( i = 0; i < 12; ++i )
		{
			int j0, j1; //, j2;
			// 12 edges of the transformed frustum (defined by the 6 *plane*s)
			double *n0 = plane[i0[i]];
			double *n1 = plane[i1[i]];
			double *cedge = corner[pedge[i]];
			double *caway1 = corner[paway1[i]];
			double *caway2 = corner[paway2[i]];
			PCVEC3 edge, absedge;
			pc_vector_3_cross(edge,n0,n1); // 3 : last coordinate is not considered here
			pc_vector_3_abs(absedge,edge);

			// 3 canonical directions (0,0,1), (1,0,0) and (0,1,0) for the unit cube
			// axis[j]=axis[j+1]=0, axis[j+2]=1
#if 0
			// reference implementation (before optimization)
			for( j0 = 2, j1 = 0, j2 = 1 ; j1 < 3 ; j0 = j1, ++j1, j2 = (j2+1)%3 )
			{
				int k;
				double tabs = absedge[j0] + absedge[j1];
				PCVEC3 axis;
				axis[j0] = -edge[j1];
				axis[j1] =  edge[j0];
				axis[j2] =  0;

				for( k = 0 ; k < 8 ; ++k )
				{
					double t = pc_vector_3_dot(corner[k],axis);
					if( fabs(t) <= tabs )
						break;
				}
				// if all points where outside [-tabs,tabs], we found a separating axis !
				if( k==8 ) return PC_FALSE;
			}
#elif 0
			// optimization1: no use to construct the axis PCVEC3, inlining the dot product
			for( j0 = 2, j1 = 0 ; j1 < 3 ; j0 = j1, ++j1 )
			{
				int k;
				double tabs = absedge[j0] + absedge[j1];
				for( k = 0 ; k < 8 ; ++k )
				{
					double t = corner[k][j1]*edge[j0] - corner[k][j0]*edge[j1];
					if( fabs(t) <= tabs )
						break;
				}
				// if all points where outside [-tabs,tabs], we found a separating axis !
				if( k==8 ) return PC_FALSE;
			}
#else
			// optimization2: no use to test all vertices, just one on the edge
			// to define the separating plane and another one away from the edge
			// to know on which side of the separating plane the frustum lies
			for( j0 = 2, j1 = 0; j1 < 3; j0 = j1, ++j1 )
			{
				double tedge, taway1, taway2;
				double tabs  = absedge[j0] + absedge[j1];

				tedge = cedge[j1]*edge[j0] - cedge[j0]*edge[j1];
				// if ()-tabs <= tedge <= tabs)
				if( fabs(tedge) <= tabs )
					continue;

				/*
				* now tedge > tabs or tedge < -tabs
				* if (0 <= tabs < tedge < taway) or (taway < tedge < -tabs <= 0)
				* ie [-tabs,tabs] < [tedge,taway) or (taway,tedge] < [-tabs,tabs]
				* we find a separating axis for an adjacent face
				* if both are separating then no intersection (as a frustum is convex)
				*/
				taway1 = caway1[j1]*edge[j0] - caway1[j0]*edge[j1];
				if( tedge*(taway1-tedge) > 0 )
					return PC_FALSE;

				taway2 = caway2[j1]*edge[j0] - caway2[j0]*edge[j1];
				if( tedge*(taway2-tedge) > 0 )
					return PC_FALSE;
			}
#endif
		}
	}
	return PC_TRUE;
}

int
pc_frustum_contains(const PCFRUSTUM *f1, const PCFRUSTUM *f2)
{
	PCMAT44 mat;
	PCVEC4 p;
	double *m, *mw;

	assert(pc_frustum_is_valid(f1) && pc_frustum_is_valid(f2));

	pc_matrix_44_multiply_matrix_44(mat,f2->bwd,f1->fwd);

	mw = mat + 12;
	for( m = mat; m != mw; m += 4 )
	{
		pc_vector_4_add(p, mw, m);
		if ( fabs(p[0]) + fabs(p[1]) + fabs(p[2]) < p[3] )
			return PC_FALSE;
		pc_vector_4_sub(p, mw, m);
		if ( fabs(p[0]) + fabs(p[1]) + fabs(p[2]) < p[3] )
			return PC_FALSE;
	}
	return PC_TRUE;
}


/*
* test whether the frustum is bounded and homogeneous points with strictly
* positive homogeneous coordinates
*/
int
pc_frustum_is_valid(const PCFRUSTUM *f)
{
	const double *m = f->fwd;
	if ( m && f->bwd && ( fabs(m[12]) + fabs(m[13]) + fabs(m[14]) < m[15] ) )
		return PC_TRUE;
	return PC_FALSE;
}

int pc_frustum_copy(PCFRUSTUM *res, const PCFRUSTUM *f)
{
	pc_matrix_44_copy(res->fwd, f->fwd);
	if(!f->bwd)
	{
		if(res->bwd)
		{
			pcfree(res->bwd);
			res->bwd = NULL;
		}
		return PC_SUCCESS;
	}

	if(!res->bwd)
	{
		res->bwd = pcalloc(16*sizeof(double));
		if(!res->bwd)
			return PC_FAILURE;
	}
	pc_matrix_44_copy(res->bwd, f->bwd);
	res->det = f->det;
	return PC_SUCCESS;
}

PCFRUSTUM *pc_frustum_make(PCMAT44 mat)
{
	PCFRUSTUM *f = pcalloc(sizeof(PCFRUSTUM));
	pc_matrix_44_copy(f->fwd, mat);
	/* f->bwd = NULL; */
	return f;
}

void pc_frustum_free(PCFRUSTUM *f)
{
	if(f->bwd)
		pcfree(f->bwd);
	/* f->bwd = NULL; */
	pcfree(f);
}

/*
* Try to make the frustum valid
* Only try to make the frustum valid by negating its matrices
*/
int
pc_frustum_make_valid(PCFRUSTUM *res, const PCFRUSTUM *f)
{
	const double *m = f->fwd;
	if ( !m || ( fabs(m[12]) + fabs(m[13]) + fabs(m[14]) >= fabs(m[15]) ) )
		return PC_FAILURE;

	if(!res->bwd)
	{
		res->bwd = pcalloc(16*sizeof(double));
		if(!res->bwd)
			return PC_FAILURE;
	}

	if( m[15] > 0 )
	{
		pc_matrix_44_copy(res->fwd, m);
		if(f->bwd)
		{
			pc_matrix_44_copy(res->bwd, f->bwd);
			res->det = f->det;
			return PC_SUCCESS;
		}
	}
	else
	{
		pc_matrix_44_multiply(res->fwd, m, -1);
		if(f->bwd)
		{
			pc_matrix_44_copy(res->bwd, f->bwd);
			res->det = f->det;
			return PC_SUCCESS;
		}
	}

	pc_matrix_44_adjugate(res->bwd,res->fwd,&res->det);
	return PC_SUCCESS;
}


/*
* Volume
*/

double pc_box_volume(const PCBOX3 b)
{
	PCVEC3 s = { PCVEC3SUB(b[1],b[0]) };
	return s[0]*s[1]*s[2];
}

double pc_frustum_volume(const PCFRUSTUM *f)
{
	assert(pc_frustum_is_valid(f));
	const double *m = f->fwd;
	double a = m[12], b = m[13], c = m[14], d = m[15];
	double a2 = a*a;
	double b2 = b*b;
	double c2 = c*c;
	double d2 = d*d;
	double n2 = d2    - a2    - b2    - c2;
	double n4 = d2*d2 - a2*a2 - b2*b2 - c2*c2;
	double num = f->det * (2*n4+n2*n2);

	/* compute all (d+-a+-b+-c) values */
	double dma = d-a;
	double dpa = d+a;
	double w[] = { dma-b, dpa-b, dma+b, dpa+b };

	/* compute (w[i] + c) * (w[i] - c) factors */
	w[0] = w[0] * w[0] - c2;
	w[1] = w[1] * w[1] - c2;
	w[2] = w[2] * w[2] - c2;
	w[3] = w[3] * w[3] - c2;

	return fabs(num/(0.375*w[0]*w[1]*w[2]*w[3]));
}
