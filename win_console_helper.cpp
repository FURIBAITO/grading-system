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

void WinConsoleHelper::SetPosition(int x, int y)
{
	SetConsoleCursorPosition(hStdOut, COORD{ (SHORT)x, (SHORT)y });
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

void WinConsoleHelper::SetConsoleSize(int x, int y)
{
	UpdateCSBI();
	COORD windowSize = { csbi.srWindow.Right - csbi.srWindow.Left + 1, csbi.srWindow.Bottom - csbi.srWindow.Top + 1 };
	if (windowSize.X > x || windowSize.Y > y)
	{
		SMALL_RECT info =
		{
			0,
			0,
			x < windowSize.X ? x - 1 : windowSize.X - 1,
			y < windowSize.Y ? y - 1 : windowSize.Y - 1
		};

		SetConsoleWindowInfo(hStdOut, TRUE, &info);
	}
	SMALL_RECT info = { 0, 0, x - 1, y - 1 };
	SetConsoleScreenBufferSize(hStdOut, { (SHORT)x, (SHORT)y });
	SetConsoleWindowInfo(hStdOut, TRUE, &info);
}

unsigned char WinConsoleHelper::GetCenterPosition(int strSize, int consoleWidth)
{
	return (consoleWidth - strSize) / 2;
}


void WinConsoleHelper::UpdateCSBI()
{
	GetConsoleScreenBufferInfo(hStdOut, &csbi);
}

WORD WinConsoleHelper::GetColorCode(WinConsoleHelper::Color textColor, WinConsoleHelper::Color backColor)
{
	return textColor + (backColor << 4);
}