#include "CG_Object.h"
#include <cmath>

namespace CG
{
	int rotation_sensitivity;
	int translation_sensitivity;
	int scale_sensitivity;

	void setDefaultSensitivity()
	{
		// need to calc in an intuitive way
		translation_sensitivity = 25;
		rotation_sensitivity = 25;
		scale_sensitivity = 25;
	}

	COLORREF BackgroundColor;		// background color
	COLORREF BoundingBoxColor;	// bounding box color
	COLORREF VertexNormalColor;	// vertex normals color
	COLORREF FaceNormalColor;		// face normals color
	COLORREF ModelColor;			// model color
	bool bIsModelColor;			// is model color set
	TView ViewProjection;		// ORTHOGRAPHIC / PERSPECTIVE
	double zNear, zFar, fovY;

	void setDefaultColors()
	{
		BackgroundColor = RGB(0, 0, 0);
		BoundingBoxColor = RGB(255, 0, 0);
		VertexNormalColor = RGB(255, 0, 255);
		FaceNormalColor = RGB(255, 0, 255);
		ModelColor = RGB(255, 255, 255);
		bIsModelColor = false;
	}

	void SetDefaultPerspectiveSettings()
	{
		zNear = 100;
		zFar = 1000;
		fovY = 90;
	}

	Vertex::Vertex(vec4& pos, vec4& normal)
	{
		localPosition = pos;
		this->normal = normal;
	}

	vec4 Object::mPosition() const
	{
		return mTransform * vec4(0, 0, 0, 1);
	}

	vec4 Object::wPosition() const
	{
		return wTransform * mTransform * vec4(0, 0, 0, 1);
	}

	void Object::Translate(vec4& amount)
	{
		wTransform = mat4::Translate(amount) * wTransform;
		wInverse = mat4::InverseTransform(wTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	void Object::LocalTranslate(vec4& amount)
	{
		mTransform = mat4::Translate(amount) * mTransform;
		mInverse = mat4::InverseTransform(mTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	// translates the object to the origin, rotates, and translates back
	mat4 RotateFromOrigin(vec4& p, mat4& r)
	{
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		return t2 * r * t1;
	}

	void Object::RotateX(double angle)
	{
		vec4 p = wPosition();
		mat4 r = mat4::RotateX(angle);
		wTransform = RotateFromOrigin(p, r) * wTransform;
		wInverse = mat4::InverseTransform(wTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	void Object::RotateY(double angle)
	{
		vec4 p = wPosition();
		mat4 r = mat4::RotateY(angle);
		wTransform = RotateFromOrigin(p, r) * wTransform;
		wInverse = mat4::InverseTransform(wTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	void Object::RotateZ(double angle)
	{
		vec4 p = wPosition();
		mat4 r = mat4::RotateZ(angle);
		wTransform = RotateFromOrigin(p, r) * wTransform;
		wInverse = mat4::InverseTransform(wTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	void Object::LocalRotateX(double angle)
	{
		vec4 p = mPosition();
		mat4 r = mat4::RotateX(angle);
		mTransform = RotateFromOrigin(p, r) * mTransform;
		mInverse = mat4::InverseTransform(mTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}
	
	void Object::LocalRotateY(double angle)
	{
		vec4 p = mPosition();
		mat4 r = mat4::RotateY(angle);
		mTransform = RotateFromOrigin(p, r) * mTransform;
		mInverse = mat4::InverseTransform(mTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}
	
	void Object::LocalRotateZ(double angle)
	{
		vec4 p = mPosition();
		mat4 r = mat4::RotateZ(angle);
		mTransform = RotateFromOrigin(p, r) * mTransform;
		mInverse = mat4::InverseTransform(mTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	void Object::Scale(vec4& amount)
	{
		vec4 p = wPosition();
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		wTransform = t2 * mat4::Scale(amount) * t1 * wTransform;
		wInverse = mat4::InverseTransform(wTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	void Object::LocalScale(vec4& amount)
	{
		vec4 p = mPosition();
		mat4 t1 = mat4(
			1, 0, 0, -p.x,
			0, 1, 0, -p.y,
			0, 0, 1, -p.z,
			0, 0, 0, 1
		);
		mat4 t2 = mat4(
			1, 0, 0, p.x,
			0, 1, 0, p.y,
			0, 0, 1, p.z,
			0, 0, 0, 1
		);
		mTransform = t2 * mat4::Scale(amount) * t1 * mTransform;
		mInverse = mat4::InverseTransform(mTransform);
		if (parent != NULL) parent->ReCalcBoundingBox(*this);
	}

	void Object::CalcBoundingBox()
	{
		bool firstVertex = true;

		// object vertices
		for (auto const& face : faces)
		{
			for (auto const& vertex : face.vertices)
			{
				if (firstVertex)
				{
					minX = vertex.localPosition.x;
					maxX = vertex.localPosition.x;
					minY = vertex.localPosition.y;
					maxY = vertex.localPosition.y;
					minZ = vertex.localPosition.z;
					maxZ = vertex.localPosition.z;
					firstVertex = false;
				}

				minX = vertex.localPosition.x < minX ? vertex.localPosition.x : minX;
				maxX = vertex.localPosition.x > maxX ? vertex.localPosition.x : maxX;
				minY = vertex.localPosition.y < minY ? vertex.localPosition.y : minY;
				maxY = vertex.localPosition.y > maxY ? vertex.localPosition.y : maxY;
				minZ = vertex.localPosition.z < minZ ? vertex.localPosition.z : minZ;
				maxZ = vertex.localPosition.z > maxZ ? vertex.localPosition.z : maxZ;
			}
		}

		// children vertices
		for (auto const& child : children)
		{
			mat4 childTransform = child.wTransform * child.mTransform;
			for (auto const& face : child.faces)
			{
				for (auto const& vertex : face.vertices)
				{
					vec4 coords = childTransform * vertex.localPosition;

					if (firstVertex)
					{
						minX = coords.x;
						maxX = coords.x;
						minY = coords.y;
						maxY = coords.y;
						minZ = coords.z;
						maxZ = coords.z;
						firstVertex = false;
					}

					minX = coords.x < minX ? coords.x : minX;
					maxX = coords.x > maxX ? coords.x : maxX;
					minY = coords.y < minY ? coords.y : minY;
					maxY = coords.y > maxY ? coords.y : maxY;
					minZ = coords.z < minZ ? coords.z : minZ;
					maxZ = coords.z > maxZ ? coords.z : maxZ;
				}
			}
		}

		GenerateBoundingBoxArray();
	}

	void Object::ReCalcBoundingBox(const Object& alteredChild)
	{
		if (!hasShape && children.size() <= 1) CalcBoundingBox();

		mat4 childTransform = alteredChild.wTransform * alteredChild.mTransform;
		for (auto const& face : alteredChild.faces)
		{
			for (auto const& vertex : face.vertices)
			{
				vec4 coords = childTransform * vertex.localPosition;

				minX = coords.x < minX ? coords.x : minX;
				maxX = coords.x > maxX ? coords.x : maxX;
				minY = coords.y < minY ? coords.y : minY;
				maxY = coords.y > maxY ? coords.y : maxY;
				minZ = coords.z < minZ ? coords.z : minZ;
				maxZ = coords.z > maxZ ? coords.z : maxZ;
			}
		}

		GenerateBoundingBoxArray();
	}

	void Object::GenerateBoundingBoxArray()
	{
		boundingBox[0].vertices.clear();
		boundingBox[1].vertices.clear();
		boundingBox[2].vertices.clear();
		boundingBox[3].vertices.clear();
		boundingBox[4].vertices.clear();
		boundingBox[5].vertices.clear();

		boundingBox[0].vertices.push_back(Vertex(vec4(minX, minY, minZ, 1)));
		boundingBox[0].vertices.push_back(Vertex(vec4(minX, maxY, minZ, 1)));
		boundingBox[0].vertices.push_back(Vertex(vec4(maxX, maxY, minZ, 1)));
		boundingBox[0].vertices.push_back(Vertex(vec4(maxX, minY, minZ, 1)));

		boundingBox[1].vertices.push_back(Vertex(vec4(minX, minY, maxZ, 1)));
		boundingBox[1].vertices.push_back(Vertex(vec4(minX, maxY, maxZ, 1)));
		boundingBox[1].vertices.push_back(Vertex(vec4(maxX, maxY, maxZ, 1)));
		boundingBox[1].vertices.push_back(Vertex(vec4(maxX, minY, maxZ, 1)));

		boundingBox[2].vertices.push_back(Vertex(vec4(minX, minY, minZ, 1)));
		boundingBox[2].vertices.push_back(Vertex(vec4(minX, minY, maxZ, 1)));
		boundingBox[2].vertices.push_back(Vertex(vec4(maxX, minY, maxZ, 1)));
		boundingBox[2].vertices.push_back(Vertex(vec4(maxX, minY, minZ, 1)));

		boundingBox[3].vertices.push_back(Vertex(vec4(minX, maxY, minZ, 1)));
		boundingBox[3].vertices.push_back(Vertex(vec4(minX, maxY, maxZ, 1)));
		boundingBox[3].vertices.push_back(Vertex(vec4(maxX, maxY, maxZ, 1)));
		boundingBox[3].vertices.push_back(Vertex(vec4(maxX, maxY, minZ, 1)));

		boundingBox[4].vertices.push_back(Vertex(vec4(minX, minY, minZ, 1)));
		boundingBox[4].vertices.push_back(Vertex(vec4(minX, minY, maxZ, 1)));
		boundingBox[4].vertices.push_back(Vertex(vec4(minX, maxY, maxZ, 1)));
		boundingBox[4].vertices.push_back(Vertex(vec4(minX, maxY, minZ, 1)));

		boundingBox[5].vertices.push_back(Vertex(vec4(maxX, minY, minZ, 1)));
		boundingBox[5].vertices.push_back(Vertex(vec4(maxX, minY, maxZ, 1)));
		boundingBox[5].vertices.push_back(Vertex(vec4(maxX, maxY, maxZ, 1)));
		boundingBox[5].vertices.push_back(Vertex(vec4(maxX, maxY, minZ, 1)));
	}

	void Camera::LookAt(vec4& eye, vec4& at, vec4& up)
	{
		up.w = 0;
		vec4 n = (eye - at).normalized();
		vec4 u = (vec4::cross(up, n)).normalized();
		vec4 v = (vec4::cross(n, u)).normalized();
		vec4 t = vec4(0.0, 0.0, 0.0, 1.0);

		cTransform = mat4::Translate(eye) * mat4(u.x, v.x, n.x, 0,
												 u.y, v.y, n.y, 0,
												 u.z, v.z, n.z, 0,
												 0,   0,   0,   1);
		cInverse = mat4(u, v, n, t) * mat4::Translate(-eye);
	}

	void Camera::Ortho(double left, double right, double bottom, double top, double zNear, double zFar)
	{
		clipPlanes[FAR_PLANE] = Plane(vec4(0, 0, -zFar), vec4(0, 0, 1));
		clipPlanes[NEAR_PLANE] = Plane(vec4(0, 0, -zNear), vec4(0, 0, -1));
		clipPlanes[TOP_PLANE] = Plane(vec4(0, top, 0), vec4(0, -1, 0));
		clipPlanes[BOTTOM_PLANE] = Plane(vec4(0, bottom, 0), vec4(0, 1, 0));
		clipPlanes[RIGHT_PLANE] = Plane(vec4(right, 0, 0), vec4(-1, 0, 0));
		clipPlanes[LEFT_PLANE] = Plane(vec4(left, 0, 0), vec4(1, 0, 0));

		// convert to default camera volume
		projection = mat4::Translate(vec4(-left - right, -bottom - top, zNear + zFar) / 2);
		projection = mat4::Scale(vec4(2 / (right - left), 2 / (top - bottom), 2 / (zFar - zNear))) * projection;
	}

	void Camera::Perspective(double fovY, double aspectRatio, double zNear, double zFar)
	{
		double angle = fovY / 2;
		angle *= DEG_TO_RAD;
		
		double yMax = zFar * tan(angle);
		double xMax = aspectRatio * yMax;

		clipPlanes[FAR_PLANE] = Plane(vec4(0, 0, -zFar), vec4(0, 0, 1));
		clipPlanes[NEAR_PLANE] = Plane(vec4(0, 0, -zNear), vec4(0, 0, -1));
		clipPlanes[TOP_PLANE] = Plane(vec4(0, 0, 0), vec4(-xMax, yMax, -zFar), vec4(xMax, yMax, -zFar));
		clipPlanes[BOTTOM_PLANE] = Plane(vec4(0, 0, 0), vec4(xMax, -yMax, -zFar), vec4(-xMax, -yMax, -zFar));
		clipPlanes[RIGHT_PLANE] = Plane(vec4(0, 0, 0), vec4(xMax, yMax, -zFar), vec4(xMax, -yMax, -zFar));
		clipPlanes[LEFT_PLANE] = Plane(vec4(0, 0, 0), vec4(-xMax, -yMax, -zFar), vec4(-xMax, yMax, -zFar));

		projection = mat4(
			1 / (aspectRatio * tan(angle)), 0, 0, 0,
			0, 1 / tan(angle), 0, 0,
			0, 0, -(zFar - zNear) / (zFar - zNear), -2 * zFar * zNear / (zFar - zNear),
			0, 0, -1, 0
		);
	}

	mat4 Camera::ToScreenSpace(double width, double height)
	{
		return mat4(
			width / 2, 0, 0, width / 2,
			0, -height / 2, 0, height / 2,
			0, 0, 1, 0,
			0, 0, 0, 1
		);
	}

	bool Camera::IsInsideFrustum(vec4& p) const
	{
		for (int i = 0; i < 6; i++)
		{
			if (clipPlanes[i].SignedDistance(p) < 0) return false;
		}
		return true;
	}
}