#ifndef CG_OBJECT
#define CG_OBJECT

#include "CG_Matrix.h"
#include <list>
#include <windef.h>

namespace CG
{
	class Vertex
	{
	public:
		vec3 globalPosition;
		vec3 normal;
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
		vec3 normal;
	};

	class Object
	{
	public:
		mat4 modelFrame;
		std::list<Face> faces;
		COLORREF color;
	};
}

#endif