#include <assert.h>
#include <npak/errors.h>
#include <stdlib.h>

char* error_strings[4] = {
  "NPAK_ERROR_OK",
  "NPAK_ERROR_IO",
  "NPAK_ERROR_NOT_FOUND",
  "NPAK_ERROR_BAD_PARAM",
};

static_assert(sizeof(error_strings) / sizeof(char*) == NPAK_ERROR_NUM);

NPAK_API npak_error* npak_error_new() {
  npak_error *e = calloc(1, sizeof(npak_error));
  e->kind = NPAK_ERROR_OK;
  return e;
}

NPAK_API void npak_error_destroy(npak_error *err) {
  free(err);
}

NPAK_API char* npak_error_string(npak_error_kind kind) {
  if (kind >= 0 && kind < NPAK_ERROR_NUM) {
    return error_strings[kind];
  } else {
    return 0;
  }
}