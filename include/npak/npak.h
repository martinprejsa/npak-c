#ifndef NPAK_NPAK_H
#define NPAK_NPAK_H

#include "npak/types.h"
#include <npak/errors.h>
#include <npak/visibility.h>

/**
 * @brief Parses a specified file.
 * 
 * @param npak the npak instance
 * @param path the file path
 * @param error instance of npak_error or 0
 */
NPAK_API void npak_load_file(npak *const npak, char const *const path,
                             npak_error *error);

/**
 * @brief Exports a specified file.
 * 
 * @param npak the npak instance
 * @param path the file path
 * @param error instance of npak_error or 0
 */
NPAK_API void npak_export_file(npak *const npak, char const *const path,
                             npak_error *error);

NPAK_API npak_assetsector_asset_header* npak_find_header(npak *const npak, char const * const path);

NPAK_API npak_datasector_data* npak_get_data(npak *const npak, npak_assetsector_asset_header *header, npak_error * error);

NPAK_API void npak_set_data(npak *const npak, npak_assetsector_asset_header *header, npak_datasector_data* data, npak_error * error);

#endif // NPAK_NPAK_H
