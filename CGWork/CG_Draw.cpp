#include "stdafx.h"
#include "CG_Object.h"
#include "CG_Draw.h"

namespace CG
{
	double dynamicRange = 300;
	ZBuffer zBuffer;
	int prevX;
	int prevY;
	double prevZ;

	ZBuffer::~ZBuffer()
	{
		free();
	}

	double* ZBuffer::operator[](int index)
	{
		return arr[index];
	}

	void ZBuffer::resize(int height, int width)
	{
		if (this->height == height && this->width == width) return;
		free();
		this->height = height;
		this->width = width;
		arr = new double* [width];
		for (int i = 0; i < width; i++) arr[i] = new double[height];
	}

	void ZBuffer::init()
	{
		for (int i = 0; i < width; i++)
		{
			for (int j = 0; j < height; j++)
			{
				arr[i][j] = -1;
			}
		}
	}

	void ZBuffer::OverridePixel(CDC* pDC, int x, int y, double z, const COLORREF& color)
	{
		SetPixel(pDC, x, y, z + 1e-5, color);
	}
	
	void ZBuffer::SetPixel(CDC* pDC, int x, int y, double z, const COLORREF& color)
	{
		if (x < 0 || x >= width || y < 0 || y >= height || z < arr[x][y]) return;
		arr[x][y] = z;
		pDC->SetPixel(x, y, color);
	}

	void ZBuffer::free()
	{
		for (int i = 0; i < width; ++i) delete[] arr[i];
		delete[] arr;
	}

	int ColorRefToPngVal(COLORREF color)
	{
		return SET_RGB(GetRValue(color), GetGValue(color), GetBValue(color));
	}

	COLORREF PngValToColorRef(int png_value)
	{
		int channels = BackgroundImage.GetNumChannels();
		if (channels == 1)
		{
			return png_value;
		}
		else if (channels == 3)
		{
			return RGB(GET_R(png_value), GET_G(png_value), GET_B(png_value));
		}
		else if (channels == 4)
		{
			// Alpha compositing

			double old_r = GET_R(png_value) / 255.0;
			double old_g = GET_G(png_value) / 255.0;
			double old_b = GET_B(png_value) / 255.0;
			double old_a = GET_A(png_value) / 255.0;

			double background_r = GetRValue(BackgroundColor) / 255.0;
			double background_g = GetGValue(BackgroundColor) / 255.0;
			double background_b = GetBValue(BackgroundColor) / 255.0;

			double new_r = ((1 - old_a) * background_r) + (old_a * old_r);
			new_r = (new_r < 1.0) ? new_r * 255 : 255;

			double new_g = ((1 - old_a) * background_g) + (old_a * old_g);
			new_g = (new_g < 1.0) ? new_g * 255 : 255;

			double new_b = ((1 - old_a) * background_b) + (old_a * old_b);
			new_b = (new_b < 1.0) ? new_b * 255 : 255;

			return RGB((int)new_r, (int)new_g, (int)new_b);
		}

		return RGB(0, 0, 0);
	}

	void DrawStretchImage(CDC* pDC, int rect_width, int rect_height)
	{
		const int image_height = BackgroundImage.GetHeight();
		const int image_width = BackgroundImage.GetWidth();

		// image device contex (bitmap)
		CDC* imgDC = new CDC();
		imgDC->CreateCompatibleDC(pDC);
		// SetTimer(1, 1, NULL);
		HBITMAP image_bit_map = CreateCompatibleBitmap(pDC->m_hDC, image_width, image_height);
		imgDC->SelectObject(image_bit_map);

		for (unsigned int y = 0; y < image_height; y++)
		{
			for (unsigned int x = 0; x < image_width; x++)
			{
				COLORREF color = PngValToColorRef(BackgroundImage.GetValue(x, y));
				imgDC->SetPixel(x, y, color);
			}
		}

		// copy and stretch imgDC to pDC
		pDC->StretchBlt(0, 0, rect_width, rect_height, imgDC, 0, 0, image_width, image_height, SRCCOPY);
		imgDC->DeleteDC();
		delete imgDC;
	}

	void DrawRepeatImage(CDC* pDC, int rect_width, int rect_height)
	{
		const int image_height = BackgroundImage.GetHeight();
		const int image_width = BackgroundImage.GetWidth();

		for (unsigned int y = 0; y < rect_height; y++)
		{
			for (unsigned int x = 0; x < rect_width; x++)
			{
				COLORREF color = PngValToColorRef(BackgroundImage.GetValue(x % image_width, y % image_height));
				pDC->SetPixel(x, y, color);
			}
		}
	}

	void DrawBackground(CRect& r, CDC* pDC)
	{
		if (IsBackgroundImageLoaded && BackgroundImageLayout == STRETCH)
		{
			DrawStretchImage(pDC, r.Width(), r.Height());
		}
		else if (IsBackgroundImageLoaded && BackgroundImageLayout == REPEAT)
		{
			DrawRepeatImage(pDC, r.Width(), r.Height());
		}
		else
		{
			pDC->FillSolidRect(&r, BackgroundColor);
		}
	}

	
	void MoveTo(int x, int y, double z)
	{
		prevX = x;
		prevY = y;
		prevZ = z;
	}

	void DrawLowLine(CDC* pDC, int x1, int y1, double z1, int x2, int y2, double z2, const COLORREF& color)
	{
		const int dx = x2 - x1;
		int dy = y2 - y1;
		const double dz = (z2 - z1);
		
		const int yi = (dy < 0 ? -1 : 1);
		dy = dy * yi;

		int d = (2 * dy) - dx;
		const int east = 2 * dy;
		const int northeast = 2 * (dy - dx);

		int x = x1, y = y1;
		double z = z1;
		while (x < x2)
		{
			z = (((x - x1) / dx) * dz) + z1;
			zBuffer.OverridePixel(pDC, x, y, z, color);
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

	void DrawHighLine(CDC* pDC, int x1, int y1, double z1, int x2, int y2, double z2, const COLORREF& color)
	{
		int dx = x2 - x1;
		const int dy = y2 - y1;
		const double dz = (z2 - z1);

		const int xi = (dx < 0 ? -1 : 1);
		dx = dx * xi;

		int d = (2 * dx) - dy;
		const int east = 2 * dx;
		const int southeast = 2 * (dx - dy);

		int x = x1, y = y1;
		double z = z1;
		while (y < y2)
		{
			z = (((y - y1) / dy) * dz) + z1;
			zBuffer.OverridePixel(pDC, x, y, z, color);
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

	void LineTo(CDC* pDC, int endX, int endY, double endZ, const COLORREF& color)
	{
		if (abs(endY - prevY) < abs(endX - prevX))
		{
			if (prevX > endX)
			{
				DrawLowLine(pDC, endX, endY, endZ, prevX, prevY, prevZ, color);
			}
			else
			{
				DrawLowLine(pDC, prevX, prevY, prevZ, endX, endY, endZ, color);
			}
		}
		else
		{
			if (prevY > endY)
			{
				DrawHighLine(pDC, endX, endY, endZ, prevX, prevY, prevZ, color);
			}
			else
			{
				DrawHighLine(pDC, prevX, prevY, prevZ, endX, endY, endZ, color);
			}
		}

		MoveTo(endX, endY, endZ);
	}

	struct PointXComparator
	{
		bool operator ()(const vec4& p1, const vec4& p2)
		{
			return p1.x < p2.x;
		}
	};

	// takes arguments in global frame
	void CalcLighting(
		const vec4& cameraPos,
		const vec4& pixelPos,
		vec4 N,
		int normalFlip,
		LightParams lightSources[8],
		int cosineFactor,
		vec4& diffuse,
		vec4& specular)
	{
		for (int i = 0; i < 8; i++)
		{
			if (!lightSources[i].enabled) continue;

			vec4 L, R, V;
			vec4 light = vec4(lightSources[i].colorR, lightSources[i].colorG, lightSources[i].colorB);
			vec4 lightPos = vec4(lightSources[i].posX, lightSources[i].posY, lightSources[i].posZ, 1);
			// calculare light direction in global frame
			if (lightSources[i].type == LIGHT_TYPE_DIRECTIONAL)
				L = vec4(lightSources[i].dirX, lightSources[i].dirY, lightSources[i].dirZ, 0);
			else
				L = pixelPos - lightPos;
			L.normalize();
			N.normalize();
			N *= normalFlip;
			R = (N * (2 * vec4::dot(N, -L)) + L).normalized();
			V = (cameraPos - pixelPos).normalized();

			diffuse += light * max(vec4::dot(L, -N), 0);
			specular += light * pow(max(vec4::dot(R, V), 0), cosineFactor);
		}
	}

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
		int shading)
	{
		vec4 ambient = vec4(ambientLight.colorR, ambientLight.colorG, ambientLight.colorB);
		vec4 diffuse, specular, finalLight, finalColor;

		if (shading == FLAT)
		{
			CalcLighting(
				cameraToGlobalFrame * vec4(0, 0, 0, 1),
				modelToGlobalFrame * faceCenter,
				globalToModelFrameTranspose * faceNormal,
				normalFlip,
				lightSources,
				cosineFactor,
				diffuse,
				specular);

			finalLight = ambient * ambientIntensity + diffuse * diffuseIntensity + specular * specularIntensity;
			// cap final light based on dynnamic range
			finalLight.x = min(dynamicRange, finalLight.x) / dynamicRange;
			finalLight.y = min(dynamicRange, finalLight.y) / dynamicRange;
			finalLight.z = min(dynamicRange, finalLight.z) / dynamicRange;

			finalColor = finalLight * vec4::ColorToVec(objectColor);
		}

		for (int y = 0; y < height; y++)
		{
			// find intersections
			Line scanLine = Line(vec4(0, y, 0, 1), vec4(width, y, 0, 1));
			std::list<vec4> intersections;
			for (auto it = edges.begin(); it != edges.end(); it++)
			{
				// skip edges parallel to the scan line
				if (it->line.p1.y == it->line.p2.y) continue;

				vec4 p;
				double t;
				if (!it->line.IntersectionXY(scanLine, t, p)) continue;
				p.FloorXY();
				if (0 < t && t < 1)
				{
					intersections.push_back(p);
				}
				else if (t == 1)
				{
					const Edge* edge1 = &(*it);
					it++;
					auto next = it != edges.end() ? it : edges.begin();
					while (next->line.p1.y == next->line.p2.y)
					{
						next++;
						if (next == edges.end()) next = edges.begin();
						if (it != edges.end()) it++;
					}
					const Edge* edge2 = &(*next);
					
					if ((edge1->line.p2.y - edge1->line.p1.y) * (edge2->line.p2.y - edge2->line.p1.y) > 0)
					{
						intersections.push_back(edge1->line.p2);
					}
					else
					{
						intersections.push_back(edge1->line.p2);
						intersections.push_back(edge2->line.p1);
					}
					if (it == edges.end()) break;
				}
			}
			intersections.sort(PointXComparator());

			// fill pixels between intersections
			for (auto it = intersections.begin(); it != intersections.end(); it++)
			{
				vec4 p1 = *it;
				it++;
				vec4 p2 = *it;
				int range = p2.x - p1.x;

				for (int t = 0; t <= range; t++)
				{
					double a = range == 0 ? 0 : (double)t / range;
					int x = p1.x + t;
					double z = p1.z * (1 - a) + p2.z * a;

					zBuffer.SetPixel(pDC, x, y, z, RGB(finalColor.x, finalColor.y, finalColor.z));
				}
			}
		}
	}
}