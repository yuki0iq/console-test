#include "common.h"
#ifdef _WIN32
#include <conio.h>
#else
#include <termios.h>
#include <stdio.h>

static struct termios old, new;

/* Initialize new terminal i/o settings */
void initTermios(int echo)
{
	tcgetattr(0, &old); /* grab old terminal i/o settings */
	new = old; /* make new settings same as old settings */
	new.c_lflag &= ~ICANON; /* disable buffered i/o */
	if (echo) {
		new.c_lflag |= ECHO; /* set echo mode */
	}
	else {
		new.c_lflag &= ~ECHO; /* set no echo mode */
	}
	tcsetattr(0, TCSANOW, &new); /* use these new terminal i/o settings now */
}

/* Restore old terminal i/o settings */
void resetTermios(void)
{
	tcsetattr(0, TCSANOW, &old);
}

/* Read 1 character - echo defines echo mode */
char getch_(int echo)
{
	char ch;
	initTermios(echo);
	ch = getchar();
	resetTermios();
	return ch;
}

/* Read 1 character without echo */
char getch(void)
{
	return getch_(0);
}

/* Read 1 character with echo */
char getche(void)
{
	return getch_(1);
}
#define _getch getch
#endif
using namespace Common;


Console::Char **screen;
pos2hu curPos = { 0, 0 };
pos2hu scrSize = { 0, 0 };
void SetColor(Common::Console::Color bg, Common::Console::Color fg)
{
#ifdef _WIN32
	WORD winColorText = fg;
	WORD winColorBkgnd = bg;
	WORD wAttr = /*(WORD)*/(0xF0 & (winColorBkgnd << 4));
	wAttr |= /*(WORD)*/(0x0F & winColorText);
	BOOL bRes = ::SetConsoleTextAttribute(::GetStdHandle(STD_OUTPUT_HANDLE), wAttr);
	int r = 0;
#else
	printf("\x1b[38;5;%im", fg);
	printf("\x1b[48;5;%im", bg);
#endif
}
void ResetColors()
{
	SetColor(Common::Console::black, Common::Console::gray);
}

char * Common::Console::fromChar(Char * pch, int len)
{
	char *pch_ = new char[len + 1];
	for (int i = 0; i < len; ++i)
		pch_[i] = pch[i].code;
	pch_[len] = '\0';
	return pch_;
}

Common::Console::Char * Common::Console::fromchar(const char * pch, int len)
{
	Char *pch_ = new Char[len];
	for (int i = 0; i < len; ++i)
		pch_[i].code = pch[i];
	return pch_;
}

void Common::Console::init(pos2hu size)
{
	screen = (Char**)calloc(size.pos_row, sizeof(Char*));
	for (int i = 0; i < size.pos_row; ++i)
		screen[i] = (Char*)calloc(size.pos_col, sizeof(Char));
	scrSize = size;

	for (int i = 0; i < size.pos_row; ++i)
		for (int j = 0; j < size.pos_col; ++j)
			screen[i][j] = { ' ', gray, black };

	flush();
}

void Common::Console::exit()
{
	for (int i = 0; i < scrSize.pos_row; ++i)
		free(screen[i]);
	free(screen);
}

Console::Char Common::Console::get(pos2hu from)
{
	return screen[from.pos_row][from.pos_col];
}

Console::Char *Common::Console::get(pos2hu from, pos2hu to)
{
	int lx = (to.pos_row - from.pos_row), ly = (to.pos_col - from.pos_col);
	Char *pch = new Char[lx * ly];
	if(pch == 0) return nullptr;
	for (int i = 0, k = 0; i < lx; ++i)
		for (int j = 0; j < ly; ++j, ++k)
			pch[k] = screen[i][j];
	return pch;
}

void Common::Console::put(pos2hu where, Char what, bool bFlush)
{
	screen[where.pos_row][where.pos_col] = what;
	if (bFlush)
	{
		SetColor(what.bg, what.fg);
		putchar(what.code);
		fflush(stdout);
	}
}

void Common::Console::put(pos2hu from, pos2hu to, Char *what, size_t size, bool bFlush)
{
	int lx = (to.pos_row - from.pos_row), ly = (to.pos_col - from.pos_col);
	for (int i = 0, k = 0; i <= lx && k < size; ++i)
		for (int j = 0; j <= ly && k < size; ++j, ++k)
			put({ (ushort)(ly + from.pos_row), (ushort)(lx = from.pos_col) }, what[k], bFlush);
}

void Common::Console::setcurpos(pos2hu where)
{
	curPos = where;
#ifdef _WIN32
	COORD pos;
	pos.X = where.pos_x;
	pos.Y = where.pos_y;
	::SetConsoleCursorPosition(::GetStdHandle(STD_OUTPUT_HANDLE), pos);
#else
	printf("\x1b[%i;%iH", where.pos_row, where.pos_col);
#endif
}

Common::pos2hu Common::Console::getcurpos()
{
	return curPos;
}

void Common::Console::flush()
{
	Color bg = none, fg = none;
	for (int i = 0; i < scrSize.pos_row; ++i)
	{
		for (int j = 0; j < scrSize.pos_col; ++j)
		{
			if (bg != screen[i][j].bg && fg != screen[i][j].fg)
			{
				bg = screen[i][j].bg;
				fg = screen[i][j].fg;
				SetColor(bg, fg);
			}
			setcurpos({ (ushort)i, (ushort)j });
			putchar(screen[i][j].code);
		}
	}
	fflush(stdout);
}

char Common::Console::getch()
{
	char ch;
	ch = (char)::_getch();
	return ch;
}

char * Common::Console::getstr(int size)
{
	char *pch = new char[size];
	for (int i = 0; i < size; ++i)
		pch[i] = getch();
	return pch;
}
