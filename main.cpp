#define _USE_MATH_DEFINES

#include <iostream>
#include <math.h>

#include "graphics.h"
#include "Vec3.h"
#include "Matrix.h"
#include <format>
#include "Pyramid.h"


void check_keys();
void render();

#define OBJECTS_NUMBER 2
#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 800

int activePyramidIndex = 0;

Pyramid *pyramids[OBJECTS_NUMBER] = { new Pyramid3(300, 200, 100), new Pyramid4(500, 500, 200) };
Pyramid *pyramid = pyramids[0];

void check_keys()
{
	//int command = toupper(getch());
	float delta = 5.f;

	if (kbhit())
	{
		getch();
		// Поворот Ось: Z Напрвления: ПЧС
		if (GetKeyState('Q') & 0x8000)
		{
			pyramid->rotate(Vec3(0, 0, -delta));
		}

		// Поворот Ось: Z Напрвления: ЧС
		if (GetKeyState('E') & 0x8000)
		{
			pyramid->rotate(Vec3(0, 0, delta));
		}

		// Поворот Ось: X Напрвления: ЧС
		if (GetKeyState('W') & 0x8000)
		{
			pyramid->rotate(Vec3(delta, 0, 0));
		}

		// Поворот Ось: X Напрвления: ПЧС
		if (GetKeyState('S') & 0x8000)
		{
			pyramid->rotate(Vec3(-delta, 0, 0));
		}

		// Поворот Ось: Y Напрвления: ПЧС
		if (GetKeyState('A') & 0x8000)
		{
			pyramid->rotate(Vec3(0, -delta,  0));
		}

		// Поворот Ось: Y Напрвления: ЧС
		if (GetKeyState('D') & 0x8000)
		{
			pyramid->rotate(Vec3(0, delta, 0));
		}

		// Мастаб -
		if (GetKeyState(VK_OEM_MINUS) & 0x8000)
		{
			pyramid->scale(1 / 1.5);
		}

		// Мастаб +
		if (GetKeyState(VK_OEM_PLUS) & 0x8000)
		{
			pyramid->scale(1.5);
		}

		// Вверх
		if (GetKeyState(VK_UP) & 0x8000)
		{
			pyramid->move(Vec3(0, -delta, 0));
		}

		// Вниз
		if (GetKeyState(VK_DOWN) & 0x8000)
		{
			pyramid->move(Vec3(0, delta, 0));
		}

		// Влево
		if (GetKeyState(VK_LEFT) & 0x8000)
		{
			pyramid->move(Vec3(-delta, 0, 0));
		}

		// Вправо
		if (GetKeyState(VK_RIGHT) & 0x8000)
		{
			pyramid->move(Vec3(delta, 0, 0));
		}

		// Вверх
		if (GetKeyState('Z') & 0x8000)
		{
			pyramid->move(Vec3(0, 0, -delta));
		}

		// Вниз
		if (GetKeyState('X') & 0x8000)
		{
			pyramid->move(Vec3(0, 0, delta));
		}

		// Переключение фигур
		if (GetKeyState(VK_SPACE) & 0x8000)
		{
			activePyramidIndex = activePyramidIndex == OBJECTS_NUMBER - 1 ? 0 : activePyramidIndex + 1;
			pyramid = pyramids[activePyramidIndex];
		}

		render();

	}
	
}

void render() {
	cleardevice();

	for (int i = 0; i < OBJECTS_NUMBER; i++) {
		int color = pyramids[i] == pyramid ? RGB(200, 45, 155) : RGB(195, 230, 13);

		pyramids[i]->render(color);
	}
	
	setcolor(WHITE);

	string line1 = format("X: {} Y: {} Z: {}", pyramid->center.x, pyramid->center.y, pyramid->center.z);
	outtextxy(WINDOW_WIDTH - textwidth(line1.data()) - 20, 0, line1.data());

	string line2 = format("RX: {} RY: {} RZ: {}", pyramid->rotateVec.x, pyramid->rotateVec.y, pyramid->rotateVec.z);
	outtextxy(WINDOW_WIDTH - textwidth(line2.data()) - 20, 18, line2.data());

	swapbuffers();
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	initwindow(WINDOW_WIDTH, WINDOW_HEIGHT);

	render();

	while (true) 
	{
		check_keys();
		delay(50);
	}

}
