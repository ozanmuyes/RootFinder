#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "rootfinder.h"
#include "aitken.h"

struct AI_Additional {
  double x0;

  double f_x;
};

struct AI_Additional *ai_additional = NULL;

void ai_initialize(struct CalculationInfo *info) {
  // iteration_max, tolerance are NOT assigned yet - do NOT use them, or assign them

  ai_additional = (struct AI_Additional *)calloc(1, sizeof(struct AI_Additional));
  info->additional = ai_additional;

  int error = -1;
  while (error != 0) {
    printf("Enter p0: ");
    ai_additional->x0 = get_input_double(&error);
  }
}

void ai_calculate_root(struct CalculationInfo *info) {
  // Here we can call f() function to evaluate the x for given expression

  while (info->iteration_index < info->iteration_max && info->root_status != root_status_root) {
    // TODO Do calculations and set info->root here
    // TODO Do calculations and set info->absolute_error here
    // TODO Change step->interval->start or step->interval->end here
    // or...
    ai_additional->f_x = f(ai_additional->x0);

    info->root = ai_additional->f_x + 1 / ((1 / (f(ai_additional->f_x) - ai_additional->f_x)) - (1 / (ai_additional->f_x - ai_additional->x0)));
    info->f_root = f(info->root);

    info->absolute_error = fabs(info->root - info->f_root);

    ai_additional->x0 = ai_additional->f_x;

    if (fabs(info->root - info->f_root) <= DEFAULT_TOLERANCE || info->absolute_error <= info->tolerance) {
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

void ai_additional_dump(struct CalculationInfo *info, char **dump) {
  ai_additional = (struct AI_Additional *) info->additional;

  // Do NOT end with new line character
  sprintf(*dump, "x: %.16lf\n\
f(root): %.16lf\n", ai_additional->x0, info->f_root);
}

void ai_finalize(struct CalculationInfo *info) {
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
