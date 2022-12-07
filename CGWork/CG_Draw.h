#ifndef CG_LINE
#define CG_LINE

#include "CG_Matrix.h"
#include <list>

namespace CG
{
	class ZBuffer
	{
		int height = 0, width = 0;
		double** arr = NULL;
	public:
		~ZBuffer();
		double* operator[](int index);
		void resize(int height, int width);
		void init();
		void SetPixel(CDC* pDC, int x, int y, double z, const COLORREF& color);
	private:
		void free();
	};

	extern ZBuffer zBuffer;

	void MoveTo(int x, int y, double z);
	void LineTo(CDC* pDC, int endX, int endY, double endZ, const COLORREF& color);
	void ScanConversion(CDC* pDC, int height, int width, const std::list<Line>& edges, const COLORREF& color);
}

#endif