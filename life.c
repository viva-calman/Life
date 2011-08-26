#include <stdio.h>
#include <stdlib.h>
#include <SDL/SDL.h>
#include "life.h"

#define WIDTH 64
#define HEIGHT 45

void InitImages(void);
void DrawIMG(SDL_Surface *img, int x, int y);
void DrawIMG(SDL_Surface *img, int x, int y, int w, int h, int sx, int sy);
void SetPoint(int x,int y);
void ClearField(void);
void ReDraw(void);
void NextGen(void);
int CheckCell(int x,int y);

SDL_Surface *background;
SDL_Surface *dot;
SDL_Surface *screen;

cell field[WIDTH][HEIGHT];
int generation=0;

int main (int argc, char *argv[])
{
	Uint8* mouse;
	bool start=false;
	bool exitstate=false;
	int clickx,clicky;
	
	ClearField();

	if(SDL_Init(SDL_INIT_VIDEO)<0)
	{
		printf("Unable to init SDL: %s\n",SDL_GetError());
		exit(1);
	}
	atexit(SDL_Quit);

	SDL_WM_SetCaption("Life game","life");
	screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_DOUBLEBUF);

	if(screen==NULL)
	{
		printf("unable to set 640x480 video %s\n",SDL_GetError());
		exit(1);
	}
	InitImages();
	DrawIMG(background,0,0);
	while(exitstate==false)
	{
		SDL_Event event;
		while(SDL_PollEvent(&event))
		{
			if(event.type==SDL_QUIT) 
			{
				exitstate=true;
			}
			if(event.type==SDL_KEYDOWN)
			{
				if(event.key.keysym.sym=SDLK_ESCAPE)
				{
					exitstate=true;
				}
			}
			if(event.type==SDL_MOUSEBUTTONUP)
			{
				clickx=event.button.x;
				clicky=event.button.y;
				if(clicky>450 && clickx<320)
				{
					if(start==false)
					{
						start=true;
					}
					else
					{
						start=false;
					}
				}
				if(clicky>450 && clickx>320)
				{
					ClearField();
					start=false;
				}
				if(clicky<450 && start==false)
				{
					SetPoint(clickx,clicky);
				}

			}
		}
		if(start==true)
		{
			SDL_Delay(500);
			NextGen();
		}
		ReDraw();


	}
	return 0;
}

void InitImages()
{
	background=SDL_LoadBMP("field.bmp");
	dot=SDL_LoadBMP("dot.bmp");
}

void DrawIMG(SDL_Surface *img, int x, int y)
{
 SDL_Rect dest;
 dest.x = x;
 dest.y = y;

 SDL_BlitSurface(img, NULL, screen, &dest);
}

void DrawIMG(SDL_Surface *img, int x, int y, int w, int h, int sx, int sy)
{
 SDL_Rect dest;
 dest.x = x;
 dest.y = y;

 SDL_Rect src;
 src.x = sx;
 src.y = sy;
 src.w = w;
 src.h = h;
 
 SDL_BlitSurface(img, &src, screen, &dest);

}

void SetPoint(int x, int y)
{
	int xm,ym;
	xm=x/10;
	ym=y/10;
	if (field[xm][ym].current==false)
	{
		field[xm][ym].current=true;
	}
	else
	{
		field[xm][ym].current=false;
	}
}

void ClearField(void)
{
	for(int i=0;i<WIDTH;i++)
	{
		for(int j=0;j<HEIGHT;j++)
		{
			field[i][j].current=false;
			field[i][j].next=false;
			field[i][j].age=0;
		}
	}
	generation=0;

}

void ReDraw()
{
	for (int i=0;i<WIDTH;i++)
	{
		for(int j=0;j<HEIGHT;j++)
		{
			if(field[i][j].current==true)
			{
				DrawIMG(dot,(i*10)+2,(j*10)+1);
			}
			else
			{
				DrawIMG(background,i*10,j*10,9,9,i*10,j*10);
			}
		}
	}
	SDL_Flip(screen);
}

int CheckCell(int x,int y)
{
	if(x<0 || y<0 || x>=WIDTH || y >=HEIGHT)
	{
		return 0;
	}
	if(field[x][y].current==true)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

void NextGen(void)
{
	short neib;
	int i,j;
	for(i=0;i<WIDTH;i++)
	{
		for(j=0;j<HEIGHT;j++)
		{
			neib=0;
			neib=neib+CheckCell(i-1,j-1);
			neib=neib+CheckCell(i-1,j);
			neib=neib+CheckCell(i-1,j+1);
			neib=neib+CheckCell(i,j-1);
			neib=neib+CheckCell(i,j+1);
			neib=neib+CheckCell(i+1,j-1);
			neib=neib+CheckCell(i+1,j);
			neib=neib+CheckCell(i+1,j+1);
			if (neib>3 || neib <2)
			{
				field[i][j].next=false;
			}
			else
			{
				if(neib==3){ field[i][j].next=true; }
				if(neib==2 && field[i][j].current==false){ field[i][j].next=false; }
				else { field[i][j].next=true; }
			}
		}
	}
	for(i=0;i<WIDTH;i++)
	{
		for(j=0;j<HEIGHT;j++)
		{
			if(field[i][j].current==field[i][j].next){ field[i][j].age++; }
			else { field[i][j].age=0; }
			field[i][j].current=field[i][j].next;
			
		}
	}
	generation++;
}
