#ifndef CG_OBJECT
#define CG_OBJECT

#include "CG_Matrix.h"
#include "PngWrapper.h"
#include <list>
//#include <windows.h>

using namespace std;

#define FAR_PLANE 0
#define NEAR_PLANE 1
#define TOP_PLANE 2
#define BOTTOM_PLANE 3
#define RIGHT_PLANE 4
#define LEFT_PLANE 5

namespace CG
{
	typedef enum SpaceType { VIEW, OBJECT } TSpace;
	typedef enum LayoutType { NONE, STRETCH, REPEAT } BackgroundLayout;

	extern int rotation_sensitivity;
	extern int translation_sensitivity;
	extern int scale_sensitivity;

	extern BackgroundLayout BackgroundImageLayout;		// background layout
	extern bool IsBackgroundImageLoaded;
	extern PngWrapper BackgroundImage;		// background image

	extern COLORREF BackgroundColor;		// background color
	extern COLORREF BoundingBoxColor;	// bounding box color
	extern COLORREF VertexNormalColor;	// vertex normals color
	extern COLORREF FaceNormalColor;		// face normals color
	extern COLORREF ModelColor;			// model color
	extern bool bIsModelColor;			// is model color set

	extern double zNear, zFar, fovY;

	void setDefaultSensitivity();
	void setDefaultColors();
	void SetDefaultPerspectiveSettings();

	class Vertex
	{
	public:
		vec4 localPosition;
		vec4 normal;

		Vertex(vec4& pos = vec4(), vec4& normal = vec4());
	};

	class Edge
	{
	public:
		Line line;
		vec4 startNormal;
		vec4 endNormal;
		Edge(Line& line, vec4& startNormal, vec4& endNormal) :
			line(line), startNormal(startNormal), endNormal(endNormal)
		{ };
	};

	class Face
	{
	public:
		std::list<Vertex> vertices;
		vec4 normal;
		vec4 center;
		std::list<Line> edges;
		std::list<Face*> adjacentFaces; // null if there is no adjacent face for an edge
	};

	bool IsConvex(Face& face);

	class Object
	{
	public:
		mat4 mTransform, mInverse;
		mat4 wTransform, wInverse;
		std::list<Face> faces;
		COLORREF color;
		double minX, maxX, minY, maxY, minZ, maxZ; // used for bounding box calculation
		bool hasShape = false; // true if the object has at least one vertex, this value should be manually set
		Face boundingBox[6];
		CG::Object* parent = NULL;
		std::list<Object> children;
		double normalScale;


		void setWTransform(mat4& transform_matrix);
		void setMTransform(mat4& transform_matrix);
		vec4 mPosition() const;
		vec4 wPosition() const;
		void Translate(vec4& amount);
		void LocalTranslate(vec4& amount);
		void RotateX(double angle);
		void RotateY(double angle);
		void RotateZ(double angle);
		void LocalRotateX(double angle);
		void LocalRotateY(double angle);
		void LocalRotateZ(double angle);
		void Scale(vec4& amount);
		void LocalScale(vec4& amount);
		void CalcBoundingBox();
		void ReCalcBoundingBox(const Object& alteredChild); // if a child transforms relative to the parent, the parent should recalculate its bounding box

	private:
		void GenerateBoundingBoxArray();
	};

	class Camera
	{
	public:
		mat4 cTransform, cInverse;
		mat4 projection; // camera frame to default volume
		Plane clipPlanes[6]; // clip planes in camera frame

		void LookAt(vec4& eye, vec4& at, vec4& up);
		void Ortho(double left, double right, double bottom, double top, double zNear, double zFar);
		void Perspective(double fovY, double aspectRatio, double zNear, double zFar);
		static mat4 ToScreenSpace(int width, int height); // default volume XY to screen space XY
		bool IsInsideFrustum(vec4& p) const;
	};
}

#endif