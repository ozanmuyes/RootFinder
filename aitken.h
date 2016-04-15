#ifndef AITKEN_H_INCLUDED
#define AITKEN_H_INCLUDED
// TODO Add this method to method list in the rootfinder.c

void ai_initialize(struct CalculationInfo *info);
void ai_calculate_root(struct CalculationInfo *info);
void ai_additional_dump(struct CalculationInfo *info, char **dump);
void ai_finalize(struct CalculationInfo *info);

#endif // AITKEN_H_INCLUDED
