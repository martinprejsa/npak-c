#include "npak/errors.h"
#include "npak/types.h"
#include <assert.h>
#include <npak/npak.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uchar.h>

#define NPAK_OF_INTRO_HEADER 0x0

#define NPAK_OF_ASSETSECTOR_HEADER 0x7D000
#define NPAK_OF_ASSETSECTOR_FIRST_HEADER 0x7D00C

#define NPAK_OF_DATASECTOR_DATA 0xF00000

#define NPAK_INTRO_HEADER_MAGIC 0x534B4150

#define NPAK_ASSETSECTOR_HEADER_MAGIC0 0x534C4946
#define NPAK_ASSETSECTOR_HEADER_MAGIC1 0x5A4C4946

#define NPAK_DATASECTOR_HEADER_MAGIC 0x454C4946

#define NPAK_CRC_INIT 0x0

void read_intro_header(FILE *const stream, npak *npak, npak_error *error) {
  uint32_t buffer[10] = {0};

  npak->intro_header = npak_intro_header_new();

  fseek(stream, NPAK_OF_INTRO_HEADER, SEEK_SET);
  if (fread(buffer, sizeof(uint32_t), 10, stream) != 10) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "failed to read intro header, unexpected EOF";
    }
    return;
  }

  npak->intro_header->magic = buffer[0];

  if (npak->intro_header->magic != NPAK_INTRO_HEADER_MAGIC) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "failed to read intro header, invalid magic";
    }
    return;
  }

  npak->intro_header->unknown0 = buffer[1];
  npak->intro_header->version = buffer[2];
  npak->intro_header->asset_count = buffer[4];
  npak->intro_header->used_asset_count = buffer[5];
  npak->intro_header->deleted_asset_count = buffer[6];
  npak->intro_header->file_size = buffer[7];
  npak->intro_header->team_version = buffer[8];
  npak->intro_header->header_sign = buffer[9];

  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
  return;
}

void read_assetsector_content_header(FILE *const stream, npak *npak,
                                     npak_error *error) {
  uint32_t buffer[3] = {0};

  npak->content_header = npak_assetsector_content_header_new();

  fseek(stream, NPAK_OF_ASSETSECTOR_HEADER, SEEK_SET);
  if (fread(buffer, sizeof(uint32_t), 3, stream) != 3) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "failed to read content header, unexpected EOF";
    }
    return;
  }

  npak->content_header->magic0 = buffer[0];
  npak->content_header->magic1 = buffer[1];
  npak->content_header->count = buffer[2];

  if (npak->content_header->magic0 != NPAK_ASSETSECTOR_HEADER_MAGIC0 ||
      npak->content_header->magic1 != NPAK_ASSETSECTOR_HEADER_MAGIC1) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "failed to read intro header, invalid magic";
    }
    return;
  }

  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
  return;
}

void read_assetsector_asset_headers(FILE *const stream, npak *npak,
                                    npak_error *error) {

  uint32_t buffer[27] = {0};
  char16_t path_buffer[256] = {0};

  npak->asset_headers = calloc(npak->content_header->count,
                               sizeof(npak_assetsector_asset_header));

  for (int index = 0; index < npak->content_header->count; index++) {
    fseek(stream,
          NPAK_OF_ASSETSECTOR_FIRST_HEADER +
              (NPAK_SIZE_ASSETSECTOR_ASSET_HEADER * index),
          SEEK_SET);
    if (fread(buffer, sizeof(uint32_t), 27, stream) != 27) {
      if (error) {
        (error)->kind = NPAK_ERROR_IO;
        (error)->description = "failed to read asset header, unexpected EOF";
      }
      return;
    }

    if (fread(path_buffer, sizeof(char16_t), 256, stream) != 256) {
      if (error) {
        (error)->kind = NPAK_ERROR_IO;
        (error)->description =
            "failed to read asset header's path, unexpected EOF";
      }
      return;
    }

    npak->asset_headers[index] = npak_assetsector_asset_header_new();
    npak->asset_headers[index]->index = index;

    npak->asset_headers[index]->prefix = buffer[0];
    npak->asset_headers[index]->magic = buffer[1];
    npak->asset_headers[index]->embedded_offset = buffer[2];
    npak->asset_headers[index]->embedded_length = buffer[3];
    npak->asset_headers[index]->data_decompressed_length0 = buffer[4];
    npak->asset_headers[index]->is_compressed = buffer[5];
    npak->asset_headers[index]->data_decompressed_length1 = buffer[6];
    npak->asset_headers[index]->unknown0 = buffer[7];
    npak->asset_headers[index]->data_decompressed_length2 = buffer[8];
    npak->asset_headers[index]->unknown1 = buffer[9];
    npak->asset_headers[index]->unknown2 = buffer[10];
    npak->asset_headers[index]->unknown3 = buffer[11];
    npak->asset_headers[index]->unknown4 = buffer[12];
    npak->asset_headers[index]->unknown5 = buffer[13];
    npak->asset_headers[index]->is_deleted = buffer[14];
    npak->asset_headers[index]->offset = buffer[15];
    npak->asset_headers[index]->is_embeded = buffer[16];
    npak->asset_headers[index]->unknown6 =
        buffer[17] + (((uint64_t)buffer[18]) << 32);
    npak->asset_headers[index]->unknown7 =
        buffer[19] + (((uint64_t)buffer[20]) << 32);
    npak->asset_headers[index]->decompressed_crc = buffer[21];
    npak->asset_headers[index]->embedded_crc = buffer[22];
    npak->asset_headers[index]->crc_identification = buffer[23];
    npak->asset_headers[index]->decompressed_checksum = buffer[24];
    npak->asset_headers[index]->embedded_checksum = buffer[25];
    npak->asset_headers[index]->unknown8 = buffer[26];
    npak->asset_headers[index]->path = calloc(256, sizeof(char16_t));
    memcpy(npak->asset_headers[index]->path, path_buffer,
           sizeof(char16_t) * 256);
  }
  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
}

void read_datasector_header(FILE *stream, npak *npak, npak_error *error) {
  if (!npak->content_header) {
    if (error) {
      (error)->kind = NPAK_ERROR_BAD_PARAM;
      (error)->description = "npak is not initialized properly";
    }
    return;
  }
  npak->data_header = npak_datasector_header_new();

  fseek(stream,
        NPAK_OF_ASSETSECTOR_FIRST_HEADER +
            npak->content_header->count * NPAK_SIZE_ASSETSECTOR_ASSET_HEADER,
        SEEK_SET);

  uint32_t buffer = 0;
  if (fread(&buffer, sizeof(uint32_t), 1, stream) != 1) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description =
          "failed to read data sector header, unexpected eof";
    }
  }

  npak->data_header->magic = buffer;

  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
}

void write_intro_header(FILE *stream, npak *npak, npak_error *error) {
  if (!npak->intro_header) {
    if (error) {
      (error)->kind = NPAK_ERROR_BAD_PARAM;
      (error)->description = "npak is not initialized properly";
    }
    return;
  }

  fseek(stream, NPAK_OF_INTRO_HEADER, SEEK_SET);
  uint32_t buffer[10] = {npak->intro_header->magic,
                         npak->intro_header->unknown0,
                         npak->intro_header->version,
                         npak->intro_header->locale,
                         npak->intro_header->asset_count,
                         npak->intro_header->used_asset_count,
                         npak->intro_header->deleted_asset_count,
                         npak->intro_header->file_size,
                         npak->intro_header->team_version,
                         npak->intro_header->header_sign};

  if (fwrite(buffer, sizeof(uint32_t), 10, stream) != 10) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "failed to write intro header, unexpected EOF";
    }
    return;
  }
  fflush(stream);
  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
}

void write_assetsector_content_header(FILE *const stream, npak *npak,
                                      npak_error *error) {

  if (!npak->content_header) {
    if (error) {
      (error)->kind = NPAK_ERROR_BAD_PARAM;
      (error)->description = "npak is not initialized properly";
    }
    return;
  }

  uint32_t buffer[3] = {
      npak->content_header->magic0,
      npak->content_header->magic1,
      npak->content_header->count,
  };

  fseek(stream, NPAK_OF_ASSETSECTOR_HEADER, SEEK_SET);
  if (fwrite(buffer, sizeof(uint32_t), 3, stream) != 3) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "failed to write content header, unexpected EOF";
    }
    return;
  }

  fflush(stream);

  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
  return;
}

void write_assetsector_asset_headers(FILE *const stream, npak *npak,
                                     npak_error *error) {
  if (!npak->asset_headers || !npak->content_header) {
    if (error) {
      (error)->kind = NPAK_ERROR_BAD_PARAM;
      (error)->description = "npak is not initialized properly";
    }
    return;
  }
  uint32_t buffer[27] = {0};

  for (int index = 0; index < npak->content_header->count; index++) {
    buffer[0] = npak->asset_headers[index]->prefix,
    buffer[1] = npak->asset_headers[index]->magic,
    buffer[2] = npak->asset_headers[index]->embedded_offset,
    buffer[3] = npak->asset_headers[index]->embedded_length,
    buffer[4] = npak->asset_headers[index]->data_decompressed_length0,
    buffer[5] = npak->asset_headers[index]->is_compressed,
    buffer[6] = npak->asset_headers[index]->data_decompressed_length1,
    buffer[7] = npak->asset_headers[index]->unknown0,
    buffer[8] = npak->asset_headers[index]->data_decompressed_length2,
    buffer[9] = npak->asset_headers[index]->unknown1,
    buffer[10] = npak->asset_headers[index]->unknown2,
    buffer[11] = npak->asset_headers[index]->unknown3,
    buffer[12] = npak->asset_headers[index]->unknown4,
    buffer[13] = npak->asset_headers[index]->unknown5,
    buffer[14] = npak->asset_headers[index]->is_deleted,
    buffer[15] = npak->asset_headers[index]->offset,
    buffer[16] = npak->asset_headers[index]->is_embeded,
    buffer[17] = (uint32_t)(npak->asset_headers[index]->unknown6),
    buffer[18] = (uint32_t)(npak->asset_headers[index]->unknown6 >> 32),
    buffer[19] = (uint32_t)(npak->asset_headers[index]->unknown7),
    buffer[20] = (uint32_t)(npak->asset_headers[index]->unknown7 >> 32),
    buffer[21] = npak->asset_headers[index]->decompressed_crc,
    buffer[22] = npak->asset_headers[index]->embedded_crc,
    buffer[23] = npak->asset_headers[index]->crc_identification,
    buffer[24] = npak->asset_headers[index]->decompressed_checksum,
    buffer[25] = npak->asset_headers[index]->embedded_checksum,
    buffer[26] = npak->asset_headers[index]->unknown8,

    fseek(stream,
          NPAK_OF_ASSETSECTOR_FIRST_HEADER +
              (NPAK_SIZE_ASSETSECTOR_ASSET_HEADER * index),
          SEEK_SET);
    if (fwrite(buffer, sizeof(uint32_t), 27, stream) != 27) {
      if (error) {
        (error)->kind = NPAK_ERROR_IO;
        (error)->description = "failed to write asset header, unexpected EOF";
      }
      return;
    }

    if (fwrite(npak->asset_headers[index]->path, sizeof(char16_t), 256,
               stream) != 256) {
      if (error) {
        (error)->kind = NPAK_ERROR_IO;
        (error)->description =
            "failed to write asset header's path, unexpected EOF";
      }
      return;
    }
    fflush(stream);
  }
  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
}

void write_datasector_header(FILE *const stream, npak *npak,
                             npak_error *error) {
  if (!npak->data_header) {
    if (error) {
      (error)->kind = NPAK_ERROR_BAD_PARAM;
      (error)->description = "npak is not initialized properly";
    }
    return;
  }
  fseek(stream,
        NPAK_OF_ASSETSECTOR_FIRST_HEADER +
            npak->content_header->count * NPAK_SIZE_ASSETSECTOR_ASSET_HEADER,
        SEEK_SET);
  if (fwrite(&npak->data_header->magic, sizeof(uint32_t), 1, stream) != 1) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description =
          "failed to write datas sector header magic, unexpected EOF";
    }
  }
  fflush(stream);
}

void npak_update_headers(npak *npak) {
  unsigned offset = 0;
  for (unsigned index = 0; index < npak->content_header->count; index++) {
    npak->asset_headers[index]->embedded_offset += offset;
    if (npak->data[index] != 0) {
      offset += npak->asset_headers[index]->embedded_length -
                npak->data[index]->length;
    }
  }
}

bool cmppath(char const *const p1, char16_t const *const p2, unsigned p2len) {
  unsigned result = 0;
  for (unsigned index = 0; index < strlen(p1) || index < p2len; index++) {
    result += p1[index] - ((char)p2[index]) - ((char)(p2[index] >> 8));
  }

  return result == 0 ? true : false;
}

NPAK_API void npak_load_file(npak *const npak, char const *const path,
                             npak_error *error) {
  assert(npak);
  assert(npak->intro_header == 0);
  assert(npak->asset_headers == 0);
  assert(npak->content_header == 0);
  assert(npak->data_header == 0);
  assert(npak->data == 0);

  if (path == 0 || strlen(path) <= 0) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "invalid filepath";
    }
    return;
  }

  FILE *file = fopen(path, "rb");
  if (file == NULL) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "invalid filepath";
    }
    return;
  }

  npak->path = calloc(strlen(path) + 1, sizeof(char));
  strcpy(npak->path, path);

  read_intro_header(file, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(file);
    return;
  }

  read_assetsector_content_header(file, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(file);
    return;
  }

  read_assetsector_asset_headers(file, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(file);
    return;
  }

  read_datasector_header(file, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(file);
    return;
  }

  npak->data =
      calloc(npak->content_header->count, sizeof(npak_datasector_data));

  fclose(file);

  if (error) {
    (error)->kind = NPAK_ERROR_OK;
    (error)->description = 0;
  }
  return;
}

NPAK_API void npak_export_file(npak *const npak, char const *const path,
                               npak_error *error) {
  assert(npak);

  if (path == 0 || strlen(path) <= 0) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "invalid filepath";
    }
    return;
  }

  FILE *new = fopen(path, "w+b");
  if (new == NULL) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "invalid filepath";
    }
    return;
  }

  FILE *original = fopen(npak->path, "rb");
  if (original == NULL) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "path to original invalid";
    }
    return;
  }

  write_intro_header(new, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(new);
    fclose(original);
    return;
  }

  write_assetsector_content_header(new, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(new);
    fclose(original);
    return;
  }

  write_assetsector_asset_headers(new, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(new);
    fclose(original);
    return;
  }

  write_datasector_header(new, npak, error);
  if (error && (error)->kind != NPAK_ERROR_OK) {
    fclose(new);
    fclose(original);
    return;
  }

  uint8_t buffer[1048576] = {0};

  for (unsigned index = 0; index < npak->content_header->count; index++) {
    if (npak->asset_headers[index]->is_deleted)
      continue;

    size_t left_to_copy = 0;

    left_to_copy = npak->asset_headers[index]->embedded_length;

    fseek(new, npak->asset_headers[index]->embedded_offset, SEEK_SET);
    fseek(original, npak->asset_headers[index]->embedded_offset, SEEK_SET);

    do {
      unsigned actual_length =
          left_to_copy > sizeof(buffer) ? sizeof(buffer) : left_to_copy;

      unsigned r = fread(buffer, sizeof(uint8_t), actual_length, original);
      if (r != actual_length) {
        fclose(new);
        fclose(original);
        if (error) {
          (error)->kind = NPAK_ERROR_IO;
          (error)->description = "unexpected EOF while reading original data";
        }
        return;
      }
      unsigned w = fwrite(buffer, sizeof(uint8_t), actual_length, new);
      if (w != actual_length) {
        fclose(new);
        fclose(original);
        if (error) {
          (error)->kind = NPAK_ERROR_IO;
          (error)->description = "failed to write data";
        }
        return;
      }

      left_to_copy -= actual_length;

      fflush(new);

      if (left_to_copy < 0) {
        fclose(new);
        fclose(original);
        if (error) {
          (error)->kind = NPAK_ERROR_IO;
          (error)->description = "size mismatch while copying";
        }
      }
    } while (left_to_copy != 0);
  }

  fflush(new);
  fclose(new);
  fclose(original);
  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }
}

NPAK_API npak_assetsector_asset_header *
npak_find_header(npak *const npak, char const *const path) {
  if (strlen(path) > 256 || strlen(path) == 0) {
    return 0;
  }

  for (unsigned index = 0; index < npak->content_header->count; index++) {
    if (cmppath(path, npak->asset_headers[index]->path, strlen(path))) {
      return npak->asset_headers[index];
    }
  }

  return 0;
}

NPAK_API npak_datasector_data *
npak_get_data(npak *const npak, npak_assetsector_asset_header *header,
              npak_error *error) {
  if (!npak) {
    if (error) {
      error->kind = NPAK_ERROR_BAD_PARAM;
      error->description = "invalid npak provided";
    }
  }

  if (!header) {
    if (error) {
      error->kind = NPAK_ERROR_BAD_PARAM;
      error->description = "invalid header provided";
    }
  }

  // Return already edited, or already requested data.
  if (npak->data[header->index] != 0) {
    return npak->data[header->index];
  } else {
    npak->data[header->index] = npak_datasector_data_new();
  }

  FILE *stream = fopen(npak->path, "r");
  if (!stream) {
    if (error) {
      error->kind = NPAK_ERROR_IO;
      error->description = "failed to open file for reading";
    }
    return 0;
  }

  uint8_t *data = malloc(header->embedded_length * sizeof(uint8_t));

  fseek(stream, header->embedded_offset, SEEK_SET);

  unsigned r = fread(data, sizeof(uint8_t), header->embedded_length, stream);
  fclose(stream);

  if (r != header->embedded_length) {
    if (error) {
      (error)->kind = NPAK_ERROR_IO;
      (error)->description = "unexpected EOF while reading asset";
    }
    return 0;
  }
  npak->data[header->index]->length = header->embedded_length;
  npak->data[header->index]->data = data;

  if (error) {
    (error)->kind = NPAK_ERROR_OK;
  }

  return npak->data[header->index];
}

NPAK_API void npak_set_data(npak *const npak,
                            npak_assetsector_asset_header *header,
                            npak_datasector_data *data, npak_error *error) {}