#ifndef __COMPUTE_TEMPERATURE__HH__
#define __COMPUTE_TEMPERATURE__HH__

/* -------------------------------------------------------------------------- */
#include "compute.hh"
#include "compute_interaction.hh"
#include "matrix.hh"
#include "my_types.hh"
#include <fftw3.h>
#include <functional>

//! Compute temperature in the system
class ComputeTemperature : public Compute {

  // Virtual implementation
public:
  ComputeTemperature();
  ~ComputeTemperature() override = default;
  
  //! Implementation of temperature computation
  void compute(System& system) override;

  Real rho;               //!< Mass density
  Real heatCapacity;      //!<  Heat capacity
  Real heatConductivity;  //!<  Heat Conductivity
  Real deltaT;            //!<  Time step for integration

};

/* -------------------------------------------------------------------------- */
#endif  //__COMPUTE_TEMPERATURE__HH__
