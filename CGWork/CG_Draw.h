#ifndef CG_LINE
#define CG_LINE

#include "CG_Matrix.h"
#include "Light.h"
#include <list>

namespace CG
{
	typedef enum Shading { FLAT, GOURAUD, PHONG } TView;

	class ZBuffer
	{
		int height = 0, width = 0;
		double** arr = NULL;
	public:
		~ZBuffer();
		double* operator[](int index);
		void resize(int height, int width);
		void init();
		void OverridePixel(CDC* pDC, int x, int y, double z, const COLORREF& color); // draws if z >= previous z
		void SetPixel(CDC* pDC, int x, int y, double z, const COLORREF& color); // draws if z > previous z
	private:
		void free();
	};

	extern ZBuffer zBuffer;

	void DrawBackground(CRect& r, CDC* pDC);
	void MoveTo(int x, int y, double z);
	void LineTo(CDC* pDC, int endX, int endY, double endZ, const COLORREF& color);
	void ScanConversion(
		CDC* pDC,
		int height,
		int width,
		const std::list<Edge>& edges,
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
		double ambientIntensity,
		double diffuseIntensity,
		double specularIntensity,
		int cosineFactor,
		int shading);
}

#endif