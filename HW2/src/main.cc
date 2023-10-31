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
    // int N = std::atoi(argv[1]); //N: the maximum iterator
    // int M = std::atoi(argv[2]); //M: if 0: ComputeArithmetic. if 1: Compute Pi
    // int freq = std::atoi(argv[3]); //freq: frequnecy to output results to the screen
    // std::string dumperType = argv[4]; // 'print' or 'write'

    // Create a stringstream object to concatenate the arguments
    std::stringstream argsConcatenated;
    // Concatenate the arguments into a single string
    for (int i = 1; i < argc; ++i) {
        argsConcatenated << argv[i] << " ";
    }
    // Extract the values from the concatenated string
    int N, M, freq;
    std::string dumperType;
    argsConcatenated >> N >> M >> freq >> dumperType;

    if (N <= 0) {
        std::cout << "N should be a positive integer." << std::endl;
    } 
    Series* numberSum = nullptr;
    if (M==0){
      // Instanciate a ComputeArithmetic object and call the compute method
      numberSum = new ComputeArithmetic();
      // ComputeArithmetic numberSum;
      // double result = numberSum.compute(N)->compute(N);
      std::cout << "The sum of numbers from 1 to " << N << " is: " << std::endl;
      // std::cout << "The sum of numbers from 1 to " << N << " is: " << result << std::endl;
      // Instanciate a PrintSeries object and call the dump method
      // PrintSeries object(freq, N, numberSum);
      // object.dump();
    }
    else if (M==1){
      // Instanciate a ComputePi object and call the compute method
      numberSum = new ComputePi();
      // ComputePi numberSum;
      // double result = numberSum.compute(N);
      std::cout << "Pi convergene using integers from 1 to " << N << " is: " << std::endl;
      // std::cout << "Pi convergene using integers from 1 to " << N << " is: " << result << std::endl;
      // Instanciate a PrintSeries object and call the dump method
      // PrintSeries object(freq, N, numberSum);
      // object.dump();
    }
    else if (M==2){ //To calculate RiemannIntegral using the main file
      std::string functionType;

      std::cout <<"Enter the lower bound (a):";
      double lowerBound;
      std::cin >> lowerBound;

      std::cout <<"Enter the upper bound (b):";
      double upperBound;
      std::cin >> upperBound;

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

      int numIntervals;
      std::cout <<"Enter the number of intervals:";
      std::cin >> numIntervals;

      //Create a Riemann Integral object

      RiemannIntegral riemann (function, lowerBound, upperBound, numIntervals);
      //Calculate the Riemann sum

      double result = riemann.CalculateRiemannSum();
      std::cout <<"Integral of function" <<functionType << "from" <<lowerBound<< "to" <<upperBound << ":" << result << std::endl;

    }
     
  double result = numberSum->compute(N);
  std::cout << result << std::endl;

  // determines to print or write 
  if (dumperType=="print"){
      PrintSeries object(freq, N, *numberSum);
      // Create and open a file stream for writing to a file
      std::ofstream outputFile("print_output.txt");
      if (outputFile.is_open()) {
        // Call dump with the file stream as the argument
        object.dump(outputFile);
        // Close the file stream when done
        outputFile.close();
    } else  {
      std::cerr << "Failed to open the output file." << std::endl;
    }
      // Call dump with the standard output stream (std::cout)
      object.dump();
      
  } else if (dumperType=="write"){
      WriteSeries object(N, *numberSum);
      object.setSeparator(' '); // choose the seperator: , or |. Otherwise create .txt file
      object.dump();
  }
  delete numberSum;
    
    return 0;
}