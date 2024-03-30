#ifndef NPAK_ERRORS_H
#define NPAK_ERRORS_H

#include <npak/visibility.h>

typedef enum npak_error_kind {
  NPAK_ERROR_OK = 0,
  NPAK_ERROR_IO,
  NPAK_ERROR_NOT_FOUND,
  NPAK_ERROR_BAD_PARAM,
  NPAK_ERROR_NUM
} npak_error_kind;

typedef struct npak_error {
  npak_error_kind kind;
  char* description;
} npak_error;


NPAK_API npak_error* npak_error_new();

NPAK_API void npak_error_destroy(npak_error *err);

/**
 * @brief Use to retrieve human readable error string from npak_error_kind.
 * 
 * @param kind error
 * @return char* human readable string
 */
NPAK_API char* npak_error_string(npak_error_kind kind);

#endif // NPAK_ERRORS_H