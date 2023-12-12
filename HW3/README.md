### Homework 3
## Introduction 
This is the homework for SP4E 2023 Assignment 3

### Authors
- Jakov Oreb: exercise 1 & 4
- Guo-Shiuan Lin: exercise 2 & 3 
 
### Prerequisites
- C++ Compiler
- python
- matplotlib
  
### Running the program
Build the program as the following: 
```
git clone https://github.com/GuoshiuanLin/SP4E_HW.git
cd SP4E_HW/HW3
mkdir build
cd build 
cmake DENABLE_FFT=ON .. # turn to OFF to disable FFT
make
```

### Running the tests
After building the program as explained in the previous step, it is possible to run the tests as following:

For the FFT transform test:
```
cd build
./test_fft
```
For the Validating tests for 
4.2 - initial homogeneous temperature and no heat flux,
4.3 - volumetric heat source of a sinusoidal form,
4.4 - volumetric heat source of a linear form:
```
cd build
./test_temp
```

test_temp file for each test refers to CSV Writer creating a file inside the build directory. 
For example for Excercise 4.2, 4.3 and 4.4 written files are "homogeneous_temperature.csv", "sinus_vol_heat.csv" and "lin_vol_heat.csv" respectively.

Generated .csv files can be used to control, track, and visualize the results generated from every test (Ex 4.2, 4.3, and 4.4) in Paraview, as explained in continuation for Exercise 4.6 

### Main Classes
#### `MaterialPoint`
`MaterialPoint` provides access to material data via `getTemperature()` and `getHeatrate()`
#### `Matrix`
`Matrix` represents a square matrix.  It defines two iterators: `MatrixIterator` for iterating over matrix elements and `MatrixIndexIterator` for iterating over matrix indices and values 
#### `MaterialPointsFactory`
It includes functionalities such as creating particles `createParticle()`, creating a simulation system `createSimulation()`. It provides a singleton instance of the material points factory via `getInstance()`.
#### `fft`
`fft` implements the Fast Fourier Transform (FFT) using the FFTW library. To perform FFT on a complex matrix: `transform(inputMatrix)`; to perform Inverse FFT on a complex matrix: `itransform(fftResult)`; to compute frequencies for a given size: `computeFrequencies(4)`. 

### Generating initial heat distribution

```python
python3 heat_distribution.py --x_limits -1.0 1.0 --y_limits -1.0 1.0 --num_particles_x 10 --num_particles_y 10 --plot --radius 0.8 --filename heat_distribution.csv
```

```x_limits```          - limits of X coordinates (upper and lower)\
```y_limits```          - limits of Y coordinates (upper and lower)\
```num_particles_x```   - number of particles (in X direction)\
```num_particles_y```   - number of particles (in Y direction)\
```radius```            - provided radius for heat distribution\
```filename```          - name of the output file\

### Executing particles code
It is important to note that there should be a folder named ```dumps``` under the build directory. To have that file, first, you have to create it, running the following:

```
cd build
mkdir dumps
./particles arg1 arg2 arg3 arg4 arg5
```
- arg1: int, number of iterations
- arg2: int, dump frequency
- arg3: str, file with initial heat distribution, e.g., ./heat_distribution.csv
- arg4: str, particle type, e.g., material_point
- arg5: float, time step

For example,
```
./particles 10 1 ./heat_distribution.csv material_point 0.0001
```

**Exercise 4.5:** 
_Implement a python script to generate a heat distribution within a provided radius as given in Equation (5).Explain in the README how you integrate this condition within the existing code._

A circular heat distribution is generated with $`x^2 + y^2 < R`$ Equation. As a result, a CSV file for the heat distribution (heat_distribution.csv) is generated with the information of positions, velocity, force, masses, temperature, and heat rate. The generated CSV file can be used as an input file for the simulation through Paraview as explained in **Exercise 4.6**. 


In the exercise, a boundary condition is specified for the temperature field, requiring it to be zero at the boundary of the domain and remain constant over time. To implement this boundary condition, a boolean variable named ```isBoundary``` is introduced in the material_point class. The purpose of this boolean member is to serve as a flag indicating whether a material point is located at the boundary. When ```isBoundary``` is set to true, the temperature is explicitly set to zero during the computation of temperatures at the boundaries. This ensures the enforcement of the specified boundary condition, maintaining a constant temperature of zero at the domain boundaries throughout the simulation.

**Exercise 4.6:** 
_Describe how to launch such a simulation which will produce dumps observable with Paraview for a grid of 512 × 512 particles._

Two options and descriptions on how to launch a simulation for a grid of 512 × 512 particles. The preferred option is Option 1.
  
**Option 1: Directly from the heat_distribution.py file produced in Excercise 4.5:**  

```
cd ..
python3 heat_distribution.py --x_limits -1.0 1.0 --y_limits -1.0 1.0 --num_particles_x 512 --num_particles_y 512 --plot --radius 0.8 --filename heat_distribution.csv
```
This will generate `heat_distribution.csv` file with a grid of 512 × 512 particles

Required steps to visualize the simulation with Paraview:
1. File > Open > 'heat_distribution.csv' > Open Data With CSV Reader
2. In the Properties window
    - Deselect 'Have Headers'
    - Change 'Field Delimiter Characters' to space as ' '
    - Click on Apply
3. On the menu, go to Filters > Search and search for 'Table to Points' and press enter
    The shortcut to perform this option is to press Ctrl + Space and start typing 'Table to Points' and after pressing enter
4. In the Properties window 
    - Choose X,Y, and Z columns as Field 0 for X Column, Field 1 for Y Column, and Field 2 for Z Column
    - Select 2D points
    - Click Apply
    - Mark an "eye" on generated "TableToPoints1"
    - Under Display (GeometryRepresentation) change Coloring to Field 10 for temperature visulatization, Field 11 for the heat visualization
    - Click show/hide color legend for the legend visualization 

**Option 2:** From the `dumps` directory, after executing the particles code:**
    
    cd build
    cd dumps

Required steps to visualize the simulation with Paraview:
Same as above, except in step 4, Under Display (GeometryRepresentation) change Coloring to Field 12 for temperature visulatization, Field 14 for the heat visualization

Results from Paraview are presented below for temperature and heat respectively:

<p align="left">
  <img src="SP4E_HW/HW3/figures/temperature_visualization.png" width="1000" />
</p>

<p align="left">
  <img src="SP4E_HW/HW3/figures/heat_visualization.png" width="1000" />
</p>


