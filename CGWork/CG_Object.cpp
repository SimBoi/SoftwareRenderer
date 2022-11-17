#include "CG_Object.h"

namespace CG
{
	vec4 Object::Center()
	{
		return vec4(mTransform[0][3], mTransform[1][3], mTransform[2][3]);
	}

	void Camera::LookAt(vec4& eye, vec4& at, vec4& up)
	{
		cTransform = mat4(1);
		//mat4.Translate(cTransform, eye);
		//mat4.Rotate();
	}
}