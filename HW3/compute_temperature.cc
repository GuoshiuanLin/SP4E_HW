// compute_temperature.cc

#include "compute_temperature.hh"
#include "fft.hh"
#include "material_point.hh"
#include <cmath>
#include <fftw3.h>

ComputeTemperature::ComputeTemperature()
    : rho(1), heatCapacity(1), heatConductivity(1), deltaT(0.00001) {}

void ComputeTemperature::compute(System& system) {
    // Obtain the number of particles and calculate the matrix size
    UInt noParticles = system.getNbParticles();
    UInt matrixSize = std::sqrt(noParticles);

    // Initialize matrices Fourier-related matrices, and coordinates matrix
    Matrix<complex> temperatureMatrix(matrixSize);
    Matrix<complex> heatMatrix(matrixSize);
    Matrix<complex> temperatureFourier;
    Matrix<complex> temperatureFourierDiff(matrixSize);
    Matrix<complex> heatFourier;
    Matrix<complex> temperatureFourierInv(matrixSize);

    Matrix<std::complex<int>> fourierCoordinates = FFT::computeFrequencies(matrixSize);

    // Construct heat and temperature matrices
    for (UInt i = 0; i < matrixSize; ++i) {
        for (UInt j = 0; j < matrixSize; ++j) {
            UInt particleIndex = i * matrixSize + j;
            auto& particle = system.getParticle(particleIndex);
            auto& materialPoint = static_cast<MaterialPoint&>(particle);

            temperatureMatrix(i, j) = materialPoint.getTemperature();
            heatMatrix(i, j) = materialPoint.getHeatRate();
        }
    }

    // STEP 1 - Fourier transform of the heat equation
    temperatureFourier = FFT::transform(temperatureMatrix);
    heatFourier = FFT::transform(heatMatrix);

    // Uncomment the line below for debugging or progress tracking
    // std::cout << "Step 2 - Fourier transform completed" << std::endl;

    // STEP 2 - Construct the detla heat / delta t matrix
    Real eqConstant = 1.0 / (rho * heatCapacity);

    for (UInt i = 0; i < matrixSize; ++i) {
        for (UInt j = 0; j < matrixSize; ++j) {
            // Extract frequency coordinates and convert to rad/sec
            Real q_x = std::real(fourierCoordinates(i, j)) * 2.0 * M_PI;
            Real q_y = std::imag(fourierCoordinates(i, j)) * 2.0 * M_PI;

            // Calculate the coordinates magnitude
            Real coordinatesMagnitude = q_x * q_x + q_y * q_y;

            // Calculate delta heat / delta t and store it in the matrix
            temperatureFourierDiff(i, j) = eqConstant * (heatFourier(i, j) - heatConductivity * temperatureFourier(i, j) * coordinatesMagnitude);
        }
    }

    // Uncomment the line below for debugging or progress tracking
    // std::cout << "Step 3 - Construction of delta heat / delta t matrix completed" << std::endl;

    // STEP 3 - Inverse Fourier Transform to obtain spatial/temporal representation
    temperatureFourierInv = FFT::itransform(temperatureFourierDiff);

    // Uncomment the line below for debugging or progress tracking
    // std::cout << "Step 4 - Inverse Fourier Transform completed" << std::endl;

    // STEP 4 - Euler integration
    // Update the temperatures of material points based on Euler integration
    for (UInt i = 0; i < matrixSize; ++i) {
        for (UInt j = 0; j < matrixSize; ++j) {
            auto& particle = system.getParticle(i * matrixSize + j);
            auto& materialPoint = static_cast<MaterialPoint&>(particle);

            if (!materialPoint.isBoundary()) {
                materialPoint.getTemperature() += deltaT * std::real(temperatureFourierInv(i, j));
            } else {
                materialPoint.getTemperature() += 0.0;
            }
            // Uncomment the line below for debugging or progress tracking
            // std::cout << "Temperature " << materialPoint.getTemperature() << std::endl;
        }
    }
}

/* -------------------------------------------------------------------------- */
