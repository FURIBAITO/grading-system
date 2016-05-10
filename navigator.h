#pragma once

#include <string>
#include <vector>

#include "pageview.h"

// Object yang mewakili sebuah daftar dari pilihan dalam menu, termasuk dengan input selectionnya
// Membutuhkan #include "win_console_helper.h"
// Setiap Navigator memiliki maksimum 10 option
class Navigator
{

public:

	// Konstanta untuk membuat navigator ini memulai dari posisi terakhir cursor console
	static const int STARTLINE_CURSORPOS = -1;
	static const int STARTLINE_FITBOTTOM = -2;
	
	// Initialisasi variabel
	Navigator(std::string header = "", int startLine = STARTLINE_CURSORPOS, bool isStartZero = false, bool isLastUnique = false) : startLine(startLine), isStartZero(isStartZero), isLastUnique(isLastUnique), header(header), optionCount(0), RefPageView(nullptr) {};

	// Fungsi menambahkan Option baru ke Navigator
	void AddOption(std::string label);

	// Fungsi untuk menampilkan Navigator ke console beserta mengaktifkan kontrol input
	virtual int GetInput();

	// Fungsi untuk membuang seluruh hasil print dari navigator ini (Header + pilihan-pilihannya)
	// Isi dari Navigator ini tidaklah terbuang
	void ClearInterface();

	// Reference ke PageView aktif (optional)
	PageView* RefPageView;

protected:

	// Object sebagai satu pilihan option dari daftar option
	struct Option
	{
		std::string Label;
		int returnValue;
	};

	// Membalikkan nilai input
	int ReturnInput(int input, int optionStart);

	// Baris dimana pilihan-pilihan akan ditampilkan
	int startLine;

	// String yang akan ditampilkan sebelum option-option keluar
	std::string header;

	// Total baris yang digunakan dalam navigator ini
	int lineUsed;

private:

	// Array dari object option
	Option arrayOption[10];

	// Index dari option terakhir di array
	int optionCount;

	// Apakah pilihan mulai dari 0?
	// (Berfungsi untuk input keyboard dan penampilan, tetapi indexnya sama seperti biasa)
	bool isStartZero;

	// Apakah pilihan terakhir itu diberi angka 0? 
	// (Berfungsi untuk input keyboard dan penampilan, tetapi indexnya sama seperti biasa)
	// Jika startZero, maka angka Option terakhir adalah 9
	bool isLastUnique;
};

// Object yang adalah komplek dari Navigator biasa. Object ini memungkinkan untuk membuat Navigator dengan banyak option.
class PagedNavigator : Navigator
{

public:

	// Constructor dari PagedNavigator
	PagedNavigator(std::string header = "", int startLine = STARTLINE_CURSORPOS) : 
		pageCount(0), 
		listedPerPage(0),
		Navigator(header, startLine, false, true) {};

	// Fungsi timpaan menambahkan pilihan ter-paged ke PagedNavigator (Dapat di browse dengan arrow kanan dan kiri)
	void AddOption(std::string label);

	// Fungsi menambahkan pilihan tidak terpaged (Selalu ada tiap page) Akan muncul di bagian paling bawah
	// tiap page.
	void AddFixedOption(std::string label);

	// Memulai menampilkan paged navigator dan menerima input. Return valuenya int, relatif dari angka paling pertama
	// FixedOption akan dibalikkan dengan angka negatif, mulai dari 0 ke arah negatif. Yang terakhir di paling atas
	int GetInput();

private:

	// Membalikkan nilai input
	int ReturnInput(int input, int optionStart, bool raw = false);
	
	// Jumlah page
	int pageCount;

	// Jumlah ListedOption
	int listedCount;
	
	// Jumlah ListedOption dalam satu page
	int listedPerPage;

	// Jumlah fixedOption
	int fixedCount;

	// Page sekarang
	int currentPage;

	// Array dari ListedOption
	std::vector<Option> listedOptions;

	// Array untuk FixedOption
	Option fixedOptions[8];
};
