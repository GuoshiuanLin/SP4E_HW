#include "compute_temperature.hh"
#include "fft.hh"
#include "material_point.hh"
#include <cmath>

/* -------------------------------------------------------------------------- */
/*!
 * @brief Compute Temperature class
 */
ComputeTemperature::ComputeTemperature()
    : Rho(1), HeatCapacity(1), HeatConductivity(1), DeltaT(0.001)
{
};

/*!
*@brief Transient heat equation in two dimensions - function for heat distribution computation
*@param system system input
*/
void ComputeTemperature::compute(System& system) {
    // Obtain the number of particles and calculate the matrix size
     UInt noParticles = system.getNbParticles();
     UInt matrixSize = std::sqrt(noParticles);

    // Initialize matrices
    Matrix<complex> temperatureMatrix(matrixSize);
    Matrix<complex> heatMatrix(matrixSize);

    // Initialize Fourier-related matrices
    Matrix<complex> temperatureFourier(matrixSize);
    Matrix<complex> temperatureFourierDiff(matrixSize);
    Matrix<complex> heatFourier(matrixSize);
    Matrix<complex> temperatureFourierInv(matrixSize);

    // Construct the coordinates matrix in Fourier space
    Matrix<std::complex<int>> fourierCoordinates = FFT::computeFrequencies(matrixSize)

    // Construct the heat and temperature matrices
    for (UInt i = 0; i < matrixSize; ++i) {
        for (UInt j = 0; j < matrixSize; ++j) {
            // Obtain the particle index
            UInt particleIndex = i * matrixSize + j;

            // Retrieve the particle from the system
            auto& Particle = system.getParticle(particleIndex);

            // Static casting to convert particle into material point
            auto& MaterialPoint = static_cast<MaterialPoint&>(Particle);

            // Populate temperature and heat rate matrices
            temperatureMatrix(i,j) = MaterialPoint.getTemperature();
            heatMatrix(i,j) = MaterialPoint.getHeatRate();

            // Uncomment the line below for debugging or visualization
            //std::cout <<"Temp: " <<temperatureMatrix(i,j) << ", Heat: " <<heatMatrix(i,j) <<std::endl;
        }
    }

    // STEP 1 - Fourier transfrom of the heat equation
    temperatureFourier = FFT::transform(temperatureMatrix);
    heatFourier = FFT::transform(heatMatrix);

    // Uncoment the line below for debugging or progress tracking
    //std::cout << "Step 2 - Fourier transform completed" <<std::endl;

    // STEP 2 - Construct the detla heat / delta t matrix
    Real eqConstant = 1.0 / (this->Rho * this->HeatCapacity);

    for (UInt i = 0; i <matrixSize; ++i) {
        for (UInt j = 0; j <matrixSize; ++j) {
            
            // Extract frequency coordinates and convert to rad/sec
            Real q_x = std::real(fourierCoordinates(i,j)) * 2.0 * M_PI;
            Real q_y = std::imag(fourierCoordinates(i,j)) * 2.0 * M_PI;

            // Calculate the coordinates magnitude
            Real coordinatesMagnitude = q_x * q_x + q_y * q_y;

            // Calculate delta heat / delta t and store it in the matrix
            temperatureFourierDiff(i,j) = eqConstant * (heatFourier(i,j) - this->HeatConductivity * temperatureFourier(i,j) * coordinatesMagnitude);
        }
    }

    // Uncoment the line below for debugging or progress tracking
    //std::cout << "Step 3 - Construction of delta heat / delta t matrix completed" << std::endl;

    // STEP 3 - Inverse Inverse Fourier Transform to obtain spatial/temporal representation
    temperatureFourierInv = FFT::itransform(temperatureFourierDiff);

    // STEP 4 - Euler integration
    //Update the temperatures of material points based on Euler integration

    for (UInt i = 0; i <matrixSize; ++i) {
        for (UInt j = 0; j<matrixSize; ++j) {
            auto& particle = system.getParticle(i * matrixSize + j);
            
            // Perform static casting to convert a base-class particle to a MaterialPoint
            auto& mp = static_cast<MaterialPoint&>(particle);

            //If not a boundary, perform Euler integration
            if (!mp.isBoundary()){
                mp.getTemperature() += this->DeltaT * std::real(temperatureFourierInv(i,j));
                } else {
                // For boundary points, set temperature to 0
                mp.getTemperature() = 0.0;
                }
            }

            // Uncoment the line below for debugging or progress tracking
            //std::cout << "Temperature " <<mp.getTemperature() << std::endl;
        }
    }

/* -------------------------------------------------------------------------- */
