#pragma once

using namespace std;

class Pyramid
{
public:
	int verticesLength = 4;
	int planesLength = 4;

	Vec3 center = Vec3(0, 0, 0);
	Vec3 rotateVec = Vec3(0, 0, 0); // usage in status only

	Vec3* vertices = new Vec3[4] {
		 Vec3(0, 0, 0), // 0 вершина
		 Vec3(1.f, 0, 0), // 1 вершина
		 Vec3(0.5, 0, pow(3, 1 / 2) / 2), // 2 вершина
		 Vec3(0.5, -pow(6, 1 / 2) / 2, pow(3, 1 / 2) / 6), // 3 вершина
	};

	int** planes = new int* [4] {
		new int[3] {0, 1, 2},
		new int[3] {0, 3, 1},
		new int[3] {0, 3, 2},
		new int[3] {1, 3, 2},
	};

	Pyramid(int x, int y, int z) {

		Vec3 center = getCenter();

		for (int i = 0; i < verticesLength; i++) {
			vertices[i] -= center;
		}
		
		this->center = Vec3(x, y, z);

		scale(200);
	}

	void move(Vec3 delta)
	{
		center += delta;
	}

	void rotate(Vec3 rotateVector)
	{
		rotateVec += rotateVector;

		if (rotateVector.x) {
			Matrix rotateX = Matrix<4, 4>::getRotationX(rotateVector.x);

			rotateVec.x = rotateVec.x > 360 ? rotateVec.x - 360 : rotateVec.x;
			rotateVec.x = rotateVec.x < 0 ? 360 + rotateVec.x : rotateVec.x;

			for (int i = 0; i < verticesLength; i++) {
				vertices[i] *= rotateX;
			}
		}

		if (rotateVector.y) {
			rotateVec.y = rotateVec.y > 360 ? rotateVec.y - 360 : rotateVec.y;
			rotateVec.y = rotateVec.y < 0 ? 360 + rotateVec.y : rotateVec.y;

			Matrix rotateY = Matrix<4, 4>::getRotationY(rotateVector.y);
			for (int i = 0; i < verticesLength; i++) {
				vertices[i] *= rotateY;
			}
		}

		if (rotateVector.z) {
			rotateVec.z = rotateVec.z > 360 ? rotateVec.z - 360 : rotateVec.z;
			rotateVec.z = rotateVec.z < 0 ? 360 + rotateVec.z : rotateVec.z;

			Matrix rotateZ = Matrix<4, 4>::getRotationZ(rotateVector.z);
			for (int i = 0; i < verticesLength; i++) {
				vertices[i] *= rotateZ;
			}
		}

	}

	Vec3 getCenter() {
		Vec3 center = Vec3(0, 0, 0);

		for (int i = 0; i < verticesLength; i++) {
			center += vertices[i];
		}

		center /= verticesLength;

		return center;
	}

	void scale(float ratio) {
		Matrix scale = Matrix<4, 4>::getScale(ratio, ratio, ratio);

		for (int i = 0; i < verticesLength; i++) {
			vertices[i] *= scale;
		}
	}

	void drawPoint(int i, Vec3 vertice, Vec3 projection) {
		string data = format("{} ({}, {}, {})", i, (int)vertice.x, (int)vertice.y, (int)vertice.z);

		setcolor(WHITE);
		outtextxy(projection.x, projection.y, data.data());
	}

	virtual void render(int color) {
		Vec3* projections = new Vec3[verticesLength];
		Vec3 camera = Vec3(0, 0, 500);

		for (int i = 0; i < verticesLength; i++) {
			Vec3 point = vertices[i];

			Matrix projectionMatrix = Matrix<4, 4>::getProjection(camera.z, center.z + point.z);
			point *= projectionMatrix;
			point += center;

			projections[i] = point;
		}


		Vec3 figCenter = Vec3(0, 0, 0, 1);
		Vec3 ViewVec3 = Vec3(0, 0, 1, 0);
		Vec3 LampVec = Vec3(1000, 800, 1500);
		
		LampVec *= -1;
		LampVec.normalize();

		Vec3 *shadow = new Vec3[verticesLength];

		// “ени
		for (int i = 0; i < verticesLength; i++) {
			Vec3 point = vertices[i] + center;

			Matrix projectionMatrix = Matrix<4, 4>::getShadowMatrix(LampVec.x, LampVec.y, LampVec.z);
			point *= projectionMatrix;

			shadow[i] =  point;
		}

		// ќтрисовка теней
		for (int i = 0; i < planesLength; i++) {
			fillTriangle(RGB(90, 90, 90), RGB(90, 90, 90), shadow[planes[i][0]], shadow[planes[i][1]], shadow[planes[i][2]]);
		}
	

		// ќтрисовка плоскостей
		for (int i = 0; i < planesLength; i++) {
			Vec3 plane = Vec3::planeVec(vertices[planes[i][0]], vertices[planes[i][1]], vertices[planes[i][2]]);

			if (plane.scalar(figCenter) < 0) {
				plane *= -1.f;
			}

			if (plane.scalar(ViewVec3) < 0) {
				fillTriangle(color, WHITE, projections[planes[i][0]], projections[planes[i][1]], projections[planes[i][2]]);

				drawPoint(planes[i][0], vertices[planes[i][0]], projections[planes[i][0]]);
				drawPoint(planes[i][1], vertices[planes[i][1]], projections[planes[i][1]]);
				drawPoint(planes[i][2], vertices[planes[i][2]], projections[planes[i][2]]);
			}
		}
	}

	void fillTriangle(int color, int borderColor, Vec3 point1, Vec3 point2, Vec3 point3)
	{
		
		float points[3][2] = { { point1.x, point1.y }, {point2.x, point2.y}, {point3.x, point3.y} };

		for (int k = 0; k < 3; k++)
		{
			for (int i = k; i < 3; i++)
			{
				if (points[k][1] > points[i][1])
				{
					float tmp[2] = { points[k][0], points[k][1] };
					points[k][0] = points[i][0];
					points[k][1] = points[i][1];
					points[i][0] = tmp[0];
					points[i][1] = tmp[1];
				}
			}
		}

		float x1, x2, tmp;

		setcolor(color);

		for (float sy = points[0][1]; sy <= points[2][1]; sy++)
		{
			x1 = points[0][0] + (sy - points[0][1]) * (points[2][0] - points[0][0]) / (points[2][1] - points[0][1]);

			if (sy < points[1][1]) {
				x2 = points[0][0] + (sy - points[0][1]) * (points[1][0] - points[0][0]) / (points[1][1] - points[0][1]);
			} else {
				if (points[2][1] == points[1][1]) x2 = points[1][0];
				else x2 = points[1][0] + (sy - points[1][1]) * (points[2][0] - points[1][0]) / (points[2][1] - points[1][1]);
			}

			if (x1 > x2) { tmp = x1; x1 = x2; x2 = tmp; }

			moveto(x1, sy);
			lineto(x2, sy);
		}

		setlinestyle(SOLID_LINE, 1, THICK_WIDTH);
		setcolor(borderColor);

		moveto(point1.x, point1.y);
		lineto(point2.x, point2.y);
		lineto(point3.x, point3.y);
		lineto(point1.x, point1.y);
	}
private:

};


class Pyramid3: public Pyramid
{
public:
	const int verticesLength = 4;
	const int planesLength = 4;

	Vec3 vertices[4] = {
		 Vec3(0, 0, 0), // 0 вершина
		 Vec3(1.f, 0, 0), // 1 вершина
		 Vec3(0.5, 0, pow(3, 1 / 2) / 2), // 2 вершина
		 Vec3(0.5, -pow(6, 1 / 2) / 2, pow(3, 1 / 2) / 6), // 3 вершина
	};

	int planes[4][3] =  {
		{0, 1, 2},
		{0, 3, 1},
		{0, 3, 2},
		{1, 3, 2},
	};

	Pyramid3(int x, int y, int z) : Pyramid(x, y, z) {};
};

class Pyramid4 : public Pyramid
{
public:


	Pyramid4(int x, int y, int z) : Pyramid(x, y, z) {
		verticesLength = 5;
		planesLength = 6;

		vertices = new Vec3[5]{
			Vec3(0.f, 0.f, 0.f), // 0 вершина
			Vec3(2.f, 0.f, 0.f), // 1 вершина
			Vec3(2.f, 0.f, 2.f), // 2 вершина
			Vec3(0.f, 0.f, 2.f), // 3 вершина
			Vec3(1.f, 2.f, 1.f), // 4 вершина
		};

		planes = new int* [6] {
			new int[3] {0,1,3},
			new int[3] {0,4,3},
			new int[3] {3,4,2},
			new int[3] {1,4,2},
			new int[3] {0,1,4},
			new int[3] {1,2,3}
		};

		Vec3 center = getCenter();

		for (int i = 0; i < verticesLength; i++) {
			vertices[i] -= center;
		}

		this->center = Vec3(x, y, z);

		rotate(Vec3(90, 0, 0));
		scale(50);
	};

	
};