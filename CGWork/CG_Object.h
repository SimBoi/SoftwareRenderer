#ifndef CG_OBJECT
#define CG_OBJECT

#include "CG_Matrix.h"
#include <list>
#include <windows.h>

namespace CG
{
	class Vertex
	{
	public:
		vec4 localPosition;
		vec4 normal;
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
		vec4 normal;
	};

	class Object
	{
	public:
		mat4 mTransform;
		std::list<Face> faces;
		COLORREF color;

		vec4 Center();
	};

	class Camera
	{
	public:
		mat4 cTransform;
		mat4 projection;

		void LookAt(vec4& eye, vec4& at, vec4& up);
		//void Ortho(…);
		//void Perspective(…)
	};
}

#endif