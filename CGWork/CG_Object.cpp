#include "CG_Object.h"
#include <cmath>

namespace CG
{
	vec4 HomogeneousToEuclidean(vec4 coords)
	{
		return coords / coords.w;
	}

	vec4 Object::mPosition()
	{
		return mTransform * vec4(0, 0, 0, 1);
	}

	vec4 Object::wPosition()
	{
		return wTransform * mTransform * vec4(0, 0, 0, 1);
	}

	void Object::Translate(vec4 amount)
	{
		wTransform = mat4::Translate(amount) * wTransform;
	}

	void Object::LocalTranslate(vec4 amount)
	{
		mTransform = mat4::Translate(amount) * mTransform;
	}

	// translates the object to the origin, rotates, and translates back
	mat4 RotateFromOrigin(vec4 p, mat4 r)
	{
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		return t2 * r * t1;
	}

	void Object::RotateX(double angle)
	{
		vec4 p = wPosition();
		mat4 r = mat4::RotateX(angle);
		wTransform = RotateFromOrigin(p, r) * wTransform;
	}

	void Object::RotateY(double angle)
	{
		vec4 p = wPosition();
		mat4 r = mat4::RotateY(angle);
		wTransform = RotateFromOrigin(p, r) * wTransform;
	}

	void Object::RotateZ(double angle)
	{
		vec4 p = wPosition();
		mat4 r = mat4::RotateZ(angle);
		wTransform = RotateFromOrigin(p, r) * wTransform;
	}

	void Object::LocalRotateX(double angle)
	{
		vec4 p = mPosition();
		mat4 r = mat4::RotateX(angle);
		mTransform = RotateFromOrigin(p, r) * mTransform;
	}
	
	void Object::LocalRotateY(double angle)
	{
		vec4 p = mPosition();
		mat4 r = mat4::RotateY(angle);
		mTransform = RotateFromOrigin(p, r) * mTransform;
	}
	
	void Object::LocalRotateZ(double angle)
	{
		vec4 p = mPosition();
		mat4 r = mat4::RotateZ(angle);
		mTransform = RotateFromOrigin(p, r) * mTransform;
	}

	void Object::Scale(vec4 amount)
	{
		vec4 p = wPosition();
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		wTransform = t2 * mat4::Scale(amount) * t1 * wTransform;
	}

	void Object::LocalScale(vec4 amount)
	{
		vec4 p = mPosition();
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		mTransform = t2 * mat4::Scale(amount) * t1 * mTransform;
	}


	void Camera::LookAt(vec4& eye, vec4& at, vec4& up)
	{
		vec4 n = (eye - at).normalized();
		vec4 u = (vec4::cross(up, n)).normalized();
		vec4 v = (vec4::cross(n, u)).normalized();
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);

		cTransform = mat4::Translate(eye) * mat4(u.x, v.x, n.x, 0,
												 u.y, v.y, n.y, 0,
												 u.z, v.z, n.z, 0,
												 0,   0,   0,   1);
		cInverse = mat4(u, v, n, t) * mat4::Translate(-eye);
	}

	mat4 Camera::Ortho(double left, double right, double bottom, double top, double zNear, double zFar)
	{
		// convert to default camera volume
		mat4 t = mat4::Translate(vec4(-left - right, -bottom - top, zNear + zFar) / 2);
		t = mat4::Scale(vec4(2 / (right - left), 2 / (top - bottom), 2 / (zFar - zNear))) * t;
		return t;
	}

	mat4 Camera::Perspective(double fovY, double aspectRatio, double zNear, double zFar)
	{
		// perspective warp
		mat4 warp = mat4(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, zFar / (zFar - zNear), zNear * zFar / (zFar - zNear),
			0, 0, -1 / zFar, 0
		);

		// calculate new camera volume after warp
		double angle = fovY / 2;
		angle *= DEG_TO_RAD;
		double top = zFar * tan(angle);
		double bottom = -top;
		double right = top * aspectRatio;
		double left = -right;

		// convert to default camera volume
		return Ortho(left, right, bottom, top, 0, zFar) * warp;
	}
}