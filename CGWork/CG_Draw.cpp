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

	struct ScanIntersection
	{
		vec4 projected;
		vec4 global;
		vec4 interpolated; // holds interpolated normals for phong shading, interpolared lighting for gouraud shading
	};

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
	vec4 CalcLighting(
		const vec4& cameraPos,
		const vec4& pixelPos,
		vec4 N,
		int normalFlip,
		LightParams lightSources[8],
		int cosineFactor,
		vec4& ambient,
		double ambientIntensity)
	{
		vec4 diffuse, specular;

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

			diffuse += light * max(vec4::dot(L, -N), 0) * lightSources[i].diffuseIntensity;
			specular += light * pow(max(vec4::dot(R, V), 0), cosineFactor) * lightSources[i].specularIntensity;
		}

		vec4 finalLight = ambient * ambientIntensity + diffuse + specular;
		// cap final light based on dynnamic range
		finalLight.x = min(dynamicRange, finalLight.x) / dynamicRange;
		finalLight.y = min(dynamicRange, finalLight.y) / dynamicRange;
		finalLight.z = min(dynamicRange, finalLight.z) / dynamicRange;

		return finalLight;
	}

	void ScanConversion(
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
		const COLORREF& objectColorref,
		const LightParams& ambientLight,
		LightParams lightSources[8],
		double ambientIntensity,
		int cosineFactor,
		int shading)
	{
		vec4 ambient = vec4(ambientLight.colorR, ambientLight.colorG, ambientLight.colorB);
		vec4 objectColor = vec4::ColorToVec(objectColorref);
		vec4 finalColor;
		vec4 cameraPos = cameraToGlobalFrame * vec4(0, 0, 0, 1);

		// calculate lighting once for the whole polygon in flat shading
		if (shading == FLAT)
		{
			vec4 finalLight = CalcLighting(
				cameraPos,
				faceCenter,
				faceNormal,
				normalFlip,
				lightSources,
				cosineFactor,
				ambient,
				ambientIntensity
			);
			finalColor = finalLight * objectColor;
		}
		// calculate lighting only on vertices in gouraud shading
		else if (shading == GOURAUD)
		{
			auto next = edges.begin();
			next++;
			for (auto curr = edges.begin(); curr != edges.end(); curr++)
			{
				vec4 vertexLight = CalcLighting(
					cameraPos,
					curr->global.line.p2,
					curr->global.endNormal,
					normalFlip,
					lightSources,
					cosineFactor,
					ambient,
					ambientIntensity
				);
				curr->shadingP2 = vertexLight;
				next->shadingP1 = vertexLight;

				next++;
				if (next == edges.end()) next = edges.begin();
			}
		}

		// calculate the y range
		int minY = height, maxY = 0;
		for (auto edge : edges)
		{
			if (minY > edge.projected.p1.y) minY = edge.projected.p1.y;
			if (minY > edge.projected.p2.y) minY = edge.projected.p2.y;
			if (maxY < edge.projected.p1.y) maxY = edge.projected.p1.y;
			if (maxY < edge.projected.p2.y) maxY = edge.projected.p2.y;
		}
		int yRange = maxY - minY + 1;

		// intersection points for each scan line
		ScanIntersection** intersections = new ScanIntersection * [yRange];
		for (int i = 0; i < yRange; i++)
		{
			intersections[i] = new ScanIntersection[2];
		}

		// scan lines
		for (int y = minY; y <= maxY; y++)
		{
			// intersections index
			int yIndex = y - minY;

			// find intersections
			Line scanLine = Line(vec4(0, y, 0, 1), vec4(width, y, 0, 1));
			int i = 0; // current intersection point, max 2 intersections per scan line (convex polygon)
			for (auto it = edges.begin(); it != edges.end() && i < 2; it++)
			{
				vec4 p;
				double t;
				if (!it->projected.IntersectionXY(scanLine, t, p)) continue;
				p.FloorXY();
				if (0 < t && t < 1)
				{
					if (shading == PHONG) intersections[yIndex][i].interpolated = it->global.startNormal * (1 - t) + it->global.endNormal * t;
					else if (shading == GOURAUD) intersections[yIndex][i].interpolated = it->shadingP1 * (1 - t) + it->shadingP2 * t;
					intersections[yIndex][i].projected = p;
					intersections[yIndex][i++].global = it->global.line[t];
				}
				else if (t == 1)
				{
					const ScanEdge* edge1 = &(*it);
					it++;
					auto next = it != edges.end() ? it : edges.begin();
					while (next->projected.p1.y == next->projected.p2.y)
					{
						next++;
						if (next == edges.end()) next = edges.begin();
						if (it != edges.end()) it++;
					}
					const ScanEdge* edge2 = &(*next);

					if ((edge1->projected.p2.y - edge1->projected.p1.y) * (edge2->projected.p2.y - edge2->projected.p1.y) > 0)
					{
						if (shading == PHONG) intersections[yIndex][i].interpolated = edge1->global.endNormal;
						else if (shading == GOURAUD) intersections[yIndex][i].interpolated = edge1->shadingP2;
						intersections[yIndex][i].projected = edge1->projected.p2;
						intersections[yIndex][i++].global = edge1->global.line.p2;
					}
					else
					{
						if (shading == PHONG) intersections[yIndex][i].interpolated = edge1->global.endNormal;
						else if (shading == GOURAUD) intersections[yIndex][i].interpolated = edge2->shadingP2;
						intersections[yIndex][i].projected = edge1->projected.p2;
						intersections[yIndex][i++].global = edge1->global.line.p2;
						if (shading == PHONG) intersections[yIndex][i].interpolated = edge2->global.startNormal;
						else if (shading == GOURAUD) intersections[yIndex][i].interpolated = edge2->shadingP1;
						intersections[yIndex][i].projected = edge2->projected.p1;
						intersections[yIndex][i++].global = edge2->global.line.p1;
					}
					if (it == edges.end()) break;
				}
			}

			// sort intersections by x
			if (intersections[yIndex][0].projected.x > intersections[yIndex][1].projected.x)
			{
				ScanIntersection tmp = intersections[yIndex][0];
				intersections[yIndex][0] = intersections[yIndex][1];
				intersections[yIndex][1] = tmp;
			}

			// fill pixels between intersections
			int xRange = intersections[yIndex][1].projected.x - intersections[yIndex][0].projected.x;
			for (int t = 0; t <= xRange; t++)
			{
				double a = xRange == 0 ? 0 : (double)t / xRange;
				int x = intersections[yIndex][0].projected.x + t;
				double z = intersections[yIndex][0].projected.z * (1 - a) + intersections[yIndex][1].projected.z * a;

				if (shading == PHONG)
				{
					vec4 pixelPos = intersections[yIndex][0].global * (1 - a) + intersections[yIndex][1].global * a;
					vec4 N = intersections[yIndex][0].interpolated * (1 - a) + intersections[yIndex][1].interpolated * a;
					vec4 finalLight = CalcLighting(
						cameraPos,
						pixelPos,
						N,
						normalFlip,
						lightSources,
						cosineFactor,
						ambient,
						ambientIntensity
					);
					finalColor = finalLight * objectColor;
				}
				else if (shading == GOURAUD)
				{
					vec4 finalLight = intersections[yIndex][0].interpolated * (1 - a) + intersections[yIndex][1].interpolated * a;
					finalColor = finalLight * objectColor;
				}
				zBuffer.SetPixel(pDC, x, y, z, RGB(finalColor.x, finalColor.y, finalColor.z));
			}
		}

		// cleanup
		for (int i = 0; i < yRange; i++)
		{
			delete[] intersections[i];
		}
		delete[] intersections;
	}
}