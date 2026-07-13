//
// TMatrix.h
//
// Copyright 2000 Sony Corporation
//

#ifndef H_TMatrix
#define H_TMatrix

#include <stdio.h>
#include <string.h>
#include <OPENR/OStatus.h>

const int SizeOfTMatrixRow      = 4;
const int SizeOfTMatrixColumn   = 4;

struct T_Matrix {                            // Total 64 Byte
    float   m_t_matrix[SizeOfTMatrixRow][SizeOfTMatrixColumn];

public:
    // Constructor
    T_Matrix( const T_Matrix& t_matrix )
        {
            *this = t_matrix;
        }
    T_Matrix( void )
        {
            for(int i = 0; i < SizeOfTMatrixRow; i++ ) {
                for(int j = 0; j < SizeOfTMatrixColumn; j++ ) {
                    if( j == i ) {
                        m_t_matrix[i][j] = 1.0;
                    } else {
                        m_t_matrix[i][j] = 0.0;
                    }
                }
            }
        }

    // Set
    OStatus SetT_Matrix (const T_Matrix& t_matrix) {
         *this = t_matrix;
         return oSUCCESS;
    }
    OStatus SetT_Matrix (const int row, const int column, const float value) {
        m_t_matrix[row][column] = value;
        return oSUCCESS;
    }

    // Operator
    T_Matrix&   operator=( const T_Matrix& t_matrix ) {
        if( &t_matrix != this ) memcpy( this, &t_matrix, sizeof(*this) );
        return *this;
    }

    // Get
    int   GetSizeOfRow( void )    { return SizeOfTMatrixRow; }
    int   GetSizeOfColumn( void ) { return SizeOfTMatrixColumn; }
    float GetT_Matrix( const int row, const int column ) {
        return m_t_matrix[row][column];
    }

    // Compare
    bool operator==( const T_Matrix& tm ) {
        return memcmp( this, &tm, sizeof(*this) ) == 0 ? true : false;
    }
    bool operator!=( const T_Matrix& tm ) {
        return !(*this == tm);
    }
};

#endif  // H_TMatrix
