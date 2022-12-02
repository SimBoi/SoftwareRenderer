#ifndef CG_LINE
#define CG_LINE

#include "CG_Matrix.h"
#include <list>

namespace CG
{
	void MoveTo(int x, int y);
	void LineTo(CDC* pDC, int endX, int endY, const COLORREF& color);
	void ScanConversion(CDC* pDC, int height, int width, const std::list<Line>& edges, const COLORREF& color);
}

#endif