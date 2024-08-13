#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>

class Calculator
{
public:
    static std::string binary_to_decimal(std::string str);
    static std::string decimal_to_binary(float arg_input);
    static std::string addition_algorithm(std::string bin_A, std::string bin_B, bool debug_mode);
    static std::string product_algorithm(std::string bin_A, std::string bin_B, bool debug_mode);
};

#endif // CALCULATOR_H
