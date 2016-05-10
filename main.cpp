#include <iostream>
#include <vector>

#include "win_console_helper.h"
#include "navigator.h"
#include "pageview.h"
#include "students.h"

void MainMenu();
void AddMenu();
void ViewMenu();
void SearchMenu();

void AddMenu()
{
	// Buat judul
	wch.SetPrintColor(wch.Yellow, wch.None);
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 24, 4 });
	std::cout << "Kwik Kian Gie School of Business";
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 28, 5 });
	std::cout << "Algoritma dan Pemograman";
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 27, 6 });
	std::cout << "Sistem Penilaian Mahasiswa";

	wch.SetPrintColor(wch.White, wch.None);
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 22, 6 });
	std::cout << "Saat ini sistem mendata " << Students.size() << " mahasiswa";

	wch.SetPrintColor(wch.BrightGreen, wch.None);
	wch.FillCharacter('-', 0, 22, 80);

	std::string name;
	int asg, mid, fin;

	do
	{
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, 23 });
		wch.FillCharacter(' ', 0, 23, 80 * 2);
		std::cout << "Masukkan nama mahasiswa [MAX 42] : ";
		std::ws(std::cin);
		std::getline(std::cin, name);
	} while (name.length() > 42);

	do
	{
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, 23 });
		wch.FillCharacter(' ', 0, 23, 80 * 2);
		std::cout << "Masukkan nilai tugas [0-100] : ";
		std::cin >> asg;
	} while (asg < 0 || asg > 100);

	do
	{
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, 23 });
		wch.FillCharacter(' ', 0, 23, 80 * 2);
		std::cout << "Masukkan nilai mid semester [0-100] : ";
		std::cin >> mid;
	} while (mid < 0 || mid > 100);

	do
	{
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, 23 });
		wch.FillCharacter(' ', 0, 23, 80 * 2);
		std::cout << "Masukkan nilai final semester [0-100] : ";
		std::cin >> fin;
	} while (fin < 0 || fin > 100);

	AddNewStudent(name);
	Students[Students.size() - 1].SubmitScore(asg, mid, fin);
	wch.ClearScreen();
	return;
}

void ViewMenu()
{
	while (true)
	{
		// Print daftar
		PageView daftar("Daftar mahasiswa", 0, 15);
		std::vector<Student> StudentsSorted = Students;
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
		for (unsigned int i = 0; i < StudentsSorted.size(); i++)
		{
			std::string item;
			item.append(StudentsSorted[i].Name);
			item.append("\t- ");
			item.append(StudentsSorted[i].ID);
			item.append("  Tugas: ");
			item.append(std::to_string(StudentsSorted[i].Score.assignment));
			item.append("  Mid: ");
			item.append(std::to_string(StudentsSorted[i].Score.mid));
			item.append("  Final: ");
			item.append(std::to_string(StudentsSorted[i].Score.final));
			item.append("  GRADE -> ");
			item.append(GradeLevels[(int)StudentsSorted[i].GetGrade()].Label);
			daftar.AddItem(item);
		}
		daftar.Show();

		Navigator mainMenuNav(" :: MENU UTAMA -> VIEW", Navigator::STARTLINE_FITBOTTOM, false, true);
		mainMenuNav.RefPageView = &daftar;
		mainMenuNav.AddOption("Kembali");

		int input = mainMenuNav.GetInput();
		wch.ClearScreen();
		switch (input)
		{
			default: return;
		}
	}
}

void SearchMenu()
{
	std::string input;
	
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 0, 23 });
	wch.FillCharacter(' ', 0, 23, 80 * 2);
	std::cout << "Masukkan nama mahasiswa yang ingin dicari : ";
	std::cin.ignore();
	std::getline(std::cin, input);
	SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 10, 8 });

	for (int i = 0; i < Students.size(); i++)
	{
		if (Students[i].Name.find(input) != std::string::npos)
		{
			std::cout << "Ditemukan!" << '\n';
			std::cout << " \t\t\tNIM\t: " << Students[i].ID << std::endl;
			std::cout << " \t\t\tTugas\t: " << Students[i].Score.assignment << std::endl;
			std::cout << " \t\t\tMid\t: " << Students[i].Score.mid << std::endl;
			std::cout << " \t\t\tFinal\t: " << Students[i].Score.final << std::endl;
			std::cout << " \t\t\tGrade\t: " << GradeLevels[(int)Students[i].GetGrade()].Label << std::endl;
			getchar();
			return;
		}
	}
	std::cout << "Tidak ditemukan.." << '\n';
	getchar();
}

void MainMenu()
{
	while (true)
	{
		wch.ClearScreen();

		// Buat judul
		wch.SetPrintColor(wch.Yellow, wch.None);
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 24, 6 });
		std::cout << "Kwik Kian Gie School of Business";
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 28, 7 });
		std::cout << "Algoritma dan Pemograman";
		SetConsoleCursorPosition(wch.GetStdOutHandle(), COORD{ 27, 8 });
		std::cout << "Sistem Penilaian Mahasiswa";

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
	wch.ShowConsoleCursor(false);
	
	// Ke main menu
	MainMenu();
}

