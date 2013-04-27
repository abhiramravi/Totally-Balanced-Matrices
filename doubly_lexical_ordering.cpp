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
//#define DEBUG
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
void exchange_cols ( bool** A, int r, int c, int p, int q )
{
    for ( int i = 0; i < r; i++ ) // I smell cilk_for
    {
        A [i] [p] = A [i] [p] xor A [i] [q];
        A [i] [q] = A [i] [p] xor A [i] [q];
        A [i] [p] = A [i] [p] xor A [i] [q];
    }
}
void exchange_rows ( bool** A, int r, int c, int p, int q )
{
    for ( int i = 0; i < c; i++ ) // I smell cilk_for
    {
        A [p] [i] = A [p] [i] xor A [q] [i];
        A [q] [i] = A [p] [i] xor A [q] [i];
        A [p] [i] = A [p] [i] xor A [q] [i];
    }
}

void re_order ( bool** A, int r, int c )
{
    int i = 0, j = 0;
    while ( i < r && A [i] [c - 1] == 0 )
        i++;
    //-- i definitely points to a 1 now
    while ( j < r && i < r )
    {
        if ( (A [j] [c - 1] == 0 && A [i] [c - 1] == 1) && (j > i) )
        {
            exchange_rows ( A, r, c, j, i );
            j++;
            i++;
            //-- To make sure that i points to a 1
            while ( A [i] [c - 1] == 0 )
                i++;
        } else
        {
            j++;
        }
    }
    //cout << i << endl;
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
int size = 2048;
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
        //srand ( 4 );
        for ( int i = 0; i < n; i++ )
            for ( int j = 0; j < m; j++ )
            {
                A [i] [j] = random () % 2;
            }
        //print_matrix ( A, n, m );
        gettimeofday ( &start_time, NULL );
        int c = m;
        while ( m > 1 )
        {

            int* d = partition_column_sum_vector ( A, n, m );
#ifdef DEBUG
            cout << "Column vector  :  ";
            for ( int i = 0; i < m; i++ )
            {
                cout << d [i] << " ";
            }
            cout << endl;
#endif

            int jwithmax = vector_max ( d, m ).index;
#ifdef DEBUG
            cout << jwithmax << " " << vector_max ( d, n ).max << endl;
#endif
            exchange_cols ( A, n, m, jwithmax, m - 1 );
#ifdef DEBUG
            print_matrix ( A, n, m );
#endif
            re_order ( A, n, m );
#ifdef DEBUG
            cout << "Reordered matrix = " << endl;
            print_matrix ( A, n, m );
#endif
            m--;
        }
        gettimeofday ( &end_time, NULL );
        cout << "Time taken for (" << n << "," << m << ") = "
                << end_time.tv_sec + end_time.tv_usec / (1000000.0) - start_time.tv_sec - start_time.tv_usec / (1000000.0) << endl;
        //cout << "Final Totally Reverse Lexicographic Matrix : " << endl;
       // print_matrix ( A, n, c );
        for ( int i = 0; i < n; i++ )
        {
            delete A [i];
        }
        delete A;

        n += 1;
        m += 1;
    }
}

