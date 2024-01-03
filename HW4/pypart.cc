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
    py::class_<Compute>(m, "Compute");
        // .def(py::init<>())
        // .def("compute", &Compute::compute);
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

    // Bind ComputeInteraction class
    // py::class_<ComputeInteraction, Compute>(m, "ComputeInteraction")
    //     .def(py::init<>())
    //     // .def("applyOnPairs", &ComputeInteraction::applyOnPairs);

    // Bind ComputeGravity class
    py::class_<ComputeGravity, Compute>(m, "ComputeGravity")
        .def(py::init<>())
        .def("setG", &ComputeGravity::setG);
        // .def("compute", &ComputeGravity::compute);

    // Bind ComputeVerletIntegration class
    py::class_<ComputeVerletIntegration, Compute>(m, "ComputeVerletIntegration")
        .def(py::init<Real>())
        .def("addInteraction", &ComputeVerletIntegration::addInteraction);

    // Bind ParticlesFactoryInterface class
    py::class_<ParticlesFactoryInterface>(m, "ParticlesFactoryInterface")
        // .def(py::init<>())
        .def_static("getInstance", &ParticlesFactoryInterface::getInstance, py::return_value_policy::reference)
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

    // // Bind MaterialPointsFactory class
    // py::class_<MaterialPointsFactory, ParticlesFactoryInterface, std::unique_ptr<MaterialPointsFactory>>(m, "MaterialPointsFactory")
    //     .def_static("getInstance", &MaterialPointsFactory::getInstance);

    // // Bind PingPongBallsFactory class
    // py::class_<PingPongBallsFactory, ParticlesFactoryInterface, std::unique_ptr<PingPongBallsFactory>>(m, "PingPongBallsFactory")
    //     .def_static("getInstance", &MaterialPointsFactory::getInstance);

    // // Bind PlanetsFactory class
    // py::class_<PlanetsFactory, ParticlesFactoryInterface, std::unique_ptr<PlanetsFactory>>(m, "PlanetsFactory")
    //     .def_static("getInstance", &MaterialPointsFactory::getInstance);

    py::class_<MaterialPointsFactory, ParticlesFactoryInterface>(m, "MaterialPointsFactory")
        .def("getInstance", &MaterialPointsFactory::getInstance, py::return_value_policy::reference);
    py::class_<PingPongBallsFactory, ParticlesFactoryInterface>(m, "PingPongBallsFactory")
        .def("getInstance", &PingPongBallsFactory::getInstance, py::return_value_policy::reference);
    py::class_<PlanetsFactory, ParticlesFactoryInterface>(m, "PlanetsFactory")
        .def("getInstance", &PlanetsFactory::getInstance, py::return_value_policy::reference);




    // Bind SystemEvolution class
    py::class_<SystemEvolution>(m, "SystemEvolution")
        // .def(py::init<std::unique_ptr<System>>())
        .def("evolve", &SystemEvolution::evolve)
        .def("addCompute", &SystemEvolution::addCompute);
        // .def("setNSteps", &SystemEvolution::setNSteps)
        // .def("setDumpFreq", &SystemEvolution::setDumpFreq)
        // .def("getSystem", &SystemEvolution::getSystem, py::return_value_policy::reference_internal); // Assuming getSystem returns a reference

    
}
