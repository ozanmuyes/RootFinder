# RootFinder
Implementation of various root finding methods (e.g. bisection, regula falsi etc.)

## Installation
```bash
# Pull the project
git clone https://github.com/ozanmuyes/RootFinder.git
# Dependencies you might need to install
# sudo apt-get update
# sudo apt-get install git build-essential
cd RootFinder

# Build and install the extension
gcc main.c - o RootFinder

# Start the program
./RootFinder
```

## Usage
1. Select method
2. Enter function
3. Enter interval
4. Follow instructions

## Remarks
1. Add
```C
#define STEP_BY_STEP
```
to main.c to enable step-by-step calculation and verbose output of each step.
2. Enter intervals other than opposite sign of each other (i.e. do NOT enter -5 and 5 as interval)
