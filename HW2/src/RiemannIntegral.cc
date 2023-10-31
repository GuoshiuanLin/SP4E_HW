

#include <iostream>
#include <fstream>
#include <sstream>      
#include <cstring>
#include <string>
#include <memory>
#include <fstream>
#include <cmath>
#include "RiemannIntegral.hh"

int main(int argc, char *argv[]){


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

return 0;
};
