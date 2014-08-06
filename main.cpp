#include <iostream>
#include <stdio.h>
#include <conio.h>
#include <time.h>
#include <random>
#include <windows.h>
#include <gdiplus.h>

#define UNICODE 1

using namespace std;
using namespace Gdiplus;

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

void draw_image(Bitmap* map, int w, int h)
{
	Color p_color;

	long colorref = RGB(0,0,0);
	p_color.SetFromCOLORREF(colorref);

	for (int x = 0; x < w; x++)
	{
		for (int y = 0; y < h; y++)
		{
			map->GetPixel(x,y,&p_color);
			colorref = p_color.ToCOLORREF();
			SetPixel(dc,x,y,colorref);
		}
	}
}

bool GetImageSizeEx(const char *fn, int *x,int *y)
{ 
    FILE *f=fopen(fn,"rb"); 

    if (f==0) return false;

    fseek(f,0,SEEK_END); 
    long len=ftell(f); 
    fseek(f,0,SEEK_SET); 

    if (len<24) {
        fclose(f); 
        return false;
    }

  // Strategy:
  // reading GIF dimensions requires the first 10 bytes of the file
  // reading PNG dimensions requires the first 24 bytes of the file
  // reading JPEG dimensions requires scanning through jpeg chunks
  // In all formats, the file is at least 24 bytes big, so we'll read that always
    unsigned char buf[24]; fread(buf,1,24,f);


  // For JPEGs, we need to read the first 12 bytes of each chunk.
  // We'll read those 12 bytes at buf+2...buf+14, i.e. overwriting the existing buf.
    if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF && buf[3]==0xE0 && buf[6]=='J' && buf[7]=='F' && buf[8]=='I' && buf[9]=='F')
    {
        long pos=2;
        while (buf[2]==0xFF)
        {
            if (buf[3]==0xC0 || buf[3]==0xC1 || buf[3]==0xC2 || buf[3]==0xC3 || buf[3]==0xC9 || buf[3]==0xCA || buf[3]==0xCB) 
                break;

            pos += 2+(buf[4]<<8)+buf[5];
            if (pos+12>len) break;
            fseek(f,pos,SEEK_SET); 
            fread(buf+2,1,12,f);   
        }
    }


    fclose(f);

  // JPEG: (first two bytes of buf are first two bytes of the jpeg file; rest of buf is the DCT frame
    if (buf[0]==0xFF && buf[1]==0xD8 && buf[2]==0xFF)
    {     
        *y = (buf[7]<<8) + buf[8]; 
        *x = (buf[9]<<8) + buf[10]; 

        return true;
    }

  // GIF: first three bytes say "GIF", next three give version number. Then dimensions
    if (buf[0]=='G' && buf[1]=='I' && buf[2]=='F')
    { 
        *x = buf[6] + (buf[7]<<8);
        *y = buf[8] + (buf[9]<<8);
        return true;
    }

  // PNG: the first frame is by definition an IHDR frame, which gives dimensions
    if ( buf[0]==0x89 && buf[1]=='P' && buf[2]=='N' && buf[3]=='G' && buf[4]==0x0D && buf[5]==0x0A && buf[6]==0x1A && buf[7]==0x0A && buf[12]=='I' && buf[13]=='H' && buf[14]=='D' && buf[15]=='R')
    {
        *x = (buf[16]<<24) + (buf[17]<<16) + (buf[18]<<8) + (buf[19]<<0);
        *y = (buf[20]<<24) + (buf[21]<<16) + (buf[22]<<8) + (buf[23]<<0);

        return true;
    }

    return false;
}

int main()
{
	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	srand(time(NULL));

	SetConsoleTitle("Console Renderer");

	HWND cnsl = GetConsoleWindow();
	dc = GetDC(cnsl);
	
	int x;
	int y;
	const WCHAR* file = L"images/wat.jpg";
	char file2[128];
	wcstombs(file2, file, 128);
	
	Bitmap* map = Bitmap::FromFile(file, false);
	
	draw_image(map, x, y);
	
	getch();

	return 0;
}
//
