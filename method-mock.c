#include <stdlib.h>
//#include <stdio.h>
#include <float.h>
//#include <math.h>

#include "rootfinder.h"
// TODO Change the below include's file name
#include "method-mock.h"

// TODO Define necessary structure for info->additional here

// TODO Uncomment below line if the method has initial condition and change prefix
//int mm_check_initial_conditions(struct CalculationInfo *info);

void mm_initialize(struct CalculationInfo *info) {
  // iteration_max, tolerance are NOT assigned yet - do NOT use them, or assign them

  // TODO Initialize info->additional here if necessary

  // TODO Get further information from user for the method

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

void mm_calculate_root(struct CalculationInfo *info) {
  // Here we can call f() function to evaluate the x for given expression

  while (info->iteration_index < info->iteration_max && info->root_status != root_status_root) {
    // TODO Do calculations and set info->root here
    // TODO Do calculations and set info->absolute_error here
    // TODO Change step->interval->start or step->interval->end here
    // or...

    if (fabs(info->f_root) <= DEFAULT_TOLERANCE || info->absolute_error <= info->tolerance) {
      // Root is found
      info->root_status = root_status_root;
    } else {
      // TODO Do calculations and set info->root here
      // TODO Do calculations and set info->absolute_error here
      // TODO Change step->interval->start or step->interval->end here
    }

    // Call step() function on each step if info->flag_step_by_step was set
    if (info->flag_step_by_step == 1) {
      step(info);
    }

    info->iteration_index += 1;
  }
}

void mm_additional_dump(struct CalculationInfo *info, char **dump) {
  // TODO Spill out information mostly about additional
  // Do NOT end with new line character
  //sprintf(*dump, "double: %.16lf", info->foo);
}

void mm_finalize(struct CalculationInfo *info) {
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
