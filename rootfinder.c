#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <float.h>
#include <math.h>
#include <stdarg.h>

#include "rootfinder.h"
//#include "tinyexpr.h"

#include "bisection.h"
#include "regula-falsi.h"
#include "newton-raphson.h"
#include "secant.h"
#include "aitken.h"
#include "fixed-point.h"

#define DEFAULT_STEP_COUNT 52000
#define DEFAULT_TOLERANCE DBL_EPSILON * 10

/**
 * REMARKS
 *
 * - "-o" parametresi en sonda olmali
 * - "-o" parametresi verilmisse "-m" ve "-f" parametreleri de verilmeli
 *
 */

char *arg_input_path = NULL; // There MAY only be one input file at most
FILE *output_file = NULL;
enum log_levels log_level = log_level_no;

// Since everything IS actually a number in computer, we can use char type to hold a number
unsigned char method_index = 0; // 0 means not set

// This variable will be used when evaluating the function in f()
double variable_x = 0;
te_expr *te_function = NULL;

char *step_choice = NULL,
     *additional_dump = NULL;
int flag_step_by_step = 1,
    step_idx = 0;

int _print(const char *format, ...);

void parse_args(int argc, char **argv);
void print_name_and_version();
void print_welcome_message();
void print_help();

enum get_expression_errors get_expression(char **expression);
enum compile_expression_errors compile_expression(char *expression);

void free_calculation_info(struct CalculationInfo *info);

void (*method_initialize)(struct CalculationInfo *info);
void (*method_calculate_root)(struct CalculationInfo *info);
void (*method_additional_dump)(struct CalculationInfo *info, char **dump);
void (*method_finalize)(struct CalculationInfo *info);

char *expression;

int main(int argc, char **argv) {
  /**
   * Set default error file before everything - either 'verbose' will set or not.
   * This is because parse_args() function MAY log some internal warnings.
   */
  freopen("rootfinder.log", "a", stderr);

  expression = (char *)calloc(128, sizeof(char));

  // Parse the arguments and expect flagXXX and argXXX variables to be changed
  parse_args(argc, argv);

  _log(__FILE__, __LINE__, log_level_debug, "Arguments parsed.");

  print_welcome_message();
  if (output_file != NULL) {
    fprintf(output_file,
"Program started.\n\
=================\n"
    );
  }

  step_choice = (char *)malloc(sizeof(char) * 8);
  additional_dump = (char *)malloc(sizeof(char) * 128);

  struct CalculationInfo *info = (struct CalculationInfo *)calloc(1, sizeof(struct CalculationInfo));
  int input_int = 0,
      input_error = -1,
      flag_exit_program = 0;
#ifndef USE_HARDCODED
  enum get_expression_errors get_expression_result = get_expression_error_na;
  enum compile_expression_errors compile_expression_result = compile_expression_error_na;
#endif // USE_HARDCODED

  while (flag_exit_program == 0) {
    if (0) {
    //if (arg_input_path != NULL) {
      // TODO Get inputs from file
    } else {
      // TODO Write print_methods_list() and get_method_index() functions

#ifndef USE_HARDCODED
      // TR Eger flag_output_stream_set 1 ise fonksiyonun da, method_index'in de, interval degerlerinin de parametre olarak verilmis olmasi beklenir.
      //    Bu beklentiler karsilanmamissa programdan çikilir.

      input_error = -1;
      while (method_initialize == NULL/* || method_calculate_root == NULL */ || input_error != 0) {
        printf("Please choose a method\n\
======================\n\
1) Bisection\n\
2) Regula Falsi\n\
3) Newton-Raphson\n\
4) Secant\n\
5) Aitken\n\
6) Fixed Point\n\
9) Quit\n\
\n\
Choice: ");

        method_index = get_input_int(&input_error);
        printf("\n");

        switch (method_index) {
          case 1: {
            method_initialize = bs_initialize;
            method_calculate_root = bs_calculate_root;
            method_additional_dump = bs_additional_dump;
            method_finalize = bs_finalize;

            _print("Method name: Bisection\n\n");

            break;
          }

          case 2: {
            method_initialize = rf_initialize;
            method_calculate_root = rf_calculate_root;
            method_additional_dump = rf_additional_dump;
            method_finalize = rf_finalize;

            _print("Method name: Regula Falsi\n\n");

            break;
          }

          case 3: {
            method_initialize = nr_initialize;
            method_calculate_root = nr_calculate_root;
            method_additional_dump = nr_additional_dump;
            method_finalize = nr_finalize;

            _print("Method name: Newton-Raphson\n\n");

            break;
          }

          case 4: {
            method_initialize = sc_initialize;
            method_calculate_root = sc_calculate_root;
            method_additional_dump = sc_additional_dump;
            method_finalize = sc_finalize;

            _print("Method name: Secant\n\n");

            break;
          }

          case 5: {
            method_initialize = ai_initialize;
            method_calculate_root = ai_calculate_root;
            method_additional_dump = ai_additional_dump;
            method_finalize = ai_finalize;

            _print("Method name: Aitken\n\n");

            break;
          }

          case 6: {
            method_initialize = fp_initialize;
            method_calculate_root = fp_calculate_root;
            method_additional_dump = fp_additional_dump;
            method_finalize = fp_finalize;

            _print("Method name: Fixed Point\n\n");

            break;
          }

          case 9: {
            // TODO Improve here

            free(expression);
            free(arg_input_path);
            te_free(te_function);
            free(step_choice);
            free_calculation_info(info);

            return 0;
          }

          // TODO Assign other methods' proper functions here (i.e. regula falsi, newton-raphson etc.)

          default: {
            printf("You made a wrong choice.\n");
          }
        }
      }
#else
      method_index = 2;
      method_initialize = rf_initialize;
      method_calculate_root = rf_calculate_root;
      method_finalize = rf_finalize;
#endif // USE_HARDCODED

      // Create passable information object and initialize it's values
      //-info = (struct CalculationInfo *)calloc(1, sizeof(struct CalculationInfo));
      info->iteration_max = DEFAULT_STEP_COUNT;
      info->flag_step_by_step = flag_step_by_step;
      info->tolerance = DEFAULT_TOLERANCE;
      info->root_status = root_status_na;
      // Since we did a calloc() call we don't need to initialize other info variables with 0 or NULL

      // Get expression as string
      //-char *expression = (char *)malloc(sizeof(char) * 128);
#ifndef USE_HARDCODED
      //-enum get_expression_errors get_expression_result = get_expression_error_na;
      get_expression_result = get_expression_error_na;
      //-enum compile_expression_errors compile_expression_result = compile_expression_error_na;
      compile_expression_result = compile_expression_error_na;

      while (get_expression_result != get_expression_error_success || compile_expression_result != compile_expression_error_success) {
        get_expression_result = get_expression(&expression);

        switch (get_expression_result) {
          case get_expression_error_success: {
            // Compile expression to function
            if ((compile_expression_result = compile_expression(expression)) != compile_expression_error_success) {
              printf("Syntax error on expression column %d.\n\n", -compile_expression_result);

              compile_expression_result = compile_expression_error_error;
              expression[0] = '\0';
            }

            break;
          }

          case get_expression_error_no_variable: {
            printf("I also like to live dangerously!\n\n");
            expression[0] = '\0';

            break;
          }

          default: {
            printf("Invalid entry for expression. Please fix the expression.\n\n");
            expression[0] = '\0';

            break;
          }
        }
      }
#else
      //strcpy(expression, "x^3 - 6*(x^2) + 4*x + 12"); // 4.5340701967227321
      strcpy(expression, "x^2-25"); // 5 - sigh
      compile_expression(expression);
#endif // USE_HARDCODED

      if (output_file != NULL) {
        fprintf(output_file, "f(x) = %s\n", expression);
      }

      if (flag_step_by_step == 1) {
        //-step_choice = (char *)malloc(sizeof(char) * 8);

        printf("After each step you will be asked to continue, skip, or terminate the program.\n\
* Hit Enter without typing anything to continue to next step,\n\
* Type 's' and hit Enter to skip the loop through to get the result,\n\
* Type 'q' and hit Enter to terminate the program,\n\
* Type a number and hit Enter to skip this many steps.\n\n");
      }

      // TODO Check with method_initalize() function, if error occurs make user to re-enter interval
      (*method_initialize)(info);

      if (info->root_status != root_status_no_root) {
#ifndef USE_HARDCODED
        // Get max iteration and/or (error) tolerance, unless method_initialization() did NOT set any of them
        if (info->iteration_max == DEFAULT_STEP_COUNT) {
          // iteration_max did NOT set on method_initialize() function

          printf("Enter maximum step count (leave empty for default): ");
          info->iteration_max = get_input_int(NULL);

          if (info->iteration_max == 0) {
            info->iteration_max = DEFAULT_STEP_COUNT;
          }
        }

        if (fabs(info->tolerance) <= DEFAULT_TOLERANCE) {
          // tolerance did NOT set on method_initialize() function

          printf("Enter absolute error (leave empty for default): ");
          info->tolerance = get_input_double(NULL);
          printf("\n");

          if (info->tolerance <= DEFAULT_TOLERANCE) {
            info->tolerance = DEFAULT_TOLERANCE;
          }
        }
#else
        info->iteration_max = DEFAULT_STEP_COUNT;
        info->tolerance = DEFAULT_TOLERANCE;
#endif // USE_HARDCODED

        (*method_calculate_root)(info);
      }
    }

    (*method_finalize)(info);

    switch (info->root_status) {
      case root_status_na:
      case root_status_no_root: {
        _print("Root isn't within the given interval.\n\n");

        break;
      }

      case root_status_root: {
        _print("Root: %.16lf\nf(Root) = %.16lf\nStep Count: %d\n\n", info->root, f(info->root), info->iteration_index);

        break;
      }

      case root_status_approx_root: {
        _print("Maximum step count reached!\nApproximate root: %.16lf\nStep Count: %d\n\n", info->root, info->iteration_index);

        break;
      }

      case root_status_error: {
        _print("Error occured while calculation. Please consult the error file (if logging enabled).\n\n");

        break;
      }
    }

    input_error = -1;
    while (input_error != 0) {
      printf("\
1) Restart program\n\
2) Restart program with same function\n\
3) Quit\n\
\n\
Choice: ");

      input_int = get_input_int(&input_error);
      printf("\n");

      switch (input_int) {
        case 1: {
          *expression = '\0';
        }

        case 2: {
          input_int = 0;
          // Do NOT change input_error here since this very while-loop depends on it

          info->iteration_index = 0;
          info->skip_to_step = 0;

          info->absolute_error = 0;

          info->root_status = root_status_na;
          info->root = 0;
          info->f_root = 0;

          // Since we call method_finalize() earlier, we do not need to deal with info->additional

          break;
        }

        case 3: {
          flag_exit_program = 1;

          break;
        }

        default: {
          printf("You made a wrong choice.\n");

          input_error = -1;
        }
      }
    }
  }

  // TODO Print "program finalized" if output file is set
  _print("Program terminated.\n\
===================\n\
\n");

  // Free the allocations
  free(expression);
  free(arg_input_path);
  te_free(te_function);
  free(step_choice);
  free_calculation_info(info);
  // TODO free output_file and other variables

  fclose(output_file);
  fclose(stderr);

  return 0;
}

void parse_args(int argc, char **argv) {
  // http://www.gnu.org/software/libc/manual/html_node/Getopt-Long-Option-Example.html#Getopt-Long-Option-Example

  int c = 0;

  while (1) {
    struct option long_options[] = {
      // --no-step Enables step-by-step calculation
      { "no-step", no_argument, &flag_step_by_step, 0 },
      // --function Accepts function string as argument
      { "function", required_argument, 0, 0 },
      // -m Indicates method index
      { "method", required_argument, 0, 'm' },
      // -o Manipulates standard output strem
      { "output", optional_argument, 0, 'o' },
      // -v with argument NULL (only fatal), v (fatal+error), vv (fatal+error+warning) or vvv (all+debug)
      { "verbose", optional_argument, 0, 'v' },
      // --version Print version of the program
      { "version", no_argument, 0, 0 },
      {0, 0, 0, 0}
    };

    /* getopt_long stores the option index here. */
    int option_index = 0;

    // a:: optional, b: required, c no parameter
    c = getopt_long(argc, argv, "v::m:o::", long_options, &option_index);

    /* Detect the end of the options. */
    if (c == -1) {
      break;
    }

    switch (c) {
      case 0: {
        /* If this option set a flag, do nothing else now. */
        if (long_options[option_index].flag != 0) {
          break;
        }

        if (strcmp(long_options[option_index].name, "version") == 0) {
          print_name_and_version();
        } else if (strcmp(long_options[option_index].name, "function") == 0) {
          strcpy(expression, optarg);
        }

        //Get option arguments
        /*if (optarg != NULL) {
          printf("arg %s", optarg);
        }*/

        break;
      }

      case 'v': {
        if (optarg == NULL) {
          log_level = log_level_fatal;
        } else if (strcmp(optarg, "v") == 0) {
          log_level = log_level_error;
        } else if (strcmp(optarg, "vv") == 0) {
          log_level = log_level_warning;
        } else if (strcmp(optarg, "vvv") == 0) {
          log_level = log_level_debug;
        } else {
          // Wrong optional argument for option - immediately log the warning
          log_level = log_level_warning;

          _log(__FILE__, __LINE__, log_level_warning, "Wrong optional argument for 'verbose' option. Verbose level set to 'warning'.");
        }

        break;
      }

      case 'o': {
        output_file = fopen(optarg == NULL ? "rootfinder.out" : optarg, "a");

        if (output_file == NULL) {
          // TODO log some warning and omit -o flag
        }

        break;
      }

      case 'm': {
        method_index = atoi(optarg);

        break;
      }
    }
  }

  /* Print any remaining command line arguments (not options, input file). */
  int remaining_argc = argc - optind; // Remaining arguments count

  if (remaining_argc > 1) {
    _log(__FILE__, __LINE__, log_level_warning, "More than one input file path specified. Last one will be used, others will be omitted.");

    arg_input_path = argv[remaining_argc + 1];
  }
}

void print_name_and_version() {
  printf("%s v%d.%d.%d\n", NAME, VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH);
}

void print_welcome_message() {
  print_name_and_version();

  printf("Floating-point precision is 16 and epsilon is %.16lf (%e).\n\n", DBL_EPSILON, DBL_EPSILON);
}

void print_help() {
  print_name_and_version();

  puts("line1\n\
line 2\n\
foo\n\
\n\
bar");

  // TODO Populate help text
}

void _log(const char *error_file_path, const int error_line, enum log_levels level, const char *message) {
  if (log_level < level) {
    return;
  }

  time_t t = time(NULL);
  struct tm tm = *localtime(&t);
  char *level_string = (char *)malloc(sizeof(char) * 6);

  switch (level) {
    case log_level_no:
      break;

    case log_level_fatal: {
      strcpy(level_string, "FATAL");

      break;
    }

    case log_level_error: {
      strcpy(level_string, "ERROR");

      break;
    }

    case log_level_warning: {
      strcpy(level_string, "WARNG");

      break;
    }

    case log_level_debug: {
      strcpy(level_string, "DEBUG");

      break;
    }
  }

  fprintf(
    stderr,
    "%04d-%02d-%02d %02d:%02d:%02d | %s @ %s:%d | %s\n",
    tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec,
    level_string, error_file_path, error_line, message
  );

  free(level_string);
}

int _print(const char *format, ...) {
  va_list arg;
  int done;

  va_start(arg, format);
  done = vfprintf(stdout, format, arg);
  if (output_file != NULL) {
    done = vfprintf(output_file, format, arg);
  }
  va_end(arg);

  return done;
}

// TODO Get rid of fflush()s if they're not necessay or avoidable
int get_input(char *output, unsigned int len) {
  if (output == NULL) {
    return -1;
  }

  if (len < 2) {
    return -2;
  }

  char *buffer = (char *)malloc(sizeof(char) * len);
  if (buffer == NULL) {
    return -1;
  }

  int idx = 0;

  if (fgets(buffer, len, stdin) != NULL) {
    while (*(buffer + idx) != '\n' && *(buffer + idx) != '\0' && idx < len) {
      *(output + idx) = *(buffer + idx);

      idx += 1;
    }

    *(output + idx) = '\0';

    if (len > idx && log_level >= log_level_warning) {
      _log(__FILE__, __LINE__, log_level_debug, "Memory leak on get_input().");
    }

    free(buffer);
    fflush(stdin);

    return idx + 1;
  }

  free(buffer);
  fflush(stdin);

  return 0;
}

// TODO Get rid of fflush()s if they're not necessay or avoidable
int get_input_wo_space(char *output, unsigned int len) {
  if (output == NULL) {
    return -1;
  }

  if (len < 2) {
    return -2;
  }

  char *buffer = (char *)malloc(sizeof(char) * len);
  if (buffer == NULL) {
    return -1;
  }

  int buffer_idx = 0,
      output_idx = 0;

  if (fgets(buffer, len, stdin) != NULL) {
    while (*(buffer + buffer_idx) != '\n' && *(buffer + buffer_idx) != '\0' && buffer_idx < len) {
      if (*(buffer + buffer_idx) == ' ') {
        buffer_idx += 1;

        continue;
      }

      *(output + output_idx) = *(buffer + buffer_idx);

      buffer_idx += 1;
      output_idx += 1;
    }

    *(output + output_idx) = '\0';

    if (len > output_idx && log_level >= log_level_warning) {
      _log(__FILE__, __LINE__, log_level_debug, "Memory leak on get_input().");
    }

    free(buffer);
    fflush(stdin);

    return output_idx + 1;
  }

  free(buffer);
  fflush(stdin);

  return 0;
}

int get_input_int(int *error) {
  char *input = malloc(sizeof(char) * 11);
  get_input(input, 11);

  if (error != NULL) {
    int i = 0,
        len = strlen(input);

    // Set NO_ERROR initially
    *error = 0;

    for (i = 0; i < len; i++) {
      if ((*(input + i) >= '0' && *(input + i) <= '9') || *(input + i) == '-') {
        continue;
      }

      *error = i + 1;

      break;
    }
  }

  int result = 0;
  sscanf(input, "%d", &result);

  free(input);

  return result;
}

double get_input_double(int *error) {
  char *input = malloc(sizeof(char) * 32);
  get_input(input, 32);

  if (error != NULL) {
    int i = 0,
        len = strlen(input);

    // Set NO_ERROR initially
    *error = 0;

    for (i = 0; i < len; i++) {
      if ((*(input + i) >= '0' && *(input + i) <= '9') || *(input + i) == '.' || *(input + i) == '-') {
        continue;
      }

      *error = i + 1;

      break;
    }
  }

  double result = 0;
  sscanf(input, "%lf", &result);

  free(input);

  return result;
}

enum compile_expression_errors compile_expression(char *expression) {
  int compilation_error = 0;

  // WARN Possibly memory leak
  te_variable vars[] = {
    { "x", &variable_x }
  };

  te_function = te_compile(expression, vars, 1, &compilation_error);

  if (te_function == NULL) {
    return -compilation_error;
  }

  return compile_expression_error_success;
}

enum compile_expression_errors compile_expression_to(char *expression, te_expr **compiled, double *x) {
  int compilation_error = 0;

  // WARN Possibly memory leak
  te_variable vars[] = {
    { "x", x }
  };

  *compiled = te_compile(expression, vars, 1, &compilation_error);

  if (*compiled == NULL) {
    return -compilation_error;
  }

  return compile_expression_error_success;
}

enum get_expression_errors get_expression(char **expression) {
  // If expression is already set show it, instead of getting from user
  if (**expression == '\0') {
    printf("Please enter an expression\n\
==========================\n\
f(x) = ");
    get_input_wo_space(*expression, 128);
  } else {
    printf("Function expression\n\
===================\n\
f(x) = %s\n",
      *expression
    );
  }

  printf("\n");

  // Easter egg
  if (strchr(*expression, 'x') == NULL) {
    // Reset "expression" to avoid infinity loop - set first character to NULL
    **expression = '\0';

    return get_expression_error_no_variable;
  }

  return get_expression_error_success;
}

inline double f(double x) {
  variable_x = x;

  return te_eval(te_function);
}

// This function MUST be called after each step of calculation if flag_step_by_step was set
void step(struct CalculationInfo *info) {
  (*method_additional_dump)(info, &additional_dump);

  _print("\
Iteration #%d (out of %d)\n\
--------------\n\
%s\n\
Error             : %.16lf\n\
Calculated Root   : %.16lf\n\
========================================\n\
\n",
    info->iteration_index + 1, info->iteration_max, additional_dump, info->absolute_error, info->root);

  if (info->skip_to_step <= info->iteration_index) {
    fgets(step_choice, 8, stdin);

    if (step_choice[0] == 's') {
      // Skip the loop through the end

      info->skip_to_step = info->iteration_max;
    } else if (step_choice[0] == 'q') {
      // TODO Do NOT exit program, just break the step-loop and let user to decide the course on show-down
      exit(2);
    } else if ('0' <= step_choice[0] && step_choice[0] <= '9') {
      sscanf(step_choice, "%d", &step_idx);

      info->skip_to_step = info->iteration_index + 1 + step_idx;
    }
  }
}

void free_calculation_info(struct CalculationInfo *info) {
  if (info->additional != NULL) {
    (*method_finalize)(info);
  }
  free(info);
}
