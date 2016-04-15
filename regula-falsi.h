#ifndef REGULA_FALSI_H_INCLUDED
#define REGULA_FALSI_H_INCLUDED

void rf_initialize(struct CalculationInfo *info);
void rf_calculate_root(struct CalculationInfo *info);
void rf_additional_dump(struct CalculationInfo *info, char **dump);
void rf_finalize(struct CalculationInfo *info);

#endif // REGULA_FALSI_H_INCLUDED
