#pragma once

#include <string>
#include <vector>
#include <cmath>

// Definisi rasio nilai
#define RATIO_DEFAULT_ASSIGNMENT 0.2
#define RATIO_DEFAULT_MIDTERM 0.4
#define RATIO_DEFAULT_FINALS 0.4

// Daftar dari seluruh range nilai dan huruf serta bobotnya dalam menghitung IPK/IPS
struct GradeLevel
{
	// Karakter yang mewakili tingkatan nilai ini
	std::string Label;

	// Batas minimum agar bisa mencapai tingkat ini
	int Minimum;

	// Nilai dalam bentuk angka IP
	double Value;
};

GradeLevel GradeLevels[] =
{
	{ "A",	 85,	 4.0 },
	{ "A-",	 80,	 3.7 },
	{ "B+",	 77,	 3.3 },
	{ "B",	 73,	 3.0 },
	{ "B-",	 70,	 2.7 },
	{ "C+",	 67,	 2.3 },
	{ "C",	 63,	 2.0 },
	{ "C-",	 60,	 1.7 },
	{ "D+",	 57,	 1.3 },
	{ "D",	 53,	 1.0 },
	{ "D-",	 50,	 0.7 },
	{ "F",	 0,		 0.0 },
};

// Enum untuk memudahkan akses ke GradeLevel array
enum class GradeIndex { A, A_min, B_plus, B, B_min, C_plus, C, C_min, D_plus, D, D_min, F };

// Definisikan tipe data index agar mempermudah klasifikasi, meluaskan batas input dan menghemat memori
typedef unsigned int	STUDENT_INDEX;

// Tambah mahasiswa baru
void AddNewStudent(std::string name);

// Student structure
struct Student
{	
	// Konstant maksimum ukuran data
	int MAXSIZE_ID = 8;
	int MAXSIZE_NAME = 42;
	int MAXSIZE_DATE = 8;

	// string 8 digit sebagai identifikasi mahasiswa yang unik
	std::string ID;

	// String 42 digit untuk 
	std::string Name;

	// Seluruh nilai tercatat dari mahasiswa ini
	struct CourseScore
	{
		// Nilai tugas
		int assignment;

		// Nilai mid semester test (UTS)
		int mid;

		// Nilai final test (UAS)
		int final;
	};
	CourseScore Score;

	// Upload nilai mahasiswa
	void SubmitScore(int assignment, int mid, int final);

	// Nilai keseluruhan (rata-rata total berdasarkan bobot) dari mata kuliah mahasiswa ini
	double GetTotalScore();

	// A/B/C/D/E dari mata kuliah mahasiswa ini
	GradeIndex GetGrade();
};

// Vector array untuk menyimpan data siswa
std::vector<Student> Students;

void AddNewStudent(std::string name)
{
	char ID[] = "00000000";
	ID[0] = '1' + 0;
	ID[1] = '0' + 5;

	//time_t t = time(NULL);
	//tm* timePtr = localtime_s(&t);

	//int year = timePtr->tm_year - 100;
	int year = 16;

	ID[2] = '0' + year / 10;
	ID[3] = '0' + year % 10;

	ID[4] = '0' + (Students.size() + 1) / 1000;
	ID[5] = '0' + (Students.size() + 1) / 100;
	ID[6] = '0' + (Students.size() + 1) / 10;
	ID[7] = '0' + (Students.size() + 1) % 10;
	ID[8] = '\0';
	Student newStudent;
	newStudent.Name = name;
	newStudent.ID = ID;
	Students.push_back(newStudent);
}

// Menginput nilai matakuliah mahasiswa
void Student::SubmitScore(int assignment, int mid, int final)
{
	Score.assignment = assignment;
	Score.mid = mid;
	Score.final = final;
}

// Nilai akhir rata-rata berdasarkan bobot dari sebuah mata kuliah
double Student::GetTotalScore()
{
	return (Score.assignment * RATIO_DEFAULT_ASSIGNMENT
			+ Score.mid * RATIO_DEFAULT_MIDTERM
			+ Score.final * RATIO_DEFAULT_FINALS);
}

// Nilai huruf dari sebuah mata kuliah
GradeIndex Student::GetGrade()
{
	// Cari pembulatannya dahulu
	double raw = GetTotalScore();
	int rounded = (int)(raw + 0.5);

	// Bandingkan dengan daftar tingkatan nilai
	for (int i = 0; i < (sizeof(GradeLevels) / sizeof(*GradeLevels)); i++)
	{
		if (rounded >= GradeLevels[i].Minimum)
		{
			return (GradeIndex)i;
		}
	}
	return GradeIndex::F;
}