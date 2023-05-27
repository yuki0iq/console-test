#pragma once
#include <string>
#include <sys/types.h>
#include <limits.h>
#include <errno.h>
#include <stdio.h>
#include <memory.h>
#ifndef _WIN32
#include <strings.h>
#else
#include <Windows.h>
#define strcasecmp _stricmp
#define usleep Sleep
#endif

namespace Common
{

enum State { Success, Error, Warning };
typedef unsigned short ushort;
template <typename PosType> struct pos2
{
	PosType pos_x;
	PosType pos_y;
};
#define pos_row pos_y
#define pos_col pos_x
typedef pos2<unsigned short> pos2hu;

namespace Console
{
enum Color
{
	black,
	dred,
	dgreen,
	dyellow,
	dblue,
	dmagenta,
	dcyan,
	gray,
	dgray,
	red,
	green,
	yellow,
	blue,
	magenta,
	cyan,
	white,
	none
};
struct Char
{
	char code = ' ';
	Color fg = gray;
	Color bg = black;
	operator char()
	{
		return code;
	}
};
char* fromChar(Char* pch, int len); //free with delete[];
Char* fromchar(const char* pch, int len); //free with delete[];

void init(pos2hu size);
void exit();
Char get(pos2hu from);
Char *get(pos2hu from, pos2hu to); //free with delete[];
void put(pos2hu where, Char what, bool bFlush = true);
void put(pos2hu from, pos2hu to, Char *what, size_t size, bool bFlush = true);
void setcurpos(pos2hu where);
pos2hu getcurpos();
void flush();
char getch();
char* getstr(int size); //free with delete[];
}


}

