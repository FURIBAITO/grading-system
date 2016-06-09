#include <iostream>
#include <vector>
#include <fstream>

#include "win_console_helper.h"
#include "navigator.h"
#include "pageview.h"
#include "students.h"

#define CONSOLE_WINDOW_CELL_HEIGHT	25
#define CONSOLE_WINDOW_CELL_WIDTH	80

void MainMenu();
void AddMenu();
void ViewMenu();
void SearchMenu();

void PrintHeader()
{
	wch.ClearScreen();
	wch.SetPosition(0, 0);
	wch.SetPrintColor(wch.BrightCyan, wch.None);
	std::cout << std::endl;
	std::cout << "  Program Penilaian" << std::endl;
	wch.SetPrintColor(wch.Cyan, wch.None);
	std::cout << "  Project Algoritma dan Pemograman 2 - Teknik Informatika Kwik Kian Gie" << std::endl;
	wch.SetPrintColor(wch.BrightCyan, wch.None);
	std::cout << std::endl;
	wch.SetPrintColor(wch.Green, wch.None);
	std::cout << "  Gunakan ";
	wch.SetPrintColor(wch.BrightGreen, wch.None);
	std::cout << "ARROW KEYS";
	wch.SetPrintColor(wch.Green, wch.None);
	std::cout << " untuk navigasi menu di paling bawah." << std::endl;
	wch.SetPrintColor(wch.White, wch.None);
	std::cout << " =============================================================================" << std::endl;
	wch.ShowConsoleCursor(false);
}


void AddMenu()
{
	PrintHeader();

	std::string name;
	int asg, mid, fin;

	wch.ShowConsoleCursor(true);
	wch.SetPrintColor(wch.BrightGreen, wch.None);

	do
	{
		std::cout << "  Masukkan nama mahasiswa [MAX 42]" << std::endl;
		wch.SetPrintColor(wch.Default, wch.None);
		std::cout << "  ";
		wch.SetPrintColor(wch.None, wch.BrightGreen);
		std::cout << " Input >> ";
		wch.SetPrintColor(wch.BrightGreen, wch.None);
		std::cout << " ";

		std::ws(std::cin);
		std::getline(std::cin, name);
		std::cout << std::endl;
	} while (name.length() > 42);

	do
	{
		std::cout << "  Masukkan nilai tugas [0-100]" << std::endl;
		wch.SetPrintColor(wch.Default, wch.None);
		std::cout << "  ";
		wch.SetPrintColor(wch.None, wch.BrightGreen);
		std::cout << " Input >> ";
		wch.SetPrintColor(wch.BrightGreen, wch.None);
		std::cout << " ";
		std::cin >> asg;
		std::cout << std::endl;
	} while (asg < 0 || asg > 100);

	do
	{
		std::cout << "  Masukkan nilai mid semester [0-100]" << std::endl;
		wch.SetPrintColor(wch.Default, wch.None);
		std::cout << "  ";
		wch.SetPrintColor(wch.None, wch.BrightGreen);
		std::cout << " Input >> ";
		wch.SetPrintColor(wch.BrightGreen, wch.None);
		std::cout << " ";
		std::cin >> mid;
		std::cout << std::endl;
	} while (mid < 0 || mid > 100);

	do
	{
		std::cout << "  Masukkan nilai final semester [0-100] : " << std::endl;
		wch.SetPrintColor(wch.Default, wch.None);
		std::cout << "  ";
		wch.SetPrintColor(wch.None, wch.BrightGreen);
		std::cout << " Input >> ";
		wch.SetPrintColor(wch.BrightGreen, wch.None);
		std::cout << " ";
		std::cin >> fin;
		std::cout << std::endl;
	} while (fin < 0 || fin > 100);

	AddNewStudent(name);
	Students[Students.size() - 1].SubmitScore(asg, mid, fin);
	AppendStudentDB(Students[Students.size() - 1]);
	return;
}

void ViewMenu()
{
	enum class SortMode {NameAscending = 1, NameDescending, NIMAscending, NIMDescending, GradeAscending, GradeDescending};
	SortMode currentSortMode = SortMode::NameAscending;
	
	while (true)
	{
		PrintHeader();
		
		// Print daftar
		PageView daftar("  | NAMA                                     |   NIM    |TGS|MID|FIN| Grade |\n  +------------------------------------------+----------+---+---+---+-------+", 6, 7);
		
		// Buat daftar khusus untuk elemen yang telah disortir
		std::vector<Student> StudentsSorted = Students;

		// Pisah antara mode sortir
		switch (currentSortMode)
		{
			case SortMode::NameAscending :
			{
				for (unsigned int i = 0; i < StudentsSorted.size() - 1; i++)
				{
					for (unsigned int j = 0; j < StudentsSorted.size() - i - 1; j++)
					{
						if (StudentsSorted[j].Name > StudentsSorted[j + 1].Name)
						{
							iter_swap(StudentsSorted.begin() + j, StudentsSorted.begin() + j + 1);
						}
					}
				}
				break;
			}

			case SortMode::NameDescending :
			{
				for (unsigned int i = 0; i < StudentsSorted.size() - 1; i++)
				{
					for (unsigned int j = 0; j < StudentsSorted.size() - i - 1; j++)
					{
						if (StudentsSorted[j].Name < StudentsSorted[j + 1].Name)
						{
							iter_swap(StudentsSorted.begin() + j, StudentsSorted.begin() + j + 1);
						}
					}
				}
				break;
			}

			case SortMode::NIMAscending:
			{
				for (unsigned int i = 0; i < StudentsSorted.size() - 1; i++)
				{
					for (unsigned int j = 0; j < StudentsSorted.size() - i - 1; j++)
					{
						if (StudentsSorted[j].ID > StudentsSorted[j + 1].ID)
						{
							iter_swap(StudentsSorted.begin() + j, StudentsSorted.begin() + j + 1);
						}
					}
				}
				break;
			}

			case SortMode::NIMDescending:
			{
				for (unsigned int i = 0; i < StudentsSorted.size() - 1; i++)
				{
					for (unsigned int j = 0; j < StudentsSorted.size() - i - 1; j++)
					{
						if (StudentsSorted[j].ID < StudentsSorted[j + 1].ID)
						{
							iter_swap(StudentsSorted.begin() + j, StudentsSorted.begin() + j + 1);
						}
					}
				}
				break;
			}

			case SortMode::GradeAscending:
			{
				for (unsigned int i = 0; i < StudentsSorted.size() - 1; i++)
				{
					for (unsigned int j = 0; j < StudentsSorted.size() - i - 1; j++)
					{
						if (StudentsSorted[j].GetTotalScore() > StudentsSorted[j + 1].GetTotalScore())
						{
							iter_swap(StudentsSorted.begin() + j, StudentsSorted.begin() + j + 1);
						}
					}
				}
				break;
			}

			case SortMode::GradeDescending:
			{
				for (unsigned int i = 0; i < StudentsSorted.size() - 1; i++)
				{
					for (unsigned int j = 0; j < StudentsSorted.size() - i - 1; j++)
					{
						if (StudentsSorted[j].GetTotalScore() < StudentsSorted[j + 1].GetTotalScore())
						{
							iter_swap(StudentsSorted.begin() + j, StudentsSorted.begin() + j + 1);
						}
					}
				}
				break;
			}
		}

		for (unsigned int i = 0; i < StudentsSorted.size(); i++)
		{
			std::string item(79, ' ');

			// Muatkan nama
			item.replace(2, 1, "|");
			item.replace(4, StudentsSorted[i].Name.size(), StudentsSorted[i].Name);
			item.replace(45, 1, "|");

			// Muatkan NIM
			item.replace(47, StudentsSorted[i].ID.size(), StudentsSorted[i].ID);
			item.replace(56, 1, "|");

			// Muatkan nilai tugas
			item.replace(57, 2, std::to_string(StudentsSorted[i].Score.assignment));
			item.replace(60, 1, "|");

			// Muatkan nilai mid
			item.replace(61, 2, std::to_string(StudentsSorted[i].Score.mid));
			item.replace(64, 1, "|");

			// Muatkan nilai final
			item.replace(65, 2, std::to_string(StudentsSorted[i].Score.final));
			item.replace(68, 1, "|");

			// Muatkan grade
			item.replace(72, 2, GradeLevels[StudentsSorted[i].GetGrade()].Label);
			item.replace(76, 1, "|");

			daftar.AddItem(item);
		}
		daftar.Show();

		Navigator viewNav(" :: MENU UTAMA -> VIEW", Navigator::STARTLINE_FITBOTTOM, false, true);
		viewNav.RefPageView = &daftar;
		viewNav.AddOption("Urutkan berdasarkan nama (MENINGKAT)");
		viewNav.AddOption("Urutkan berdasarkan nama (MENURUN)");
		viewNav.AddOption("Urutkan berdasarkan NIM (MENINGKAT)");
		viewNav.AddOption("Urutkan berdasarkan NIM (MENURUN)");
		viewNav.AddOption("Urutkan berdasarkan nilai (MENINGKAT)");
		viewNav.AddOption("Urutkan berdasarkan nilai (MENURUN)");
		viewNav.AddOption("Kembali");

		int input = viewNav.GetInput();
		if (input == 0) return;
		else
		{
			currentSortMode = (SortMode)input;
			continue;
		}
	}
}

void SearchMenu()
{
	PrintHeader();
	
	std::string input;
	
	wch.SetPosition(0, CONSOLE_WINDOW_CELL_HEIGHT - 4);
	wch.FillCharacter(' ', 0, 23, 80 * 2);
	std::cout << "Gunakan pencarian ini untuk mencari siswa." << std::endl;
	std::cout << "Anda tidak perlu mengetik nama lengkap mahasiswa." << std::endl;
	std::cout << "Cari : ";
	std::ws(std::cin);
	std::getline(std::cin, input);

	// Cari semua entry yang memiliki kata yang diinput, simpan semua index yang ditemukan
	std::vector<int> foundIndex;
	for (std::size_t i = 0; i < Students.size(); i++)
	{
		if (Students[i].Name.find(input) != std::string::npos)
		{
			foundIndex.push_back(i);
		}
	}

	// Jika setidaknya ditemukan 1
	if (foundIndex.size() > 0)
	{
		// Jika hanya ditemukan 1, langsung print yang ditemukan
		if (foundIndex.size() == 1)
		{
			wch.ClearScreen();
			wch.SetPosition(3, 2);
			wch.SetPrintColor(wch.BrightCyan, wch.None);
			std::cout << "Mahasiswa ditemukan!" << '\n';
			wch.SetPrintColor(wch.White, wch.None);
			std::cout << "   Nama                 : " << Students[foundIndex[0]].Name << std::endl;
			std::cout << "   NIM                  : " << Students[foundIndex[0]].ID << std::endl;
			std::cout << "   Nilai Tugas          : " << Students[foundIndex[0]].Score.assignment << std::endl;
			std::cout << "   Nilai Mid-Semester   : " << Students[foundIndex[0]].Score.mid << std::endl;
			std::cout << "   Nilai Final-Semester : " << Students[foundIndex[0]].Score.final << std::endl;
			std::cout << "   Grade                : " << GradeLevels[Students[foundIndex[0]].GetGrade()].Label << std::endl;
		}

		// Jika ditemukan banyak, munculkan pilihan
		else
		{
			std::string header = "Ditemukan ";
			header.append(std::to_string(foundIndex.size()));
			header.append(" entry mahasiswa!\n");
			header.append("Pilih entry data yang ingin anda lihat :\n   +-----------+-----------------------------------------------------+--------+\n   | NIM       | NAMA                                                | NILAI  |\n   +-----------+-----------------------------------------------------+--------+");
			PagedNavigator foundSelect(header, Navigator::STARTLINE_FITBOTTOM);
			for (int i = 0; i < foundIndex.size(); i++)
			{
				// Buat baris entry (kosongkan dulu)
				std::string entryLine(74, ' ');

				// Muatkan NIM
				entryLine.replace(0, Students[foundIndex[i]].ID.size(), Students[foundIndex[i]].ID);
				entryLine.replace(10, 1, "|");

				// Muatkan nama
				entryLine.replace(12, Students[foundIndex[i]].Name.size(), Students[foundIndex[i]].Name);
				entryLine.replace(64, 1, "|");

				// Muatkan nilai
				entryLine.replace(68, GradeLevels[Students[foundIndex[i]].GetGrade()].Label.size(), GradeLevels[Students[foundIndex[i]].GetGrade()].Label);

				// Masukkan ke PagedNavigator
				foundSelect.AddOption(entryLine);
			}
			// Tambahkan opsi kembali
			foundSelect.AddFixedOption("Kembali");

			// Mulai meminta input
			int input = foundSelect.GetInput();
			if (input == 0) return;
			else
			{
				wch.ClearScreen();
				wch.SetPosition(3, 2);
				wch.SetPrintColor(wch.BrightCyan, wch.None);
				std::cout << "Data Mahasiswa" << '\n';
				wch.SetPrintColor(wch.White, wch.None);
				std::cout << "   Nama                 : " << Students[foundIndex[input - 1]].Name << std::endl;
				std::cout << "   NIM                  : " << Students[foundIndex[input - 1]].ID << std::endl;
				std::cout << "   Nilai Tugas          : " << Students[foundIndex[input - 1]].Score.assignment << std::endl;
				std::cout << "   Nilai Mid-Semester   : " << Students[foundIndex[input - 1]].Score.mid << std::endl;
				std::cout << "   Nilai Final-Semester : " << Students[foundIndex[input - 1]].Score.final << std::endl;
				std::cout << "   Grade                : " << GradeLevels[Students[foundIndex[input - 1]].GetGrade()].Label << std::endl;
			}
		}
	}
	else
	{
		wch.SetPosition(wch.GetCenterPosition(sizeof("Tidak ada mahasiswa yang sesuai dengan kata pencarian anda!")), 8);
		wch.SetPrintColor(wch.None, wch.BrightRed);
		std::cout << "Tidak ada mahasiswa yang sesuai dengan kata pencarian anda!";
	}
	getchar();

	/*std::cout << "Ditemukan!" << '\n';
	std::cout << " \t\t\tNIM\t: " << Students[i].ID << std::endl;
	std::cout << " \t\t\tTugas\t: " << Students[i].Score.assignment << std::endl;
	std::cout << " \t\t\tMid\t: " << Students[i].Score.mid << std::endl;
	std::cout << " \t\t\tFinal\t: " << Students[i].Score.final << std::endl;
	std::cout << " \t\t\tGrade\t: " << GradeLevels[Students[i].GetGrade()].Label << std::endl;*/
}

void MainMenu()
{
	while (true)
	{
		PrintHeader();

		// Buat judul
		wch.SetPrintColor(wch.Yellow, wch.None);
		SetConsoleCursorPosition(wch.GetStdOutHandle(), 
								 COORD{ wch.GetCenterPosition(sizeof("Kwik Kian Gie School of Business")) , 10 });
		std::cout << "Kwik Kian Gie School of Business";
		wch.SetPrintColor(wch.White, wch.None);
		SetConsoleCursorPosition(wch.GetStdOutHandle(),
								 COORD{ wch.GetCenterPosition(sizeof("Menu Utama")) , 11 });
		std::cout << "Menu Utama";
		SetConsoleCursorPosition(wch.GetStdOutHandle(),
								 COORD{ wch.GetCenterPosition(sizeof("Saat ini sistem mendata x mahasiswa")) , 12 });

		std::cout << "Saat ini sistem mendata " << Students.size() << " mahasiswa";

		// Buat menu seleksi 
		Navigator mainMenuNav(" :: MENU UTAMA", Navigator::STARTLINE_FITBOTTOM, false, true);
		mainMenuNav.AddOption("Tambah mahasiswa");
		mainMenuNav.AddOption("Lihat daftar mahasiswa");
		mainMenuNav.AddOption("Cari mahasiswa");
		mainMenuNav.AddOption("Keluar program");
		
		int input = mainMenuNav.GetInput();
		wch.ClearScreen();
		switch (input)
		{
			case 1: AddMenu(); break;
			case 2: ViewMenu(); break;
			case 3: SearchMenu(); break;
			default: return;
		}
	}
}

int main()
{
	wch.SetConsoleSize(CONSOLE_WINDOW_CELL_WIDTH, CONSOLE_WINDOW_CELL_HEIGHT);
	ReadStudentDB();
	MainMenu();
}

