/**
 * @file sys_store.c
 * @brief System-level abstraction over hardware persistent storage.
 */

#include "sys_store.h"
#include "sys_settings_types.h"
#include "hw_store.h"

void sys_store_init(void)
{
    hw_store_init();
}

bool sys_store_load(uint16_t addr, void * buf, size_t len)
{
    if (buf == NULL || len == 0)
    {
        return false;
    }

    return hw_store_read(addr, buf, len);
}

bool sys_store_save(uint16_t addr, const void * buf, size_t len)
{
    if (buf == NULL || len == 0)
    {
        return false;
    }

    return hw_store_write(addr, buf, len);
}

void sys_store_commit(void)
{
    hw_store_commit();
}

void sys_store_settings_init(Settings * dst)
{
    hw_store_settings_init(dst);
}

void sys_store_read_settings(Settings * dst)
{
    hw_store_read_settings(dst);
}

void sys_store_write_settings(const Settings * src)
{
    hw_store_write_settings(src);
}

