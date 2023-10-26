#include <iostream>
#include "series.hh"

int main(int argc, char *argv[]) {
    int N = std::atoi(argv[1]); //N: the maximum iterator
    int M = std::atoi(argv[2]); //M: if 0: ComputeArithmetic. if 1: Compute Pi

    if (N <= 0) {
        std::cout << "N should be a positive integer." << std::endl;
    } else {
      if (M==0){
        ComputeArithmetic numberSum;
        double result = numberSum.compute(N);
        std::cout << "The sum of numbers from 1 to " << N << " is: " << result << std::endl;
      }
      else if (M==1){
        ComputePi numberSum;
        double result = numberSum.compute(N);
        std::cout << "Pi convergene using integers from 1 to " << N << " is: " << result << std::endl;
      }
    }

    return 0;
}