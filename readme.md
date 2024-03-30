# NPAK library
Platform agnostic library to edit and repack NTreev's PAK format. 

Following [rgnt's NPAK theory](https://github.com/Story-Of-Alicia/alicia_modwork/blob/master/theory/assets/pak.md).

# Usage
```c
  npak *handle = npak_init();
  npak_error *error = npak_error_new();

  npak_load_file(handle, "file.pak", error);
  if(error->kind != NPAK_ERROR_NO) {
      if(error->description)
        puts(error->description);
  }

  npak_assetsector_asset_header *h 
    = npak_find_header(handle, "graphics/ui/refresh/myinfo_msg_dialog.xml");
  if (h != 0) {
    npak_datasector_data *d = npak_get_data(handle, h, error);
    if(error->kind != NPAK_ERROR_NO) {
      if(error->description)
        puts(error->description);
    }
  }
    

  npak_export_file(handle,
    "/home/martin/workspace/src/res.pak.new", error);
  if(error->kind != NPAK_ERROR_NO) {
      if(error->description)
        puts(error->description);
  }

  npak_destroy(handle);
  npak_error_destroy(error);
```