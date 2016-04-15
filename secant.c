#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>

#include "rootfinder.h"
// TODO Change the below include's file name
#include "secant.h"

struct SC_Additional {
  double pkm1; // p_k-1
  double pkm2; // p_k-2

  double f_pkm1;
  double f_pkm2;
};

struct SC_Additional *sc_additional = NULL;

void sc_initialize(struct CalculationInfo *info) {
  // iteration_max, tolerance are NOT assigned yet - do NOT use them, or assign them

  sc_additional = (struct SC_Additional *)calloc(1, sizeof(struct SC_Additional));
  info->additional = sc_additional;

  int error = -1;
  while (error != 0) {
    printf("Enter p0: ");
    sc_additional->pkm2 = get_input_double(&error);
  }

  error = -1;
  while (error != 0) {
    printf("Enter p1: ");
    sc_additional->pkm1 = get_input_double(&error);
  }
}

void sc_calculate_root(struct CalculationInfo *info) {
  // Here we can call f() function to evaluate the x for given expression

  sc_additional = (struct SC_Additional *) info->additional;

  while (info->iteration_index < info->iteration_max && info->root_status != root_status_root) {
    // root = p_k-1 - ((f(p_k-1) * (p_k-1 - p_k-2)) / (f(p_k-1) - f(p_k-2)))
    // root = pkm1 - ((f_pkm1 * (pkm1 - pkm2)) / (f_pkm1 - f_pkm2))
    sc_additional->f_pkm2 = f(sc_additional->pkm2);
    sc_additional->f_pkm1 = f(sc_additional->pkm1);
    info->root = sc_additional->pkm1 - ((sc_additional->f_pkm1 * (sc_additional->pkm1 - sc_additional->pkm2)) / (sc_additional->f_pkm1 - sc_additional->f_pkm2));

    // CONTINUE FROM HERE
    // TODO Do calculations and set info->absolute_error here
    info->absolute_error = sc_additional->pkm2 - sc_additional->pkm1;

    sc_additional->pkm2 = sc_additional->pkm1;
    sc_additional->f_pkm2 = sc_additional->f_pkm1;
    sc_additional->pkm1 = info->root;
    sc_additional->f_pkm1 = f(sc_additional->pkm1);
    info->f_root = sc_additional->f_pkm1;

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

void sc_additional_dump(struct CalculationInfo *info, char **dump) {
  sc_additional = (struct SC_Additional *) info->additional;

  // Do NOT end with new line character
  sprintf(*dump, "p_k-2: %.16lf\n\
p_k-1: %.16lf\n", sc_additional->pkm2, sc_additional->pkm1);
}

void sc_finalize(struct CalculationInfo *info) {
  free(sc_additional);
  info->additional = NULL;
}
