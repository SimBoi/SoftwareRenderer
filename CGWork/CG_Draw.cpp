#include "stdafx.h"
#include "CG_Draw.h"

namespace CG
{
	int prevX;
	int prevY;

	void MoveTo(int x, int y)
	{
		prevX = x;
		prevY = y;
	}

	void DrawLowLine(CDC* pDC, int x1, int y1, int x2, int y2, const COLORREF& color)
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

	void DrawHighLine(CDC* pDC, int x1, int y1, int x2, int y2, const COLORREF& color)
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


	void LineTo(CDC* pDC, int endX, int endY, const COLORREF& color)
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

	struct PointXComparator
	{
		bool operator ()(const vec4& p1, const vec4& p2)
		{
			return p1.x < p2.x;
		}
	};

	void ScanConversion(CDC* pDC, int height, int width, const std::list<Line>& edges, const COLORREF& color)
	{
		for (int y = 0; y < height; y++)
		{
			// find intersections
			Line scanLine = Line(vec4(0, y, 0, 1), vec4(width, y, 0, 1));
			std::list<vec4> intersections;
			for (auto it = edges.begin(); it != edges.end(); it++)
			{
				vec4 p;
				double t;
				if (!(*it).IntersectionXY(scanLine, t, p)) continue;
				p.Floor();
				if (0 < t && t < 1)
				{
					intersections.push_back(p);
				}
				else if (t == 1)
				{
					const Line* edge1 = &(*it);
					it++;
					const Line* edge2 = it != edges.end() ? &(*it) : &(*edges.begin());
					if ((edge1->p2.y - edge1->p1.y) * (edge2->p2.y - edge2->p1.y) > 0) intersections.push_back((*edge1)[1]);
					if (it == edges.end()) break;
				}
			}
			intersections.sort(PointXComparator());

			// fill pixels between intersections
			auto it = intersections.begin();
			int sign = -1;
			for (int x = 0; x < width; x++)
			{
				while (it != intersections.end() && x == (int)((*it).x))
				{
					sign *= -1;
					it++;
				}
				if (sign == 1) pDC->SetPixel(x, y, color);

				if (sign == 1 && x > 1400)
				{
					sign = 1;
				}
			}
		}
	}
}