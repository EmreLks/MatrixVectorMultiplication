// Hw1.cpp : Defines the entry point for the console application.
#include <iostream>
#include <fstream>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MAX_WIDTH = 720;
const int MAX_HEIGHT = 720;

void VectorMatrixMultiplication(int w, int h, double *i_matrix, double *i_vector, double *o_vector);
void GetMatrixFromFile(int &w, int &h, double *i_matrix);
void GetVectorFromFile(int &h, double *i_vector);
void WriteResultToFile(int h, double *o_vector);

int main()
{
	int m_w = 0, m_h = 0, v_h;
	double i_matrix[MAX_WIDTH * MAX_HEIGHT];
	double i_vector[MAX_WIDTH];
	double o_vector[MAX_HEIGHT];

	// Get input matrix from file.
	GetMatrixFromFile(m_w, m_h, i_matrix);

	// Get input vector from file.
	GetVectorFromFile(v_h, i_vector);
    
    auto start = high_resolution_clock::now();
	// Apply algorithm.
	VectorMatrixMultiplication(m_w, m_h, i_matrix, i_vector, o_vector);

	WriteResultToFile(m_h, o_vector);

	auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

	return 0;
}
void GetVectorFromFile(int &h, double *i_vector)
{
	ifstream infile;
	infile.open("BigX.txt");

	// First line consists width and height.
	int w = 0;/*unused*/
	infile >> h >> w;

	if (h > MAX_HEIGHT)
	{
		h = MAX_HEIGHT;
	}
	// Else do nothing.

	int index = 0;

	while (!infile.eof() && index < MAX_HEIGHT)
	{
		infile >> i_vector[index];
		index++;
	}
	// End of the loop.

	infile.close();
}

void GetMatrixFromFile(int &w, int &h, double *i_matrix)
{
	ifstream infile;
	infile.open("BigA.txt");

	// First line consists width and height.
	infile >> h >> w;

	if (h > MAX_HEIGHT)
	{
		h = MAX_HEIGHT;
	}
	// Else do nothing.

	if (w > MAX_WIDTH)
	{
		w = MAX_WIDTH;
	}
	// Else do nothing.

	long long index = 0;

	while (!infile.eof() && index < (MAX_HEIGHT * MAX_WIDTH))
	{
		infile >> i_matrix[index];
		index++;
	}
	// End of the loop.

	infile.close();
}
void WriteResultToFile(int h, double *o_vector)
{
    ofstream oVectorFile;
    oVectorFile.open ("serialResult.txt");

    oVectorFile<<h<<" "<<"1\n";

    for (int i = 0; i < h; i++)
    {
        oVectorFile <<o_vector[i]<<"\n";
    }
    // End of the loop.
    oVectorFile.close();
}

void VectorMatrixMultiplication(int w, int h, double *i_matrix, double *i_vector, double *o_vector)
{
	for (int i = 0; i < h; i++)
	{
		o_vector[i] = 0.0;

		for (int j = 0; j < w; j++)
		{
			o_vector[i] += i_vector[j] * i_matrix[(i * w) + j];
		}
		// End of the loop.
	}
	// End of the loop.
}
