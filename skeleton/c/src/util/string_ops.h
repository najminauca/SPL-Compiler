#ifndef STRING_OPS_H
#define STRING_OPS_H

#include <stdio.h>

/**
 * Copies a string to a newly allocated memory area.
 *
 * @param src A pointer to the string that is copied.
 * @return The pointer to the copied string
 */
char const *copy(char const *src);

/**
 * Formats a string (in accordance to the rules of printf), but in memory and returns the result as a newly copied string.
 * This function uses an internal buffer of 1024 byte, so no string longer than that can be formatted!
 * The result is then copied to a new memory location.
 *
 * @param fmt The format string.
 * @param ... Arguments to the formatting.
 * @return A pointer to the formatted, copied string.
 */
char const *format(char const *fmt, ...);

/**
 * Formats an int default_value to a string, with a default default_value for n == INT_MIN
 *
 * @param n The default_value to format
 * @param default_value The default default_value in case n is INT_MIN
 * @return The formatted string.
 */
char const *formatInt(int n, char const *default_value);

/**
 * Outputs a string a given number of times to an output stream.
 * @param output The stream to print the string to.
 * @param n The number of repetitions
 * @param str The string to output.
 */
void repeat(FILE *output, int n, const char *str);

#endif