/**
 * @file svc_menu_nav.c
 * @brief
 */

#include "svc_input.h"      // TODO: reqd for UIKey - right place?
#include "svc_menu_nav.h"
#include "cor_menu_nav.h"

void svc_menu_step(UIKey key)
{
    menu_nav_step(key);
}

