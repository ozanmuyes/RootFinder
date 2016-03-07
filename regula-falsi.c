#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "common.h"
#include "regula-falsi.h"

int rf_check_initial_conditions(struct Interval *interval) {
  // 1st condition - is function continuous in the given interval
  // HACK We will assume that function continuous in the given interval

  // 2nd condition - f(a)*f(b)<0
  if (f(interval->start) * f(interval->end) >= 0) {
    return 2;
  }

  // So far so good - return without an error code
  return 0;
}

int rf_find_root(struct Interval *interval, unsigned int max_iterations, double tolerance, double *root) {
  unsigned int iteration_idx = 0;
  double last_root = 0,
         midpoint = 0,
         error = 0;
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
    last_root = midpoint;
    midpoint = interval->end - (((interval->end) * f(interval->end)) / (f(interval->end) - f(interval->start)));
    error = fabs(last_root - midpoint);

    if (error < tolerance || f(midpoint) == 0) {
      break;
    }

    /*
     * Check initial conditions for [interval.start, midpoint];
     * - If passes use them as new interval.
     * - If NOT passes use [midpoint, interval.end] as new interval.
     */
    candidate_interval->start = interval->start;
    candidate_interval->end = midpoint;
    if (rf_check_initial_conditions(candidate_interval) == 0) {
      interval->end = midpoint;

#ifdef STEP_BY_STEP
      bHasIntervalEndChanged = 1;
#endif // STEP_BY_STEP
    } else {
      interval->start = midpoint;

#ifdef STEP_BY_STEP
      bHasIntervalEndChanged = 0;
#endif // STEP_BY_STEP
    }

#ifdef STEP_BY_STEP
    if (bSkipThroughEnd == 0) {
      printf("Iteration Index   : %d\nError             : %.16lf\nIs [a, c] Selected: %s\nCalculated Root   : %.16lf\n\n", iteration_idx + 1, error, bHasIntervalEndChanged == 0 ? "No" : "Yes", midpoint);

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

  // Clean-up
  free(candidate_interval);
#ifdef STEP_BY_STEP
  free(szStep);
#endif // STEP_BY_STEP

  *root = midpoint;
}
