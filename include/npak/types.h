#ifndef NPAK_TYPES_H
#define NPAK_TYPES_H

#include <npak/visibility.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <uchar.h>

#define NPAK_SIZE_INTRO_HEADER 40
#define NPAK_SIZE_ASSETSECTOR_CONTENT_HEADER 12
#define NPAK_SIZE_ASSETSECTOR_ASSET_HEADER 620
#define NPAK_SIZE_DATASECTOR_HEADER 4

typedef struct npak_intro_header {
  uint32_t magic;
  uint32_t unknown0;
  uint32_t version;
  uint32_t locale;
  uint32_t asset_count;
  uint32_t used_asset_count;
  uint32_t deleted_asset_count;
  uint32_t file_size;
  uint32_t team_version;
  uint32_t header_sign;
} npak_intro_header;

typedef struct npak_assetsector_content_header {
  uint32_t magic0;
  uint32_t magic1;
  uint32_t count;
} npak_assetsector_content_header;

typedef struct npak_assetsector_asset_header {
  unsigned index;
  
  uint32_t prefix;
  uint32_t magic;
  uint32_t embedded_offset;
  uint32_t embedded_length;
  uint32_t data_decompressed_length0;
  uint32_t is_compressed;
  uint32_t data_decompressed_length1;
  uint32_t unknown0;
  uint32_t data_decompressed_length2;
  uint32_t unknown1;
  uint32_t unknown2;
  uint32_t unknown3;
  uint32_t unknown4;
  uint32_t unknown5;
  uint32_t is_deleted;
  uint32_t offset;
  uint32_t is_embeded;
  uint64_t unknown6;
  uint64_t unknown7;
  uint32_t decompressed_crc;
  uint32_t embedded_crc;
  uint32_t crc_identification;
  uint32_t decompressed_checksum;
  uint32_t embedded_checksum;
  uint32_t unknown8;
  char16_t *path;
} npak_assetsector_asset_header;

typedef struct npak_datasector_header {
  uint32_t magic;
} npak_datasector_header;

typedef struct npak_datasector_data {
  size_t length;
  uint8_t *data;
} npak_datasector_data;

typedef struct npak {
  char *path;
  npak_intro_header *intro_header;
  npak_assetsector_content_header *content_header;
  npak_assetsector_asset_header **asset_headers;
  npak_datasector_header *data_header;
  npak_datasector_data **data;
} npak;

/**
 * @brief Creates new instance of npak_intro_header.
 * 
 * @return npak_intro_header* 
 */
NPAK_API npak_intro_header* npak_intro_header_new();

/**
 * @brief Destroys an instance of npak_intro_header.
 * 
 * @param obj the instance to destroy 
 */
NPAK_API void npak_intro_header_destroy(npak_intro_header *obj);

/**
 * @brief Creates new instance of npak_assetsector_content_header.
 * 
 * @return npak_assetsector_content_header* 
 */
NPAK_API npak_assetsector_content_header* npak_assetsector_content_header_new();

/**
 * @brief Destroys an instance of npak_assetsector_content_header.
 * 
 * @param obj the instance to destroy 
 */
NPAK_API void npak_assetsector_content_header_destroy(npak_assetsector_content_header *obj);

/**
 * @brief Creates new instance of npak_assetsector_asset_header.
 * 
 * @return npak_assetsector_asset_header* 
 */
NPAK_API npak_assetsector_asset_header* npak_assetsector_asset_header_new();

/**
 * @brief Destroys an instance of npak_assetsector_content_header.
 * 
 * @param obj the instance to destroy 
 */
NPAK_API void npak_assetsector_asset_header_destroy(npak_assetsector_asset_header *obj);

/**
 * @brief Creates new instance of npak_datasector_header.
 * 
 * @return npak_datasector_header* 
 */
NPAK_API npak_datasector_header* npak_datasector_header_new();

/**
 * @brief Destroys an instance of npak_datasector_header.
 * 
 * @param obj the instance to destroy 
 */
NPAK_API void npak_datasector_header_destroy(npak_datasector_header *obj);

/**
 * @brief Creates new instance of npak_datasector_data.
 * 
 * @return npak_datasector_data* 
 */
NPAK_API npak_datasector_data* npak_datasector_data_new();

/**
 * @brief Destroys an instance of npak_datasector_data.
 * 
 * @param obj the instance to destroy 
 */
NPAK_API void npak_datasector_data_destroy(npak_datasector_data *obj);

/**
 * @brief Creates new instance of npak.
 * 
 * @return npak* 
 */
NPAK_API npak *npak_init();

/**
 * @brief Destroys an instance of npak.
 * 
 * @param obj the instance to destroy 
 */
NPAK_API void npak_destroy(npak *obj);

#endif // NPAK_TYPES_H
