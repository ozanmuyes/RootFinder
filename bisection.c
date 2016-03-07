#include <math.h>

#include "common.h"
#include "bisection.h"

int bs_check_initial_conditions(struct Interval *interval) {
  // 4.1. HACK We are considering the function is continuous
  // TODO If error occures at this very stage return 1

  // 4.2 f(a)*f(b)<0
  // HACK Change f() with te_eval(teFunct)
  double fis = f(interval->start),
         fie = f(interval->end);
  if (fis * fie >= 0) {
    return 2;
  }

  // 4.3. Check next initial condition - if exists...

  // ...finally return no error
  return 0;
}

/**
 * Finds the root of given function by Bisection method.
 * WHEN root FOUND THIS FUNCTION RETURNS 0, OTHERWISE 1.
 *
 * Remarks;
 * - max_iterations has higher precedence than tolerance
 * - interval.start MUST be less than interval.end
 * - If 0 is passed for max_iterations UINT_MAX will be used
 * - If 0 or any negative integer passed for tolerance DBL_EPSILON will be used
 */
void bs_find_root(struct Interval *interval, struct FindRootInfo *info, double *root) {
  info->last_root = info->midpoint;
  info->midpoint = (interval->start + interval->end) / 2;
  info->error = fabs(info->last_root - info->midpoint);

  *root = info->midpoint;
}
