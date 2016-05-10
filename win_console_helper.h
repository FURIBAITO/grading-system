#pragma once

#include <Windows.h>
#include <string>

class WinConsoleHelper
{
public:
	// Daftar dari seluruh warna yang bisa digunakan windows
	enum Color { None, Blue, Green, Cyan, Red, Purple, DarkYellow, Default, Grey, BrightBlue, BrightGreen, BrightCyan, BrightRed, Pink, Yellow, White };

	// Constructor untuk class WinConsoleHelper
	WinConsoleHelper() : hStdOut(GetStdHandle(STD_OUTPUT_HANDLE)) { GetConsoleScreenBufferInfo(hStdOut, &csbi); }

	// Set warna dari text yang akan diprint oleh console
	void SetPrintColor(Color textColor, Color backColor);

	// Set warna background di posisi tertentu sebanyak count
	void SetColor(Color textColor, Color backColor, int posX, int posY, int count);

	// Penuhi letak console dengan karakter
	void FillCharacter(char fillChar, int posX, int posY, int count);

	// Clearscreen
	void ClearScreen();

	// Show/hide console
	void ShowConsoleCursor(bool showFlag);


	// Dapatkan handle StdOut
	const HANDLE& GetStdOutHandle();

	// Dapatkan letak cursor di console
	int GetCursorPosX();
	int GetCursorPosY();

private:
	// Dapatkan buffer info baru
	void UpdateCSBI();

	// Variabel handle ke standard output stream
	HANDLE hStdOut;

	// Variabel informasi buffer pada console screen
	CONSOLE_SCREEN_BUFFER_INFO csbi;

	// Nilai WORD dari warna yang dipilih
	WORD GetColorCode(Color textColor, Color backColor);
};

extern WinConsoleHelper wch;