#ifndef SECANT_H_INCLUDED
#define SECANT_H_INCLUDED

void sc_initialize(struct CalculationInfo *info);
void sc_calculate_root(struct CalculationInfo *info);
void sc_additional_dump(struct CalculationInfo *info, char **dump);
void sc_finalize(struct CalculationInfo *info);

#endif // SECANT_H_INCLUDED
