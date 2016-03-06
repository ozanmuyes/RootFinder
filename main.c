#include "common.h"
#include "bisection.h"

void tinyexpr_test(void);

int main() {
  // 1. Method selection
  int method_idx = 1;

/*mtd_sel:
  printf("Please select a method;\n\
1) Bisection\n\
2) Regula Falsi\n\n\
Choice: ");

  method_idx = get_input_int();

  switch(method_idx) {
    case 1: {
      printf("bis");

      break;
    }

    case 2: {
      printf("RF");

      break;
    }

    default: {
      printf("\nInvalid entry. ");

      goto mtd_sel;
    }
  }

  printf("\n");*/

  // 2. Function string entry
  char *function_string = (char *)malloc(sizeof(char) * 128);

fun_ent:
  printf("f(x) = ");
  get_input_wo_space(function_string, 128);

  if (strchr(function_string, 'x') == NULL) {
    printf("I also like to live dangerously!\n\n");

    goto fun_ent;
  }

  int compilation_result = compile_function(function_string);

  if (compilation_result < 0) {
    printf("Syntax error at column %d. Please fix your entry.\n\n", -compilation_result);

    goto fun_ent;
  }

  // 3. Interval entry
  struct Interval *interval = (struct Interval *)malloc(sizeof(struct Interval));

int_ent:
  printf("Enter interval start value: ");
  interval->start = get_input_double();

  printf("Enter interval end value: ");
  interval->end = get_input_double();

  // Check whether interval values is proper
  if (interval->start > interval->end) {
    // Since interval.start is greater than interval.end, swap each other
    double fTemp = interval->start;
    interval->start = interval->end;
    interval->end = fTemp;
  }

  // 4. Check initial conditions
  int check_result = 0;

  switch(method_idx) {
    case 1: {
      check_result = bs_check_initial_conditions(interval);

      break;
    }
  }

  if (check_result > 0) {
    // Terminate with error (1; initial conditions did NOT pass)

    printf("Initial conditions can not obtained by given inputs. Please try again.\n\n");

    goto int_ent;
  }

  // 5. Enter iteration count or error tolerance
  // TODO Get these inputs from user
  unsigned int max_iterations = 0;
  double tolerance = 0;

  // 6. The Method
  printf("Floating-point precision is 16 and epsilon is %.16lf (%e).\n\n", DBL_EPSILON, DBL_EPSILON);

  double root = 0;

  switch(method_idx) {
    case 1: {
      bs_find_root(interval, max_iterations, tolerance, &root);

      break;
    }
  }

  // 7. Result
  printf("The root of 'f(x) = %s' is '%.16lf'\n", function_string, root);

  return 0;

  // 8. Showdown
  printf("\n1) Restart the method with same inputs,\n\
2) Restart the program to select another method and/or change input(s)\n\
3) Quit\n\n\
Choice: ");
  // TODO Continue from here

  // After calculation free the function
  //te_free(func);

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
