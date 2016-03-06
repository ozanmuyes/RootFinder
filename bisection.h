#ifndef BISECTION_H_INCLUDED
#define BISECTION_H_INCLUDED

#include "common.h"

int bs_check_initial_conditions(struct Interval *interval);
void bs_find_root(struct Interval *interval, unsigned int max_iterations, double tolerance, double *root);

#endif // BISECTION_H_INCLUDED
