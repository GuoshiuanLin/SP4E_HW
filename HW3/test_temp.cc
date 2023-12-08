#include "material_points_factory.hh"
#include "csv_reader.hh"
#include "csv_writer.hh"
#include "compute_temperature.hh"
#include "material_point.hh"
#include "system.hh"
#include "vector.hh"

#include <cmath>
#include <iostream>
#include <string>
#include <gtest/gtest.h>
#include <cstdlib>

/*****************************************************************/
// Fixture class for testing random particles

// Fixture class for testing random particles and material points
class RandomMaterialPoint : public ::testing::Test {
protected:
    // SetUp function is called before each test case
    void SetUp() override {
        // Initialize MaterialPointsFactory and set up particle grid parameters
        MaterialPointsFactory::getInstance();

        // Set up grid parameters
        initializeGrid();

        // Initialize points based on grid parameters
        initializeParticles();

        // Set up temperature computation using a shared pointer
        initializeTemperatureComputation();
    }

    // System for testing
    System system;

    // Vector to store material points
    std::vector<MaterialPoint> points;

    // Shared pointer for temperature computation
    std::shared_ptr<ComputeTemperature> temperature;

    // Grid parameters
    UInt nParticles;
    Real gridLength;
    Real gridResolution;
    Real upperLimit;
    Real lowerLimit;

private:
    // Set up grid parameters
    void initializeGrid() {
        nParticles = 10;                          // Number of particles in the grid
        lowerLimit = -1.0;                        // Lower Limit of the grid.
        upperLimit = 1.0;                         // Upper Limit of the grid.
        gridLength = upperLimit - lowerLimit;     // Total length of the grid

        // Calculate the resolution of the grid based on the number of particles
        gridResolution = gridLength / (nParticles - 1.0);
    }

    // Initialize points based on grid parameters
    void initializeParticles() {
        for (UInt j = 0; j < nParticles; ++j) {
            for (UInt i = 0; i < nParticles; ++i) {
                MaterialPoint particle;

                // Set the position of the particle based on the grid
                particle.getPosition()[0] = lowerLimit + i * gridResolution;
                particle.getPosition()[1] = lowerLimit + j * gridResolution;

                // Apply boundary constraints
                particle.isBoundary() = isOnBoundary(particle.getPosition());

                points.push_back(particle);
            }
        }
    }

    // Check if a position is on the boundary
    bool isOnBoundary(const Vector& position) const {
        return (
            position[0] == lowerLimit ||
            position[0] == upperLimit ||
            position[1] == lowerLimit ||
            position[1] == upperLimit
        );
    }

    // Set up temperature computation using a shared pointer
    void initializeTemperatureComputation() {
        temperature = std::make_shared<ComputeTemperature>();

        // Configure temperature computation parameters
        temperature->deltaT = 1e-5;            // Time step integration
        temperature->rho = 1.0;                // Mass density
        temperature->heatCapacity = 1.0;       // Heat capacity
        temperature->heatConductivity = 1.0;   // Heat conductivity
    }
};

/*****************************************************************/
// Exercise 4-2 - TEST for an initial homogeneous temperature and no heat flux

TEST_F(RandomMaterialPoint, HomogeneousTemperature) {
    /*!
    @brief Test to check results of homogeneous temperature
    */

    // Set initial conditions
    Real initialTemperature = 10.0;
    UInt timeSteps = 50;

    // Add particles with homogeneous initial temperature to the system
    for (const auto& initialPoint : points) {
        MaterialPoint particle;
        particle.getTemperature() = initialTemperature;
        particle.getHeatRate() = 0.0;
        system.addParticle(std::make_shared<MaterialPoint>(particle));
    }

    // Simulate the evolution of temperature over time steps
    for (UInt i = 0; i < timeSteps; ++i) {
        temperature->compute(system);
    }

    // Write to CSV
    CsvWriter writer("homogeneous_temperature.csv");
    writer.compute(system);

    // Check if the final temperature of each particle is close to the initial temperature 
    // and if the heat rate remains zero
    for (auto& particle : system) {
        auto& materialPoint = static_cast<MaterialPoint&>(particle);

        // Uncomment the line below for debugging or visualization
        // std::cout << "Initial Temperature: " << initialTemperature << ", Final Temperature: " << materialPoint.getTemperature() << std::endl;

        ASSERT_NEAR(materialPoint.getTemperature(), initialTemperature, 1e-2);
        ASSERT_NEAR(materialPoint.getHeatRate(), 0.0, 1e-5);
    }

    std::cout << "**********************************************************************************" << std::endl;
    std::cout << "Exercise 4-2 - TEST for an initial homogeneous temperature and no heat flux passed" << std::endl;
    std::cout << "**********************************************************************************" << std::endl;
}

/*****************************************************************/
// Exercise 4-3 - TEST for a volumetric heat source 

TEST_F(RandomMaterialPoint, Volumetric_Heat_Source_Ex3) {
    /*!
    @brief Test for a volumetric heat source with a sinusoidal equilibrium temperature
    */

    // Number of simulation time steps
    const UInt timeSteps = 50;

    // Initialize variables
    Real xCoord;
    Real yCoord;
    Real temperatureEq;

    // Add particles to the system with sinusoidal equilibrium temperature
    for (auto& particle : points) {
        xCoord = particle.getPosition()[0];
        yCoord = particle.getPosition()[1];

        // Set particle properties based on sinusoidal functions
        particle.getTemperature() = std::sin(2.0 * M_PI * xCoord / gridLength);
        particle.getHeatRate() = std::pow((2.0 * M_PI) / gridLength, 2.0) * std::sin(2.0 * M_PI * xCoord / gridLength);

        system.addParticle(std::make_shared<MaterialPoint>(particle));
    }

    // Simulate the evolution of temperature over the time steps
    for (UInt i = 0; i < timeSteps; ++i) {
        temperature->compute(system);
    }

    // Write to CSV
    CsvWriter writer("sinus_vol_heat.csv");
    writer.compute(system);

    // Check if the final temperature of each particle matches the expected equilibrium temperature
    for (auto& particle : system) {
        auto& materialPoint = static_cast<MaterialPoint&>(particle);

        // Retrieve particle location
        xCoord = materialPoint.getPosition()[0];
        yCoord = materialPoint.getPosition()[1];

        // Calculate expected equilibrium temperature using the same sinusoidal function
        temperatureEq = std::sin(2.0 * M_PI * xCoord / gridLength);

        // Assert that the final temperature is close to the expected equilibrium temperature
        ASSERT_NEAR(materialPoint.getTemperature(), temperatureEq, 1e-1);
    }

    std::cout << "*************************************************************************************************" << std::endl;
    std::cout << "Exercise 4-3 - TEST for a volumetric heat source with a sinusoidal equilibrium temperature passed" << std::endl;
    std::cout << "*************************************************************************************************" << std::endl;
}

/*****************************************************************/
// Exercise 4-4 - TEST for a volumetric heat source with linear equilibrium temperature

TEST_F(RandomMaterialPoint, Volumetric_Heat_Source_Ex4) {
    /*!
    @brief Test for a volumetric heat source with linear equilibrium temperature
    */

    // Number of simulation time steps
    const UInt timeSteps = 10;
    bool printDebugInfo = false; // Set to true if you want to print debug info

    // Set the properties
    temperature->rho = 1.0;
    temperature->heatCapacity = 1.0;
    temperature->heatConductivity = 1.0;

    // Variables 
    Real heatRate;
    Real temperatureValue;
    Real temperatureEq;
    Real xCoord;
    Real yCoord;

    // Add particle to the system
    for (auto& particle : points) {
        // Particle location
        xCoord = particle.getPosition()[0];
        yCoord = particle.getPosition()[1];
            // Set heat source with provided form
    if (std::abs(xCoord - 0.5) < gridResolution / 2.0) {
        heatRate = 1.0;
    } else if (std::abs(xCoord + 0.5) < gridResolution / 2.0) {
        heatRate = -1.0;
    } else {
        heatRate = 0.0;
    }
    particle.getHeatRate() = heatRate;

    // Set temperature field
    if (xCoord == -1.0 || xCoord == 1.0 || yCoord == -1.0 || yCoord == 1.0) {
        temperatureValue = 0.0;
    } else if (xCoord > -1.0 && xCoord <= -0.5) {
        temperatureValue = -1.0 * xCoord - 1.0;
    } else if (xCoord > -0.5 && xCoord <= 0.5) {
        temperatureValue = xCoord;
    } else if (xCoord > 0.5) {
        temperatureValue = -1.0 * xCoord + 1.0;
    }
    particle.getTemperature() = temperatureValue;

    system.addParticle(std::make_shared<MaterialPoint>(particle));
}

// Temperature evolution
for (UInt i = 0; i < timeSteps; ++i) {
    temperature->compute(system);
}

// Write to CSV
CsvWriter writer("lin_vol_heat.csv");
writer.compute(system);

// Check if temperature remains the same
for (auto& particle : system) {
    auto& materialPoint = static_cast<MaterialPoint&>(particle);

    // Particle location
    xCoord = materialPoint.getPosition()[0];
    yCoord = materialPoint.getPosition()[1];

    // Compute equilibrium temperature
    if (xCoord == -1.0 || xCoord == 1.0 || yCoord == -1.0 || yCoord == 1.0) {
        temperatureEq = 0.0;
    } else if (xCoord > -1.0 && xCoord <= -0.5) {
        temperatureEq = -1.0 * xCoord - 1.0;
    } else if (xCoord > -0.5 && xCoord <= 0.5) {
        temperatureEq = xCoord;
    } else if (xCoord > 0.5) {
        temperatureEq = -1.0 * xCoord + 1.0;
    }

    // Check if the temperature is close to the equilibrium value
    ASSERT_NEAR(materialPoint.getTemperature(), temperatureEq, 1e-1);

    // Optional debug info
    if (printDebugInfo) {
        std::cout << "Location " << materialPoint.getPosition();
        std::cout << " Temperature " << materialPoint.getTemperature() << " Eq " << temperatureEq << std::endl;
    }
}

std::cout << "*********************************************************************************************" << std::endl;
std::cout << "Exercise 4-4 - TEST for a volumetric heat source with a linear equilibrium temperature passed" << std::endl;
std::cout << "*********************************************************************************************" << std::endl;

}
