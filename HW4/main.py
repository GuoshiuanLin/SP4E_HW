#!/bin/env python3

import sys
import argparse
import pypart

from pypart import MaterialPointsFactory, ParticlesFactoryInterface
from pypart import PingPongBallsFactory, PlanetsFactory
from pypart import CsvWriter
from pypart import ComputeTemperature
from pypart import ComputeGravity
from pypart import ComputeVerletIntegration

import exercise5
import exercise6


def main(nsteps, freq, filename, particle_type, timestep):

    # print(pypart.__file__)

    if particle_type == "planet":
        PlanetsFactory.getInstance()
    elif particle_type == "ping_pong":
        PingPongBallsFactory.getInstance()
    elif particle_type == "material_point":
        MaterialPointsFactory.getInstance()
    else:
        print("Unknown particle type: ", particle_type)
        sys.exit(-1)

    factory = ParticlesFactoryInterface.getInstance()

    def createComputes(self, timestep):

        if particle_type == "planet":

            try:
                compute_grav = ComputeGravity()
                compute_verlet = ComputeVerletIntegration(timestep)

                G = 6.67384e-11  # m^3 * kg^-1 * s^-2
                UA = 149597870.700  # km
                earth_mass = 5.97219e24  # kg
                G /= (UA * 1e3)**3  # UA^3 * kg^-1 * s^-2
                G *= earth_mass    # UA^3 * earth_mass^-1 * s^-2
                G *= (60*60*24)**2  # UA^3 * earth_mass^-1 * day^-2

                compute_grav.setG(G)
                compute_verlet.addInteraction(compute_grav)
                self.system_evolution.addCompute(compute_verlet)

            except Exception as e:
                help(compute_grav)
                raise e

        elif particle_type == 'material_point':

            try:
                compute_temp = ComputeTemperature()
                compute_temp.conductivity = 1
                compute_temp.L = 2
                compute_temp.capacity = 1
                compute_temp.density = 1
                compute_temp.deltat = 1
                self.system_evolution.addCompute(compute_temp)
            except Exception as e:
                help(compute_temp)
                raise e

    evol = factory.createSimulation(filename, timestep, createComputes)

    dumper = CsvWriter("out.csv")
    dumper.write(evol.getSystem())

    evol.setNSteps(nsteps)
    evol.setDumpFreq(freq)
    evol.evolve()


if __name__ == "__main__":
    
    # Parse command-line arguments
    parser = argparse.ArgumentParser(description='Particles code')
    parser.add_argument('--exercise', type=str, help='Specify the name of the exercise (main, compute, launchsim, optimization)')
    
    # Common arguments for exercises
    parser.add_argument('--planet_name', type=str, help='Specify the name of the planet')
    parser.add_argument('--plot', action='store_true', help='Plot the resulting trajectories')

    # Arguments for the main exercise
    parser.add_argument('--nb_steps', type=int, help='Specify the number of steps to perform')
    parser.add_argument('--freq', type=int, help='Specify the frequency for dumps')
    parser.add_argument('--filename', type=str, help='Start/input filename')
    parser.add_argument('--particle_type', type=str, help='Particle type')
    parser.add_argument('--timestep', type=float, help='Timestep')

    # Arguments for specific exercises
    parser.add_argument('--directory', type=str, help='Specify the directory containing the output files')
    parser.add_argument('--directory_ref', type=str, help='Specify the directory containing the reference output files')
    parser.add_argument('--scale', type=int, help='Scale factor for the velocity')
    parser.add_argument('--max_iter', type=int, help='Maximum iteration for the optimization')
    parser.add_argument('--init_guess', type=float, help='Initial guess for the optimization')

    args = parser.parse_args()

    # Execute the selected exercise based on the provided argument
    if args.exercise == 'main':
        main(args)
    elif args.exercise == 'compute':
        exercise5.compute(args.planet_name, args.directory, args.directory_ref, args.plot)
    elif args.exercise == 'launchsim':
        exercise6.launchsim(args.planet_name, args.filename, args.nb_steps, args.freq, args.scale, args.plot)
    else:
        print("Unknown exercise:", args.exercise)
        sys.exit(-1)

