#ifndef CG_MATRIX
#define CG_MATRIX

#define DEG_TO_RAD 0.0174532925;

namespace CG
{
	class vec3 {
	public:
		double x;
		double y;
		double z;
		vec3(double x = 0, double y = 0, double z = 0) :
			x(x), y(y), z(z) {}
		double operator[](int i) const;
		double& operator[](int i);
		vec3 operator+(const vec3& v) const;
		vec3 operator*(const double s) const;
		vec3 operator*(const vec3& v) const;
	};

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
		vec4 operator*(const double s) const;
		vec4 operator/(const double s) const;
		vec4 operator-(const vec4& other) const;
		vec4 operator-() const;
		vec4 normalized() const;
		static double dot(const vec4& u, const vec4& v);
		static vec4 cross(const vec4& u, const vec4& v);
	};

	class mat3 {
		vec3 _m[3];
	public:
		mat3(const double d = 1); // diaglonal matrix with m[i][i] = d for every i
		mat3(const vec3& a, const vec3& b, const vec3& c);
		mat3(double m00, double m01, double m02,
			double m10, double m11, double m12,
			double m20, double m21, double m22);
		vec3 operator[](int i) const;
		vec3& operator[](int i);
		mat3 operator+(const mat3& m) const;
		mat3 operator*(const double s) const;
		mat3 operator*(const mat3& m) const;
	};

	class mat4 {
		vec4 _m[4];
	public:
		mat4(const double d = 1); // diaglonal matrix with m[i][i] = d for every i
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
		static mat4 Translate(vec4& amount);
		static mat4 RotateX(double angle);
		static mat4 RotateY(double angle);
		static mat4 RotateZ(double angle);
		static mat4 Scale(vec4& amount);
	};

	class Line
	{
	public:
		vec4 p0;
		vec4 v;
		Line(vec4& p1, vec4& p2);
		double GetT(vec4& p); // returns, for point p, the equavelant t in the equation p = p0 + t*v
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
		vec4 Intersection(Line line, double& t) const; // find intersection point between line and plane, returns point and updates the reference to t
	};
}

#endif