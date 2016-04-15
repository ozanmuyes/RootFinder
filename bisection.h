#ifndef BISECTION_H_INCLUDED
#define BISECTION_H_INCLUDED

void bs_initialize(struct CalculationInfo *info);
void bs_calculate_root(struct CalculationInfo *info);
void bs_additional_dump(struct CalculationInfo *info, char **dump);
void bs_finalize(struct CalculationInfo *info);

#endif // BISECTION_H_INCLUDED
