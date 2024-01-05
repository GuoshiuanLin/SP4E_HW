#include <pybind11/pybind11.h>

namespace py = pybind11;

#include "compute_temperature.hh"
#include "csv_writer.hh"
#include "material_points_factory.hh"
#include "ping_pong_balls_factory.hh"
#include "planets_factory.hh"
#include "compute.hh"
#include "compute_interaction.hh"
#include "compute_gravity.hh"
#include "compute_temperature.hh"
#include "compute_verlet_integration.hh"
#include "particles_factory_interface.hh"

PYBIND11_MODULE(pypart, m) {

  m.doc() = "pybind of the Particles project";

  // bind the routines here
  // Bind CsvWriter class
    py::class_<CsvWriter>(m, "CsvWriter")
        .def(py::init<const std::string&>())
        .def("write", &CsvWriter::write);

  // Bind Compute class
    py::class_<Compute, std::shared_ptr<Compute>>(m, "Compute");

  // Bind ComputeTemperature and make the private members accessible
    py::class_<ComputeTemperature>(m, "ComputeTemperature")
        .def(py::init<>())
        .def_property("conductivity", &ComputeTemperature::getConductivity, &ComputeTemperature::getConductivity)
        .def_property("capacity", &ComputeTemperature::getCapacity, &ComputeTemperature::getCapacity)
        .def_property("density", &ComputeTemperature::getDensity, &ComputeTemperature::getDensity)
        .def_property("L", &ComputeTemperature::getL, &ComputeTemperature::getL)
        .def_property("delta_t", &ComputeTemperature::getDeltat, &ComputeTemperature::getDeltat)
        .def_readwrite("implicit", &ComputeTemperature::implicit);

    // Bind ComputeInteraction class
    py::class_<ComputeInteraction, Compute, std::shared_ptr<ComputeInteraction>>(m, "ComputeInteraction");

    // Bind ComputeGravity class
    py::class_<ComputeGravity, Compute, std::shared_ptr<ComputeGravity>>(m, "ComputeGravity")
        .def(py::init<>())
        .def("setG", &ComputeGravity::setG);

    // Bind ComputeVerletIntegration class
    py::class_<ComputeVerletIntegration, Compute, std::shared_ptr<ComputeVerletIntegration>>(m, "ComputeVerletIntegration")
        .def(py::init<Real>())
        .def("addInteraction", &ComputeVerletIntegration::addInteraction);

    // Bind ParticlesFactoryInterface class
    // Define 2 createSimulation with 2 and 3 arguments respectively because we didn't manage to use the overload_cast method
    py::class_<ParticlesFactoryInterface>(m, "ParticlesFactoryInterface")
        .def("getInstance", &ParticlesFactoryInterface::getInstance, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (ParticlesFactoryInterface::*)(const std::string &, Real))&ParticlesFactoryInterface::createSimulation, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (ParticlesFactoryInterface::*)(const std::string &, Real,py::function))&ParticlesFactoryInterface::createSimulation, py::return_value_policy::reference);
    py::class_<MaterialPointsFactory, ParticlesFactoryInterface>(m, "MaterialPointsFactory")
        .def("getInstance", &MaterialPointsFactory::getInstance, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (MaterialPointsFactory::*)(const std::string &, Real))&MaterialPointsFactory::createSimulation, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (MaterialPointsFactory::*)(const std::string &, Real,py::function))&MaterialPointsFactory::createSimulation, py::return_value_policy::reference);
    py::class_<PingPongBallsFactory, ParticlesFactoryInterface>(m, "PingPongBallsFactory")
        .def("getInstance", &PingPongBallsFactory::getInstance, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (PingPongBallsFactory::*)(const std::string &, Real))&PingPongBallsFactory::createSimulation, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (PingPongBallsFactory::*)(const std::string &, Real,py::function))&PingPongBallsFactory::createSimulation, py::return_value_policy::reference);
    py::class_<PlanetsFactory, ParticlesFactoryInterface>(m, "PlanetsFactory")
        .def("getInstance", &PlanetsFactory::getInstance, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (PlanetsFactory::*)(const std::string &, Real))&PlanetsFactory::createSimulation, py::return_value_policy::reference)
        .def("createSimulation",(SystemEvolution & (PlanetsFactory::*)(const std::string &, Real,py::function))&PlanetsFactory::createSimulation, py::return_value_policy::reference);
    
    py::class_<System>(m, "System");
    

    // Bind SystemEvolution class
    py::class_<SystemEvolution>(m, "SystemEvolution")
        .def("evolve", &SystemEvolution::evolve)
        .def("addCompute", &SystemEvolution::addCompute)
        .def("setNSteps", &SystemEvolution::setNSteps)
        .def("setDumpFreq", &SystemEvolution::setDumpFreq)
        .def("getSystem", &SystemEvolution::getSystem); 
  
}
