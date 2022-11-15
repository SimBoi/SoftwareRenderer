#ifndef CG_MATRIX
#define CG_MATRIX

namespace CG
{
	class vec3 {
	public:
		float x;
		float y;
		float z;
		vec3(float x = 0, float y = 0, float z = 0) :
			x(x), y(y), z(z) {}
		float operator[](int i) const;
		float& operator[](int i);
		vec3 operator+(const vec3& v) const;
		vec3 operator*(const float s) const;
		vec3 operator*(const vec3& v) const;
	};

	class vec4 {
	public:
		float x;
		float y;
		float z;
		float w;
		vec4(float x = 0, float y = 0, float z = 0, float w = 0) :
			x(x), y(y), z(z), w(w) {}
		float operator[](int i) const;
		float& operator[](int i);
		vec4 operator+(const vec4& v) const;
		vec4 operator*(const float s) const;
		vec4 operator*(const vec4& v) const;
	};

	class mat3 {
		vec3 _m[3];
	public:
		mat3(const float d = 1); // diaglonal matrix with m[i][i] = d for every i
		mat3(const vec3& a, const vec3& b, const vec3& c);
		mat3(float m00, float m01, float m02,
			float m10, float m11, float m12,
			float m20, float m21, float m22);
		vec3 operator[](int i) const;
		vec3& operator[](int i);
		mat3 operator+(const mat3& m) const;
		mat3 operator*(const float s) const;
		mat3 operator*(const mat3& m) const;
	};

	class mat4 {
		vec4 _m[4];
	public:
		mat4(const float d = 1); // diaglonal matrix with m[i][i] = d for every i
		mat4(const vec4& a, const vec4& b, const vec4& c, const vec4& d);
		mat4(float m00, float m01, float m02, float m03,
			float m10, float m11, float m12, float m13,
			float m20, float m21, float m22, float m23,
			float m30, float m31, float m32, float m33);
		vec4 operator[](int i) const;
		vec4& operator[](int i);
		mat4 operator+(const mat4& m) const;
		mat4 operator*(const float s) const;
		mat4 operator*(const mat4& m) const;
	};
}

#endif