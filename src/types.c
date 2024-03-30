#include <npak/types.h>
#include <stdlib.h>

NPAK_API npak_intro_header *npak_intro_header_new() {
  return calloc(1, sizeof(npak_intro_header));
}

NPAK_API void npak_intro_header_destroy(npak_intro_header *header) {
  free(header);
}

NPAK_API npak_assetsector_content_header *
npak_assetsector_content_header_new() {
  return calloc(1, sizeof(npak_assetsector_content_header));
}

NPAK_API void npak_assetsector_content_header_destroy(
    npak_assetsector_content_header *header) {
  free(header);
}

NPAK_API npak_assetsector_asset_header *npak_assetsector_asset_header_new() {
  return calloc(1, sizeof(npak_assetsector_asset_header));
}

NPAK_API void
npak_assetsector_asset_header_destroy(npak_assetsector_asset_header *header) {
  if (header) {
    free(header->path);
    free(header);
  }
}


NPAK_API npak_datasector_header* npak_datasector_header_new() {
  return calloc(1, sizeof(npak_datasector_data));
}

NPAK_API void npak_datasector_header_destroy(npak_datasector_header *obj) {
  free(obj);
}


NPAK_API npak_datasector_data* npak_datasector_data_new() {
  return calloc(1, sizeof(npak_datasector_data));
}

NPAK_API void npak_datasector_data_destroy(npak_datasector_data *obj) {
  if (obj)
    free(obj->data);
  free(obj);
}

NPAK_API npak *npak_init() { return calloc(1, sizeof(struct npak)); }

NPAK_API void npak_destroy(npak *npak) {
  if(npak) {
    for (unsigned index = 0; index < npak->content_header->count; index++) {
      npak_assetsector_asset_header_destroy(npak->asset_headers[index]);
      if (npak->data[index] != 0) {
        npak_datasector_data_destroy(npak->data[index]);
      }
    }
    npak_assetsector_content_header_destroy(npak->content_header);
    npak_intro_header_destroy(npak->intro_header);
    
    free(npak->data);
    free(npak->asset_headers);
    free(npak->data_header);
    free(npak->path);
    free(npak); 
  }
}