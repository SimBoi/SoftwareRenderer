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

	}

	void Object::RotateY(double angle)
	{

	}

	void Object::RotateZ(double angle)
	{

	}

	void Object::LocalRotateX(double angle)
	{

	}
	
	void Object::LocalRotateY(double angle)
	{

	}
	
	void Object::LocalRotateZ(double angle)
	{

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