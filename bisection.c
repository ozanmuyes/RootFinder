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
 *
 * Remarks;
 * - max_iterations has higher precedence than tolerance
 * - interval.start MUST be less than interval.end
 * - If 0 is passed for max_iterations UINT_MAX will be used
 * - If 0 or any negative integer passed for tolerance DBL_EPSILON will be used
 */
void bs_find_root(struct Interval *interval, unsigned int max_iterations, double tolerance, double *root) {
  if (max_iterations == 0) {
    max_iterations = UINT_MAX;
  }

  if (tolerance <= 0) {
    tolerance = DBL_EPSILON;
  }

  unsigned int iteration_idx = 0;
  double fLastRoot = 0,
         fIntervalMiddle = 0,
         fError = 0;
  struct Interval *candidate_interval = (struct Interval *)malloc(sizeof(struct Interval));

#ifdef STEP_BY_STEP
  int bHasIntervalEndChanged = 0,
      bSkipThroughEnd = 0,
      bVerboseSkipThroughEnd = 0;
  char *szStep = (char *)malloc(sizeof(char) * 8);
  unsigned int iStep = 0,
               iSkipTo = 0;

  printf("After each step you will be asked to continue, skip, or terminate the program.\n\
* Hit Enter without typing anything to continue to next step,\n\
* Type 's' and hit Enter to skip the loop through to get the result,\n\
* Type 'v' and hit Enter to verbose skip the loop through to get the result,\n\
* Type 'q' and hit Enter to terminate the program,\n\
* Type a number and hit Enter to skip this many steps.\n\n");
#endif // STEP_BY_STEP

  while (max_iterations > iteration_idx) {
    fLastRoot = fIntervalMiddle;
    fIntervalMiddle = (interval->start + interval->end) / 2;
    fError = fabs(fLastRoot - fIntervalMiddle);

    if (fError < tolerance || f(fIntervalMiddle) == 0) {
      break;
    }

    /*
     * Check initial conditions for [interval.start, fIntervalMiddle];
     * - If passes use them as new interval.
     * - If NOT passes use [fIntervalMiddle, interval.end] as new interval.
     */
    candidate_interval->start = interval->start;
    candidate_interval->end = fIntervalMiddle;
    if (bs_check_initial_conditions(candidate_interval) == 0) {
      interval->end = fIntervalMiddle;

#ifdef STEP_BY_STEP
      bHasIntervalEndChanged = 1;
#endif // STEP_BY_STEP
    } else {
      interval->start = fIntervalMiddle;

#ifdef STEP_BY_STEP
      bHasIntervalEndChanged = 0;
#endif // STEP_BY_STEP
    }

#ifdef STEP_BY_STEP
    if (bSkipThroughEnd == 0) {
      printf("Iteration Index   : %d\nError             : %.16lf\nIs [a, c] Selected: %s\nCalculated Root   : %.16lf\n\n", iteration_idx + 1, fError, bHasIntervalEndChanged == 0 ? "No" : "Yes", fIntervalMiddle);

      if (iSkipTo == iteration_idx) {
        iSkipTo = 0;
      }

      if (iSkipTo == 0 && bVerboseSkipThroughEnd == 0) {
        fgets(szStep, 8, stdin);

        if (szStep[0] == 's') {
          // Skip the loop through the end

          bSkipThroughEnd = 1;
        } else if (szStep[0] == 'v') {
          bVerboseSkipThroughEnd = 1;
        } else if (szStep[0] == 'q') {
          exit(2);
        } else if ('0' <= szStep[0] && szStep[0] <= '9') {
          sscanf(szStep, "%d", &iStep);

          iSkipTo = iteration_idx + iStep + 1;
        }
      }
    }
#endif // STEP_BY_STEP

    // The iteration counter MUST be incremented at the end of the loop,
    // so we can keep track of in which step we are in, in the loop codes.
    iteration_idx += 1;
  }

  *root = fIntervalMiddle;
}
