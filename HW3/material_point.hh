#ifndef __MATERIAL_POINT__HH__
#define __MATERIAL_POINT__HH__

/* -------------------------------------------------------------------------- */
#include "particle.hh"

//! Class for MaterialPoint
class MaterialPoint : public Particle {
  /* ------------------------------------------------------------------------ */
  /* Methods                                                                  */
  /* ------------------------------------------------------------------------ */

public:

  void printself(std::ostream& stream) const override;
  void initself(std::istream& sstr) override;

  Real & getTemperature(){return temperature;};
  Real & getHeatRate(){return heat_rate;};
  bool & isBoundary(){return boundary;};
  
private:
  Real temperature;
  Real heat_rate;
  bool boundary;
};

/* -------------------------------------------------------------------------- */
#endif  //__MATERIAL_POINT__HH__
