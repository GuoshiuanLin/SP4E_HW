import argparse
import csv
from pathlib import Path
import numpy as np
import matplotlib.pyplot as plt

def readPosition(planet_name: str, csv_file_path: str) -> np.ndarray:
    """Reads the position of the planet from a given CSV file.

    Parameters
    ----------
    planet_name : str
        Name of the planet.
    csv_file_path : str
        The path of the step-0000x.csv file.

    Returns
    -------
    np.ndarray
        Position of the planet at one time step.
    """
    with open(csv_file_path, 'r') as file:
        csvreader = csv.reader(file, delimiter=' ')

        for row in csvreader:
            # Check for case - insensitive match 
            if planet_name.lower() in [x.lower() for x in row]:
                # drop empty elements and convert to numpy array
                new_row = np.array([float(x) for x in row if x != ''])
                return new_row[:3]
    # If no match is found, return an array of NaN
    return np.array([np.nan, np.nan, np.nan])

def readPositions(planet_name: str, directory: str) -> np.ndarray:
    """Reads the position of the planet from all the files in a given directory.

    Parameters
    ----------
    planet_name : str
        Name of the planet.
    directory : str
        The path of the step-0000x.csv file.

    Returns
    -------
    np.ndarray
        Position of the planet for the entire course of the simulation.
    """
    # Get a list of CSV file names sorted by step
    csv_file_names = sorted(Path(directory).rglob('step-*.csv'))
    ndays = len(csv_file_names)
    planet_trajectory = np.empty((ndays, 3))

    # Read positions for each day
    for i, csv_file_name in enumerate(csv_file_names):
        planet_trajectory[i] = readPosition(planet_name, csv_file_name)

    return planet_trajectory

def computeError(positions: np.ndarray, positions_ref: np.ndarray) -> float:
    """Computes the error between the computed trajectory and the reference one.

    Parameters
    ----------
    positions : np.ndarray
        Computed trajectory.
    positions_ref : np.ndarray
        Reference trajectory.

    Returns
    -------
    float
        Error between the computed trajectory and the reference one.
    """
    # Compute Euclidean distance between corresponding positions
    return np.sqrt(np.sum((np.linalg.norm(positions - positions_ref, axis=1)**2)))

def compute(planet_name: str, directory: str, directory_ref: str, plot: int):
    # Read positions from given directories
    positions = readPositions(planet_name, directory)
    print(positions)

    positions_ref = readPositions(planet_name, directory_ref)

    # Compute the error between the reference and the computed trajectories
    error = computeError(positions, positions_ref)
    print(f'Error in {planet_name} trajectory is: {error}')

    if plot:
        # Plotting the trajectories
        fig, axs = plt.subplots(1, 3, figsize=(12, 3))
        for i, ax_name in enumerate(['x', 'y', 'z']):
            axs[i].plot(positions[:, i], label='computed', lw=1.7, color='red')
            axs[i].plot(positions_ref[:, i], ':', label='reference', lw=1.7, color='blue')
            axs[i].set_ylabel(ax_name)
            axs[i].set_xlabel('Days [#]')

        axs[2].legend()
        plt.suptitle(f'{planet_name} position (error: {round(error, 3)})')
        fig.savefig(f'{planet_name}_positions.png')
        plt.tight_layout()
        plt.show()

# Command-line argument parsing
if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Script to compare planet positions')
    parser.add_argument('--planet_name', type=str, help='Name of the planet')
    parser.add_argument('--directory', type=str, help='Directory containing the output files')
    parser.add_argument('--directory_ref', type=str, help='Directory containing the reference output files')
    parser.add_argument('--plot', action='store_true', help='Plot the resulting trajectories')

    args = parser.parse_args()

    # Call the compute function with parsed arguments
    compute(args.planet_name, args.directory, args.directory_ref, args.plot)
