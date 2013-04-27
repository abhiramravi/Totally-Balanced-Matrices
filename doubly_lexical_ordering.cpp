/*
 * doubly_lexical_ordering.cpp
 *
 *  Created on: Apr 27, 2013
 *      Author: Abhiram R & Akhilesh Godi
 */

#include <iostream>
#include <cstdlib>
#include <sys/time.h>
//#include <cilk/cilk.h>

using namespace std;

int* partition_column_sum_vector ( bool** M, int m, int n )
{
    int* d = new int [m];
    for ( int j = 0; j < m; j++ ) //I smell cilk_for
    {
        d [j] = 0;
        for ( int i = 0; i < n; i++ )
        {
            d [j] += M [i] [j];
        }
    }
    return d;
}
struct result
{
    int max;
    int index;
};
result vector_max ( int* d, int m )
{
    if ( m == 1 )
    {
        result r;
        r.index = 0;
        r.max = d [0];
        return r;
    }
    result r1 = vector_max ( d, m / 2 );
    result r2 = vector_max ( d + m / 2, m / 2 ); // i smell cilk_spawn

    result r;
    if ( r1.max > r2.max )
    {
        r.index = r1.index;
        r.max = r1.max;
        return r;
    } else
    {
        r.index = r2.index + m / 2;
        r.max = r2.max;
    }

    return r;
}

void doubly_lexical_ordering ( bool** A, int m, int n )
{

}

void print_matrix ( bool** A, int m, int n )
{
    for ( int i = 0; i < m; i++ )
    {
        for ( int j = 0; j < n; j++ )
            cout << A [i] [j] << " ";
        cout << endl;
    }

}
int size = 8;
int main ( int argc, char** argv )
{
    int n = size;
    int m = size;
    struct timeval start_time;
    struct timeval end_time;

    while ( n <= size )
    {

        bool** A = new bool* [n];
        for ( int i = 0; i < n; i++ )
        {
            A [i] = new bool [m];

        }

        for ( int i = 0; i < n; i++ )
            for ( int j = 0; j < m; j++ )
            {
                A [i] [j] = random () % 2;
            }
        print_matrix ( A, n, m );
        gettimeofday ( &start_time, NULL );
        cout << "Output : \n ";
        int* d = partition_column_sum_vector ( A, n, m );
        for ( int i = 0; i < m; i++ )
        {
            cout << d [i] << " ";
        }
        cout << endl;

        cout << vector_max ( d, n ).index << " " << vector_max ( d, n ).max << endl;
        gettimeofday ( &end_time, NULL );

        cout << "Time taken for (" << n << "," << m << ") = "
                << end_time.tv_sec + end_time.tv_usec / (1000000.0) - start_time.tv_sec - start_time.tv_usec / (1000000.0) << endl;
        for ( int i = 0; i < n; i++ )
        {
            delete A [i];
        }
        delete A;

        n += 1;
        m += 1;
    }
}

