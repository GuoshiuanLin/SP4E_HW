#include <iostream>
#include <fstream>
#include <sstream>      
#include <cstring>
#include <string>
#include <memory>
#include <fstream>
#include <cmath>
#include "series.hh"
#include "dumper.hh"
#include "RiemannIntegral.hh"


int main(int argc, char *argv[]) {

    // Create a stringstream object to concatenate the arguments
    std::stringstream argsConcatenated;
    // Concatenate the arguments into a single string
    for (int i = 1; i < argc; ++i) {
        argsConcatenated << argv[i] << " ";
    }
    // Extract the values from the concatenated string
    int maxiter, freq;
    std::string SeriesType;
    std::string dumperType;
    argsConcatenated >> maxiter >> SeriesType >> freq >> dumperType;

    // make sure maximum iterator is above 0
    if (maxiter <= 0) {
        std::cout << "N should be a positive integer." << std::endl;
    } 
    Series* numberSum = nullptr;
    if (SeriesType=="ComputeArithmetic"){
      // Instanciate a ComputeArithmetic object 
      numberSum = new ComputeArithmetic();
      // show final result to the screen
      std::cout << "The sum of numbers from 1 to " << maxiter << " is: " << std::endl;
    }
    else if (SeriesType=="ComputePi"){
      // Instanciate a ComputePi object 
      numberSum = new ComputePi();
      // show final result to the screen
      std::cout << "Pi convergene using integers from 1 to " << maxiter << " is: " << std::endl;
    }
    else if (SeriesType=="RiemannIntegral"){ 
      // Let the user define the lower and upper bound and the funtion to instanciate a RiemannIntegral object 
      std::string functionType;
      // User should enter the lower and upper bound
      std::cout <<"Enter the lower bound (a):";
      double lowerBound;
      std::cin >> lowerBound;

      std::cout <<"Enter the upper bound (b):";
      double upperBound;
      std::cin >> upperBound;
      // User should choose one of the functions 'cube', 'cos', or 'sin' to implement
      std::cout << "Choose the function type ('cube', 'cos', or 'sin'): ";
      std::cin >> functionType;   

      //Define the function to be integrated 
      std::function<double(double)>function;

      if (functionType == "cube")
      {
        //Case 1 - f(x)=x^3
        auto calcCube = [](double x) {return 1.0 * x * x * x; };
        function = calcCube;
      }
      else if (functionType == "cos") {
      //Case 2 - f(x)=cos(x)
      auto cosine = [](double x) {return std::cos(x); };
      function = cosine;
      }
      else if (functionType == "sin"){
      //Case 3 - f(x)=sin(x)
      auto sin = [](double x) {return std::sin(x); }; 
      function = sin;
      }
      else {
      std::cout << "Invalid function type. Choose 'cube', 'cos', or 'sin'."<<std::endl;
      return 1; //Exit with and error code
      }

      //Instanciate a RiemannIntegral object 
      numberSum = new RiemannIntegral(function, lowerBound, upperBound);
      //Calculate the Riemann integral
      double result = numberSum->compute(maxiter);
      // print the final result to the screen
      std::cout <<"Integral of function " <<functionType << " from " <<lowerBound<< " to " <<upperBound << ": " << result << std::endl;
    }
     
  // print result of the summation of ComputeArithmetic or ComputePi to the screen
  double result = numberSum->compute(maxiter);
  std::cout << result << std::endl;

  // determines to print or write 
  if (dumperType=="print"){
      // save results to print_output.txt and print results to screen 
      PrintSeries print_series(freq, maxiter, *numberSum);
      // Create and open a file stream for writing to a file
      std::ofstream outputFile("print_output.txt");
      if (outputFile.is_open()) {
        // Call dump with the file stream as the argument
        print_series.dump(outputFile);
        // Close the file stream when done
        outputFile.close();
    } else  {
      std::cerr << "Failed to open the output file." << std::endl;
    }
      // Call dump with the standard output stream (std::cout)
      print_series.dump();
      
  } else if (dumperType=="write"){
      // write results to file
      WriteSeries write_series(maxiter, *numberSum);
      write_series.setSeparator(' '); // choose the seperator: , or |. Otherwise create .txt file
      write_series.dump();
  }
  delete numberSum;
    
    return 0;
}