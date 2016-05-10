#include "win_console_helper.h"

WinConsoleHelper wch;

void WinConsoleHelper::SetPrintColor(WinConsoleHelper::Color textColor, WinConsoleHelper::Color backColor)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), GetColorCode(textColor, backColor));
}

void WinConsoleHelper::SetColor(WinConsoleHelper::Color textColor, WinConsoleHelper::Color backColor, int x, int y, int count)
{
	COORD startPos = { (SHORT)x, (SHORT)y };
	DWORD garbage;
	FillConsoleOutputAttribute(hStdOut, GetColorCode(textColor, backColor), count, startPos, &garbage);
}

void WinConsoleHelper::FillCharacter(char fillChar, int x, int y, int count)
{
	COORD startPos = { (SHORT)x, (SHORT)y };
	DWORD garbage;
	FillConsoleOutputCharacter(hStdOut, fillChar, count, startPos, &garbage);
}

void WinConsoleHelper::ClearScreen()
{
	FillCharacter(' ', 0, 0, 80 * 25);
	SetColor(Default, None, 0, 0, 80 * 25);
}

void WinConsoleHelper::ShowConsoleCursor(bool showFlag)
{
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(hStdOut, &cursorInfo);
	cursorInfo.bVisible = showFlag;
	SetConsoleCursorInfo(hStdOut, &cursorInfo);
}

const HANDLE& WinConsoleHelper::GetStdOutHandle()
{
	return hStdOut;
}

int WinConsoleHelper::GetCursorPosX()
{
	UpdateCSBI();
	return csbi.dwCursorPosition.X;
}

int WinConsoleHelper::GetCursorPosY()
{
	UpdateCSBI();
	return csbi.dwCursorPosition.Y;
}

void WinConsoleHelper::UpdateCSBI()
{
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
}

WORD WinConsoleHelper::GetColorCode(WinConsoleHelper::Color textColor, WinConsoleHelper::Color backColor)
{
	return textColor + (backColor << 4);
}