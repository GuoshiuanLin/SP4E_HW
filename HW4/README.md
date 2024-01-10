### Homework 4
### Updated:
Now the part on pybind (pypart.cc) is done!

## Introduction 
This is the homework for SP4E 2023 Assignment 4

### Authors
- Jakov Oreb
- Guo-Shiuan Lin

### Prerequisites

- cmake minimum required (VERSION 3.1)
- C++ Compiler (-std=c++14)
- FFTW version 3.3.10
- python 3.11
- numpy v 1.23.4
- matplotlib v 3.6.2
- pybind11 v2.12.0

### Compile the code before running simulation

Build the program as the following: 
```
git clone https://github.com/GuoshiuanLin/SP4E_HW.git
cd SP4E_HW/HW4
mkdir build
cd build
cmake ..
make
```

### Important Note:
If you receive submodule error (for example: "eigen is missing") run the following commands in the repository's root direction:
```
$ git submodule add -f https://gitlab.com/libeigen/eigen.git
$ git submodule add -f https://github.com/pybind/pybind11.git
```

If you receive and error "google test is missing you can clone googletest from the GitHub and build it separately, as following:
```
git clone https://github.com/google/googletest.git
cd googletest
mkdir build
cd build
cmake ..
make
```
After following this note, you will be able to build the progra

### Create dumps folder for holding output files

`mkdir dumps`

### To run the simulation
`python main.py` `nsteps` `freq` `filename` `particle_type` `timestep`

`nsteps`: number of steps to perform

`freq`: frequency for dumps

`filename`:start/input filename, e.g., `../init.csv`

`particle_type`: `planet`, `ping_pong`, or `material_point`

`timestep`: time step

E.g., 

`python main.py 30 1 ../init.csv planet 1`

### Exercise 4: Units of the code
To simulate the planet trajectories for 365 days with a timestep of 1 day, run the following command:

```bash
$ python3 main.py --exercise main --nb_steps 365 --freq 1 --filename ../init.csv --particle_type planet --timestep 1
```

Required arguments to run the exercise:

- ```--exercise```: Specify the name of the exercise. Choose from the following options: 
    - "main": Ensure the script works for all types of particles 
    - "compute": Compare planet positions
    - "launchsim": Launch simulation with scaled velocity input and compute the error in the trajectory
    - "optimize": Optimize initial velocity input and plot the evolution of the error vs the scaling factor
- ```--nb_steps```: Specify the number of steps to perform
- ```--freq```: Specify the frequency for data dumps
- ```--filename```: Specify the start/input filename
- ```--particle_type```: Specify the type of the particle
- ```--timestep```: Specify the timestep

### Exercise 5 : Compute the error of the simulation

To compute the error of the simulation use this script:
```bash
$ python3 main.py --exercise compute --planet_name mercury --directory dumps --directory_ref trajectories
$ python3 main.py --exercise compute --planet_name mercury --directory dumps --directory_ref trajectories --plot
```

To plot the trajectories, add `--plot` to the command. Plot will be saved as a figure.

Required arguments to run the exercise:

- ```--exercise```: Specify the name of the exercise. Choose from the following options: 
    - "main": Ensure the script works for all types of particles 
    - "compute": Compare planet positions
    - "launchsim": Launch simulation with scaled velocity input and compute the error in the trajectory
    - "optimize": Optimize initial velocity input and plot the evolution of the error vs the scaling factor
- ```--planet_name```: Specify the planet name     
- ```--directory```: Specify the directory containing output files
- ```--directory_ref```: Specify the directory containing reference output files
- ```--plot```: Plot the resulting trajectories

### Exercise 6 : Launching the particle code from python by generating the input

To launch the simulation with scaled velocity input and compute the error in the trajectory use this script. 
Example:
```bash
$ python3 main.py --exercise launchsim --planet_name mercury --filename ../init.csv --nb_steps 365 --freq 1 --scale 1 
$ python3 main.py --exercise launchsim --planet_name mercury --filename ../init.csv --nb_steps 365 --freq 1 --scale 1 --plot
```

To plot the trajectories, add `--plot` to the command. Plot will be saved as a figure.

Required arguments to run the exercise:

- ```--exercise```: Specify the name of the exercise. Choose from the following options: 
    - "main": Ensure the script works for all types of particles 
    - "compute": Compare planet positions
    - "launchsim": Launch simulation with scaled velocity input and compute the error in the trajectory
    - "optimize": Optimize initial velocity input and plot the evolution of the error vs the scaling factor
- ```--planet_name```: Specify the planet name   
- ```--filename```: Specify the start/input filename
- ```--nb_steps```: Specify the number of steps to perform
- ```--freq```: Specify the frequency for data dumps
- ```--scale```: Specify scale factor for the velocity
- ```--plot```: Plot the resulting trajectories

### Exercise 7 : Optimization

To optimize initial velocity input and plot the evolution of the error against the scaling factor:
```bash
$ python3 main.py --exercise optimization --planet_name mercury --filename ../init.csv --nb_steps 365 --freq 1 --max_iter 20 --init_guess 5 --plot
```

Required arguments to run the exercise:

- ```--exercise```: Specify the name of the exercise. Choose from the following options: 
    - "main": Ensure the script works for all types of particles 
    - "compute": Compare planet positions
    - "launchsim": Launch simulation with scaled velocity input and compute the error in the trajectory
    - "optimize": Optimize initial velocity input and plot the evolution of the error vs the scaling factor
- ```--planet_name```: Specify the planet name   
- ```--filename```: Specify the start/input filename
- ```--nb_steps```: Specify the number of steps to perform
- ```--freq```: Specify the frequency for data dumps
- ```--max_iter```: Specify maximum number of iterations for the optimization
- ```--init_guess```: Specify initial guess for the optimization
- ```--plot```: Plot the resulting trajectories

### Textual Answers
1.2. In class ParticlesFactory, createSimulation function has been overloaded to take functor as one of its argument. Comment on what this function is doing?

It means that `createSimulation` can accept a callable object or function with a specific signature. The purpose of providing this overloaded version is to allow users to customize or extend the behavior of the createSimulation function by changing the arguments in `createComputes`.
In this case, users can choose between "planet" and 'material_point'.

2.2. How will you ensure that references to Compute objects type are correctly managed in the pythonbindings?

One can use the py::return_value_policy when binding methods that return references or pointers to Compute objects. The correct policy depends on the ownership and lifetime management strategy of the underlying C++ objects.
