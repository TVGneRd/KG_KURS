#pragma once

#include "Vec3.h"

template <int ROWS, int COLUMNS>
class Matrix {

public:

	float **matrix;
	int rows;
	int columns;

	Matrix() {};

	Matrix(float arr[ROWS][COLUMNS]) {
		matrix = new float *[ROWS];

		rows = ROWS;
		columns = COLUMNS;

		for (int i = 0; i < ROWS; i++) {
			matrix[i] = new float[COLUMNS];

			for (int j = 0; j < COLUMNS; j++) {
				matrix[i][j] = arr[i][j];
			}
		}
	}

	float* operator[](int n) {
		return matrix[n];
	}

	static Matrix getScale(float sx, float sy, float sz) {
		float a[4][4] = {
			{sx, 0, 0, 0},
			{0, sy, 0, 0},
			{0, 0, sz, 0},
			{0, 0,  0,  1.f},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getTransition(float dx, float dy, float dz) {
		float a[4][4] = {
			{1, 0, 0, dx},
			{0, 1, 0, dy},
			{0, 0, 1, dz},
			{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getProjection(float cameraZ, float pointZ) {
	
		float zK = (cameraZ) / (cameraZ - pointZ);

		float a[4][4] = {
			{zK, 0, 0, 0},
			{0, zK, 0, 0},
			{0, 0, 1, -pointZ},
			{0, 0,  0,  1},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getShadowMatrix(float x, float y, float z) {

		float a[4][4] = {
			{1, 0, -x / z,	0},
			{0, 1, -y / z,	0},
			{0, 0,		0,	0},
			{0, 0,		0,	1},
		};

		return Matrix<4, 4>(a);
	}

	
	static Matrix getRotationX(int angle) {
		float rad = M_PI / 180 * angle;

		float a[4][4] = {
				{1, 0, 0, 0},
				{0, cos(rad), -sin(rad), 0},
				{0, sin(rad), cos(rad), 0},
				{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getRotationY(int angle) {
		float rad = M_PI / 180 * angle;

		float a[4][4] = {
				{cos(rad), 0, sin(rad), 0},
				{0, 1, 0, 0},
				{-sin(rad), 0, cos(rad), 0},
				{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}

	static Matrix getRotationZ(int angle) {
		float rad = M_PI / 180 * angle;

		float a[4][4] = {
				{cos(rad), -sin(rad), 0, 0},
				{sin(rad), cos(rad), 0, 0},
				{0, 0, 1, 0},
				{0, 0, 0, 1},
		};

		return Matrix<4, 4>(a);
	}
	
private:
};
