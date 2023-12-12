#include "fft.hh"
#include "my_types.hh"
#include "matrix.hh"
#include <sstream>
# include <fstream> 
#include <gtest/gtest.h>

/*****************************************************************/
TEST(FFT, transform) {
  UInt N = 512;
  Matrix<complex> m(N);

  Real k = 2 * M_PI / N;
  for (auto&& entry : index(m)) {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto& val = std::get<2>(entry);
    val = cos(k * i);
  }

  Matrix<complex> res = FFT::transform(m);

  for (auto&& entry : index(res)) {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto& val = std::get<2>(entry);
    if (std::abs(val) > 1e-10)
      std::cout << i << "," << j << " = " << val << std::endl;

    if (i == 1 && j == 0)
      ASSERT_NEAR(std::abs(val), N * N / 2, 1e-10);
    else if (i == N - 1 && j == 0)
      ASSERT_NEAR(std::abs(val), N * N / 2, 1e-10);
    else
      ASSERT_NEAR(std::abs(val), 0, 1e-10);
  }
}
/*****************************************************************/

TEST(FFT, inverse_transform) {
  // transforms a signal matrix (m) using FFT::transform, and then applies the inverse transform using FFT::itransform
  UInt N = 512;
  Matrix<complex> m(N);

  Real k = 2 * M_PI / N;
  for (auto&& entry : index(m)) {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto& val = std::get<2>(entry);
    val = cos(k * i);
  }

  Matrix<complex> transformed = FFT::transform(m);
  Matrix<complex> inverseTransformed = FFT::itransform(transformed);

  for (auto&& entry : index(m)) {
    int i = std::get<0>(entry);
    int j = std::get<1>(entry);
    auto& originalVal = std::get<2>(entry);
    auto& inverseVal = inverseTransformed(i, j);

    // Check if the inverse-transformed value is approximately equal to the original
    ASSERT_NEAR(std::real(inverseVal), std::real(originalVal), 1e-10);
    ASSERT_NEAR(std::imag(inverseVal), std::imag(originalVal), 1e-10);
  }
}
/*****************************************************************/

TEST(FFT, ComputeFrequenciesTest) {

    const std::string csvFilePath = "numpy_computed_frequencies.csv";

    // Read data from the CSV file
    std::ifstream file(csvFilePath);
    ASSERT_TRUE(file.is_open());

    // Assuming the CSV file has the same size for rows and columns
    const int size = 10; /* specify the size of the matrix */
    
    // Compute the expected result using the FFT function
    Matrix<std::complex<int>> expectedResult = FFT::computeFrequencies(size);

    // Iterate through the CSV file and compare values with the expected result
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            // Read the complex number from the CSV file
            std::complex<int> csvValue;
            file >> csvValue;

            // Compare with the expected result
            EXPECT_EQ(csvValue, expectedResult(i, j)) << "Mismatch at position (" << i << ", " << j << ")";
        }
    }

    // Close the file
    file.close();
}
