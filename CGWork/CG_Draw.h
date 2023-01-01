#ifndef CG_LINE
#define CG_LINE

#include "CG_Matrix.h"
#include "Light.h"
#include <list>
#include "CG_Buffer.h"

namespace CG
{
	extern double dynamicRange;
	extern ZBuffer zBuffer;

	typedef enum Shading { FLAT, GOURAUD, PHONG } Shading;
	typedef enum RenderMode { WIREFRAME, SOLID} RenderMode;

	class ScanEdge
	{
	public:
		Line projected;
		Edge global;
		vec4 shadingP1;
		vec4 shadingP2;
		ScanEdge(Line camera, Line projected, Edge global) :
			projected(projected), global(global)
		{ };
	};

	COLORREF PngValToColorRef(int png_value);
	int ColorRefToPngVal(COLORREF color);

	void DrawBackground(CRect& r, CDC* pDC);
	void MoveTo(int x, int y, double z);
	void LineTo(CDC* pDC, int endX, int endY, double endZ, const COLORREF& color);
	void ShadowScanConversion(int height, int width, std::list<Line>& edges, ZBuffer& shadowZBuffer);
	// faceCenter and faceNormal should be in global frame
	void DrawScanConversion(
		CDC* pDC,
		int height,
		int width,
		std::list<ScanEdge>& edges,
		const mat4& projectionToGlobalFrame,
		const mat4& cameraToGlobalFrame,
		const mat4& modelToGlobalFrame,
		const mat4& globalToModelFrameTranspose,
		const vec4& faceCenter,
		const vec4& faceNormal,
		int normalFlip,
		const COLORREF& objectColor,
		const LightParams& ambientLight,
		LightParams lightSources[8],
		int cosineFactor,
		int shading);
}

#endif