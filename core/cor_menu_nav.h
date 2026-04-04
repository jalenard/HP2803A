/**
 * @file cor_menu_nav.h
 * @brief
 */

#ifndef COR_MENU_NAV_H
#define COR_MENU_NAV_H

#include "cor_menu_nav.h"
#include "cor_menu_types.h"
#include "svc_input.h"

extern NodeID active_node_id;
extern NodeID cursor_node_id;

void menu_nav_init(void);
const MenuNode* menu_nav_get_node(NodeID node_id);
void menu_nav_step(UIKey key);

#endif

