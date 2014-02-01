#ifndef PULSE_CHECK_H
#define PULSE_CHECK_H
#include <stdlib.h>
/*!
 * This functions determines how well a value fits a single sech pulse
 * of the form a*sech(d*x-b)+c. The function returns a score between zero and 1,
 * with 1 being the best and zero being the worst.
 * @param invals A pointer ot the list of values that create the function
 * @param numvals The length of the array
 * @return The score of the function, between zero and 1
 */
double pulse_check(double* invals, double* tvals, size_t numvals);
#endif
