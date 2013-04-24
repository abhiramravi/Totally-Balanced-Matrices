/*
 * gamma_free.cpp
 *
 *  Created on: Apr 25, 2013
 *      Author: Abhiram R ( CS10B060 )
 */

#include <iostream>
#include <cstdlib>
#include <sys/time.h>
//#include <cilk/cilk.h>

using namespace std;

/*
 * Here we compare if two 2 x 2 matrices are the same
 */
bool compare_gamma_and_submat ( bool** X, bool** Y )
{
    if ( (X [0] [0] xor Y [0] [0]) || (X [0] [1] xor Y [0] [1]) || (X [1] [0] xor Y [1] [0]) || (X [1] [1] xor Y [1] [1]) )
        return false;
    return true;
}

/*
 * Brute force algorithm that searches for a gamma submatrix
 * Only works till 45 x 45 size matrices. Too long after that.
 */
bool is_gamma_free_brute ( bool** A, int n, int m )
{
    for ( int i = 0; i < n - 1; i++ )
    {
        for ( int j = 0; j < m - 1; j++ )
        {
            for ( int k = i; k < n; k++ )
            {
                for ( int l = j; l < m; l++ )
                {
                    if ( A [i] [j] == 1 && A [i] [l] == 1 && A [k] [j] == 1 && A [k] [l] == 0 )
                        return false;
                }
            }
        }
    }
    return true;
}

int main ( int argc, char** argv )
{
    int n = 2;
    int m = 2;
    struct timeval start_time;
    struct timeval end_time;

    while ( n <= 1000 )
    {

        bool** A = new bool* [n];
        for ( int i = 0; i < n; i++ )
        {
            A [i] = new bool [m];

        }

        for ( int i = 0; i < n; i++ )
            for ( int j = 0; j < m; j++ )
            {
                A [i] [j] = 0; //random() % 2;
            }
        gettimeofday ( &start_time, NULL );
        cout << "Is the matrix gamma free? " << is_gamma_free_brute ( A, n, m ) << endl;
        gettimeofday ( &end_time, NULL );

        cout << "Time taken for (" << n << "," << m << ") = " << end_time.tv_sec + end_time.tv_usec / (1000000.0) - start_time.tv_sec - start_time.tv_usec / (1000000.0) << endl;
        for ( int i = 0; i < n; i++ )
        {
            delete A [i];
        }
        delete A;

        n  += 1;
        m  += 1;
    }
}

