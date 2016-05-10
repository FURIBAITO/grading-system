#pragma once

#include <string>
#include <vector>

class PageView 
{
public:

	// Konstanta untuk membuat navigator ini memulai dari posisi terakhir cursor console
	static const int STARTLINE_CURSORPOS = -1;

public:
	PageView(std::string header = "", int startLine = STARTLINE_CURSORPOS, int itemPerPage = 10) :
		pageCount(0),
		itemPerPage(itemPerPage),
		header(header) {}

	// Apakah aktif
	bool IsActive;

	// Tambahkan item untuk ditampilkan
	void AddItem(std::string item);

	// Memulai menampilkan
	void Show();

	// Menghapus tampilan
	void Clear();

	// Pindah page
	void TurnPageRight();
	void TurnPageLeft();

private:

	// Baris dimana pilihan-pilihan akan ditampilkan
	int startLine;

	// String yang akan ditampilkan sebelum option-option keluar
	std::string header;

	// Total baris yang digunakan dalam navigator ini
	int lineUsed;

	// Jumlah page
	int pageCount;

	// Jumlah item
	int itemCount;

	// Item tiap halaman
	int itemPerPage;

	// Page sekarang
	int currentPage;

	// Array dari ListedOption
	std::vector<std::string> listedItems;
};
