#include <stdlib.h>
#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

#include "common.h"
#include "tinyexpr.h"
#include "bisection.h"
//#include "regula-falsi.h"

void tinyexpr_test(void);
int find_root(struct Interval *interval, unsigned int max_iterations, double tolerance, double *root);

// Proxy functions to method functions
int (*method_check_initial_conditions)(struct Interval *);
void (*method_find_root)(struct Interval *, struct FindRootInfo *, double *);

int main() {
  // 1. Method selection
mtd_sel:
  printf("Please select a method;\n\
1) Bisection\n\
2) Regula Falsi\n\n\
Choice: ");

  int method_idx = get_input_int();

  // 1.1. Set check initial conditions proxy function based on selected method
  switch (method_idx) {
    case 1: {
      method_check_initial_conditions = &bs_check_initial_conditions;
      method_find_root = &bs_find_root;

      break;
    }

    case 2: {
      //method_check_initial_conditions = &rf_check_initial_conditions;
      //method_find_root = &rf_find_root;

      break;
    }

    default: {
      printf("\nInvalid entry. ");

      goto mtd_sel;
    }
  }

  // 2. Function string entry
  char *function_string = (char *)malloc(sizeof(char) * 128);

fun_ent:
  printf("f(x) = ");
  get_input_wo_space(function_string, 128);

  // Easter egg
  if (strchr(function_string, 'x') == NULL) {
    printf("I also like to live dangerously!\n\n");

    goto fun_ent;
  }

  int compilation_result = compile_function(function_string);

  if (compilation_result < 0) {
    printf("Syntax error at column %d. Please fix your entry.\n\n", -compilation_result);

    goto fun_ent;
  }

  // 3. Interval entry
  struct Interval *interval = (struct Interval *)malloc(sizeof(struct Interval));

int_ent:
  printf("Enter interval start value: ");
  interval->start = get_input_double();

  printf("Enter interval end value: ");
  interval->end = get_input_double();

  // Check whether interval values is proper
  if (interval->start > interval->end) {
    // Since interval.start is greater than interval.end, swap each other
    double fTemp = interval->start;
    interval->start = interval->end;
    interval->end = fTemp;
  }

  // 4. Check initial conditions
  if ((*method_check_initial_conditions)(interval) > 0) {
    // Terminate with error (1; initial conditions did NOT pass)

    printf("Initial conditions can not obtained by given inputs. Please try again.\n\n");

    goto int_ent;
  }

  // 5. Enter iteration count or error tolerance
  unsigned int max_iterations = 0;
  double tolerance = 0;

  // TODO Get max_iterations
  // TODO Get tolerance

  if (max_iterations == 0) {
    max_iterations = UINT_MAX;
  }

  if (tolerance <= 0) {
    tolerance = DBL_EPSILON;
  }

  // 6. The Method
  printf("Floating-point precision is 16 and epsilon is %.16lf (%e).\n\n", DBL_EPSILON, DBL_EPSILON);

  double root = 0;

  find_root(interval, max_iterations, tolerance, &root);

  // 7. Result
  printf("The root of 'f(x) = %s' is '%.16lf'\n", function_string, root);

  return 0;

  // 8. Showdown
  printf("\n1) Restart the method with same inputs,\n\
2) Restart the program to select another method and/or change input(s)\n\
3) Quit\n\n\
Choice: ");
  // TODO Continue from here

  // After calculation free the function
  //te_free(func);
  //free(interval);
  // TODO Add other pointers to be freed

  return 0;
}

void tinyexpr_test(void) {
  // Obtain expression as string
  const char *expression = "x^2+3*x-7";

  // Define variables for future use
  double variable_x;

  te_variable variables[] = {
    { "x", &variable_x }
  };

  // Define a variable to hold compilation error
  int error = 0;

  // Compile the expression string to function with it's variables
  te_expr *func = te_compile(expression, variables, 1, &error);

  if (func == NULL) {
    // Parse error occured

    printf("Parse error occured as %d.\n", error);

    return;
  }

  // Finally obtain inputs for variables and evaluate the function
  double input_for_x = 3.14;

  // Set variable values BEFORE evaluate the function
  variable_x = input_for_x;

  double result = te_eval(func);

  printf("f(%.2f) = %.2f", variable_x, result);

  // After function no longer needed to use free the memory
  te_free(func);
}

int find_root(struct Interval *interval, unsigned int max_iterations, double tolerance, double *root) {
  // Interval is undefined
  if (interval == NULL) {
    return 1;
  }

  // Out value is undefined
  if (root == NULL) {
    return 2;
  }

  unsigned int iteration_idx = 0;
  struct Interval *candidate_interval = (struct Interval *)malloc(sizeof(struct Interval));
  // TODO Copy interval to candidate_interval

  struct FindRootInfo *info = (struct FindRootInfo *)malloc(sizeof(struct FindRootInfo));
  info->max_iterations = max_iterations;
  info->tolerance = tolerance;
  info->error = 0;
  info->last_root = 0;
  info->midpoint = 0;
  info->interval_start_changed = 0;
  info->interval_end_changed = 0;

#ifdef STEP_BY_STEP
  int skip_through_end_flag = 0,
      verbose_skip_through_end_flag = 0;
  char *step_choice = (char *)malloc(sizeof(char) * 8);
  unsigned int step_idx = 0,
               skip_to_step = 0;

  printf("After each step you will be asked to continue, skip, or terminate the program.\n\
* Hit Enter without typing anything to continue to next step,\n\
* Type 's' and hit Enter to skip the loop through to get the result,\n\
* Type 'v' and hit Enter to verbose skip the loop through to get the result,\n\
* Type 'q' and hit Enter to terminate the program,\n\
* Type a number and hit Enter to skip this many steps.\n\n");
#endif // STEP_BY_STEP

  while (max_iterations > iteration_idx) {
    (*method_find_root)(candidate_interval, info, root);

    // FIX Change the set and check order for internal_xxx_cnahged
    if ((info->interval_start_changed == 0 && info->interval_end_changed == 0) ||
        info->error < info->tolerance
    ) {
      // Root found
      break;
    }

    // TODO Detect interval change somehow
    candidate_interval->start = interval->start;
    candidate_interval->end = info->midpoint;

    if ((*method_check_initial_conditions)(candidate_interval) == 0) {
      candidate_interval->end = info->midpoint;

      info->interval_start_changed = 0;
      info->interval_end_changed = 1;
    } else {
      candidate_interval->start = info->midpoint;

      info->interval_start_changed = 1;
      info->interval_end_changed = 0;
    }

#ifdef STEP_BY_STEP
    if (skip_through_end_flag == 0) {
      printf("Iteration Index   : %d\nError             : %.16lf\nIs [a, c] Selected: %s\nCalculated Root   : %.16lf\n\n", iteration_idx + 1, info->error, info->interval_end_changed == 0 ? "No" : "Yes", info->midpoint);

      if (skip_to_step == iteration_idx) {
        skip_to_step = 0;
      }

      if (skip_to_step == 0 && verbose_skip_through_end_flag == 0) {
        fgets(step_choice, 8, stdin);

        if (step_choice[0] == 's') {
          // Skip the loop through the end

          skip_through_end_flag = 1;
        } else if (step_choice[0] == 'v') {
          verbose_skip_through_end_flag = 1;
        } else if (step_choice[0] == 'q') {
          exit(2);
        } else if ('0' <= step_choice[0] && step_choice[0] <= '9') {
          sscanf(step_choice, "%d", &step_idx);

          skip_to_step = iteration_idx + step_idx + 1;
        }
      }
    }
#endif // STEP_BY_STEP

    // The iteration counter MUST be incremented at the end of the loop,
    // so we can keep track of in which step we are in, in the loop codes.
    iteration_idx += 1;
  }

  // Free the allocations...
  free(candidate_interval);

  // ... and return with success
  return 0;
}
