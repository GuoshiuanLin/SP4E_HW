#include <pybind11/pybind11.h>
#include <pybind11/functional.h>

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
#include "system.hh"
#include "system_evolution.hh"

PYBIND11_MODULE(pypart, m) {

  m.doc() = "pybind of the Particles project";

  // bind the routines here
  // Bind ComputeTemperature and make the private members accessible
    py::class_<ComputeTemperature>(m, "ComputeTemperature")
        .def(py::init<>())
        .def("compute", &ComputeTemperature::compute)
        .def_property("conductivity", &ComputeTemperature::getConductivity, &ComputeTemperature::getConductivity)
        .def_property("capacity", &ComputeTemperature::getCapacity, &ComputeTemperature::getCapacity)
        .def_property("density", &ComputeTemperature::getDensity, &ComputeTemperature::getDensity)
        .def_property("L", &ComputeTemperature::getL, &ComputeTemperature::getL)
        .def_property("delta_t", &ComputeTemperature::getDeltat, &ComputeTemperature::getDeltat)
        .def_readwrite("implicit", &ComputeTemperature::implicit);

    // Bind CsvWriter class
    py::class_<CsvWriter>(m, "CsvWriter")
        .def(py::init<const std::string&>())
        .def("write", &CsvWriter::write);
    
    // // Bind Compute class
    // py::class_<Compute>(m, "Compute")
    //     .def(py::init<>())
    //     .def("compute", &Compute::compute);

    // Bind ComputeInteraction class
    py::class_<ComputeInteraction, Compute>(m, "ComputeInteraction")
        .def(py::init<>())
        .def("applyOnPairs", &ComputeInteraction::applyOnPairs);

    // Bind ComputeGravity class
    py::class_<ComputeGravity, Compute>(m, "ComputeGravity")
        .def(py::init<>())
        .def("compute", &ComputeGravity::compute);

    // Bind ComputeVerletIntegration class
    py::class_<ComputeVerletIntegration, Compute>(m, "ComputeVerletIntegration")
        .def(py::init<>())
        .def("compute", &ComputeVerletIntegration::compute);
    

    // Bind MaterialPointsFactory class
    py::class_<MaterialPointsFactory>(m, "MaterialPointsFactory")
        .def(py::init<>())
        .def("getInstance", &MaterialPointsFactory::getInstance);///

    // Bind PingPongBallsFactory class
    py::class_<PingPongBallsFactory>(m, "PingPongBallsFactory")
        .def(py::init<>())
        .def("getInstance", &PingPongBallsFactory::getInstance);///

    // Bind PlanetsFactory class
    py::class_<PlanetsFactory>(m, "PlanetsFactory")
        .def(py::init<>())
        .def("getInstance", &PlanetsFactory::getInstance);///

    // Bind ParticlesFactoryInterface class
    py::class_<ParticlesFactoryInterface>(m, "ParticlesFactoryInterface")
        .def(py::init<>())
        .def("getInstance", &ParticlesFactoryInterface::getInstance)
        .def("createSimulation",
            py::overload_cast<const std::string&, Real, py::function>(
                &ParticlesFactoryInterface::createSimulation<py::function>),
            py::return_value_policy::reference)
        .def(
            "createSimulation",
            [](ParticlesFactoryInterface& self, const std::string& fname,
            Real timestep) -> SystemEvolution& {
            return self.createSimulation(fname, timestep);
            },
            py::return_value_policy::reference);

    // Bind SystemEvolution class
    py::class_<SystemEvolution>(m, "SystemEvolution")
        .def(py::init<std::unique_ptr<System>>())
        .def("evolve", &SystemEvolution::evolve)
        .def("addCompute", &SystemEvolution::addCompute)
        .def("setNSteps", &SystemEvolution::setNSteps)
        .def("setDumpFreq", &SystemEvolution::setDumpFreq)
        .def("getSystem", &SystemEvolution::getSystem, py::return_value_policy::reference_internal); // Assuming getSystem returns a reference

    
}

PYBIND11_MODULE(pypart, m) {
    m.doc() = "pybind of the Particles project";

    bindComputeTemperature(m);
    bindCsvWriter(m);
    bindCompute(m);
    bindComputeInteraction(m);
    bindComputeGravity(m);
    bindComputeVerletIntegration(m);
    bindParticlesFactory(m);
    bindMaterialPointsFactory(m);
    bindPingPongBallsFactory(m);
    bindPlanetsFactory(m);
    bindParticlesFactoryInterface(m);
}
