# ALU-IEEE-754 Converter

The ALU-IEEE-754 Converter is a C++ application designed to perform arithmetic operations on floating-point numbers following the IEEE 754 standard. It provides a graphical user interface (GUI) for users to input numbers and select operations, facilitating accurate and efficient computations.

## Features

- **IEEE 754 Compliance**: Ensures that all floating-point arithmetic adheres to the IEEE 754 standard.
- **Basic Arithmetic Operations**: Supports addition, subtraction, multiplication, and division.
- **Graphical User Interface**: Offers an intuitive GUI for easy interaction.

## Prerequisites

- **Qt Framework**: The application utilizes the Qt framework for its GUI components. Ensure that you have the Qt development environment installed on your system.

## Usage

- **Input Numbers**: Enter the floating-point numbers you wish to operate on into the designated input fields.
- **Select Operation**: Choose the desired arithmetic operation (addition, subtraction, multiplication, or division) from the available options.
- **Compute Result**: Click the "Calculate" button to perform the operation. The result will be displayed on the interface.

## Project Structure

- `main.cpp`: The entry point of the application.
- `calculator.cpp` and `calculator.h`: Contain the logic for performing arithmetic operations.
- `mainwindow.cpp`, `mainwindow.h`, and `mainwindow.ui`: Define the GUI and its interactions.
- `ALU_converter.pro`: The Qt project file containing build configurations.

## License

This project is licensed under the MIT License.
