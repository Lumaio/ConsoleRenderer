#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <random>
#include <windows.h>
#include <gdiplus.h>

using namespace std;

HDC dc;

void rect(int x, int y, int w, int h)
{
	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			int r = rand()%255;
			int g = rand()%255;
			int b = rand()%255;
			SetPixelV(dc, x+i, y+j, RGB(r,g,b));
		}
	}
}
void DrawCircle(int x0, int y0, int radius)
{
	int x = radius, y = 0;
	int radiusError = 1-x; 	

	while(x >= y)
	{
		int r = rand()%255;
		int g = rand()%255;
		int b = rand()%255;
		SetPixelV(dc, x + x0, y + y0, RGB(0,g,b));
		SetPixelV(dc, y + x0, x + y0, RGB(0,g,b));
		SetPixelV(dc, -x + x0, y + y0, RGB(0,g,b));
		SetPixelV(dc, -y + x0, x + y0, RGB(0,g,b));
		SetPixelV(dc, -x + x0, -y + y0, RGB(0,g,b));
		SetPixelV(dc, -y + x0, -x + y0, RGB(0,g,b));
		SetPixelV(dc, x + x0, -y + y0, RGB(0,g,b));
		SetPixelV(dc, y + x0, -x + y0, RGB(0,g,b));
		y++;
		if (radiusError<0)
		{
			radiusError += 2 * y + 1;
		}
		else
		{
			x--;
			radiusError += 2 * (y - x + 1);
		}
	}
}

void DrawFilledCircle(int x0, int y0, int radius)
{
    int x = radius;
    int y = 0;
    int xChange = 1 - (radius << 1);
    int yChange = 0;
    int radiusError = 0;

    for(int y=-radius; y<=radius; y++)
	{
		for(int x=-radius; x<=radius; x++)
		{
			if(x*x+y*y <= radius*radius)
			{
				int r = rand()%255;
				int g = rand()%255;
				int b = rand()%255;
				SetPixelV(dc, x0+x, y0+y, RGB(r,g,b));
			}
		}
	}
}

void draw_image()
{
	Gdiplus::Bitmap* map = Gdiplus::Bitmap::FromFile(L"nic.jpg", false);
	Gdiplus::Color p_color;
	
	long colorref = RGB(0,0,0);
	p_color.SetFromCOLORREF(colorref);
	
	for (int x = 0; x < 714; x++)
	{
		for (int y = 0; y < 536; y++)
		{
			map->GetPixel(x,y,&p_color);
			colorref = p_color.ToCOLORREF();
			SetPixelV(dc,x,y,colorref);
		}
	}
}

int main()
{
	const int w = 100;
	const int h = 50;
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	srand(time(NULL));
	
	SetConsoleTitle("wat?");
	
	HWND cnsl = GetConsoleWindow();
	dc = GetDC(cnsl);
	
	draw_image();
	getch();
	
	return 0;
}
//