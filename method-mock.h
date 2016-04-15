// TODO Change the below definition's name
#ifndef METHOD_MOCK_H_INCLUDED
#define METHOD_MOCK_H_INCLUDED
// TODO Add this file to rootfinder.c as an include
// TODO Add this method to method list in the rootfinder.c

// TODO Change mm prefixes as per method name
void mm_initialize(struct CalculationInfo *info);
void mm_calculate_root(struct CalculationInfo *info);
void mm_additional_dump(struct CalculationInfo *info, char **dump);
void mm_finalize(struct CalculationInfo *info);

#endif // METHOD_MOCK_H_INCLUDED
