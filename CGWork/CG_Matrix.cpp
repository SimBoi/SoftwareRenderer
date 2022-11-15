#include "CG_Matrix.h"

using namespace CG;

///////////// vec3

float vec3::operator[](int i) const
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	else throw;
}

float& vec3::operator[](int i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	else throw;
}

vec3 vec3::operator+(const vec3& v) const
{
	return vec3(x + v.x, y + v.y, z + v.z);
}

vec3 vec3::operator*(const float s) const
{
	return vec3(s * x, s * y, s * z);
}

vec3 vec3::operator*(const vec3& v) const
{
	return vec3(x * v.x, y * v.y, z * v.z);
}

///////////// vec4

float vec4::operator[](int i) const
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	if (i == 3) return w;
	else throw;
}

float& vec4::operator[](int i)
{
	if (i == 0) return x;
	if (i == 1) return y;
	if (i == 2) return z;
	if (i == 3) return w;
	else throw;
}

vec4 vec4::operator+(const vec4& v) const
{
	return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}

vec4 vec4::operator*(const float s) const
{
	return vec4(s * x, s * y, s * z, s * w);
}

vec4 vec4::operator*(const vec4& v) const
{
	return vec4(x * v.x, y * v.y, z * v.z, w * v.w);
}

///////////// mat3

mat3::mat3(const float d)
{
	_m[0].x = d; _m[1].y = d; _m[2].z = d;
}

mat3::mat3(const vec3& a, const vec3& b, const vec3& c)
{
	_m[0] = a; _m[1] = b; _m[2] = c;
}

mat3::mat3(float m00, float m01, float m02,
	float m10, float m11, float m12,
	float m20, float m21, float m22)
{
	_m[0] = vec3(m00, m01, m02);
	_m[1] = vec3(m10, m11, m12);
	_m[2] = vec3(m20, m21, m22);
}

vec3 mat3::operator[](int i) const
{
	return _m[i];
}

vec3& mat3::operator[](int i)
{
	return _m[i];
}

mat3 mat3::operator+(const mat3& m) const
{
	return mat3(_m[0] + m[0], _m[1] + m[1], _m[2] + m[2]);
}

mat3 mat3::operator*(const float s) const
{
	return mat3(_m[0] * s, _m[1] * s, _m[2] * s);
}

mat3 mat3::operator*(const mat3& m) const
{
	mat3 a(0.0);
	for (int i = 0; i < 3; ++i)
		for (int j = 0; j < 3; ++j)
			for (int k = 0; k < 3; ++k)
				a[i][j] += _m[i][k] * m[k][j];
	return a;
}

///////////// mat4

mat4::mat4(const float d)
{
	_m[0].x = d; _m[1].y = d; _m[2].z = d; _m[3].w = d;
}

mat4::mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d)
{
	_m[0] = a; _m[1] = b; _m[2] = c; _m[3] = d;
}

mat4::mat4(float m00, float m01, float m02, float m03,
	float m10, float m11, float m12, float m13,
	float m20, float m21, float m22, float m23,
	float m30, float m31, float m32, float m33)
{
	_m[0] = vec4(m00, m01, m02, m03);
	_m[1] = vec4(m10, m11, m12, m13);
	_m[2] = vec4(m20, m21, m22, m23);
	_m[3] = vec4(m30, m31, m32, m33);
}

vec4 mat4::operator[](int i) const
{
	return _m[i];
}

vec4& mat4::operator[](int i)
{
	return _m[i];
}

mat4 mat4::operator+(const mat4& m) const
{
	return mat4(_m[0] + m[0], _m[1] + m[1], _m[2] + m[2], _m[3] + m[3]);
}

mat4 mat4::operator*(const float s) const
{
	return mat4(_m[0] * s, _m[1] * s, _m[2] * s, _m[3] * s);
}

mat4 mat4::operator*(const mat4& m) const
{
	mat4 a(0.0);
	for (int i = 0; i < 4; ++i)
		for (int j = 0; j < 4; ++j)
			for (int k = 0; k < 4; ++k)
				a[i][j] += _m[i][k] * m[k][j];
	return a;
}