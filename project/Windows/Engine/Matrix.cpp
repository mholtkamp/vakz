#include "Matrix.h"
#include "VMath.h"
#include <math.h>
#include <string.h> // for memset, memcpy

float Matrix::s_arTransValues[MATRIX_VALUE_COUNT];
float Matrix::s_arTempValues[MATRIX_VALUE_COUNT];

//*****************************************************************************
// Constructor
//*****************************************************************************
Matrix::Matrix()
{
    LoadIdentity();
}

//*****************************************************************************
// Destructor
//*****************************************************************************
Matrix::~Matrix()
{

}

//*****************************************************************************
// LoadIdentity
//*****************************************************************************
void Matrix::LoadIdentity()
{
    int i = 0;
    int j = 0;

    for (i = 0; i < MATRIX_SIZE; i++)
    {
        for (j = 0; j < MATRIX_SIZE; j++)
        {
            m_arValues[i*MATRIX_SIZE + j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

//*****************************************************************************
// Translate
//*****************************************************************************
void Matrix::Translate(float fX,
                       float fY,
                       float fZ)
{
    // Clear the trans values array to hold new translation matrix.
    memset(s_arTransValues, 0, sizeof(float) * MATRIX_VALUE_COUNT);

    // Set to identity matrix
    s_arTransValues[0]  = 1.0f;
    s_arTransValues[5]  = 1.0f;
    s_arTransValues[10] = 1.0f;
    s_arTransValues[15] = 1.0f;

    // Add translation element
    s_arTransValues[12] = fX;
    s_arTransValues[13] = fY;
    s_arTransValues[14] = fZ;

    // Perform matrix multiplication with arrays
    Multiply(m_arValues, s_arTransValues, s_arTempValues);

    // Copy result back to value array
    memcpy(m_arValues, s_arTempValues, sizeof(float) * MATRIX_VALUE_COUNT);
}

//*****************************************************************************
// Rotate
//*****************************************************************************
void Matrix::Rotate(float fAngle,
                    float fX,
                    float fY,
                    float fZ)
{
    float fRadians = 0.0f;
    float fMagnitude = 0.0f;
    float c = 0.0f;
    float s = 0.0f;
    float x = fX;
    float y = fY;
    float z = fZ;

    // Convert angle in degrees to radians
    fRadians = fAngle * DEGREES_TO_RADIANS;

    // Calculate (once) the trig values needed in matrix generation.
    c = cos(fRadians);
    s = sin(fRadians);

    // Check if vector is normalized already.
    fMagnitude = sqrt(x*x + y*y + z*z);
    if (fMagnitude != 1.0f)
    {
        // Check for divide-by-zero errors
        if (fMagnitude == 0.0f)
        {
            // Don't do anything with improper arguments.
            return;
        }

        x = x/fMagnitude;
        y = y/fMagnitude;
        z = z/fMagnitude;
    }

    // Clear transformation values
    memset(s_arTransValues, 0, sizeof(float) * MATRIX_VALUE_COUNT);

    // Set to identity matrix
    s_arTransValues[0]  = 1.0f;
    s_arTransValues[5]  = 1.0f;
    s_arTransValues[10] = 1.0f;
    s_arTransValues[15] = 1.0f;

    // Add rotation element
    s_arTransValues[0] = x*x*(1 - c) + c;
    s_arTransValues[1] = y*x*(1 - c) + z*s;
    s_arTransValues[2] = x*z*(1 - c) - y*s;
    
    s_arTransValues[4] = x*y*(1 - c) - z*s;
    s_arTransValues[5] = y*y*(1 - c) + c;
    s_arTransValues[6] = y*z*(1 - c) + x*s;

    s_arTransValues[8]  = x*z*(1 - c) + y*s;
    s_arTransValues[9]  = y*z*(1 - c) - x*s;
    s_arTransValues[10] = z*z*(1 - c) + c;

    // Perform matrix multiplication with arrays
    Multiply(m_arValues, s_arTransValues, s_arTempValues);

    // Copy result back to value array
    memcpy(m_arValues, s_arTempValues, sizeof(float) * MATRIX_VALUE_COUNT);
}

//*****************************************************************************
// Scale
//*****************************************************************************
void Matrix::Scale(float fX,
                   float fY,
                   float fZ)
{
    // Clear the trans values array to hold new transformation matrix.
    memset(s_arTransValues, 0, sizeof(float) * MATRIX_VALUE_COUNT);

    // Set to identity matrix
    s_arTransValues[0]  = 1.0f;
    s_arTransValues[5]  = 1.0f;
    s_arTransValues[10] = 1.0f;
    s_arTransValues[15] = 1.0f;

    // Add scale element
    s_arTransValues[0]  = fX;
    s_arTransValues[5]  = fY;
    s_arTransValues[10] = fZ;

    // Perform matrix multiplication with arrays
    Multiply(m_arValues, s_arTransValues, s_arTempValues);

    // Copy result back to value array
    memcpy(m_arValues, s_arTempValues, sizeof(float) * MATRIX_VALUE_COUNT);
}

//*****************************************************************************
// Frustum
//*****************************************************************************
void Matrix::Frustum(float fLeft,
                     float fRight,
                     float fBottom,
                     float fTop,
                     float fNear,
                     float fFar)
{
    // Clear the trans values array to hold new transformation matrix.
    memset(s_arTransValues, 0, sizeof(float) * MATRIX_VALUE_COUNT);

    // Add frustum element (no need to set identity)
    s_arTransValues[0]  = (2*fNear)/(fRight - fLeft);
    s_arTransValues[5]  = (2*fNear)/(fTop - fBottom);
    s_arTransValues[8]  = (fRight + fLeft)/(fRight - fLeft);
    s_arTransValues[9]  = (fTop + fBottom)/(fTop - fBottom);
    s_arTransValues[10] = -1.0f * (fFar + fNear)/(fFar - fNear);
    s_arTransValues[11] = -1.0f;
    s_arTransValues[14] = -1.0f * (2*fFar*fNear)/(fFar - fNear);

    // Perform matrix multiplication with arrays
    Multiply(m_arValues, s_arTransValues, s_arTempValues);

    // Copy result back to value array
    memcpy(m_arValues, s_arTempValues, sizeof(float) * MATRIX_VALUE_COUNT);
}

//*****************************************************************************
// Ortho
//*****************************************************************************
void Matrix::Ortho(float fLeft,
                   float fRight,
                   float fBottom,
                   float fTop,
                   float fNear,
                   float fFar)
{
    // Clear the trans values array to hold new transformation matrix.
    memset(s_arTransValues, 0, sizeof(float) * MATRIX_VALUE_COUNT);

    // Set to identity matrix
    s_arTransValues[0]  = 1.0f;
    s_arTransValues[5]  = 1.0f;
    s_arTransValues[10] = 1.0f;
    s_arTransValues[15] = 1.0f;

    // Add orthographic element
    s_arTransValues[0]  =  2.0f/(fRight - fLeft);
    s_arTransValues[5]  =  2.0f/(fTop - fBottom);
    s_arTransValues[10] = -2.0f/(fFar - fNear);
    s_arTransValues[12] = -1.0f*(fRight + fLeft)/(fRight - fLeft);
    s_arTransValues[13] = -1.0f*(fTop + fBottom)/(fTop - fBottom);
    s_arTransValues[14] = -1.0f*(fFar + fNear)/(fFar - fNear);

    // Perform matrix multiplication with arrays
    Multiply(m_arValues, s_arTransValues, s_arTempValues);

    // Copy result back to value array
    memcpy(m_arValues, s_arTempValues, sizeof(float) * MATRIX_VALUE_COUNT);
}

//*****************************************************************************
// Clear
//*****************************************************************************
void Matrix::Clear()
{
    memset(m_arValues, 0, sizeof(float) * MATRIX_VALUE_COUNT);
}

//*****************************************************************************
// GetArray
//*****************************************************************************
float* Matrix::GetArray()
{
    return m_arValues;
}

//*****************************************************************************
// GetArrayValues
//*****************************************************************************
void Matrix::GetArrayValues(float* pArray)
{
    if (pArray != 0)
    {
        memcpy(pArray, m_arValues, sizeof(float) * MATRIX_VALUE_COUNT);
    }
}

//*****************************************************************************
// operator*
//*****************************************************************************
Matrix Matrix::operator*(const Matrix& matOther)
{
    // Create result matrix to return
    Matrix matTwo = const_cast<Matrix&>(matOther);
    Matrix matResult;
    matResult.Clear();

    // Multiply the arrays of this and matOther
    Multiply(this->GetArray(),
             matTwo.GetArray(),
             matResult.GetArray());

    // Return the result matrix
    return matResult;
}

//*****************************************************************************
// Multiply
//*****************************************************************************
void Matrix::Multiply(float* pOne,
                      float* pTwo,
                      float* pResult)
{
    int i = 0;
    int j = 0;
    int k = 0;
    float fSum = 0.0f;

    for (i = 0; i < MATRIX_SIZE; i++)
    {
        for (j = 0; j < MATRIX_SIZE; j++)
        {
            fSum = 0.0f;
            for (k = 0; k < MATRIX_SIZE; k++)
            {
                fSum += pOne[j + k*4] * pTwo[i*4 + k];
            }
            pResult[i*4 + j] = fSum;
        }
    }
}
