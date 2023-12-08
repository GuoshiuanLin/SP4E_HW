### Homework 3

### Running the program
Build the program as the following: 
```
cd SP4E_HW/HW3
mkdir build
cd build 
cmake ..
make
```

### Running the tests
After building the program as explained in the previous step, it is possible to run the tests as following:

For the FFT transform test:
```
cd build
./test_fft
```
For the Kepler test:
```
cd build
./test_kepler
```
For the Validating tests for 
4.2 - initial homogeneous temperature and no heat flux,
4.3 - volumetric heat source of a sinusoidal form,
4.4 - volumetric heat source of a linear form:
```
cd build
./test_temp
```

test_temp file for each test is referring to CSV Writer creates a file inside the build directory. 
For example for Excercise 4.2, 4.3 and 4.4 written files are "homogeneous_temperature.csv", "sinus_vol_heat.csv" and "lin_vol_heat.csv" respectively.

Generated .csv files can be used to control, track and visualize the results generated from every test (Ex 4.2, 4.3 and 4.4) in Paraview, as explained in continuation for Exercise 4.6 


