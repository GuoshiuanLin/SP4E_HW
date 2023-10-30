#include <iostream>
#include <fstream>
#include "series.hh"
#include "dumper.hh"

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