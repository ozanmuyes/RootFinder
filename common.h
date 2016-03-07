#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

// To support step-by-step execution of method define STEP_BY_STEP
#define STEP_BY_STEP

//#define VERBOSE_MODE

struct Interval {
  double start;
  double end;
};

struct FindRootInfo {
  unsigned int max_iterations;
  double tolerance;
  double error;
  double last_root;
  double midpoint;
  // If both false then root is found
  unsigned int interval_start_changed;
  unsigned int interval_end_changed;
};

int get_input(char *output, unsigned int len);
int get_input_wo_space(char *output, unsigned int len);
int get_input_int();
double get_input_double();

int compile_function(const char *expression);
double f(double variable_x);

#endif // COMMON_H_INCLUDED
