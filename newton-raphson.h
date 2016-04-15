#ifndef NEWTON_RAPHSON_H_INCLUDED
#define NEWTON_RAPHSON_H_INCLUDED

void nr_initialize(struct CalculationInfo *info);
void nr_calculate_root(struct CalculationInfo *info);
void nr_additional_dump(struct CalculationInfo *info, char **dump);
void nr_finalize(struct CalculationInfo *info);

#endif // NEWTON_RAPHSON_H_INCLUDED
