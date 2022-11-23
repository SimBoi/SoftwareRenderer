#ifndef CG_OBJECT
#define CG_OBJECT

#include "CG_Matrix.h"
#include <list>
#include <windows.h>

namespace CG
{
	vec4 HomogeneousToEuclidean(vec4 coords);

	class Vertex
	{
	public:
		vec4 localPosition;
		vec4 normal;

		Vertex(vec4 pos = vec4(), vec4 normal = vec4());
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
		mat4 wTransform;
		std::list<Face> faces;
		COLORREF color;
		double minX, maxX, minY, maxY, minZ, maxZ; // used for bounding box calculation
		bool hasShape = false; // true if the object has at least one vertex, this value should be manually set
		Face boundingBox[6];
		CG::Object* parent = NULL;
		std::list<Object> children;
		// mouse sensitivity
		int translation_sensitivity, rotation_sensitivity, scale_sensitivity;

		vec4 mPosition();
		vec4 wPosition();
		void Translate(vec4 amount);
		void LocalTranslate(vec4 amount);
		void RotateX(double angle);
		void RotateY(double angle);
		void RotateZ(double angle);
		void LocalRotateX(double angle);
		void LocalRotateY(double angle);
		void LocalRotateZ(double angle);
		void Scale(vec4 amount);
		void LocalScale(vec4 amount);
		void CalcBoundingBox();
		void ReCalcBoundingBox(const Object& alteredChild); // if a child transforms relative to the parent, the parent should recalculate its bounding box
		void setDefaultSensitivity();

	private:
		void GenerateBoundingBoxArray();
	};

	class Camera
	{
	public:
		mat4 cTransform;
		mat4 cInverse;
		mat4 projection;

		void LookAt(vec4& eye, vec4& at, vec4& up);
		static mat4 Ortho(double left, double right, double bottom, double top, double zNear, double zFar);
		static mat4 Perspective(double fovY, double aspectRatio, double zNear, double zFar);
		static mat4 ToScreenSpace(double width, double height);
	};
}

#endif