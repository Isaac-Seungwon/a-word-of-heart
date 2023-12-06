#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <SDL2/SDL.h>

#ifndef _MAX_PATH
#define _MAX_PATH 260
#endif
#define MAX_COLS 32768/

int mkdir(const char *dirname);
int chdir(const char *dirname2);

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Event event;

SDL_Rect src;
SDL_Rect flagsrc;
SDL_Rect hitsrc;

SDL_Rect infotext;
SDL_Rect infosrc;
int infotimer = 0;
int infotimers = 0;

SDL_Rect mention;
SDL_Rect mentionsrc;
int mentiontimer = 0;
int random = 0;

SDL_Rect intro;
SDL_Rect introsrc;
SDL_Rect introm[3];
SDL_Rect intromsrc[3];
int choice = 0;

// 함정
SDL_Rect spikesrc1[100];
SDL_Rect spikesrc2[100];
SDL_Rect spikesrc3[100];
SDL_Rect spikesrc4[100];
SDL_Rect trap1[100];
SDL_Rect trap2[100];
SDL_Rect trap3[100];
SDL_Rect trap4[100];
SDL_Point tr1[100];
SDL_Point tr2[100];
SDL_Point tr3[100];
SDL_Point tr4[100];

SDL_Rect player;
SDL_Rect point;
SDL_Rect block[100];
SDL_Rect hit;
SDL_Rect back;
SDL_Point p[100];
SDL_Point pp;
SDL_Point ppp;
SDL_Point pppp;

// 열쇠 및 자물쇠
int open1 = 1;
int open2 = 1;
int locktimer = 0;
SDL_Rect locks1[100];
SDL_Rect locksrc1;
SDL_Point lockp1[100];
SDL_Rect keys1[100];
SDL_Rect keysrc1;
SDL_Point keyp1[100];
SDL_Rect locks2[100];
SDL_Rect locksrc2;
SDL_Point lockp2[100];
SDL_Rect keys2[100];
SDL_Rect keysrc2;
SDL_Point keyp2[100];

int loc = 1;
int stage = 1;
int laststage = 1;
int start = -1;
double angle = 0;

int first = 1;
int running = 1;
int left = 0;
int right = 0;
int cangle = 0;
int ccangle = 0;
int rtimer = 50;
int flagtimer = 0;
int ticks;
int clear = 0;
int i;
int restart = 0;
int spiketimer1[100];
int spiketimers1[100];
int spiketimer2[100];
int spiketimers2[100];
int spiketimer3[100];
int spiketimers3[100];
int spiketimer4[100];
int spiketimers4[100];
float s = 3;

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
#define S 40

//사용자 누적 스테이지 불러오기
int loadstage()
{
	int v;

	//파일을 읽기 바이너리 모드로 불러옴
	FILE *file = fopen("stage.txt", "rb");

	if(file == NULL)
	{
		return 0;
	}
	fread((void*)&v, sizeof(int), 1, file);
	v ^= 0xA361Ba3d; //암호 해독 0xA295fa3d

	fclose(file);

	return v;
}

//사용자 누적 스테이지 저장
void savestage(int score)
{
	//파일을 쓰기 바이너리 모드로 불러움
	FILE *file = fopen("stage.txt", "wb");

	if(file == NULL)
	{
		return;
	}

	score^=0xA361Ba3d; //스코어 암호화
	fwrite((void*)&score, sizeof(int), 1, file);

	fclose(file);
}

// 충돌함수
int checkCollision(SDL_Rect *a, SDL_Rect *b){
	int t = 0, result = 0;

	if ((a->x <= b->x && b->x <= a->x + a->w) || (a->x <= b->x + b->w && b->x + b->w <= a->x + a->w) || (b->x <= a->x && a->x <= b->x + b->w) || (b->x <= a->x + a->w && a->x + a->w <= b->x + b->w)) t++;
	if ((a->y <= b->y && b->y <= a->y + a->h) || (a->y <= b->y + b->h && b->y + b->h <= a->y + a->h) || (b->y <= a->y && a->y <= b->y + b->h) || (b->y <= a->y + a->h && a->y + a->h <= b->y + b->h)) t++;
	if (t == 2) result = 1;

	return result;
}

void renderblock(int n){
	block[n].x = S * (n % 10) + 120;
	block[n].y = S * (n / 10) + 40;
}

void renderlock1(int n){
	locks1[n].x = S * (n % 10) + 120;
	locks1[n].y = S * (n / 10) + 40;
}

void renderlock2(int n){
	locks2[n].x = S * (n % 10) + 120;
	locks2[n].y = S * (n / 10) + 40;
}

void renderkey1(int n)
{
	keys1[n].x = S * (n % 10) + 120;
	keys1[n].y = S * (n / 10) + 40;
}

void renderkey2(int n)
{
	keys1[n].x = S * (n % 10) - 20 + 120;
	keys1[n].y = S * (n / 10) + 20 + 40;
}

void renderkey3(int n)
{
	keys2[n].x = S * (n % 10) + 120;
	keys2[n].y = S * (n / 10) + 40;
}

void renderkey4(int n)
{
	keys2[n].x = S * (n % 10) - 20 + 120;
	keys2[n].y = S * (n / 10) + 20 + 40;
}

void renderspike(int a, int b, int c, int d){
	trap1[a].x = S * (a % 10) + 120;
	trap1[a].y = S * (a / 10) + 40;

	trap2[b].x = S * (b % 10) + 120;
	trap2[b].y = S * (b / 10) + 40;

	trap3[c].x = S * (c % 10) + 120;
	trap3[c].y = S * (c / 10) + 40;

	trap4[d].x = S * (d % 10) + 120;
	trap4[d].y = S * (d / 10) + 40;
}

void renderplxy1(int n)
{
	player.x = S * (n % 10) + 10 + 120;
	player.y = S * (n / 10) + 10 + 40;
}

void renderplxy2(int n)
{
	player.x = S * (n % 10) - 10 + 120;
	player.y = S * (n / 10) + 30 + 40;
}

void renderpoxy1(int n)
{
	point.x = S * (n % 10) + 120;
	point.y = S * (n / 10) + 40;
}

void renderpoxy2(int n)
{
	point.x = S * (n % 10) - 15 + 120;
	point.y = S * (n / 10) + 20 + 40;
}

int checkCollision(SDL_Rect *a, SDL_Rect *b){
	int t = 0, result = 0;

	if ((a->x <= b->x && b->x <= a->x + a->w) || (a->x <= b->x + b->w && b->x + b->w <= a->x + a->w) || (b->x <= a->x && a->x <= b->x + b->w) || (b->x <= a->x + a->w && a->x + a->w <= b->x + b->w)) t++;
	if ((a->y <= b->y && b->y <= a->y + a->h) || (a->y <= b->y + b->h && b->y + b->h <= a->y + a->h) || (b->y <= a->y && a->y <= b->y + b->h) || (b->y <= a->y + a->h && a->y + a->h <= b->y + b->h)) t++;
	if (t == 2) result = 1;

	return result;
}

void renderstage()
{
	int i;

	switch (stage)
	{
	case 1:
		if (start == 0 || start == -3)
		{
			renderplxy2(15);
			renderpoxy2(75);
		}
		break;
	case 2:
		infotimer = 0;
		infosrc.y = 36;
		for (i = 51; i <= 84; i++)
		{
			renderblock(i);
			if (i == 54) i = 60;
			if (i == 64) i = 70;
			if (i == 74) i = 80;
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(23);
			renderpoxy2(77);
		}
		break;
	case 3:
		infotimer = 0;
		infosrc.y = 36 * 2;
		for (i = 14; i <= 55; i+=10)
		{
			renderblock(i);
			if (i == 54) i = 5;
		}
		if (start == 0 || start == -3)
		{
			renderplxy1(22);
			renderpoxy1(27);
		}
		break;
	case 4:
		for (i = 13; i <= 63; i += 10)
		{
			renderblock(i);
		}
		for (i = 36; i <= 86; i += 10)
		{
			renderblock(i);
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy2(78);
		}
		break;
	case 5:
		infotimer = 0;
		infosrc.y = 36 * 3;
		renderkey1(61);
		renderkey1(62);
		renderlock1(34);
		renderlock1(35);
		for (i = 13; i <= 63; i += 10)
		{
			renderblock(i);
		}
		for (i = 64; i <= 66; i++)
		{
			renderblock(i);
		}
		for (i = 36; i <= 56; i += 10)
		{
			renderblock(i);
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy2(45);
		}
		break;
	case 6:
		infotimer = 0;
		infosrc.y = 36 * 4;
		renderkey2(75);
		renderlock1(67);
		renderlock1(68);
		
		renderkey4(45);
		renderlock2(16);
		renderlock2(26);
		for (i = 13; i <= 63; i += 10)
		{
			renderblock(i);
		}
		for (i = 64; i <= 66; i++)
		{
			renderblock(i);
		}
		for (i = 36; i <= 38; i ++)
		{
			renderblock(i);
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy2(18);
		}
		break;
	case 7:
		infotimer = 0;
		infosrc.y = 36 * 5;
		renderspike(0, 0, 15, 0);
		renderspike(0, 0, 24, 0);
		renderspike(75, 0, 0, 0);
		renderspike(84, 0, 0, 0);
		for (i = 13; i <= 53; i += 10)
		{
			renderblock(i);
		}
		renderblock(14);
		renderblock(85);
		for (i = 46; i <= 86; i += 10)
		{
			renderblock(i);
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy2(78);
		}
		break;
	case 8:
		if (first == 1){
			spikesrc3[15].x = 160;
			spiketimer3[15] = 112;
			first = 0;
		}
		renderspike(0, 0, 15, 0);
		renderspike(84, 0, 0, 0);
		for (i = 12; i <= 72; i += 10)
		{
			renderblock(i);
		}
		for (i = 13; i <= 63; i += 10)
		{
			renderblock(i);
		}
		for (i = 14; i <= 34; i += 10)
		{
			renderblock(i);
		}
		for (i = 65; i <= 85; i += 10)
		{
			renderblock(i);
		}
		for (i = 36; i <= 86; i += 10)
		{
			renderblock(i);
		}
		for (i = 27; i <= 87; i += 10)
		{
			renderblock(i);
		}
		if (start == 0 || start == -3)
		{
			renderplxy1(11);
			renderpoxy1(88);
		}
		break;
	case 9:
		renderkey1(62);
		renderlock1(44);
		renderlock1(54);
		renderkey3(32);
		renderlock2(45);
		renderlock2(55);

		for (i = 12; i <= 35; i++)
		{
			renderblock(i);
			if (i == 17) i = 22;
			if (i == 26) i = 33;
		}
		for (i = 64; i <= 87; i++)
		{
			renderblock(i);
			if (i == 65) i = 72;
			if (i == 76) i = 81;
		}
		for (i = 42; i <= 57; i += 10)
		{
			renderblock(i);
			if (i == 52) i = 37;
		}
		if (start == 0 || start == -3)
		{
			renderplxy1(41);
			renderpoxy1(58);
		}
		break;
	case 10:
		for (i = 13; i <= 89; i++)
		{
			renderblock(i);
			if (i == 19) i = 23;
			if (i == 29) i = 34;
			if (i == 39) i = 45;
			if (i == 49) i = 56;
			if (i == 59) i = 67;
			if (i == 69) i = 78;
			if (i == 79) i = 89;
		}
		for (i = 31; i <= 97; i++)
		{
			renderblock(i);
			if (i == 31) i = 40;
			if (i == 42) i = 50;
			if (i == 53) i = 60;
			if (i == 64) i = 70;
			if (i == 75) i = 80;
			if (i == 86) i = 90;
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy2(78);
		}
		break;
	case 11:
		renderspike(0, 0, 34, 0);
		renderspike(0, 0, 35, 0);
		renderspike(0, 43, 0, 0);
		renderspike(0, 53, 0, 0);
		renderspike(0, 0, 0, 46);
		renderspike(0, 0, 0, 56);
		renderspike(64, 0, 0, 0);
		renderspike(65, 0, 0, 0);
		
		for (i = 12; i <= 72; i += 10)
		{
			renderblock(i);
		}
		for (i = 27; i <= 87; i += 10)
		{
			renderblock(i);
		}
		for (i = 73; i <= 75; i++)
		{
			renderblock(i);
		}
		for (i = 24; i <= 26; i++)
		{
			renderblock(i);
		}
		for (i = 44; i <= 55; i++)
		{
			renderblock(i);
			if (i == 45) i = 53;
		}
		if (start == 0 || start == -3)
		{
			renderplxy1(11);
			renderpoxy1(88);
		}
		/*
		for (i = 12; i <= 72; i += 10)
		{
			renderblock(i);
		}
		for (i = 17; i <= 77; i += 10)
		{
			renderblock(i);
		}
		for (i = 24; i <= 95; i += 1)
		{
			renderblock(i);
			if (i == 25) i = 33;
			if (i == 35) i = 43;
			if (i == 45) i = 53;
			if (i == 55) i = 63;
			if (i == 65) i = 73;
			if (i == 75) i = 83;
			if (i == 85) i = 93;
		}
		if (start == 0)
		{
			renderplxy1(11);
			renderpoxy1(18);
		}
		*/
		break;
	case 12:
		if (first == 1){
			spiketimer2[21] = 100;
			spiketimer2[31] = 95;
			spiketimer2[41] = 90;
			spiketimer2[51] = 85;
			spiketimer2[61] = 80;
			spiketimer2[71] = 75;

			spiketimer1[81] = 70;
			spiketimer1[82] = 65;
			spiketimer1[83] = 60;
			spiketimer1[84] = 55;
			spiketimer1[85] = 50;
			spiketimer1[86] = 45;
			spiketimer1[87] = 40;
			spiketimer1[88] = 35;

			spiketimer4[28] = 5;
			spiketimer4[38] = 10;
			spiketimer4[48] = 15;
			spiketimer4[58] = 20;
			spiketimer4[68] = 25;
			spiketimer4[78] = 30;

			spiketimer3[13] = -25;
			spiketimer3[14] = -20;
			spiketimer3[15] = -15;
			spiketimer3[16] = -10;
			spiketimer3[17] = -5;
			spiketimer3[18] = 0;
			first = 0;
		}

		renderspike(0, 21, 0, 0);
		renderspike(0, 31, 0, 0);
		renderspike(0, 41, 0, 0);
		renderspike(0, 51, 0, 0);
		renderspike(0, 61, 0, 0);
		renderspike(0, 71, 0, 0);

		renderspike(81, 0, 0, 0);
		renderspike(82, 0, 0, 0);
		renderspike(83, 0, 0, 0);
		renderspike(84, 0, 0, 0);
		renderspike(85, 0, 0, 0);
		renderspike(86, 0, 0, 0);
		renderspike(87, 0, 0, 0);
		renderspike(88, 0, 0, 0);

		renderspike(0, 0, 0, 28);
		renderspike(0, 0, 0, 38);
		renderspike(0, 0, 0, 48);
		renderspike(0, 0, 0, 58);
		renderspike(0, 0, 0, 68);
		renderspike(0, 0, 0, 78);
		
		renderspike(0, 0, 13, 0);
		renderspike(0, 0, 14, 0);
		renderspike(0, 0, 15, 0);
		renderspike(0, 0, 16, 0);
		renderspike(0, 0, 17, 0);
		renderspike(0, 0, 18, 0);

		/*
		renderspike(0, 23, 0, 0);
		renderspike(0, 33, 0, 0);
		renderspike(0, 43, 0, 0);
		renderspike(0, 53, 0, 0);

		renderspike(63, 0, 0, 0);
		renderspike(64, 0, 0, 0);
		renderspike(65, 0, 0, 0);
		renderspike(66, 0, 0, 0);
		*/

		for (i = 12; i <= 72; i += 10)
		{
			renderblock(i);
		}
		for (i = 73; i <= 77; i++)
		{
			renderblock(i);
		}
		for (i = 27; i <= 67; i += 10)
		{
			renderblock(i);
		}
		for (i = 24; i <= 27; i++)
		{
			renderblock(i);
		}
		for (i = 34; i <= 54; i += 10)
		{
			renderblock(i);
		}
		renderblock(55);
		if (start == 0 || start == -3)
		{
			renderplxy1(11);
			renderpoxy1(35);
		}
		break;
	case 13:
		renderspike(51, 0, 0, 0);
		renderspike(62, 0, 0, 0);
		renderspike(73, 0, 0, 0);
		renderspike(84, 0, 0, 0);
		renderspike(85, 0, 0, 0);
		renderspike(76, 0, 0, 0);
		renderspike(67, 0, 0, 0);
		renderspike(58, 0, 0, 0);

		renderspike(0, 0, 53, 0);
		renderspike(0, 0, 56, 0);
		renderspike(0, 0, 64, 0);
		renderspike(0, 0, 65, 0);

		for (i = 13; i <= 46; i++)
		{
			renderblock(i);
			if (i == 16) i = 22;
			if (i == 26) i = 32;
			if (i == 36) i = 42;
		}
		for (i = 61; i <= 83; i++)
		{
			renderblock(i);
			if (i == 61) i = 70;
			if (i == 72) i = 80;
		}
		for (i = 68; i <= 88; i++)
		{
			renderblock(i);
			if (i == 68) i = 76;
			if (i == 78) i = 85;
		}
		renderblock(54);
		renderblock(55);
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy2(18);
		}
		break;
	case 14:
		renderkey2(42);
		renderlock1(76);
		renderkey4(48);
		renderlock2(73);

		for (i = 11; i <= 31; i++)
		{
			renderblock(i);
			if (i == 13) i = 20;
			if (i == 22) i = 30;
		}
		for (i = 16; i <= 38; i++)
		{
			renderblock(i);
			if (i == 18) i = 26;
			if (i == 28) i = 37;
		}
		for (i = 61; i <= 83; i++)
		{
			renderblock(i);
			if (i == 61) i = 70;
			if (i == 72) i = 80;
		}
		for (i = 68; i <= 88; i++)
		{
			renderblock(i);
			if (i == 68) i = 76;
			if (i == 78) i = 85;
		}
		renderblock(34);
		renderblock(35);
		for (i = 43; i <= 56; i++)
		{
			renderblock(i);
			if (i == 46) i = 52;
		}
		renderblock(64);
		renderblock(65);
		if (start == 0 || start == -3)
		{
			renderplxy2(15);
			renderpoxy2(75);
		}
		break;
	case 15:
		renderkey2(73);
		renderlock1(17);
		renderkey3(87);
		renderlock2(62);
		renderlock2(63);

		renderspike(51, 0, 0, 0);
		renderspike(54, 0, 0, 0);
		renderspike(0, 62, 0, 0);
		renderspike(0, 0, 0, 63);
		renderspike(0, 0, 71, 0);
		renderspike(0, 0, 74, 0);
		for (i = 23; i <= 28; i++)
		{
			renderblock(i);
		}
		for (i = 45; i <= 85; i += 10)
		{
			renderblock(i);
		}
		for (i = 47; i <= 85; i += 10)
		{
			renderblock(i);
		}
		renderblock(61);
		renderblock(64);
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy1(18);
		}
		break;
	case 16:
		renderkey2(12);
		renderlock1(16);
		renderlock1(17);
		renderlock1(18);
		renderlock1(26);
		renderlock1(28);
		renderlock1(36);
		renderlock1(37);
		renderlock1(38);
		renderkey4(58);
		renderlock2(31);
		renderlock2(32);

		renderblock(13);
		renderblock(23);
		renderblock(33);
		renderblock(46);
		renderblock(47);
		renderblock(48);
		renderblock(56);
		renderblock(66);
		renderblock(63);
		renderblock(64);
		renderblock(65);

		renderspike(81, 0, 0, 0);
		renderspike(82, 0, 0, 0);
		renderspike(87, 0, 0, 0);
		renderspike(88, 0, 0, 0);
		renderspike(0, 0, 74, 0);
		renderspike(0, 0, 75, 0);
		if (start == 0 || start == -3)
		{
			renderplxy2(15);
			renderpoxy1(27);
		}
		break;
	case 17:
		if (first == 1){
			spiketimers3[24] = 50;
			spiketimers1[25] = 50;
			spiketimer1[25] = 25;
			
			// spiketimer2[57] = 50;
			// spiketimer3[75] = 50;
			first = 0;
		}

		renderspike(0, 0, 24, 0);
		renderspike(25, 0, 0, 0);
		/*
		renderspike(0, 0, 0, 47);
		renderspike(0, 57, 0, 0);
		renderspike(0, 0, 75, 0);
		renderspike(74, 0, 0, 0);
		*/
		renderkey1(72);
		renderlock1(52);
		renderlock1(16);
		renderlock1(17);
		renderlock1(18);
		renderlock1(36);
		renderlock1(37);
		renderlock1(38);
		renderlock1(26);
		renderlock1(28);
		renderkey3(27);
		renderlock2(42);

		renderblock(14);
		renderblock(15);
		renderblock(34);
		renderblock(35);
		renderblock(64);
		renderblock(65);
		renderblock(84);
		renderblock(85);
		renderblock(41);
		renderblock(43);
		renderblock(44);
		renderblock(45);
		renderblock(46);
		renderblock(48);
		renderblock(51);
		renderblock(53);
		renderblock(54);
		renderblock(55);
		renderblock(56);
		renderblock(58);
		if (start == 0 || start == -3)
		{
			renderplxy1(77);
			renderpoxy1(22);
		}
		/*
		for (i = 11; i <= 31; i++)
		{
			renderblock(i);
			if (i == 13) i = 20;
			if (i == 22) i = 30;
		}
		for (i = 16; i <= 38; i++)
		{
			renderblock(i);
			if (i == 18) i = 25;
			if (i == 28) i = 35;
		}
		for (i = 61; i <= 83; i++)
		{
			renderblock(i);
			if (i == 61) i = 70;
			if (i == 72) i = 80;
		}
		for (i = 68; i <= 88; i++)
		{
			renderblock(i);
			if (i == 68) i = 76;
			if (i == 78) i = 85;
		}
		renderblock(34);
		renderblock(35);
		for (i = 43; i <= 56; i++)
		{
			renderblock(i);
			if (i == 46) i = 52;
		}
		renderblock(64);
		renderblock(65);
		if (start == 0)
		{
			renderplxy2(15);
			renderpoxy2(48);
		}
		*/
		break;
	case 18:
		renderkey1(52);
		renderlock1(74);
		renderlock1(75);
		renderkey3(57);
		renderlock2(64);
		renderlock2(65);

		renderblock(11);
		renderblock(18);
		renderblock(32);
		renderblock(37);
		renderblock(51);
		renderblock(58);
		renderblock(72);
		renderblock(77);
		for (i = 33; i <= 36; i++)
		{
			renderblock(i);
		}
		for (i = 23; i <= 76; i += 10)
		{
			renderblock(i);
			if (i == 73) i = 16;
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(15);
			renderpoxy2(45);
		}
		break;
	case 19:
		if (first == 1){
			spiketimer1[81] = 80;
			spiketimer1[83] = 70;
			spiketimer1[85] = 60;
			spiketimer1[87] = 50;
			spiketimer4[78] = 40;
			spiketimer3[67] = 30;
			spiketimer3[65] = 20;
			
			spiketimers4[43] = 60;
			spiketimers4[23] = 60;
			spiketimers1[14] = 60;
			spiketimers2[25] = 60;
			spiketimers2[45] = 60;
			spiketimer4[43] = 0;
			spiketimer4[23] = -10;
			spiketimer1[14] = -20;
			spiketimer2[25] = -30;
			spiketimer2[45] = -40;
			first = 0;
		}

		renderspike(81, 0, 0, 0);
		renderspike(83, 0, 0, 0);
		renderspike(85, 0, 0, 0);
		renderspike(87, 0, 0, 0);
		renderspike(0, 0, 0, 78);
		renderspike(0, 0, 67, 0);
		renderspike(0, 0, 65, 0);

		renderspike(0, 0, 0, 43);
		renderspike(0, 0, 0, 23);
		renderspike(14, 0, 0, 0);
		renderspike(0, 25, 0, 0);
		renderspike(0, 45, 0, 0);

		renderkey2(54);
		renderlock1(37);
		renderlock1(38);
		renderkey4(18);
		renderlock2(12);
		renderlock2(16);
		renderlock2(26);
		renderlock2(36);

		for (i = 22; i <= 72; i += 10)
		{
			renderblock(i);
		}
		for (i = 24; i <= 44; i += 10)
		{
			renderblock(i);
		}
		for (i = 55; i <= 58; i++)
		{
			renderblock(i);
		}
		for (i = 73; i <= 77; i++)
		{
			renderblock(i);
		}
		renderblock(74);
		if (start == 0 || start == -3)
		{
			renderplxy1(21);
			renderpoxy1(11);
		}
		break;
	case 20:
		if (first == 1){
			spiketimer1[41] = 100;
			spiketimer2[52] = 100;
			spiketimer3[61] = 100;

			spiketimer1[62] = 75;
			spiketimer4[71] = 75;
			spiketimer3[82] = 75;
			spiketimer2[73] = 75;

			spiketimer1[74] = 50;
			spiketimer2[85] = 50;
			spiketimer4[83] = 50;

			spiketimer4[75] = 25;
			spiketimer3[86] = 25;
			spiketimer2[77] = 25;

			first = 0;
		}
		renderkey1(35);
		renderlock1(55);
		renderkey3(37);
		renderlock2(56);
		renderlock2(36);

		renderspike(41, 0, 0, 0);
		renderspike(0, 52, 0, 0);
		renderspike(0, 0, 61, 0);
		
		renderspike(62, 0, 0, 0);
		renderspike(0, 0, 0, 71);
		renderspike(0, 0, 82, 0);
		renderspike(0, 73, 0, 0);

		renderspike(74, 0, 0, 0);
		renderspike(0, 85, 0, 0);
		renderspike(0, 0, 0, 83);
		
		renderspike(0, 0, 0, 75);
		renderspike(0, 0, 86, 0);
		renderspike(0, 77, 0, 0);

		renderblock(25);
		renderblock(27);
		renderblock(44);
		renderblock(48);
		renderblock(51);
		renderblock(45);
		renderblock(72);
		renderblock(76);
		renderblock(84);
		renderblock(88);
		for (i = 13; i <= 63; i += 10)
		{
			renderblock(i);
		}
		for (i = 64; i <= 67; i++)
		{
			renderblock(i);
		}
		renderblock(26);
		for (i = 46; i <= 46; i += 10)
		{
			renderblock(i);
		}
		if (start == 0 || start == -3)
		{
			renderplxy2(12);
			renderpoxy1(54);
		}
		break;
	case 21:
		/*
		renderkey1(34);
		renderkey1(35);
		renderlock1(23);
		renderlock1(26);
		renderlock1(32);
		renderlock1(33);
		renderlock1(36);
		renderlock1(37);
		renderlock1(42);
		renderlock1(43);
		renderlock1(44);
		renderlock1(45);
		renderlock1(46);
		renderlock1(47);
		renderlock1(53);
		renderlock1(54);
		renderlock1(55);
		renderlock1(56);
		renderlock1(64);
		renderlock1(65);
		*/
		
		renderkey1(34);
		renderkey3(35);
		renderlock1(23);
		renderlock2(26);
		renderlock1(32);
		renderlock1(33);
		renderlock2(36);
		renderlock2(37);
		renderlock1(42);
		renderlock1(43);
		renderlock1(44);
		renderlock2(45);
		renderlock2(46);
		renderlock2(47);
		renderlock1(53);
		renderlock1(54);
		renderlock2(55);
		renderlock2(56);
		renderlock1(64);
		renderlock2(65);

		renderspike(54, 0, 0, 0);
		renderspike(55, 0, 0, 0);
		renderblock(64);
		renderblock(65);
		if (start == 0 || start == -3)
		{
			renderplxy2(15);
			renderpoxy2(75);
		}
		break;
	}
}

void reset ()
{
	angle = 0;
	loc = 1;
	start = 0;
	cangle = 0;
	clear = 1;
	left = 0;
	right = 0;
	first = 1;
	open1 = 1;
	open2 = 1;
	infotimer = 0;
	mentiontimer = 0;

	src.x = S;
	keysrc1.x = 0; keysrc1.w = S;
	locksrc1.x = 0; locksrc1.w = S;
	keysrc2.x = 0; keysrc2.w = S;
	locksrc2.x = 0; locksrc2.w = S;
	infosrc.y = 0;

	if (stage == 21) random = 11;
	if (random == 0) mentionsrc.y = 0;
	else mentionsrc.y = random * 36;

	for (i = 0; i < 100; i++)
	{
		spikesrc1[i].x = 0;
		spikesrc2[i].x = 0;
		spikesrc3[i].x = 0;
		spikesrc4[i].x = 0;
	
		spiketimer1[i] = 0;
		spiketimer2[i] = 0;
		spiketimer3[i] = 0;
		spiketimer4[i] = 0;

		spiketimers1[i] = 100;
		spiketimers2[i] = 100;
		spiketimers3[i] = 100;
		spiketimers4[i] = 100;

		keys1[i].x = -500;
		keys1[i].y = -500;

		locks1[i].x = -500;
		locks1[i].y = -500;
		
		keys2[i].x = -500;
		keys2[i].y = -500;

		locks2[i].x = -500;
		locks2[i].y = -500;

		trap1[i].x = -500;
		trap1[i].y = -500;
		
		trap2[i].x = -500;
		trap2[i].y = -500;

		trap3[i].x = -500;
		trap3[i].y = -500;

		trap4[i].x = -500;
		trap4[i].y = -500;
	}
}

int main(int argc, char** argv){
	const Uint8 *keystate;

	HWND hConsole = GetConsoleWindow();
	ShowWindow(hConsole , SW_HIDE);
	
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_Init(SDL_INIT_AUDIO);

	window = SDL_CreateWindow("A Word of Heart", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
	renderer = SDL_CreateRenderer(window, -1, 0);

	SDL_Surface* bmp1_1 = SDL_LoadBMP("key1.bmp");
	SDL_Surface* bmp1_2 = SDL_LoadBMP("key2.bmp");
	SDL_Surface* bmp2 = SDL_LoadBMP("hitbox.bmp");
	SDL_Surface* bmp3 = SDL_LoadBMP("flag.bmp");
	SDL_Surface* bmp5 = SDL_LoadBMP("spike.bmp");
	SDL_Surface* bmp4 = SDL_LoadBMP("wall.bmp");
	SDL_Surface* bmp6 = SDL_LoadBMP("background.bmp");
	SDL_Surface* bmp7 = SDL_LoadBMP("box.bmp");
	SDL_Surface* bmp8_1 = SDL_LoadBMP("lock1.bmp");
	SDL_Surface* bmp8_2 = SDL_LoadBMP("lock2.bmp");
	SDL_Surface* bmp9 = SDL_LoadBMP("text1.bmp");
	SDL_Surface* bmp10 = SDL_LoadBMP("text2.bmp");
	SDL_Surface* bmp11 = SDL_LoadBMP("introscreen.bmp");
	SDL_Surface* bmp12 = SDL_LoadBMP("intromessage.bmp");
	SDL_Texture* key1 = SDL_CreateTextureFromSurface(renderer, bmp1_1);
	SDL_Texture* key2 = SDL_CreateTextureFromSurface(renderer, bmp1_2);
	SDL_Texture* hitbox = SDL_CreateTextureFromSurface(renderer, bmp2);
	SDL_Texture* flag = SDL_CreateTextureFromSurface(renderer, bmp3);
	SDL_Texture* spike = SDL_CreateTextureFromSurface(renderer, bmp5);
	SDL_Texture* wall = SDL_CreateTextureFromSurface(renderer, bmp4);
	SDL_Texture* background = SDL_CreateTextureFromSurface(renderer, bmp6);
	SDL_Texture* box = SDL_CreateTextureFromSurface(renderer, bmp7);
	SDL_Texture* lock1 = SDL_CreateTextureFromSurface(renderer, bmp8_1);
	SDL_Texture* lock2 = SDL_CreateTextureFromSurface(renderer, bmp8_2);
	SDL_Texture* text1 = SDL_CreateTextureFromSurface(renderer, bmp9);
	SDL_Texture* text2 = SDL_CreateTextureFromSurface(renderer, bmp10);
	SDL_Texture* introscreen = SDL_CreateTextureFromSurface(renderer, bmp11);
	SDL_Texture* intromessage = SDL_CreateTextureFromSurface(renderer, bmp12);
	SDL_FreeSurface(bmp1_1);
	SDL_FreeSurface(bmp1_2);
	SDL_FreeSurface(bmp2);
	SDL_FreeSurface(bmp3);
	SDL_FreeSurface(bmp4);
	SDL_FreeSurface(bmp5);
	SDL_FreeSurface(bmp6);
	SDL_FreeSurface(bmp7);
	SDL_FreeSurface(bmp8_1);
	SDL_FreeSurface(bmp8_2);
	SDL_FreeSurface(bmp9);
	SDL_FreeSurface(bmp10);
	SDL_FreeSurface(bmp11);
	SDL_FreeSurface(bmp12);

	keystate = SDL_GetKeyboardState(0);

	// src (애니메이션)
	src.x = 0; src.y = 0; src.w = 100; src.h = 100;
	keysrc1.x = 0; keysrc1.y = 0; keysrc1.w = 40; keysrc1.h = 40;
	locksrc1.x = 0; locksrc1.y = 0; locksrc1.w = 40; locksrc1.h = 40;
	keysrc2.x = 0; keysrc2.y = 0; keysrc2.w = 40; keysrc2.h = 40;
	locksrc2.x = 0; locksrc2.y = 0; locksrc2.w = 40; locksrc2.h = 40;

	for (i = 0; i < 100; i++)
	{
		spikesrc1[i].x = 0; spikesrc1[i].y = 0; spikesrc1[i].w = 40; spikesrc1[i].h = 40;
		spikesrc2[i].x = 0; spikesrc2[i].y = 40; spikesrc2[i].w = 40; spikesrc2[i].h = 40;
		spikesrc3[i].x = 0; spikesrc3[i].y = 80; spikesrc3[i].w = 40; spikesrc3[i].h = 40;
		spikesrc4[i].x = 0; spikesrc4[i].y = 120; spikesrc4[i].w = 40; spikesrc4[i].h = 40;
	}
	flagsrc.x = 0; flagsrc.y = 0; flagsrc.w = 40; flagsrc.h = 40;
	hitsrc.x = 0; hitsrc.y = 0; hitsrc.w = 10; hitsrc.h = 10;

	infosrc.x = 0; infosrc.y = 0; infosrc.w = 450; infosrc.h = 36;
	infotext.x = 320 - 225; infotext.y = 480 - 38; infotext.w = 450; infotext.h = 36;
	introsrc.x = 0; introsrc.y = 0; introsrc.w = 640; introsrc.h = 480;
	intromsrc[0].x = 0; intromsrc[0].y = 0; intromsrc[0].w = 223; intromsrc[0].h = 19;
	intromsrc[1].x = 0; intromsrc[1].y = 19; intromsrc[1].w = 223; intromsrc[1].h = 19;
	intromsrc[2].x = 0; intromsrc[2].y = 38; intromsrc[2].w = 223; intromsrc[2].h = 19;

	mentionsrc.x = 0; mentionsrc.y = -37; mentionsrc.w = 450; mentionsrc.h = 36;
	mention.x = 320 - 225; mention.y = 3; mention.w = 450; mention.h = 36;
	intro.x = 0; intro.y = 0; intro.w = 640; intro.h = 480;
	introm[0].x = 206; introm[0].y = 330; introm[0].w = 223; introm[0].h = 19;
	introm[1].x = 206; introm[1].y = 330; introm[1].w = 223; introm[1].h = 19;
	introm[2].x = 206; introm[2].y = 350; introm[2].w = 223; introm[2].h = 19;

	back.x = 0; back.y = 0; back.w = 640; back.h = 480;
	player.x = S * (15 % 10) - 10 + 120; player.y = S * (15 / 10) + 30 + 40; player.w = 20; player.h = 20;
	point.x = S * (75 % 10) - 15 + 120; point.y = S * (75 / 10) + 20 + 40; point.w = S; point.h = S;
	hit.w = 10; hit.h = 10;

	// 누적 스테이지 불러오기
	laststage = loadstage();
	if (laststage == 0) laststage = 1;

	if (!(laststage >= 0 && laststage <= 21))
	{
		laststage = 1;
		savestage(stage);
	}

	for (i = 0; i < 100; i++)
	{
		spiketimers1[i] = 100;
		spiketimers2[i] = 100;
		spiketimers3[i] = 100;
		spiketimers4[i] = 100;

		block[i].x = -500;
		block[i].y = -500;
		block[i].w = S;
		block[i].h = S;
		
		keys1[i].x = -500;
		keys1[i].y = -500;
		keys1[i].w = S;
		keys1[i].h = S;
		
		locks1[i].x = -500;
		locks1[i].y = -500;
		locks1[i].w = S;
		locks1[i].h = S;

		keys2[i].x = -500;
		keys2[i].y = -500;
		keys2[i].w = S;
		keys2[i].h = S;
		
		locks2[i].x = -500;
		locks2[i].y = -500;
		locks2[i].w = S;
		locks2[i].h = S;

		trap1[i].x = -500;
		trap1[i].y = -500;
		trap1[i].w = S;
		trap1[i].h = S;
		
		trap2[i].x = -500;
		trap2[i].y = -500;
		trap2[i].w = S;
		trap2[i].h = S;

		trap3[i].x = -500;
		trap3[i].y = -500;
		trap3[i].w = S;
		trap3[i].h = S;

		trap4[i].x = -500;
		trap4[i].y = -500;
		trap4[i].w = S;
		trap4[i].h = S;
	}

	while (running){
		ticks = SDL_GetTicks();
		while (SDL_PollEvent(&event))
			switch (event.type){
			case SDL_QUIT:
				running = 0;
				break;
			case SDL_KEYDOWN:
				if (keystate[SDL_SCANCODE_DOWN]) if (start == 0 && src.x < 30) start = 1;

				if(start == -2)
				{
					if (keystate[SDL_SCANCODE_UP]) choice = 0;
					if (keystate[SDL_SCANCODE_DOWN]) choice = 1;
					if (keystate[SDL_SCANCODE_SPACE])
					{
						if (choice == 1) stage = laststage;

						infotimer = 0, start = -3;
					}
				}
				if (keystate[SDL_SCANCODE_SPACE]) if (start == -1) infotimer = 0, start = -2;
			break;
		}

		if (stage > laststage)
		{
			savestage(stage);
		}

		// 배경
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);

		for (i = 0; i < 100; i++)
		{
			// 바깥 벽
			if (i % 10 == 0 || i % 10 == 9 || i / 10 == 0 || i / 10 == 9)
			{
				renderblock(i);
			}

			// 함정
			SDL_RenderCopyEx(renderer, spike, &spikesrc1[i], &trap1[i], angle, &tr1[i], SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, spike, &spikesrc2[i], &trap2[i], angle, &tr2[i], SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, spike, &spikesrc3[i], &trap3[i], angle, &tr3[i], SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, spike, &spikesrc4[i], &trap4[i], angle, &tr4[i], SDL_FLIP_NONE);

			// 안쪽 벽
			SDL_RenderCopyEx(renderer, wall, &src, &block[i], angle, &p[i], SDL_FLIP_NONE);
		
			// 열쇠
			SDL_RenderCopyEx(renderer, key1, &keysrc1, &keys1[i], angle, &keyp1[i], SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, key2, &keysrc2, &keys2[i], angle, &keyp2[i], SDL_FLIP_NONE);

			// 문
			SDL_RenderCopyEx(renderer, lock1, &locksrc1, &locks1[i], angle, &lockp1[i], SDL_FLIP_NONE);
			SDL_RenderCopyEx(renderer, lock2, &locksrc2, &locks2[i], angle, &lockp2[i], SDL_FLIP_NONE);
		}

		if(open1 == 0)
		{
			if(keysrc1.x < 25)
			{
				keysrc1.x += 2;
				keysrc1.w -= 4;
				locksrc1.x += 2;
				locksrc1.w -= 4;
				SDL_SetTextureAlphaMod(key1, 255 - (int)keysrc1.x * 10);
				SDL_SetTextureAlphaMod(lock1, 255 - (int)keysrc1.x * 10);
			}
		}
		else
		{
			SDL_SetTextureAlphaMod(key1, 255);
			SDL_SetTextureAlphaMod(lock1, 255);
		}
		if(open2 == 0)
		{
			if(keysrc2.x < 25)
			{
				keysrc2.x += 2;
				keysrc2.w -= 4;
				locksrc2.x += 2;
				locksrc2.w -= 4;
				SDL_SetTextureAlphaMod(key2, 255 - (int)keysrc2.x * 10);
				SDL_SetTextureAlphaMod(lock2, 255 - (int)keysrc2.x * 10);
			}
		}
		else
		{
			SDL_SetTextureAlphaMod(key2, 255);
			SDL_SetTextureAlphaMod(lock2, 255);
		}
		
		// 캐릭터
		SDL_RenderCopyEx(renderer, box, &src, &player, angle, &pp, SDL_FLIP_NONE); 

		// 피격점
		hit.x = player.x + 5;
		hit.y = player.y + 5;
		if (start == 1)
		SDL_RenderCopyEx(renderer, hitbox, &hitsrc, &hit, angle, &pppp, SDL_FLIP_NONE);

		renderstage();

		if (left == 0 && right == 0)
		{
			flagtimer++;
			for (i = 0; i < 100; i++)
			{
				spiketimer1[i]++;
				spiketimer2[i]++;
				spiketimer3[i]++;
				spiketimer4[i]++;
			}
		}

		if (flagsrc.x == 0 && flagtimer >= 20){
			flagsrc.x = 40;
			flagtimer = 0;
		}
		else if (flagsrc.x == 40 && flagtimer >= 20){
			flagsrc.x = 0;
			flagtimer = 0;
		}

		for (i = 0; i < 100; i++)
		{
			if (spikesrc1[i].x == 0 && spiketimer1[i] >= spiketimers1[i]){
				spikesrc1[i].x = 40;
			}
			else if (spikesrc1[i].x == 40 && spiketimer1[i] >= spiketimers1[i] + 4){
				spikesrc1[i].x = 80;
			}
			else if (spikesrc1[i].x == 80 && spiketimer1[i] >= spiketimers1[i] + 4){
				spikesrc1[i].x = 120;
			}
			else if (spikesrc1[i].x == 120 && spiketimer1[i] >= spiketimers1[i] + 4){
				spikesrc1[i].x = 160;
			}
			else if (spikesrc1[i].x == 160 && spiketimer1[i] >= (spiketimers1[i] * 2) + 12){
				spikesrc1[i].x = 0;
				spiketimer1[i] = 0;
			}

			if (spikesrc2[i].x == 0 && spiketimer2[i] >= spiketimers2[i]){
				spikesrc2[i].x = 40;
			}
			else if (spikesrc2[i].x == 40 && spiketimer2[i] >= spiketimers2[i] + 4){
				spikesrc2[i].x = 80;
			}
			else if (spikesrc2[i].x == 80 && spiketimer2[i] >= spiketimers2[i] + 4){
				spikesrc2[i].x = 120;
			}
			else if (spikesrc2[i].x == 120 && spiketimer2[i] >= spiketimers2[i] + 4){
				spikesrc2[i].x = 160;
			}
			else if (spikesrc2[i].x == 160 && spiketimer2[i] >= (spiketimers2[i] * 2) + 12){
				spikesrc2[i].x = 0;
				spiketimer2[i] = 0;
			}

			if (spikesrc3[i].x == 0 && spiketimer3[i] >= spiketimers3[i]){
				spikesrc3[i].x = 40;
			}
			else if (spikesrc3[i].x == 40 && spiketimer3[i] >= spiketimers3[i] + 4){
				spikesrc3[i].x = 80;
			}
			else if (spikesrc3[i].x == 80 && spiketimer3[i] >= spiketimers3[i] + 4){
				spikesrc3[i].x = 120;
			}
			else if (spikesrc3[i].x == 120 && spiketimer3[i] >= spiketimers3[i] + 4){
				spikesrc3[i].x = 160;
			}
			else if (spikesrc3[i].x == 160 && spiketimer3[i] >= (spiketimers3[i] * 2) + 12){
				spikesrc3[i].x = 0;
				spiketimer3[i] = 0;
			}

			if (spikesrc4[i].x == 0 && spiketimer4[i] >= spiketimers4[i]){
				spikesrc4[i].x = 40;
			}
			else if (spikesrc4[i].x == 40 && spiketimer4[i] >= spiketimers4[i] + 4){
				spikesrc4[i].x = 80;
			}
			else if (spikesrc4[i].x == 80 && spiketimer4[i] >= spiketimers4[i] + 4){
				spikesrc4[i].x = 120;
			}
			else if (spikesrc4[i].x == 120 && spiketimer4[i] >= spiketimers4[i] + 4){
				spikesrc4[i].x = 160;
			}
			else if (spikesrc4[i].x == 160 && spiketimer4[i] >= (spiketimers4[i] * 2) + 12){
				spikesrc4[i].x = 0;
				spiketimer4[i] = 0;
			}
		}

		SDL_RenderCopyEx(renderer, flag, &flagsrc, &point, angle, &ppp, SDL_FLIP_NONE); // 목적지

		// 회전 중앙
		for (i = 0; i < 100; i++)
		{
			p[i].x = 320 - block[i].x;
			p[i].y = 240 - block[i].y;
			
			keyp1[i].x = 320 - keys1[i].x;
			keyp1[i].y = 240 - keys1[i].y;

			lockp1[i].x = 320 - locks1[i].x;
			lockp1[i].y = 240 - locks1[i].y;
			
			keyp2[i].x = 320 - keys2[i].x;
			keyp2[i].y = 240 - keys2[i].y;

			lockp2[i].x = 320 - locks2[i].x;
			lockp2[i].y = 240 - locks2[i].y;

			tr1[i].x = 320 - trap1[i].x;
			tr1[i].y = 240 - trap1[i].y;
			
			tr2[i].x = 320 - trap2[i].x;
			tr2[i].y = 240 - trap2[i].y;
			
			tr3[i].x = 320 - trap3[i].x;
			tr3[i].y = 240 - trap3[i].y;

			tr4[i].x = 320 - trap4[i].x;
			tr4[i].y = 240 - trap4[i].y;
		}
		
		pp.x = 320 - player.x, pp.y = 240 - player.y;
		ppp.x = 320 - point.x, ppp.y = 240 - point.y;
		pppp.x = 320 - hit.x, pppp.y = 240 - hit.y;
		
		rtimer++;
		if (start == 1)
		{
			// 낙하 속도 증가
			//if (cangle == 19 || cangle == 0) if(s < 6.7) s = s + 0.035;
			s = 5;

			// 화면이 비정상적인 각도에서 멈추는 버그 발생 시 초기화
			if (cangle == 19 && (int)angle % 90 != 0)
			{ 
				start = 0;
				angle = 0; 
				loc = 1;
			}

			if (rtimer >= 22)
			{
				if (keystate[SDL_SCANCODE_LEFT]) left = 1, loc--, cangle = 0, rtimer = 0;
				else if (keystate[SDL_SCANCODE_RIGHT]) right = 1, loc++, cangle = 0, rtimer = 0;
						
				if (loc >= 5) loc = 1;
				if (loc <= 0) loc = 4;
				if (angle == 360) angle = 0;
				else if (angle == -360) angle = 0;
			}
			
			if (left == 1)
			{
				cangle++;
				if (cangle < 19) angle -= 5;
				else left = 0;
			}
			else if (right == 1)
			{
				cangle++;
				if (cangle < 19) angle += 5;
				else right = 0;
			}
			else
			{
				switch (loc)
				{
					case 1: player.y += s; break;
					case 2: player.x += s; break;
					case 3: player.y -= s; break;
					case 4: player.x -= s; break;
				}
			}
		}
		else if (start == 0)
		{
			// 화면 상단부에 띄워지는 mention, 하단부 info
			infotimer++;

			if (infotimer < 30) SDL_RenderCopy(renderer, text1, &infosrc, &infotext);
			else if (infotimer == 60) infotimer = 0;

			if (mentiontimer < 80)
			{
				mentiontimer++;
				SDL_RenderCopy(renderer, text2, &mentionsrc, &mention);
			}
		}
		if (stage == 21 && left == 0 && right == 0) SDL_RenderCopy(renderer, text2, &mentionsrc, &mention);

		//개발용 스테이지 변환
		if(start == 0 || start == 1){
			if(rtimer >= 5)
			{
				if (keystate[SDL_SCANCODE_SPACE])
				{
					if (keystate[SDL_SCANCODE_Z])
					{
						if(stage > 1)
						{
							stage--;
							angle = 0;
							loc = 1;
							for (i = 0; i <= 100; i++)
							{
								block[i].x = -500;
								block[i].y = -500;
							}
							start = 0;
							rtimer = 0;

							reset();
						}
					}
					else if (keystate[SDL_SCANCODE_X])
					{
						if(stage < 21)
						{
							stage++;
							angle = 0;
							loc = 1;
							for (i = 0; i <= 100; i++)
							{
								block[i].x = -500;
								block[i].y = -500;
							}
							start = 0;
							rtimer = 0;
							if(stage == 21) random = 11;

							reset();
						}
					}
				}
			}
		}

		// 키 획득
		for (i = 0; i < 100; i++)
		{
			if (checkCollision(&player, &keys1[i]))
			{
				open1 = 0;
			}
			if (checkCollision(&player, &keys2[i]))
			{
				open2 = 0;
			}
		}

		// 스테이지 성공
		if (checkCollision(&player, &point))
		{	
			if (stage < 8)
			{		
				srand(time(NULL));
				random = rand() % (3 - 0 + 1) + 0;
			}
			else
			{
				srand(time(NULL));
				random = rand() % (10 - 0 + 1) + 0;
			}

			for (i = 0; i < 100; i++)
			{
				block[i].x = -500;
				block[i].y = -500;
			}
			if (stage < 21)
			{
				stage++;
			}
			reset();
		}

		// 스테이지 실패
		if(start == 1)
		{
			for (i = 0; i < 100; i++)
			{
				if (open1 == 1)
				{
					if (checkCollision(&hit, &locks1[i]))
					{
						srand(time(NULL));
						random = rand() % (20 - 12 + 1) + 12;
						reset();
					}
				}
				if (open2 == 1)
				{
					if (checkCollision(&hit, &locks2[i]))
					{
						srand(time(NULL));
						random = rand() % (20 - 12 + 1) + 12;
						reset();
					}
				}

				if (spikesrc1[i].x == 0)
				{
					if (checkCollision(&hit, &trap1[i]))
					{
						srand(time(NULL));
						random = rand() % (20 - 12 + 1) + 12;
						reset();
					}
				}
				if (spikesrc2[i].x == 0)
				{
					if (checkCollision(&hit, &trap2[i]))
					{
						srand(time(NULL));
						random = rand() % (20 - 12 + 1) + 12;
						reset();
					}
				}
				if (spikesrc3[i].x == 0)
				{
					if (checkCollision(&hit, &trap3[i]))
					{
						srand(time(NULL));
						random = rand() % (20 - 12 + 1) + 12;
						reset();
					}
				}
				if (spikesrc4[i].x == 0)
				{
					if (checkCollision(&hit, &trap4[i]))
					{
						srand(time(NULL));
						random = rand() % (20 - 12 + 1) + 12;
						reset();
					}
				}

				if (checkCollision(&hit, &block[i]))
				{
					srand(time(NULL));
					random = rand() % (20 - 12 + 1) + 12;
					reset();
				}
			}
		}
		// 화면 이펙트 초기화
		if (clear == 1)
		{
			if (src.x > 10)
			{
				src.x -= 1.5;
			}
			else if (src.x > 4)
			{
				src.x -= 1;
			}
			else if (src.x > 0)
			{
				src.x -= 0.5;
			}
			else{
				clear = 0;
			}
		}
		
		SDL_RenderCopy(renderer, introscreen, &introsrc, &intro);

		if (start == -1)
		{
			infotimer++;

			if (infotimer < 25) SDL_RenderCopy(renderer, intromessage, &intromsrc[0], &introm[0]);
			else if (infotimer == 50) infotimer = 0;
		}
		if (start == -2)
		{
			infotimer++;

			switch(choice)
			{
				case 0:
					if(infotimer < 25)
					{
						SDL_RenderCopy(renderer, intromessage, &intromsrc[1], &introm[1]);
					}
					else if (infotimer == 50) infotimer = 0;
					
					SDL_RenderCopy(renderer, intromessage, &intromsrc[2], &introm[2]);
					break;
				case 1:
					if(infotimer < 25)
					{
						SDL_RenderCopy(renderer, intromessage, &intromsrc[2], &introm[2]);
					}
					else if (infotimer == 50) infotimer = 0;
					SDL_RenderCopy(renderer, intromessage, &intromsrc[1], &introm[1]);
					break;
			}
		}
		if (start == -3)
		{
			infotimers++;

			if(infotimers > 6) SDL_SetTextureAlphaMod(introscreen, 250);
			if(infotimers > 6 * 2) SDL_SetTextureAlphaMod(introscreen, 200);
			if(infotimers > 6 * 3) SDL_SetTextureAlphaMod(introscreen, 150);
			if(infotimers > 6 * 4) SDL_SetTextureAlphaMod(introscreen, 100);
			if(infotimers > 6 * 5) SDL_SetTextureAlphaMod(introscreen, 50);
			if(infotimers > 6 * 6) SDL_SetTextureAlphaMod(introscreen, 0);
			if(infotimers > 6 * 7) start = 0, infotimers = 0;
		}

		SDL_RenderPresent(renderer);

		ticks = SDL_GetTicks() - ticks;
		if (30 - ticks > 0) SDL_Delay(30 - ticks);
	}

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);
	SDL_Quit();

	return 0;
}