#include "CG_Object.h"

namespace CG
{
	vec4 Object::Center()
	{
		return vec4(mTransform[0][3], mTransform[1][3], mTransform[2][3]);
	}

	void Object::Translate(vec4 amount)
	{
		wTransform = mat4::Translate(amount) * wTransform;
	}

	void Object::LocalTranslate(vec4 amount)
	{
		mTransform = mat4::Translate(amount) * mTransform;
	}

	void Object::RotateX(double angle)
	{
		vec4 p = wTransform * vec4(0, 0, 0, 0);
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 r = mat4::RotateX(angle);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		wTransform = t2 * r * t1 * wTransform;
	}

	void Object::RotateY(double angle)
	{
		vec4 p = wTransform * vec4(0, 0, 0, 0);
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 r = mat4::RotateY(angle);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		wTransform = t2 * r * t1 * wTransform;
	}

	void Object::RotateZ(double angle)
	{
		vec4 p = wTransform * vec4(0, 0, 0, 0);
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 r = mat4::RotateZ(angle);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		wTransform = t2 * r * t1 * wTransform;
	}

	void Object::LocalRotateX(double angle)
	{
		wTransform = wTransform * mat4::RotateX(angle);
	}
	
	void Object::LocalRotateY(double angle)
	{
		wTransform = wTransform * mat4::RotateY(angle);
	}
	
	void Object::LocalRotateZ(double angle)
	{
		wTransform = wTransform * mat4::RotateZ(angle);
	}

	void Object::Scale(vec4 amount)
	{
		wTransform = mat4::Scale(amount) * wTransform;
	}

	void Object::LocalScale(vec4 amount)
	{
		mTransform = mat4::Scale(amount) * mTransform;
	}


	void Camera::LookAt(vec4& eye, vec4& at, vec4& up)
	{
		vec4 n = (eye - at).normalize();
		vec4 u = (vec4::cross(up, n)).normalize();
		vec4 v = (vec4::cross(n, u)).normalize();
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);

		cTransform = mat4::Translate(eye) * mat4(u.x, v.x, n.x, 0,
												 u.y, v.y, n.y, 0,
												 u.z, v.z, n.z, 0,
												 0,   0,   0,   1);
		cInverse = mat4(u, v, n, t) * mat4::Translate(-eye);
	}
}