#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "rootfinder.h"
#include "fixed-point.h"

void fp_initialize(struct CalculationInfo *info) {
  // iteration_max, tolerance are NOT assigned yet - do NOT use them, or assign them

  int error = -1;
  while (error != 0) {
    printf("Enter p0: ");
    info->root = get_input_double(&error);
  }
}

void fp_calculate_root(struct CalculationInfo *info) {
  // Here we can call f() function to evaluate the x for given expression

  while (info->iteration_index < info->iteration_max && info->root_status != root_status_root) {
    // TODO Do calculations and set info->root here
    // TODO Do calculations and set info->absolute_error here
    // TODO Change step->interval->start or step->interval->end here
    // or...
    info->f_root = f(info->root);

    info->absolute_error = fabs(info->f_root - info->root);

    info->root = info->f_root;

    if (info->absolute_error <= info->tolerance) {
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

void fp_additional_dump(struct CalculationInfo *info, char **dump) {
  // TODO Spill out information mostly about additional
  // Do NOT end with new line character
  sprintf(*dump, "x: %.16lf", info->root);
}

void fp_finalize(struct CalculationInfo *info) {
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
