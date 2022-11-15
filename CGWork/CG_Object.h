#ifndef CG_OBJECT
#define CG_OBJECT

#include "CG_Matrix.h"

namespace CG
{
	class Vertix
	{
	public:
		vec3 globalPosition;
		vec3 normal;
	};

	class Face
	{
	public:
		Vertix* vertices;
		int size;
		vec3 normal;
	};

	class Object
	{
	public:
		Face* faces;
	};
}

#endif