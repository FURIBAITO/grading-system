#include <iostream>
#include <conio.h>
#include "pageview.h"
#include "win_console_helper.h"

// Menambah item
void PageView::AddItem(std::string label)
{
	listedItems.push_back(label);
	itemCount++;
}


void PageView::Show()
{
	// pageCount	 : jumlah page
	pageCount = 1 + itemCount / itemPerPage;

	// Cek apakah diset untuk print pada posisi cursor, atau ditentukan
	if (startLine >= 0)
	{
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, (SHORT)startLine });
	}
	else
	{
		startLine = wch.GetCursorPosY();
	}

	// Print header
	wch.SetPrintColor(wch.BrightCyan, wch.None);
	std::cout << header << std::endl;

	// Dapatkan letak dimana item akan diprint
	int optionStart = wch.GetCursorPosY();

	// Reset lineUsed used
	lineUsed = (optionStart - startLine) + itemPerPage;

	// Selected adalah index (mulai dari 0) yang menjadi penunjuk ke baris yang dituju (di highlight)
	int selected = 0;

	// Boolean untuk menandai apakah perintah terakhir adalah perubahan halaman
	bool pageChange = false;

	// Print semua item pada halaman pertama ini
	wch.SetPrintColor(wch.White, wch.None);
	for (int i = 0; i < itemPerPage; i++)
	{
		if (currentPage * itemPerPage + i >= itemCount);
		else std::cout << listedItems[currentPage * itemPerPage + i];
		std::cout << std::endl;
	}

	// Print pembatas
	wch.SetPrintColor(wch.BrightGreen, wch.None);
	std::cout << "-----";
	wch.SetPrintColor(wch.None, wch.BrightGreen);
	std::cout << " Hal. ";
	std::cout << currentPage + 1 << "/" << pageCount << " ";
	wch.SetPrintColor(wch.BrightGreen, wch.None);
	std::cout << "-----------------------------------------------------------------";

	IsActive = true;
}

void PageView::Clear()
{
	wch.FillCharacter(' ', 0, startLine, lineUsed * 80);
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, (SHORT)(wch.GetCursorPosY() - lineUsed) });

	IsActive = false;
}

// Pindah page
void PageView::TurnPageRight()
{
	Clear();
	currentPage++;
	currentPage += pageCount;
	currentPage %= pageCount;
	Show();
}

void PageView::TurnPageLeft()
{
	Clear();
	currentPage--;
	currentPage += pageCount;
	currentPage %= pageCount;
	Show();
}