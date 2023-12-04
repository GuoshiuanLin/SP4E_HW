"""
    Python script to generate a heat distribution within a provided radius as given in Equation (5)
    Usage example:
    python3 heat_distribution.py --x_limits -1.0 1.0 --y_limits -1.0 1.0 --num_particles_x 1000 --num_particles_y 1000 --plot --radius 0.5 --filename heat_distribution.csv     
"""

import argparse
import numpy as np

import matplotlib.pyplot as plt


def arg_parser():
    parser = argparse.ArgumentParser(description="Configure program parameters via command-line arguments")
    parser.add_argument('--x_limits', type=float, nargs='+', default=[-1., 1.],
                        help='Upper and lower limits for the x-axis')
    parser.add_argument('--y_limits', type=float, nargs='+', default=[-1., 1.], 
                        help='Upper and lower limits for the y-axis')
    parser.add_argument('--num_particles_x', type=int, default=10, 
                        help="Number of particles in the x-direction")
    parser.add_argument('--num_particles_y', type=int, default=10, 
                        help="Number of particles in the y-direction")
    parser.add_argument('--plot', action='store_true', 
                        help="Plots the heat distribution")
    parser.add_argument('--radius', type=float, default=0.1, 
                        help="Radius of the heat distribution")
    parser.add_argument('--filename', default="heat_distribution.csv", 
                        help="Output path for saving heat data")
    return parser.parse_args()

def heat_function(x, y, radius):
    """
    Compute the heat distribution at coordinates (x, y).

    Parameters
    ----------
    x : float
        x-coordinate.
    y : float
        y-coordinate.
    radius : float
        Radius of the circular region.

    Returns
    -------
    int
        1 if the coordinates (x, y) are inside the circle of the given radius, 0 otherwise.
    """

    #Check if the coordinates are inside the circular region
    if x**2 + y**2 < radius:
        return 1

    return 0

def boundary_constraint(x, y, x_limits, y_limits):
    """
    Enforce a zero temperature at the boundary and assign a random value otherwise.

    Parameters
    ----------
    x : float
        x-coordinate.
    y : float
        y-coordinate.
    x_limits : np.ndarray
        Array containing the x boundaries.
    y_limits : np.ndarray
        Array containing the y boundaries.

    Returns
    -------
    float
        Temperature value. 0 at the boundary, random value otherwise.
    """
    # Check if the coordinates are at the boundary
    if x in x_limits or y in y_limits:
        return 0

    # Assign a random temperature value if not at the boundary
    return np.random.randint(-10, 10)

def plot_heat_distribution(num_particles_x, num_particles_y, temperature_values):
    """
    Plots the heat distribution with temperature color code.

    Parameters
    ----------
    num_particles_x : int
        Number of particles in the x direction.
    num_particles_y : int
        Number of particles in the y direction.
    temperature_values : np.ndarray
        Array containing the temperature values.
    """

    # Create a 2D plot of the temperature distribution
    
    plt.subplots()
    plt.pcolormesh(temperature_values.reshape((num_particles_x, num_particles_y)))

    # Add colorbar and labels
    cbar = plt.colorbar()
    cbar.set_label('Temperature', rotation=90)
    plt.xlabel('Particle Index in X Direction')
    plt.ylabel('Particle Index in Y Direction')
    plt.title('Temperature Distribution in a Grid')

    # Display the plot
    plt.show()
    
def main():
    # Parse command-line arguments
    args = arg_parser()

    # Extract parameters from arguments
    x_limits, y_limits = args.x_limits, args.y_limits
    num_particles_x, num_particles_y = args.num_particles_x, args.num_particles_y
    radius, filename, plot = args.radius, args.filename, args.plot

    # Calculate grid size
    grid_size = num_particles_x * num_particles_y

    # Generate x and y values
    x_values = np.linspace(x_limits[0], x_limits[1], num_particles_x)
    y_values = np.linspace(y_limits[0], y_limits[1], num_particles_y)

    # Initialize arrays for positions, velocity, force, masses, temperature, and heat rate
    positions = np.random.random((grid_size, 3))
    velocity = np.zeros((grid_size, 3))
    force = velocity.copy()
    masses = 1e9 * (np.random.random((grid_size, 1)) + 1)
    heat_rate = np.zeros((grid_size, 1))
    temperature = np.zeros((grid_size, 1))

    # Populate arrays with values
    ind = 0
    for x in x_values:
        for y in y_values:
            positions[ind, :] = np.array([x, y, 0])
            temperature[ind] = boundary_constraint(x, y, x_limits, y_limits)
            heat_rate[ind] = heat_function (x, y, radius)
            ind += 1

    # Combine data arrays horizontally
    file_data = np.hstack((positions, velocity, force, masses, temperature, heat_rate))

    # Save data to a file
    np.savetxt(filename, file_data, delimiter=" ")
    print(f"Heat distribution file is saved at {filename}")

    # Plot heat distribution if specified
    if plot:
        plot_heat_distribution(num_particles_x, num_particles_x, temperature)

if __name__ == "__main__":
    main()