#pragma once

#include "Matrix.h"

class Vec3
{
public:
	float x = 0.f;
	float y = 0.f;
	float z = 0.f;
	float w = 0.f;

	Vec3()
	{
		Vec3(0.f, 0.f, 0.f);
	}

	Vec3(const Vec3& old)
	{
		this->x = old.x;
		this->y = old.y;
		this->z = old.z;
		this->w = old.w;
	}

	Vec3(float x, float y, float z, float w = 1.f)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	void operator+=(Vec3 vec) {
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
	}

	void operator/=(float s) {
		this->x /= s;
		this->y /= s;
		this->z /= s;
	}

	void operator*=(float s) {
		this->x *= s;
		this->y *= s;
		this->z *= s;
		this->w *= s;
	}

	void operator-=(Vec3 vec) {
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
	}

	Vec3 operator+(Vec3 vec) {

		return Vec3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	}

	Vec3 operator*(Vec3 vec) {

		return Vec3(this->x * vec.x, this->y * vec.y, this->z * vec.z);
	}

	Vec3 operator-(Vec3 vec) {

		return Vec3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
	}

	void operator*=(Matrix<4, 4> m) {
		float x = this->x;
		float y = this->y;
		float z = this->z;
		float w = this->w;

		this->x = m[0][0] * x + m[0][1] * y + m[0][2] * z + m[0][3] * w;
		this->y = m[1][0] * x + m[1][1] * y + m[1][2] * z + m[1][3] * w;
		this->z = m[2][0] * x + m[2][1] * y + m[2][2] * z + m[2][3] * w;
		this->w = m[3][0] * x + m[3][1] * y + m[3][2] * z + m[3][3] * w;
	}

	float getDist() {
		return sqrtf(this->x * this->x + this->y * this->y + this->z * this->z);
	}

	void normalize() {
		float s = getDist();

		this->x /= s;
		this->y /= s;
		this->z /= s;
	}

	float scalar(Vec3 vec) {
		return this->x * vec.x + this->y * vec.y + this->z * vec.z + this->w * vec.w;
	}

	static Vec3 getNormal(Vec3 p1, Vec3 p2, Vec3 p3) {
		float a[3][3] = {
			{1, p2.x - p1.x, p3.x - p1.x},
			{1, p2.y - p1.y, p3.y - p1.y},
			{1, p2.z - p1.z, p3.z - p1.z},
		};

		return Vec3(a[0][0] * a[1][1] * a[2][2] - a[0][0] * a[1][2] * a[2][1], a[1][0] * a[2][1] * a[1][2], a[2][0] * a[1][1] * a[0][2] - a[2][0] * a[0][1] * a[1][2]);
	}

	static Vec3 planeVec(Vec3 p1, Vec3 p2, Vec3 p3)
	{
		float nx = (p2.y - p1.y) * (p3.z - p1.z) - (p3.y - p1.y) * (p2.z - p1.z);
		float ny = (p3.x - p1.x) * (p2.z - p1.z) - (p2.x - p1.x) * (p3.z - p1.z);
		float nz = (p2.x - p1.x) * (p3.y - p1.y) - (p3.x - p1.x) * (p2.y - p1.y);

		float length = sqrt((nx * nx) + (ny * ny) + (nz * nz));

		nx = nx / length;
		ny = ny / length;
		nz = nz / length;

		float A = nx;
		float B = ny;
		float C = nz;
		float D = -(nx * p1.x + ny * p1.y + nz * p1.z);

		return Vec3(A, B, C, D);
	}

private:


};
