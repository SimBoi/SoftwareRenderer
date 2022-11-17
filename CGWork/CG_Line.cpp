#include "stdafx.h"
#include "CG_Line.h"

namespace CG
{
	int prevX;
	int prevY;

	void MoveTo(int x, int y)
	{
		prevX = x;
		prevY = y;
	}

	void DrawLowLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
	{
		const int dx = x2 - x1;
		int dy = y2 - y1;

		const int yi = (dy < 0 ? -1 : 1);
		dy = dy * yi;

		int d = (2 * dy) - dx;
		const int east = 2 * dy;
		const int northeast = 2 * (dy - dx);

		int x = x1, y = y1;
		while (x < x2)
		{
			pDC->SetPixel(x, y, color);
			if (d > 0)
			{
				d += northeast;
				y += yi;
			}
			else
			{
				d += east;
			}
			x++;
		}
	}

	void DrawHighLine(CDC* pDC, int x1, int y1, int x2, int y2, COLORREF color)
	{
		int dx = x2 - x1;
		const int dy = y2 - y1;

		const int xi = (dx < 0 ? -1 : 1);
		dx = dx * xi;

		int d = (2 * dx) - dy;
		const int east = 2 * dx;
		const int southeast = 2 * (dx - dy);

		int x = x1, y = y1;
		while (y < y2)
		{
			pDC->SetPixel(x, y, color);
			if (d > 0)
			{
				d += southeast;
				x += xi;
			}
			else
			{
				d += east;
			}
			y++;
		}
	}


	void LineTo(CDC* pDC, int endX, int endY, COLORREF color)
	{
		if (abs(endY - prevY) < abs(endX - prevX))
		{
			if (prevX > endX)
			{
				DrawLowLine(pDC, endX, endY, prevX, prevY, color);
			}
			else
			{
				DrawLowLine(pDC, prevX, prevY, endX, endY, color);
			}
		}
		else
		{
			if (prevY > endY)
			{
				DrawHighLine(pDC, endX, endY, prevX, prevY, color);
			}
			else
			{
				DrawHighLine(pDC, prevX, prevY, endX, endY, color);
			}
		}

		prevX = endX;
		prevY = endY;
	}
}