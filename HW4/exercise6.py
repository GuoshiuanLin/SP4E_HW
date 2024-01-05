import argparse

import csv
import numpy as np
import matplotlib.pyplot as plt

from pypart import ParticlesFactoryInterface, PlanetsFactory
from pypart import CsvWriter
from pypart import ComputeGravity
from pypart import ComputeVerletIntegration

from exercise5 import readPositions, computeError

def generateInput(scale: float, planet_name: str, input_filename: str, output_filename: str) -> None:
    """Generates an input file with scaled velocities for a specific planet.

    Parameters
    ----------
    scale : float
        Scale factor of the velocity.
    planet_name : str
        Name of the planet.
    input_filename : str
        File containing the initial values.
    output_filename : str
        File containing the scaled values.
    """
    new_csv_content = []
    with open(input_filename, 'r') as file:
        csvreader = csv.reader(file, delimiter=' ')
        for row in csvreader:
            new_row = [x for x in row if x != '']
            if planet_name.lower() in row:
                new_row[3:6] = np.array(new_row[3:6], dtype=float) * scale
            new_csv_content.append(new_row)

    np.savetxt(output_filename, new_csv_content, delimiter=" ", fmt="%s")
    print(f'Scaled values are saved in {output_filename}')

def launchParticles(input: str, nb_steps: int, freq: int) -> None:
    """Launches the particles simulation.

    Parameters
    ----------
    input : str
        Input file.
    nb_steps : int
        Number of steps.
    freq : int
        Frequency of the output.
    """
    timestep = 1

    PlanetsFactory.getInstance()
    factory = ParticlesFactoryInterface.getInstance()

    def createComputes(self, timestep):
        compute_grav = ComputeGravity()
        compute_verlet = ComputeVerletIntegration(timestep)

        G = 6.67384e-11  # m^3 * kg^-1 * s^-2
        UA = 149597870.700  # km
        earth_mass = 5.97219e24  # kg
        G /= (UA * 1e3)**3  # UA^3 * kg^-1 * s^-2
        G *= earth_mass  # UA^3 * earth_mass^-1 * s^-2
        G *= (60 * 60 * 24)**2  # UA^3 * earth_mass^-1 * day^-2

        compute_grav.setG(G)
        compute_verlet.addInteraction(compute_grav)
        self.system_evolution.addCompute(compute_verlet)

    evol = factory.createSimulation(input, timestep, createComputes)
    dumper = CsvWriter("out.csv")
    dumper.write(evol.getSystem())
    evol.setNSteps(nb_steps)
    evol.setDumpFreq(freq)
    evol.evolve()
    print('Simulation for planet is saved at out.csv')

def runAndComputeError(scale: float, planet_name: str, input: str, nb_steps: int, freq: int, plot: bool = False) -> float:
    """Runs the simulation and computes the error.

    Parameters
    ----------
    scale : float
        Scale factor of the velocity.
    planet_name : str
        Name of the planet.
    input : str
        Init file.
    nb_steps : int
        Number of simulation steps.
    freq : int
        Frequency of logging.

    Returns
    -------
    float
        Error between the computed trajectory and the reference one.
    """
    generateInput(scale, planet_name, input, 'scaled_init.csv')
    launchParticles('scaled_init.csv', nb_steps, freq)

    positions = readPositions(planet_name, 'dumps')
    positions_ref = readPositions(planet_name, 'trajectories')
    print(positions)

    error = computeError(positions, positions_ref)

    if plot:
        fig, axs = plt.subplots(1, 3, figsize=(12, 3))
        for i, ax_name in enumerate(['x', 'y', 'z']):
            axs[i].plot(positions[:, i], label='computed', lw=1.7, color='firebrick')
            axs[i].plot(positions_ref[:, i], ':', label='reference', lw=1.7, color='steelblue')
            axs[i].set_ylabel(ax_name)
            axs[i].set_xlabel('Days [#]')

        axs[2].legend()
        plt.suptitle(f'{planet_name} position (error: {round(error,3)}, scale: {scale})')
        fig.savefig(f'{planet_name}_positions.png')
        plt.tight_layout()
        plt.show()

    return error

def launchsim(planet_name: str, filename: str, nb_steps: int, freq: int, scale: int, plot: int):
    runAndComputeError(scale, planet_name, filename, nb_steps, freq, plot)

# Command-line argument parsing
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Script to launch simulation with scaled velocity input and compute the error in the trajectory')
    parser.add_argument('--planet_name', type=str, help='Name of the planet')
    parser.add_argument('--filename', type=str, help='Input filename')
    parser.add_argument('--nb_steps', type=int, help='Number of simulation steps')
    parser.add_argument('--freq', type=int, help='Frequency of logging')
    parser.add_argument('--scale', type=int, help='Scale factor for the velocity')
    parser.add_argument('--plot', action='store_true', help='Plot the resulting trajectories')

    args = parser.parse_args()

    launchsim(args.planet_name, args.filename, args.nb_steps, args.freq, args.scale, args.plot)
