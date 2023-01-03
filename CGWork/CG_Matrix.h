#ifndef CG_MATRIX
#define CG_MATRIX
#include <wtypes.h>
#include <cmath>

#define DEG_TO_RAD 0.0174532925
#define EPSILON 1e-5

namespace CG
{
	class vec4 {
	public:
		double x;
		double y;
		double z;
		double w;
		vec4(double x = 0, double y = 0, double z = 0, double w = 0) :
			x(x), y(y), z(z), w(w) {}
		double operator[](int i) const;
		double& operator[](int i);
		vec4 operator+(const vec4& v) const;
		vec4& operator+=(const vec4& v);
		vec4 operator*(const double s) const;
		vec4& operator*=(const double s);
		vec4 operator*(const vec4& v) const;
		vec4 operator/(const double s) const;
		vec4 operator/(const vec4& v) const;
		vec4 operator-(const vec4& other) const;
		vec4 operator-() const;
		bool operator==(const vec4& other) const;
		bool operator!=(const vec4& other) const;
		void RoundXY();
		void normalize();
		vec4 normalized() const;
		void ToArray(double arr[4]) const;
		static double dot(const vec4& u, const vec4& v);
		static vec4 cross(const vec4& u, const vec4& v);
		static bool AreParallel(const vec4& u, const vec4& v);
		static vec4 HomogeneousToEuclidean(vec4& coords);
		static vec4 ColorToVec(const COLORREF& color);
	};

	vec4 coordsKey(vec4& coords, double range);

	class vec4Hash
	{
	public:
		size_t operator()(const vec4& v) const
		{
			return (unsigned int)(v.x + v.y + v.z);
		}
	};

	class mat4 {
		vec4 _m[4];
	public:
		mat4(const double d = 1); // diaglonal matrix with m[i][i] = d for every i
		mat4(const double mat[4][4]);
		mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d);
		mat4(double m00, double m01, double m02, double m03,
			double m10, double m11, double m12, double m13,
			double m20, double m21, double m22, double m23,
			double m30, double m31, double m32, double m33);
		vec4 operator[](int i) const;
		vec4& operator[](int i);
		mat4 operator+(const mat4& m) const;
		mat4 operator*(const double s) const;
		mat4 operator*(const mat4& m) const;
		vec4 operator*(const vec4& u) const;
		void To2DArray(double mat[4][4]) const;
		static mat4 Transpose(const mat4& m);
		static mat4 InverseTransform(const mat4& t);
		static mat4 Translate(vec4& amount);
		static mat4 RotateX(double angle);
		static mat4 RotateY(double angle);
		static mat4 RotateZ(double angle);
		static mat4 Scale(vec4& amount);

		bool operator==(const mat4& other) const;
		bool operator!=(const mat4& other) const;
		static mat4 InterpolatedMatrix(mat4& A, mat4& B, double t);
	};

	class Line
	{
	public:
		vec4 p1;
		vec4 p2;
		Line(vec4& p1, vec4& p2);
		vec4 operator[](double t) const;
		bool operator==(const Line& other) const;
		bool operator!=(const Line& other) const;
		bool IntersectionXY(Line& other, double& t, vec4& p) const;
	};

	Line lineKey(Line& l, double range);

	class LineHash
	{
	public:
		size_t operator()(const Line& l) const
		{
			vec4Hash hash;
			return (unsigned int)(hash(l.p1) + hash(l.p2));
		}
	};

	class Plane
	{
		double A, B, C, D;
		vec4 p0;
		vec4 n;
	public:
		Plane(double A = 1, double B = 1, double C = 1, double D = 1);
		Plane(vec4& p, vec4& n);
		Plane(vec4& p, vec4& u, vec4& v);
		double SignedDistance(vec4& p) const;
		bool Intersection(const Line& line, double& t, vec4& p) const; // find intersection point between line and plane, returns point and updates the reference to t
	};
}

#endif