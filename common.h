#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <float.h>
#include <math.h>

#include "tinyexpr.h"

// To support step-by-step execution of method define STEP_BY_STEP
#define STEP_BY_STEP

//#define VERBOSE_MODE

struct Interval {
  double start;
  double end;
};

int get_input(char *output, unsigned int len);
int get_input_wo_space(char *output, unsigned int len);
int get_input_int();
double get_input_double();

int compile_function(const char *expression);
double f(double variable_x);

#endif // COMMON_H_INCLUDED
