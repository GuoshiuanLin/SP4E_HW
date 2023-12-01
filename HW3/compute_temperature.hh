#ifndef __COMPUTE_TEMPERATURE__HH__
#define __COMPUTE_TEMPERATURE__HH__

/* -------------------------------------------------------------------------- */
#include "compute.hh"
#include "compute_interaction.hh"
#include "matrix.hh"
#include "my_types.hh"
#include <functional>


//! Compute contact interaction between ping-pong balls
class ComputeTemperature : public Compute {

  // Virtual implementation
public:

  ComputeTemperature();
  ~ComputeTemperature() override = default;
  
  //! Penalty contact implementation
  void compute(System& system) override;

  Real Rho;               // Mass density
  Real HeatCapacity;      // Heat capacity
  Real HeatConductivity;  // Heat Conductivity
  Real DeltaT;            // Time step for integration

};

/* -------------------------------------------------------------------------- */
#endif  //__COMPUTE_TEMPERATURE__HH__
