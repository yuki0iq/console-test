#include "common.h"

#include <iostream>
using namespace std;
using namespace Common;

void SetColorsCHARP(Console::Char *pch, int size, Console::Color fg, Console::Color bg)
{
	for (int i = 0; i < size; ++i)
	{
		pch[i].fg = fg;
		pch[i].bg = bg;
	}
}

int main(int argc, char **argv)
{
	/*Console::init({ 80, 24 });
	Console::put({ 0, 0 }, { (ushort)strlen(pch), 24 }, pch2, strlen(pch), true);
	char c = Console::getch();*/

	const char *pch;
	Console::Char *pch2;

	pch = "Green On YelloW!";
	pch2 = Console::fromchar(pch, strlen(pch));
	SetColorsCHARP(pch2, strlen(pch), Console::green, Console::dyellow);
	Console::put({ 0, 2 }, { (ushort)strlen(pch), 24 }, pch2, strlen(pch), true);
	
	Console::getch();

	return 0;
}
