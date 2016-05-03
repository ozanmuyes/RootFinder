#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "rootfinder.h"
#include "regula-falsi.h"

struct RF_Interval {
  double start; // a.k.a point 'a'
  double end; // a.k.a point 'b'
};

// HACK Do I need to reload rf_interval in every subsequent function call of rf_initialize on this file?
// TODO Use (*((struct RF_Interval *)info->additional))->start instead of rf_interval->start
//      And then is rf_finalize() function necessary?
struct RF_Interval *rf_interval = NULL;

// TODO Maybe moving these to info will be more suitable
double rf_last_root = 0,
       rf_last_a = 0,
       rf_last_b = 0;

int rf_check_initial_conditions(struct CalculationInfo *info);

void rf_initialize(struct CalculationInfo *info) {
  // iteration_max, tolerance are NOT assigned yet - do NOT use them, or assign them

  // Allocate and set info->additional
  rf_interval = (struct RF_Interval *)calloc(1, sizeof(struct RF_Interval));
  info->additional = rf_interval;

  // Get necessary inputs specific to the method
  int input_error = -1;
  while (input_error != 0) {
    printf("Enter interval start value: ");
    rf_interval->start = get_input_double(&input_error);
  }

  input_error = -1;
  while (input_error != 0) {
    printf("Enter interval end value: ");
    rf_interval->end = get_input_double(&input_error);

    // interval start and interval end can NOT be equal
    if (fabs(rf_interval->start - rf_interval->end) <= DBL_EPSILON) {
      input_error = -1;
    }
  }

  // Set root to DBL_EPSILON when there is no root
  int check_initial_conditions_result = rf_check_initial_conditions(info);
  switch (check_initial_conditions_result) {
    case 0: {
      // TODO Get method specific inputs and set in info here

      break;
    }

    case 1: {
      // Function is not continuous
      info->root = DBL_EPSILON;
      info->root_status = root_status_no_root;

      break;
    }

    case 2: {
      info->root = DBL_EPSILON;
      info->root_status = root_status_multiple_roots;

      break;
    }
  }
}

void rf_calculate_root(struct CalculationInfo *info) {
  // Here we can call f() function to evaluate the x for given expression
  // We MUST check and call step() in each step

  double temp, // this will help when swapping points
         f_a,
         f_b;
  rf_interval = (struct RF_Interval *) info->additional;

  while (info->iteration_index < info->iteration_max && info->root_status != root_status_root) {
    f_a = f(rf_interval->start);
    if (isnan(f_a)) {
      // log some warning and break the loop
      _log(__FILE__, __LINE__, log_level_warning, "Could not calculate the function value of interval start value (NaN).");

      info->root_status = root_status_error;

      break;
    }

    f_b = f(rf_interval->end);
    if (isnan(f_b)) {
      // log some warning and break the loop
      _log(__FILE__, __LINE__, log_level_warning, "Could not calculate the function value of interval end value (NaN).");

      info->root_status = root_status_error;

      break;
    }

    // Store last calculated root to find absolute error
    rf_last_root = info->root;
    // root = b - (((b - a) * f(b)) / f(b) - f(a)
    info->root = rf_interval->end - (((rf_interval->end - rf_interval->start) * f_b) / (f_b - f_a));
    info->absolute_error = fabs(rf_last_root - info->root);
    info->f_root = f(info->root);

    rf_last_a = rf_interval->start;
    rf_last_b = rf_interval->end;

    // There is a very little chance to find the root by f(c)
    if (fabs(info->f_root) <= DBL_EPSILON || fabs(info->absolute_error) <= info->tolerance) {
      // Root is found
      info->root_status = root_status_root;
    } else {
      // Try [a, c] and check if initial conditions can be obtained
      temp = rf_interval->end; // temp is 'b'
      rf_interval->end = info->root;

      if (rf_check_initial_conditions(info) != 0) {
        // [c, b] is the new rf_interval

        // So substitute 'a' with 'c' and restore 'b' - so new rf_interval will be [c, b]
        rf_interval->start = rf_interval->end;
        rf_interval->end = temp;
      }

      // [a, c] is the new rf_interval
    }

    // Call step() function on each step if info->flag_step_by_step was set
    if (info->flag_step_by_step == 1) {
      step(info);
    }

    info->iteration_index += 1;
  }
}

void rf_additional_dump(struct CalculationInfo *info, char **dump) {
  rf_interval = (struct RF_Interval *) info->additional;

  // Do NOT end with new line character
  sprintf(*dump, "a: %.16lf\n\
b: %.16lf", rf_last_a, rf_last_b);
}

void rf_finalize(struct CalculationInfo *info) {
  free(rf_interval);
  info->additional = NULL;
}

int rf_check_initial_conditions(struct CalculationInfo *info) {
  rf_interval = (struct RF_Interval *) info->additional;
  // 1st Check for the function continuity within given interval
  // HACK We assume that the function -f(x)- is continuous within given interval

  // 2nd Check if f(a)*f(b)<0
  if (f(rf_interval->start) * f(rf_interval->end) >= 0) {
    return 2;
  }

  return 0;
}
