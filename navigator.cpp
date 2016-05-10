#include <iostream>
#include <conio.h>
#include "navigator.h"
#include "win_console_helper.h"

// Fungsi untuk mentoggle highlight warna (seperti diselect)
void ToggleHighlight(bool on, int lineNumber)
{
	wch.SetColor(on ? wch.None : wch.BrightGreen, on ? wch.BrightGreen : wch.None, 0, lineNumber, 80);
}

// Fungsi menghitung jumlah baris yang dibutuhkan untuk string yang dimasukkan (Asumsi 80 = max karakter dalam 1 baris di console)
int CountLines(std::string &input)
{
	// Lines adalah jumlah baris, sedangkan charLine adalah jumlah karakter yang dihitung dalam 1 baris
	int lines = 1;
	int charLine = 0;
	for (std::size_t i = 0; i < input.length(); i++)
	{
		// Jika ditemukan tanda newline (enter), langsung tambahkan total baris dan reset charLine
		if (input[i] == '\n')
		{
			lines++;
			charLine = 0;
		}
		// Hitung jumlah karakter dalam 1 baris, dan ketika mencapai 80, tambahkan total baris
		else
		{
			charLine++;
			if (charLine >= 80)
			{
				charLine = 0;
				lines++;
			}
		}
	}
	return lines;
}

// Menambah pilihan dalam menu navigator (MAX 10)
void Navigator::AddOption(std::string label)
{
	// Maksimuk option 10, jadi langsung saja set ke arraynya
	arrayOption[optionCount].Label = label;

	// Cek jika patokan input mulai dari 0 atau 1
	if (optionCount == 0)
	{
		arrayOption[optionCount].returnValue = (isStartZero) ? 0 : 1;
	}
	else
	{
		arrayOption[optionCount].returnValue = arrayOption[optionCount - 1].returnValue + 1;
	}
	optionCount++;
}

// Mulai proses menunggu input untuk navigasi 
int Navigator::GetInput()
{
	// Cek dan ubah patokan input pilihan terakhir berdasarkan isLastUnique
	if (isLastUnique)
	{
		arrayOption[optionCount - 1].returnValue = (isStartZero) ? 9 : 0;
	}

	// Set print console warna hijau
	wch.SetPrintColor(wch.BrightGreen, wch.None);

	// Cek apakah diset untuk print pada posisi cursor, atau ditentukan
	if (startLine >= 0)
	{
		// Kalau ditentukan, langsung pindah ke tempat yang dituju
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, (SHORT)startLine });
	}
	else if (startLine == STARTLINE_CURSORPOS)
	{
		// Print di posisi cursor sekarang
		startLine = wch.GetCursorPosY();
	}
	else
	{
		// Print di bagian bawah layar console, otomatis akan dipaskan di paling bawah console (Asumsi tinggi console 25)
		startLine = 25 - CountLines(header) - optionCount;
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, (SHORT)startLine });
	}

	// Print header
	std::cout << header << std::endl;

	// Print seluruh pilihan yang ada
	int optionStart = wch.GetCursorPosY();
	for (int i = 0; i < optionCount; i++)
	{
		std::cout << " [" << arrayOption[i].returnValue << "] " << arrayOption[i].Label;
		if (i < optionCount - 1) std::cout << std::endl;
	}

	// Reset lineUsed used (Digunakan untuk proses clear)
	lineUsed = (optionStart - startLine) + optionCount;

	// Mulai loop untuk menunggu input user
	// Selected adalah index (mulai dari 0) yang menjadi penunjuk ke baris yang dituju (di highlight)
	int selected = 0;
	while (true)
	{
		// Highlight yang diselect terlebih dahulu
		ToggleHighlight(true, optionStart + selected);

		// Dapatkan input dari keyboard
		char input = _getch();

		// Setelah beberapa percobaan, diketahui bahwa dengan menekan tombol arrow,
		// getch() terpanggil 2x, dengan return pertamanya adalah alpha (ASCII 224)
		// dan return keduanya adalah P, H, M, K
		if (input == (char)224)
		{
			// Jika terdeteksi input arrow, maka matikan toggle highlight selected sebelumnya
			ToggleHighlight(false, optionStart + selected);

			// Ambil char buffer kedua dari arrow
			switch (_getch())
			{ 
				// Arrow bawah
				case 'P':
				{
					selected++;
					break;
				}
				// Arrow atas
				case 'H':
				{
					selected--;
					break;
				}

				// Arrow kanan
				case 'M':
				{
					if (RefPageView != nullptr)
					{
						if (RefPageView->IsActive)
						{
							RefPageView->TurnPageRight();
						}
					}
					break;
				}

				// Arrow kiri
				case 'K':
				{
					if (RefPageView != nullptr)
					{
						if (RefPageView->IsActive)
						{
							RefPageView->TurnPageLeft();
						}
					}
					break;
				}
			}

			// Buat selected circular agar kece
			selected += optionCount;
			selected %= optionCount;
		}

		// Untuk input enter, return hasil Navigator ini
		else if (input == '\r')
		{
			return ReturnInput(selected, optionStart + selected);
		}

		// Untuk input angka secara langsung
		else if (isdigit(input))
		{
			// Cek jika angka yang diinput ada dalam pilihan
			int digitPressed = input - '0';
			for (int i = 0; i < optionCount; i++)
			{
				if (arrayOption[i].returnValue == digitPressed)
				{
					ToggleHighlight(false, optionStart + selected);
					return ReturnInput(i, optionStart + selected);
				}
			}
		}
	}
}

// Balikkan input dari Navigator ini
// Menerima int sebagai index dari array option di Navigator ini
int Navigator::ReturnInput(int input, int selectedLine)
{
	// Hapus dari print menu
	ClearInterface();

	// Matikan highlight
	ToggleHighlight(false, selectedLine);

	// Return value yang dimaksud
	return arrayOption[input].returnValue;
}

// Hapus seluruh karakter yang dipakai oleh Navigator ini
void Navigator::ClearInterface()
{
	wch.FillCharacter(' ', 0, startLine, lineUsed * 80);
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, (SHORT)(wch.GetCursorPosY() - lineUsed) });
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Bagian dibawah ini adalah untuk PagedNavigator, yaitu turunan dari Navigator biasa, bisa menampung jumlah option //
// tanpa batas dan option-option ini dapat ditampilkan dengan bentuk halaman-halaman yang bisa dinavigasikan dengan //
// arrow kanan-kiri                                                                                                 //
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Menambah pilihan dalam menu navigator, option yang dimaksud adalah option Listed, yang artinya
// akan ditampilkan dalam bentuh halaman-halaman tanpa batas
void PagedNavigator::AddOption(std::string label)
{
	// Karena tidak ada batas, digunakan Vector dari STL C++
	listedOptions.push_back(Option());
	listedOptions[listedCount].Label = label;
	listedOptions[listedCount].returnValue = listedCount + 1;
	listedCount++;
}

// Menambah pilihan dalam menu navigator, option yang dimaksud adalah option Fixed, yang artinya
// akan ditampilkan di bagian paling bawah tiap halaman, MAX 9
void PagedNavigator::AddFixedOption(std::string label)
{
	fixedOptions[fixedCount].Label = label;
	fixedCount++;
}

int PagedNavigator::GetInput()
{
	// Urutkan return value pada fixedOptions
	for (int i = 0; i < fixedCount; i++)
	{
		fixedOptions[i].returnValue = i - fixedCount + 1;
	}

	// Dapatkan data dasar untuk memudahkan operasi paging (peng-halaman? :x )
	// listedPerPage : jumlah option tiap page
	// pageCount	 : jumlah page
	listedPerPage = 10 - fixedCount;
	pageCount = 1 + listedCount / listedPerPage;

	// Set print console warna hijau
	wch.SetPrintColor(wch.BrightGreen, wch.None);

	// Cek apakah diset untuk print pada posisi cursor, atau ditentukan
	if (startLine >= 0)
	{
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, (SHORT)startLine });
	}
	else if (startLine == STARTLINE_CURSORPOS)
	{
		startLine = wch.GetCursorPosY();
	}
	else
	{
		startLine = 25 - CountLines(header) - listedPerPage - fixedCount - 1;
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, (SHORT)startLine });
	}

	// Print header
	std::cout << header << std::endl;

	// Dapatkan letak dimana pilihan-pilihan akan diprint
	int optionStart = wch.GetCursorPosY();

	// Reset lineUsed used
	lineUsed = (optionStart - startLine) + listedPerPage + fixedCount;

	// Selected adalah index (mulai dari 0) yang menjadi penunjuk ke baris yang dituju (di highlight)
	int selected = 0;

	// Boolean untuk menandai apakah perintah terakhir adalah perubahan halaman
	bool pageChange = false;

	// Print dulu semua option pada halaman pertama ini
	for (int i = 0; i < listedPerPage; i++)
	{
		if (currentPage * listedPerPage + i >= listedCount);
		else std::cout << " [" << i + 1 << "] " << listedOptions[currentPage * listedPerPage + i].Label;
		std::cout << std::endl;
	}

	// Print pembatas antara listed dengan fixed
	std::cout << "-----";
	wch.SetPrintColor(wch.None, wch.BrightGreen);
	std::cout << " Page ";
	std::cout << currentPage + 1 << "/" << pageCount << " ";
	wch.SetPrintColor(wch.BrightGreen, wch.None);
	std::cout << "-----------------------------------------------------------------";

	// Print fixed optionnya
	for (int i = 0; i < fixedCount; i++)
	{
		std::cout << " [" << (i == fixedCount - 1 ? 0 : listedPerPage + i + 1) << "] " << fixedOptions[i].Label;
		if (i != fixedCount - 1) std::cout << std::endl;
	}

	// Mulai loop untuk menunggu input user
	while (true)
	{
		// Jika terakhir dilakukan perubahan halaman
		if (pageChange)
		{
			// Hapus interface
			ClearInterface();

			// Print header
			std::cout << header << std::endl;

			// Print semua option pada halaman ini
			for (int i = 0; i < listedPerPage; i++)
			{
				if (currentPage * listedPerPage + i < listedCount) 
					std::cout << " [" << i + 1 << "] " << listedOptions[currentPage * listedPerPage + i].Label;
				std::cout << std::endl;
			}

			// Print pembatas antara listed dengan fixed
			std::cout << "-----";
			wch.SetPrintColor(wch.None, wch.BrightGreen);
			std::cout << " Page ";
			std::cout << currentPage + 1 << "/" << pageCount << " ";
			wch.SetPrintColor(wch.BrightGreen, wch.None);
			std::cout << "-----------------------------------------------------------------";
			
			// Print fixed optionnya
			for (int i = 0; i < fixedCount; i++)
			{
				std::cout << " [" << (i == fixedCount - 1 ? 0 : listedPerPage + i + 1) << "] " << fixedOptions[i].Label;
				if (i != fixedCount - 1) std::cout << std::endl;
			}

			// Jika selected sebelumnya menunjuk ke entry yang valid, tetapi di halaman sekarang menjadi tidak valid,
			// maka ubah selected ke listed paling terakhir
			if (listedPerPage * currentPage + selected >= listedCount && selected < listedPerPage)
			{
				selected = (listedCount % listedPerPage) - 1;
			}

			// Set flag pagechange kembali ke false
			pageChange = false;
		}

		// Highlight yang diselect terlebih dahulu
		ToggleHighlight(true, optionStart + selected + (selected >= listedPerPage ? 1 : 0));

		// Setelah beberapa percobaan, diketahui bahwa dengan menekan tombol arrow,
		// getch() terpanggil 2x, dengan return pertamanya adalah alpha (ASCII 224)
		// dan return keduanya adalah P, H, M, K
		char input = _getch();

		// Jika diinput arrow
		if (input == (char)224)
		{
			// matikan highlight terakhir (Jika fixed yang dihighlight, +1)
			ToggleHighlight(false, optionStart + selected + (selected >= listedPerPage ? 1 : 0));

			// Ambil nilai buffer kedua dari arrow key
			char lastCh = _getch();

			// Switching
			switch (lastCh)
			{
				// Arrow bawah	
				case 'P':
				{
					selected++;
					break;
				}

				// Arrow atas
				case 'H':
				{
					selected--;
					break;
				}

				// Arrow kanan
				case 'M':
				{
					currentPage++;
					pageChange = true;
					break;
				}

				// Arrow kiri
				case 'K':
				{
					currentPage--;
					pageChange = true;
					break;
				}
			}

			// Buat selected dan currentPage circular
			selected += listedPerPage + fixedCount;
			selected %= listedPerPage + fixedCount;
			currentPage += pageCount;
			currentPage %= pageCount;

			// Untuk memperbaiki jika pada halaman ini list tidak full (di halaman terakhir)
			// agar selected dapat 'lompat' melewati entry kosong dengan baik
			if (!pageChange && currentPage * listedPerPage + selected >= listedCount && selected < listedPerPage)
			{
				if (lastCh == 'H') selected = (listedCount % listedPerPage) - 1;
				else selected = listedPerPage;
			}
		}

		// Untuk input enter
		else if (input == '\r')
		{
			// Dipisahkan antara input listed dan fixed
			if (selected < listedPerPage) return ReturnInput(listedPerPage * currentPage + selected, optionStart + selected, true);
			else return ReturnInput(selected - listedPerPage, optionStart + selected + 1);
		}

		// Untuk input digit langsung
		else if (isdigit(input))
		{
			int digitPressed = input - '0';

			// Dipisahkan juga antara listed dengan fixed
			if (digitPressed <= listedPerPage && digitPressed > 0)
			{
				if (currentPage * listedPerPage + digitPressed <= listedCount)
				{
					return ReturnInput(listedPerPage * currentPage + digitPressed - 1, optionStart + digitPressed - 1, true);
				}
			}
			else if (digitPressed < listedPerPage + fixedCount || digitPressed == 0)
			{
				return ReturnInput(digitPressed - 1 - listedPerPage, optionStart + digitPressed);
			}
		}
	}
}

// Return input
int PagedNavigator::ReturnInput(int input, int selectedLine, bool listed)
{
	if (listed)
	{
		ClearInterface();
		ToggleHighlight(false, selectedLine);
		return listedOptions[input].returnValue;
	}
	else
	{
		ClearInterface();
		ToggleHighlight(false, selectedLine);
		return fixedOptions[input].returnValue;
	}
}