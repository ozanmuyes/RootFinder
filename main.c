#include <stdio.h>
#include <stdlib.h>

#include "tinyexpr.h"

void tinyexpr_test(void);

int main() {
  tinyexpr_test();
  return 0;
}

void tinyexpr_test(void) {
  // Obtain expression as string
  const char *expression = "x^2+3*x-7";

  // Define variables for future use
  double variable_x;

  te_variable variables[] = {
    { "x", &variable_x }
  };

  // Define a variable to hold compilation error
  int error = 0;

  // Compile the expression string to function with it's variables
  te_expr *func = te_compile(expression, variables, 1, &error);

  if (func == NULL) {
    // Parse error occured

    printf("Parse error occured as %d.\n", error);

    return;
  }

  // Finally obtain inputs for variables and evaluate the function
  double input_for_x = 3.14;

  // Set variable values BEFORE evaluate the function
  variable_x = input_for_x;

  double result = te_eval(func);

  printf("f(%.2f) = %.2f", variable_x, result);

  // After function no longer needed to use free the memory
  te_free(func);
}
