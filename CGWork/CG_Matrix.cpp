#include "CG_Matrix.h"
#include <cmath>

namespace CG
{
	///////////// vec4

	double vec4::operator[](int i) const
	{
		if (i == 0) return x;
		if (i == 1) return y;
		if (i == 2) return z;
		if (i == 3) return w;
		else throw;
	}

	double& vec4::operator[](int i)
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

	vec4& vec4::operator+=(const vec4& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
		return *this;
	}

	vec4 vec4::operator*(const double s) const
	{
		return vec4(s * x, s * y, s * z, s * w);
	}

	vec4 vec4::operator/(const double s) const
	{
		return *this * (1 / s);
	}

	vec4 vec4::operator-(const vec4& other) const
	{
		return vec4(x - other.x, y - other.y, z - other.z, w - other.w);
	}

	vec4 vec4::operator-() const
	{
		return *this * -1;
	}

	bool vec4::operator==(const vec4& other) const
	{
		return (x == other.x && y == other.y && z == other.z);
	}

	bool vec4::operator!=(const vec4& other) const
	{
		return !(*this == other);
	}

	vec4 vec4::normalized() const
	{
		double length = sqrt(x * x + y * y + z * z);
		return *this / length;
	}

	double vec4::dot(const vec4& u, const vec4& v)
	{
		return u.x * v.x + u.y * v.y + u.z * v.z + u.w * v.w;
	}

	vec4 vec4::cross(const vec4& u, const vec4& v)
	{
		return vec4(u.y * v.z - u.z * v.y, u.z * v.x - u.x * v.z, u.x * v.y - u.y * v.x, 0);
	}

	bool vec4::AreParallel(const vec4& u, const vec4& v)
	{
		vec4 n1 = u.normalized();
		vec4 n2 = v.normalized();
		return (n1 == n2 || n1 == -n2);
	}

	vec4 vec4::HomogeneousToEuclidean(vec4& coords)
	{
		return coords / coords.w;
	}

	///////////// mat4

	mat4::mat4(const double d)
	{
		_m[0][0] = d; _m[1][1] = d; _m[2][2] = d; _m[3][3] = d;
	}

	mat4::mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d)
	{
		_m[0] = a; _m[1] = b; _m[2] = c; _m[3] = d;
	}

	mat4::mat4(double m00, double m01, double m02, double m03,
		double m10, double m11, double m12, double m13,
		double m20, double m21, double m22, double m23,
		double m30, double m31, double m32, double m33)
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

	mat4 mat4::operator*(const double s) const
	{
		return mat4(_m[0] * s, _m[1] * s, _m[2] * s, _m[3] * s);
	}

	mat4 mat4::operator*(const mat4& m) const
	{
		mat4 a(0);
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				for (int k = 0; k < 4; ++k)
					a[i][j] += _m[i][k] * m[k][j];
		return a;
	}

	vec4 mat4::operator*(const vec4& u) const
	{
		vec4 v(0);
		for (int i = 0; i < 4; ++i)
			for (int j = 0; j < 4; ++j)
				v[i] += _m[i][j] * u[j];
		return vec4::HomogeneousToEuclidean(v);
	}

	mat4 mat4::Translate(vec4& amount)
	{
		mat4 t = mat4(1);
		t[0][3] = amount.x;
		t[1][3] = amount.y;
		t[2][3] = amount.z;
		return t;
	}

	mat4 mat4::RotateX(double angle)
	{
		angle *= DEG_TO_RAD;
		return mat4(
			1, 0, 0, 0,
			0, cos(angle), -sin(angle), 0,
			0, sin(angle), cos(angle), 0,
			0, 0, 0, 1);
	}
	
	mat4 mat4::RotateY(double angle)
	{
		angle *= DEG_TO_RAD;
		return mat4(
			cos(angle), 0, sin(angle), 0,
			0, 1, 0, 0,
			-sin(angle), 0, cos(angle), 0,
			0, 0, 0, 1);
	}
	
	mat4 mat4::RotateZ(double angle)
	{
		angle *= DEG_TO_RAD;
		return mat4(
			cos(angle), -sin(angle), 0, 0,
			sin(angle), cos(angle), 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}

	mat4 mat4::Scale(vec4& amount)
	{
		mat4 t;
		t[0][0] = amount.x;
		t[1][1] = amount.y;
		t[2][2] = amount.z;
		t[3][3] = 1;
		return t;
	}

	///////////// Line

	Line::Line(vec4& p1, vec4& p2)
	{
		p0 = p1;
		v = p2 - p1;
		v.w = 0;
	}

	double Line::GetT(vec4& p)
	{
		if (v.x != 0) return (p.x - p0.x) / v.x;
		else if (v.y != 0) return (p.y - p0.y) / v.y;
		else return (p.z - p0.z) / v.z;
	}

	///////////// Plane

	Plane::Plane(double A, double B, double C, double D) : A(A), B(B), C(C), D(D) { }

	Plane::Plane(vec4& p, vec4& n)
	{
		this->n = n.normalized();
		A = this->n.x;
		B = this->n.y;
		C = this->n.z;
		D = -A * p.x - B * p.y - C * p.z;
		p0 = p;
	}

	Plane::Plane(vec4& p, vec4& u, vec4& v)
	{
		n = vec4::cross(u, v).normalized();
		A = n.x;
		B = n.y;
		C = n.z;
		D = -A * p.x - B * p.y - C * p.z;
		p0 = p;
	}

	double Plane::SignedDistance(vec4& p) const
	{
		return vec4::dot(p - p0, n);
	}

	vec4 Plane::Intersection(Line line, double& t) const
	{
		t = -(vec4::dot(n, line.p0) + D) / (vec4::dot(n, line.v));
		return line.p0 + line.v * t;
	}
}