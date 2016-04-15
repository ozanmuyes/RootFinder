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
3. Enter interval or start point(s)
4. Follow instructions

## Command-line Parameters
* -v (verbose flag) indicates verbosity level;
  * -v for fatal,
  * -vv for fatal + error,
  * -vvv for fatal + error + warning,
  * -vvvv for fatal + error + warning + debug logging.
* --no-step Disables step-by-step calculation
* --function Accepts function string as argument
* -o Manipulates standard output stream and dump the output to "rootfinder.out" file
* --version Print version of the program
