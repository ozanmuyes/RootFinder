#ifndef BISECTION_H_INCLUDED
#define BISECTION_H_INCLUDED

int bs_check_initial_conditions(struct Interval *interval);
void bs_find_root(struct Interval *interval, struct FindRootInfo *info, double *root);

#endif // BISECTION_H_INCLUDED
