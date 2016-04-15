#ifndef FIXED_POINT_H_INCLUDED
#define FIXED_POINT_H_INCLUDED

void fp_initialize(struct CalculationInfo *info);
void fp_calculate_root(struct CalculationInfo *info);
void fp_additional_dump(struct CalculationInfo *info, char **dump);
void fp_finalize(struct CalculationInfo *info);

#endif // FIXED_POINT_H_INCLUDED
