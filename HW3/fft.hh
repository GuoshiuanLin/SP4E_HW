#ifndef FFT_HH
#define FFT_HH
/* ------------------------------------------------------ */
#include "matrix.hh"
#include "my_types.hh"
#include <fftw3.h>
/* ------------------------------------------------------ */

struct FFT {

  static Matrix<complex> transform(Matrix<complex>& m);
  static Matrix<complex> itransform(Matrix<complex>& m);

  static Matrix<std::complex<int>> computeFrequencies(int size);
};

/* ------------------------------------------------------ */

inline Matrix<complex> FFT::transform(Matrix<complex>& m_in) {
  // Assuming m_in is a square matrix
  int size = m_in.size();

  // FFTW input and output arrays
  fftw_complex* in = reinterpret_cast<fftw_complex*>(m_in.data());
  fftw_complex* out = fftw_alloc_complex(size * size); //allocate the output complex array

  // FFTW plan for forward transform
  fftw_plan plan = fftw_plan_dft_2d(size, size, in, out, FFTW_FORWARD, FFTW_ESTIMATE);

  // Execute the plan
  fftw_execute(plan);

  // Create a new matrix to store the result
  Matrix<complex> result(size);
  for (UInt i = 0; i < size; ++i) {
    for (UInt j = 0; j < size; ++j) {
      result(i, j) = complex(out[j * size + i][0], out[j * size + i][1]);
    }
    }

  // Clean up FFTW resources
  fftw_destroy_plan(plan);
  fftw_free(out);

  return result;
}

/* ------------------------------------------------------ */

inline Matrix<complex> FFT::itransform(Matrix<complex>& m_in) {
  // Assuming m_in is a square matrix
  int size = m_in.size();

  // FFTW input and output arrays
  fftw_complex* in = reinterpret_cast<fftw_complex*>(m_in.data());
  fftw_complex* out = fftw_alloc_complex(size * size);

  // FFTW plan for inverse transform
  fftw_plan plan = fftw_plan_dft_2d(size, size, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);

  // Execute the plan
  fftw_execute(plan);

  // Create a new matrix to store the result
  Matrix<complex> result(size);
  for (UInt i = 0; i < size; ++i) {
    for (UInt j = 0; j < size; ++j) {
      result(i, j) = complex(out[j * size + i][0] / (size * size), out[j * size + i][1] / (size * size));
    }
  }

  // Clean up FFTW resources
  fftw_destroy_plan(plan);
  fftw_free(out);

  return result;
}

/* ------------------------------------------------------ */

inline Matrix<std::complex<int>> FFT::computeFrequencies(int size) {
  Matrix<std::complex<int>> result(size);
  // Calculate the wavenumber components kx and ky based on the indices i and j
  for (UInt i = 0; i < size; ++i) {
    for (UInt j = 0; j < size; ++j) {
      // Compute wavenumber components based on the indices
      int kx = (i <= size / 2) ? i : i - size;
      int ky = (j <= size / 2) ? j : j - size;

      // Set the wavenumber in the result matrix
      result(i, j) = std::complex<int>(kx, ky);
    }
  }

  return result;
}

#endif  // FFT_HH
