# Import necessary modules

import argparse
import numpy as np
import matplotlib.pyplot as plt
from scipy.optimize import fmin

# Import functions from exercise6 module

from exercise6 import generateInput, launchParticles, runAndComputeError

# Define the optimization function with clear and descriptive parameters
def optimization(planet_name: str, filename: str, nb_steps: int, freq: int, max_iter: int, init_guess: int, plot: int):
    # Use the fmin function from scipy.optimize for optimization
    opt_init_vel, fopt, iter, funcalls, warnflag, allvecs = fmin(
        runAndComputeError,
        [init_guess],
        args=(planet_name, filename, nb_steps, freq),
        maxiter=max_iter,
        full_output=True,
        retall=True,
        disp=True,
        xtol=1e-4,
        ftol=1e-4
    )

    # Extract optimization results
    scalings = allvecs
    errors = np.zeros(len(scalings))

    # Calculate errors for each scaling factor at every optimization step
    for i, scaling in enumerate(scalings):
        errors[i] = runAndComputeError(scaling, planet_name, filename, nb_steps, freq)

    # Print errors and scaling factors
    print('errors:', errors)
    print('scalings:', scalings)

    # Plot error vs scaling factor
    plt.figure()
    plt.semilogy(scalings, errors, marker=".")
    plt.xlabel("scaling")
    plt.ylabel("error")
    plt.grid()
    plt.show()

# Guard statement to ensure the code is executed only when this script is run directly
if __name__ == "__main__":
    # Parse command-line arguments using argparse
    parser = argparse.ArgumentParser(description='Optimization of particle simulation')
    parser.add_argument('--planet_name', type=str, help='Name of the planet')
    parser.add_argument('--filename', type=str, help='Input filename')
    parser.add_argument('--nb_steps', type=int, help='Number of steps')
    parser.add_argument('--freq', type=int, help='Frequency for dumps')
    parser.add_argument('--max_iter', type=int, help='Maximum number of iterations')
    parser.add_argument('--init_guess', type=int, help='Initial guess')
    parser.add_argument('--plot', type=int, help='Plotting flag')

    # Parse and store command-line arguments
    args = parser.parse_args()

    # Call the optimization function with parsed arguments
    optimization(args.planet_name, args.filename, args.nb_steps, args.freq, args.max_iter, args.init_guess, args.plot)
