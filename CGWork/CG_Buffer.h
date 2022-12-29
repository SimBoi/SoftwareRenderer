#ifndef CG_BUFFER
#define CG_BUFFER

#include "stdafx.h"
#include <wtypes.h>

namespace CG
{
	class ZBuffer
	{
		int height = 0, width = 0;
		double** arr = NULL;

	public:
		~ZBuffer()
		{
			free();
		}

		double* operator[](int index)
		{
			return arr[index];
		}

		void resize(int height, int width)
		{
			if (this->height == height && this->width == width) return;
			free();
			this->height = height;
			this->width = width;
			arr = new double* [width];
			for (int i = 0; i < width; i++) arr[i] = new double[height];
		}

		void init()
		{
			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < height; j++)
				{
					arr[i][j] = -1;
				}
			}
		}

		// updates depth for point (x, y)
		bool SetDepth(int x, int y, double z)
		{
			if (x < 0 || x >= width || y < 0 || y >= height || z < arr[x][y]) return false;
			arr[x][y] = z;
			return true;
		}

		// draws if z >= previous z
		void OverridePixel(CDC* pDC, int x, int y, double z, const COLORREF& color)
		{
			SetPixel(pDC, x, y, z + 1e-5, color);
		}

		// draws if z > previous z
		void SetPixel(CDC* pDC, int x, int y, double z, const COLORREF& color)
		{
			if (!SetDepth(x, y, z)) return;
			pDC->SetPixel(x, y, color);
		}

		bool IsVisible(int x, int y, double z)
		{
			if (x < 0 || x >= width || y < 0 || y >= height) return false;
			return z >= arr[x][y] - 1e-4;
		}

	private:
		void free()
		{
			for (int i = 0; i < width; ++i) delete[] arr[i];
			delete[] arr;
		}
	};
}

#endif