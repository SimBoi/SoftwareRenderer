#ifndef CG_LINE
#define CG_LINE

namespace CG
{
	void MoveTo(int x, int y);
	void LineTo(CDC* pDC, int endX, int endY, COLORREF color = RGB(0, 0, 0));
}

#endif