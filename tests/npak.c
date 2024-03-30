#include "npak/errors.h"
#include "npak/types.h"
#include <npak/npak.h>
#include <assert.h>

int main() {
  npak *handle = npak_init();
  npak_error *error = npak_error_new();

  npak_load_file(handle, "res.pak", error);
  assert(error->kind == NPAK_ERROR_OK);

  npak_assetsector_asset_header *h = npak_find_header(handle, "graphics/ui/refresh/myinfo_msg_dialog.xml");
  assert(h != 0);

  npak_get_data(handle, h, error);
  assert(error->kind == NPAK_ERROR_OK);

  assert(error->kind == NPAK_ERROR_OK);

  npak_export_file(handle, "res.pak.new", error);
  assert(error->kind == NPAK_ERROR_OK);
  
  npak_destroy(handle);
  npak_error_destroy(error);
}