#ifndef ROOTFINDER_H_INCLUDED
#define ROOTFINDER_H_INCLUDED

#include "tinyexpr.h"

#define DEFAULT_STEP_COUNT 13000
#define DEFAULT_TOLERANCE DBL_EPSILON * 10

static const char NAME[] = "RootFinder";

/**
 * Semantic version of the program.
 * See http://semver.org/ for detailed information.
 */
static const int VERSION_MAJOR = 0;
static const int VERSION_MINOR = 2;
static const int VERSION_PATCH = 1;

enum log_levels {
  log_level_no = 0,
  log_level_fatal,
  log_level_error,
  log_level_warning,
  log_level_debug
};

enum get_expression_errors {
  get_expression_error_na = -1,
  get_expression_error_success = 0,
  get_expression_error_no_variable
};

enum compile_expression_errors {
  /**
   * Since expression length is 128 characters, there could NOT
   * be syntax error on 129th chracter. So we can use this number
   * (also negating it for purposes) to indicate not set error for
   * compile espression error.
   */
  compile_expression_error_na = -129,
  compile_expression_error_success = 0,
  compile_expression_error_error
};

enum root_statuses {
  root_status_na = -1,
  root_status_no_root = 0, // There is no root within the given interval
  root_status_root = 1, // Root has been found within the given interval
  root_status_approx_root, // Maximum step count has been reached and approximate root has been calculated
  root_status_multiple_roots,
  root_status_error
};

/**
 * This structure SHOULD be created by rootfinder. Purpose of this structure is
 * passing the necessary data between method's function(s) and rootfinder methods.
 */
struct CalculationInfo {
  unsigned int iteration_max;
  unsigned int iteration_index;
  int flag_step_by_step;
  unsigned int skip_to_step;

  double tolerance;
  double absolute_error;

  /**
   * Since double comparison is not reliable we need to explicitly indicate
   * whether root is found or not.
   */
  enum root_statuses root_status;
  double root;
  double f_root;

  /**
   * This variable is reserved for the method, it can be used to transfer further
   * information about the method between steps.
   * Do NOT change it's value in any of the rootfinder's methods.
   */
  void *additional;
};

void _log(const char *error_file_path, const int error_line, enum log_levels level, const char *message);
int get_input(char *output, unsigned int len);
int get_input_wo_space(char *output, unsigned int len);
int get_input_int(int *error);
double get_input_double(int *error);
inline double f(double x);
void step(struct CalculationInfo *info);
enum compile_expression_errors compile_expression_to(char *expression, te_expr **compiled, double *x);

#endif // ROOTFINDER_H_INCLUDED
