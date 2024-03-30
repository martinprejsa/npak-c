#include <npak/errors.h>
#include <npak/types.h>
#include <string.h>
#include <assert.h>

void function(npak_error ** error) {
  if(error) {
    (*error)->description = "Alright";
    (*error)->kind = NPAK_ERROR_OK;
  }
}

int main() {
  npak_error * error = npak_error_new();

  function(&error);

  assert(strcmp(error->description, "Alright") == 0);
  assert(error->kind == NPAK_ERROR_OK);
  assert(npak_error_string(error->kind) != 0);
  assert(npak_error_string(0x9999) == 0);
  assert(npak_error_string(-1) == 0);
  npak_error_destroy(error);
}