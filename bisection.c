#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "rootfinder.h"
#include "bisection.h"

struct BS_Interval {
  double start; // a.k.a point 'a'
  double end; // a.k.a point 'b'
};

struct BS_Interval *bs_interval = NULL;

// TODO Maybe moving these to info will be more suitable
double bs_last_a = 0,
       bs_last_b = 0;

int bs_check_initial_conditions(struct CalculationInfo *info);

void bs_initialize(struct CalculationInfo *info) {
  // iteration_max, tolerance are NOT assigned yet - do NOT use them, or assign them

  bs_interval = (struct BS_Interval *)calloc(1, sizeof(struct BS_Interval));
  info->additional = bs_interval;

  // Get necessary inputs specific to the method
  int input_error = -1;
  while (input_error != 0) {
    printf("Enter interval start value: ");
    bs_interval->start = get_input_double(&input_error);
  }

  input_error = -1;
  while (input_error != 0) {
    printf("Enter interval end value: ");
    bs_interval->end = get_input_double(&input_error);

    // interval start and interval end can NOT be equal
    if (fabs(bs_interval->start - bs_interval->end) <= DBL_EPSILON) {
      input_error = -1;
    }
  }

  // Set root to DBL_EPSILON when there is no root
  int check_initial_conditions_result = bs_check_initial_conditions(info);
  if (check_initial_conditions_result > 0) {
    // TODO Print the index of failed initial condition (check_initial_conditions_result is the failed condition index)

    info->root = DBL_EPSILON;
    info->root_status = root_status_no_root;
  } else {
    // TODO Get method specific inputs and set in info here
  }
}

void bs_calculate_root(struct CalculationInfo *info) {
  // Here we can call f() function to evaluate the x for given expression

  double temp; // this will help when swapping points
  bs_interval = (struct BS_Interval *) info->additional;

  while (info->iteration_index < info->iteration_max && info->root_status != root_status_root) {
    info->root = (bs_interval->start + bs_interval->end) / 2;
    info->absolute_error = fabs(bs_interval->end - info->root);
    info->f_root = f(info->root);

    bs_last_a = bs_interval->start;
    bs_last_b = bs_interval->end;

    // There is a very little chance to find the root by f(c)
    if (fabs(info->f_root) <= DBL_EPSILON || fabs(info->absolute_error) <= info->tolerance) {
      // Root is found
      info->root_status = root_status_root;
    } else {
      // Try [a, c] and check if initial conditions can be obtained
      temp = bs_interval->end; // temp is 'b'
      bs_interval->end = info->root;

      if (bs_check_initial_conditions(info) != 0) {
        // [c, b] is the new interval

        // So substitute 'a' with 'c' and restore 'b' - so new interval will be [c, b]
        bs_interval->start = bs_interval->end;
        bs_interval->end = temp;
      }

      // [a, c] is the new interval
    }

    // Call step() function on each step if info->flag_step_by_step was set
    if (info->flag_step_by_step == 1) {
      step(info);
    }

    info->iteration_index += 1;
  }

  if (info->root_status == root_status_na) {
    info->root_status = root_status_approx_root;
  }
}

void bs_additional_dump(struct CalculationInfo *info, char **dump) {
  bs_interval = (struct BS_Interval *) info->additional;

  // Do NOT end with new line character
  sprintf(*dump, "a: %.16lf\n\
b: %.16lf\n", bs_last_a, bs_last_b);
}

void bs_finalize(struct CalculationInfo *info) {
  free(info->additional);
  info->additional = NULL;
}

int bs_check_initial_conditions(struct CalculationInfo *info) {
  bs_interval = (struct BS_Interval *) info->additional;
  // 1st Check for the function continuity within given interval
  // HACK We assume that the function -f(x)- is continuous within given interval

  // 2nd Check if f(a)*f(b)<0
  if (f(bs_interval->start) * f(bs_interval->end) >= 0) {
    return 2;
  }

  return 0;
}
