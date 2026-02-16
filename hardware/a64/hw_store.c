/**
 * @file hw_store.c
 * @brief A64 hardware backend for persistent storage.
 */

#include <stdbool.h>
#include "sys_settings_types.h"
#include "hw_store.h"
#include "file_drv.h"

static Settings saved_settings;

void hw_store_init(void)
{
    file_drv_init();
}

bool hw_store_read(uint16_t addr, void *data, size_t len)
{
    return file_drv_read(addr, data, len);
}

bool hw_store_write(uint16_t addr, const void *data, size_t len)
{
    return file_drv_write(addr, data, len);
}

void hw_store_commit(void)
{
    file_drv_commit();
}

bool hw_store_settings_init(Settings * dst)
{
    return file_drv_read_settings(dst);
}

bool hw_store_read_settings(Settings * dst)
{
    return file_drv_read_settings(dst);
}

void hw_store_write_settings(const Settings * src)
{
    file_drv_write_settings(src);
}

const Settings * get_saved_settings(void)
{
    return &saved_settings;
}

Settings * get_saved_settings_mutable(void)
{
    return &saved_settings;
}

