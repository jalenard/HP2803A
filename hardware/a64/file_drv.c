/**
 * @file file_drv.c
 * @brief Binary file-based emulation of non-volatile storage for A64.
 */

#include <stdbool.h>
#include <stdio.h>
#include "sys_settings_types.h"
#include "utils_crc.h"
#include "file_drv.h"

#define SETTINGS_MAGIC   0xBEEF
#define STORE_FILE_PATH  "/home/jamesl/5_Programs/C/HP2803A/tmp/settings_a64.bin"
#define STORE_SIZE_BYTES 1024U       // TODO: Match EEPROM capacity -> hw_store.h?
#define SETTINGS_ADDR    0u          // starting offset within store.bin

typedef struct
{
    uint16_t magic;
    uint16_t crc;
    Settings data;
} FileSettings;

static void ensure_drv_file(void)
{
    FILE * fp = fopen(STORE_FILE_PATH, "rb");
    if (fp)
    {
        fclose(fp);
        return;
    }

    fp = fopen(STORE_FILE_PATH, "wb");
    if (fp)
    {
        unsigned char fill = 0xFF;
        for (size_t i = 0; i < STORE_SIZE_BYTES; ++i)
        {
            fwrite(&fill, 1, 1, fp);
        }
        fclose(fp);
    }
}

void file_drv_init(void)
{
    ensure_drv_file();
}

bool file_drv_read(uint16_t addr, void * data, size_t len)
{
    if (!data || addr + len > STORE_SIZE_BYTES)
    {
        return false;
    }

    FILE * fp = fopen(STORE_FILE_PATH, "rb");
    if (!fp)
    {
        return false;
    }

    fseek(fp, addr, SEEK_SET);
    size_t n = fread(data, 1, len, fp);
    fclose(fp);
    return n == len;
}

bool file_drv_write(uint16_t addr, const void * data, size_t len)
{
    if (!data || addr + len > STORE_SIZE_BYTES)
    {
        return false;
    }

    FILE * fp = fopen(STORE_FILE_PATH, "r+b");
    if (!fp)
    {
        return false;
    }

    fseek(fp, addr, SEEK_SET);
    size_t n = fwrite(data, 1, len, fp);
    fclose(fp);
    return n == len;
}

void file_drv_commit(void)
{
    /* no-op */
}

static uint16_t calc_settings_crc(const Settings * s)
{
    return crc16((const uint8_t *)s, sizeof(Settings));
}

bool file_drv_read_settings(Settings * dst)
{
    FileSettings block;
    if (!file_drv_read(SETTINGS_ADDR, &block, sizeof(block)))
    {
        return false;
    }

    if (block.magic != SETTINGS_MAGIC)
    {
        return false;
    }

    uint16_t expected_crc = calc_settings_crc(&block.data);
    if (block.crc != expected_crc)
    {
        return false;
    }

    *dst = block.data;
    return true;
}

void file_drv_write_settings(const Settings * src)
{
    FileSettings block;
    block.magic = SETTINGS_MAGIC;
    block.data  = *src;
    block.crc   = calc_settings_crc(src);

    file_drv_write(SETTINGS_ADDR, &block, sizeof(block));
    file_drv_commit();
}

