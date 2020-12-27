// Hw1.cpp : Defines the entry point for the console application.
#include <iostream>
#include <fstream>
#include <mpi.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

const int MAX_WIDTH = 720;
const int MAX_HEIGHT = 720;

void VectorMatrixMultiplication(int h, double *i_matrix, double *i_vector, double *o_vector);
void GetMatrixFromFile(int &w, int &h, double *i_matrix);
void GetVectorFromFile(int &h, double *i_vector);
void WriteResultToFile(int h, double *o_vector);

int main(int argc, char **argv)
{
    MPI_Init(&argc, &argv);

	int m_w = 0, m_h = 0, v_h = 0, comm_sz = 0, my_rank = 0;
	double i_matrix[MAX_HEIGHT * MAX_WIDTH];
	double i_vector[MAX_WIDTH];
	double o_vector[MAX_HEIGHT];

	// Apply algorithm.
	MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    // Processor 0 reads data from file.
    if (my_rank == 0)
    {
    	// Get input matrix from file.
        GetMatrixFromFile(m_w, m_h, i_matrix);
        // Get input vector from file.
        GetVectorFromFile(v_h, i_vector);
    }
    // Else do nothing.
    
    auto start = high_resolution_clock::now();
    VectorMatrixMultiplication(m_h, i_matrix, i_vector, o_vector);
    auto stop = high_resolution_clock::now();

	auto duration = duration_cast<microseconds>(stop - start);

	if (my_rank == 0)
	{
        cout << "Time taken by function: " << duration.count()<< " microseconds" << endl;
	}
	// Else do nothing.


    MPI_Finalize();

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
    oVectorFile.open ("collectiveResult.txt");

    oVectorFile<<h<<" "<<"1\n";

    for (int i = 0; i < h; i++)
    {
        oVectorFile <<o_vector[i]<<"\n";
    }
    // End of the loop.
    oVectorFile.close();

}

void VectorMatrixMultiplication(int h, double *i_matrix, double *i_vector, double *o_vector)
{
    int my_rank = 0, procCount = 0;

    MPI_Comm_rank (MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size (MPI_COMM_WORLD, &procCount);

    double localresult[MAX_HEIGHT / procCount];
    double localMatrix[MAX_HEIGHT][MAX_WIDTH];

    double timer = MPI_Wtime();

    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Scatter(i_matrix, (MAX_WIDTH*MAX_HEIGHT)/procCount, MPI_DOUBLE, localMatrix, (MAX_WIDTH*MAX_HEIGHT)/procCount, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(i_vector, MAX_HEIGHT, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    for (int i = 0; i < (MAX_HEIGHT/procCount); i++)
    {
        for (int j = 0;j < MAX_WIDTH; j++)
        {
            localresult[i] += i_vector[j] * localMatrix[i][j];
        }
        // End of the loop.
    }
    // End of the loop.

    // Gather result.
     MPI_Gather(localresult, (MAX_HEIGHT)/procCount, MPI_DOUBLE, o_vector, (MAX_HEIGHT)/procCount, MPI_DOUBLE, 0, MPI_COMM_WORLD);


    timer = MPI_Wtime() - timer;


    if (my_rank==0)
    {
        WriteResultToFile(h, o_vector);
        cout << "Time Needed for all ops = "<<timer<<endl;
    }
    // Else do nothing.s
}
