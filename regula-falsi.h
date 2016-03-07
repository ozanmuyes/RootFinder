#ifndef REGULA_FALSI_H_INCLUDED
#define REGULA_FALSI_H_INCLUDED

#include "common.h"

int rf_check_initial_conditions(struct Interval *interval);
int rf_find_root(struct Interval *interval, unsigned int max_iterations, double tolerance, double *root);

#endif // REGULA_FALSI_H_INCLUDED
