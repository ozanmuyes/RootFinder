#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "rootfinder.h"
#include "newton-raphson.h"

// TODO Define necessary structure for info->additional here
char *nr_prime_expression = NULL;
te_expr *nr_function_prime = NULL;
double nr_x_for_f_prime = 0,
       nr_last_root = 0;

enum compile_expression_errors nr_compile_expression_result;

// TODO Uncomment below line if the method has initial condition and change prefix
//int mm_check_initial_conditions(struct CalculationInfo *info);
double nr_f_prime(double x);

void nr_initialize(struct CalculationInfo *info) {
  // iteration_max, tolerance are NOT assigned yet - do NOT use them, or assign them

  // TODO Initialize info->additional here if necessary
  nr_prime_expression = (char *)malloc(sizeof(char) * 128);

  // TODO Get further information from user for the method

  // Get function derivative expression and compile it
  nr_compile_expression_result = compile_expression_error_na;

  while (nr_compile_expression_result != compile_expression_error_success) {
    printf("f'(x) = ");
    get_input_wo_space(nr_prime_expression, 128);
    printf("\n");

    if ((nr_compile_expression_result = compile_expression_to(nr_prime_expression, &nr_function_prime, &nr_x_for_f_prime)) != compile_expression_error_success) {
      printf("Syntax error on expression column %d.\n\n", -nr_compile_expression_result);

      nr_compile_expression_result = compile_expression_error_error;
    }
  }

  // Get necessary inputs specific to the method
  int input_error = -1;
  while (input_error != 0) {
    printf("Enter x0: ");
    info->root = get_input_double(&input_error);
  }

  // TODO Remove after test
  info->absolute_error = 3.14;

  // TODO Check initial conditions
  /*
  int check_initial_conditions_result = mm_check_initial_conditions(info);
  if (check_initial_conditions_result > 0) {
    // check_initial_conditions_result is the failed condition index

    info->root = DBL_EPSILON;
    info->root_status = root_status_no_root;
  } else {
    // TODO Get method specific inputs and set in info here
  }
  */
}

void nr_calculate_root(struct CalculationInfo *info) {
  // Here we can call f() function to evaluate the x for given expression

  while (info->iteration_index < info->iteration_max && info->root_status != root_status_root) {
    // TODO Do calculations and set info->root here
    // TODO Do calculations and set info->absolute_error here
    // TODO Change step->interval->start or step->interval->end here
    // or...

    nr_last_root = info->root;
    info->root = info->root - (f(info->root) / nr_f_prime(info->root));
    info->f_root = f(info->root);
    info->absolute_error = info->root - nr_last_root;

    if (fabs(info->f_root) <= DBL_EPSILON || fabs(info->absolute_error) <= info->tolerance) {
      // Root is found
      info->root_status = root_status_root;
    }

    // Call step() function on each step if info->flag_step_by_step was set
    if (info->flag_step_by_step == 1) {
      step(info);
    }

    info->iteration_index += 1;
  }
}

void nr_additional_dump(struct CalculationInfo *info, char **dump) {
  // Do NOT end with new line character
  sprintf(*dump, "double: %.16lf", info->absolute_error);
}

void nr_finalize(struct CalculationInfo *info) {
  // TODO Do method-wide clean-up (e.g. free pointers, free additional and set info->additional to NULL etc.)
}

/*
int mm_check_initial_conditions(struct CalculationInfo *info) {
  // 1st Check for the first condition - if could NOT pass return 1
  // 2nd Check for the second condition - if could NOT pass return 2
  // ...

  // ...finally return 0 if all conditions obtained
  return 0;
}
*/

double nr_f_prime(double x) {
  nr_x_for_f_prime = x;

  return te_eval(nr_function_prime);
}
