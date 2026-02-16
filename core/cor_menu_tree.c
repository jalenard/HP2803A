/* menu_tree.c */

#include <stddef.h>
#include "cor_menu_tree.h"
#include "utils.h"

/*  Usage:
 *  STATIC_ASSERT(sizeof(menu_text) / sizeof(menu_text[0]) == TEXT_COUNT, menu_text_size_mismatch);
 *  STATIC_ASSERT(sizeof(menu) / sizeof(menu[0]) == NODE_COUNT, menu_size_mismatch);
 */
#define STATIC_ASSERT(cond, msg) typedef char static_assertion_##msg[(cond) ? 1 : -1]

const char* const menu_text[] =
{
    " <BASE> ",
    "Display1",
    "Display2",
    "Units   ",
    "Resoln. ",
    "Brightn.",
    "Sockets ",
    "Probe A ",
    "Probe B ",
    "Transmit",
    "Settings",
    "Calibrn.",
    "Probe DB",
    "T1      ",
    "T2      ",
    "T1-T2   ",
    "T2-T1   ",
    "Delta T1",
    "Delta T2",
    "Min T1  ",
    "Min T2  ",
    "Max T1  ",
    "Max T2  ",
    "Count1  ",
    "Count2  ",
    "C       ",
    "F       ",
    "K       ",
    "Low     ",
    "Medium  ",
    "High    ",
    "Main    ",
    "Other   ",
    "Front   ",
    "Back    ",
    "idx[0]  ",
    "idx[1]  ",
    "idx[2]  ",
    "On      ",
    "Off     ",
    "Save    ",
    "Discard ",
    "Default ",
    "Edit    ",
    "Core    ",
    "CF      "
};

//_Static_assert(sizeof(menu_text) / sizeof(menu_text[0]) == TEXT_COUNT, "char* menu_text[] array size mismatch");
STATIC_ASSERT(sizeof(menu_text) / sizeof(menu_text[0]) == TEXT_COUNT, menu_text_size_mismatch);

const MenuNode menu[] =
{
    /*  node_id             text_id          parent                 first child            next sibling          action_id */
    {NODE_BASE,             TEXT_BLANK,      NODE_NONE,             NODE_DISPLAY1,         NODE_NONE,            CTX_NONE},

    {NODE_DISPLAY1,         TEXT_DISPLAY1,   NODE_BASE,             NODE_DISP1_T1,         NODE_DISPLAY2,        CTX_NONE},
    {NODE_DISPLAY2,         TEXT_DISPLAY2,   NODE_BASE,             NODE_DISP2_T1,         NODE_UNITS,           CTX_NONE},
    {NODE_UNITS,            TEXT_UNITS,      NODE_BASE,             NODE_CELSIUS,          NODE_RESOLUTION,      CTX_NONE},
    {NODE_RESOLUTION,       TEXT_RESOLUTION, NODE_BASE,             NODE_SCALE2,           NODE_BRIGHTNESS,      CTX_NONE},
    {NODE_BRIGHTNESS,       TEXT_BRIGHTNESS, NODE_BASE,             NODE_BRT_MAIN,         NODE_SOCKETS,         CTX_BRIGHTNESS},
    {NODE_SOCKETS,          TEXT_SOCKETS,    NODE_BASE,             NODE_FRONT,            NODE_PROBE_A,         CTX_NONE},
    {NODE_PROBE_A,          TEXT_PROBE_A,    NODE_BASE,             NODE_PROBE_A_IDX0,     NODE_PROBE_B,         CTX_NONE},
    {NODE_PROBE_B,          TEXT_PROBE_B,    NODE_BASE,             NODE_PROBE_B_IDX0,     NODE_TRANSMIT,        CTX_NONE},
    {NODE_TRANSMIT,         TEXT_TRANSMIT,   NODE_BASE,             NODE_TRANSMIT_ON,      NODE_SETTINGS,        CTX_NONE},
    {NODE_SETTINGS,         TEXT_SETTINGS,   NODE_BASE,             NODE_SAVE,             NODE_CALIBRATE,       CTX_NONE},
    {NODE_CALIBRATE,        TEXT_CALIBRATE,  NODE_BASE,             NODE_NONE,             NODE_PROBE_DB,        CTX_NONE},
    {NODE_PROBE_DB,         TEXT_PROBE_DB,   NODE_BASE,             NODE_PROBE_EDIT,       NODE_NONE,            CTX_NONE},

    {NODE_DISP1_T1,         TEXT_T1,         NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_T2,        CTX_DISP1_T1},
    {NODE_DISP1_T2,         TEXT_T2,         NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_T1MINUST2, CTX_DISP1_T2},
    {NODE_DISP1_T1MINUST2,  TEXT_T1MINUST2,  NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_T2MINUST1, CTX_DISP1_T1MINUST2},
    {NODE_DISP1_T2MINUST1,  TEXT_T2MINUST1,  NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_DELTAT1,   CTX_DISP1_T2MINUST1},
    {NODE_DISP1_DELTAT1,    TEXT_DELTAT1,    NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_DELTAT2,   CTX_DISP1_DELTAT1},
    {NODE_DISP1_DELTAT2,    TEXT_DELTAT2,    NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_MINT1,     CTX_DISP1_DELTAT2},
    {NODE_DISP1_MINT1,      TEXT_MINT1,      NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_MINT2,     CTX_DISP1_MINT1},
    {NODE_DISP1_MINT2,      TEXT_MINT2,      NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_MAXT1,     CTX_DISP1_MINT2},
    {NODE_DISP1_MAXT1,      TEXT_MAXT1,      NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_MAXT2,     CTX_DISP1_MAXT1},
    {NODE_DISP1_MAXT2,      TEXT_MAXT2,      NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_COUNT1,    CTX_DISP1_MAXT2},
    {NODE_DISP1_COUNT1,     TEXT_COUNT1,     NODE_DISPLAY1,         NODE_NONE,             NODE_DISP1_COUNT2,    CTX_DISP1_COUNT1},
    {NODE_DISP1_COUNT2,     TEXT_COUNT2,     NODE_DISPLAY1,         NODE_NONE,             NODE_NONE,            CTX_DISP1_COUNT2},

    {NODE_DISP2_T1,         TEXT_T1,         NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_T2,        CTX_DISP2_T1},
    {NODE_DISP2_T2,         TEXT_T2,         NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_T1MINUST2, CTX_DISP2_T2},
    {NODE_DISP2_T1MINUST2,  TEXT_T1MINUST2,  NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_T2MINUST1, CTX_DISP2_T1MINUST2},
    {NODE_DISP2_T2MINUST1,  TEXT_T2MINUST1,  NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_DELTAT1,   CTX_DISP2_T2MINUST1},
    {NODE_DISP2_DELTAT1,    TEXT_DELTAT1,    NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_DELTAT2,   CTX_DISP2_DELTAT1},
    {NODE_DISP2_DELTAT2,    TEXT_DELTAT2,    NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_MINT1,     CTX_DISP2_DELTAT2},
    {NODE_DISP2_MINT1,      TEXT_MINT1,      NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_MINT2,     CTX_DISP2_MINT1},
    {NODE_DISP2_MINT2,      TEXT_MINT2,      NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_MAXT1,     CTX_DISP2_MINT2},
    {NODE_DISP2_MAXT1,      TEXT_MAXT1,      NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_MAXT2,     CTX_DISP2_MAXT1},
    {NODE_DISP2_MAXT2,      TEXT_MAXT2,      NODE_DISPLAY2,         NODE_NONE,             NODE_DISP2_COUNT1,    CTX_DISP2_MAXT2},
    {NODE_DISP2_COUNT1,     TEXT_COUNT1,     NODE_DISPLAY1,         NODE_NONE,             NODE_DISP2_COUNT2,    CTX_DISP2_COUNT1},
    {NODE_DISP2_COUNT2,     TEXT_COUNT2,     NODE_DISPLAY1,         NODE_NONE,             NODE_NONE,            CTX_DISP2_COUNT2},

    {NODE_CELSIUS,          TEXT_CELSIUS,    NODE_UNITS,            NODE_NONE,             NODE_FAHRENHEIT,      CTX_CELSIUS},
    {NODE_FAHRENHEIT,       TEXT_FAHRENHEIT, NODE_UNITS,            NODE_NONE,             NODE_KELVIN,          CTX_FAHRENHEIT},
    {NODE_KELVIN,           TEXT_KELVIN,     NODE_UNITS,            NODE_NONE,             NODE_NONE,            CTX_KELVIN},

    {NODE_SCALE2,           TEXT_LOW,        NODE_RESOLUTION,       NODE_NONE,             NODE_SCALE3,          CTX_SCALE2},
    {NODE_SCALE3,           TEXT_MEDIUM,     NODE_RESOLUTION,       NODE_NONE,             NODE_SCALE4,          CTX_SCALE3},
    {NODE_SCALE4,           TEXT_HIGH,       NODE_RESOLUTION,       NODE_NONE,             NODE_NONE,            CTX_SCALE4},

    {NODE_BRT_MAIN,         TEXT_BRT_MAIN,   NODE_BRIGHTNESS,       NODE_BRT_MAIN_VALUE,   NODE_BRT_OTHER,       CTX_BRT_MAIN_VALUE},
    {NODE_BRT_OTHER,        TEXT_BRT_OTHER,  NODE_BRIGHTNESS,       NODE_BRT_OTHER_VALUE,  NODE_NONE,            CTX_BRT_OTHER_VALUE},

    {NODE_BRT_MAIN_VALUE,   TEXT_BLANK,      NODE_BRT_MAIN,         NODE_NONE,             NODE_NONE,            CTX_NONE},
    {NODE_BRT_OTHER_VALUE,  TEXT_BLANK,      NODE_BRT_OTHER,        NODE_NONE,             NODE_NONE,            CTX_NONE},

    {NODE_FRONT,            TEXT_FRONT,      NODE_SOCKETS,          NODE_NONE,             NODE_BACK,            CTX_FRONT},
    {NODE_BACK,             TEXT_BACK,       NODE_SOCKETS,          NODE_NONE,             NODE_NONE,            CTX_BACK},

    {NODE_PROBE_A_IDX0,     TEXT_IDX0,       NODE_PROBE_A,          NODE_NONE,             NODE_PROBE_A_IDX1,    CTX_PROBE_A_IDX0},
    {NODE_PROBE_A_IDX1,     TEXT_IDX1,       NODE_PROBE_A,          NODE_NONE,             NODE_PROBE_A_IDX2,    CTX_PROBE_A_IDX1},
    {NODE_PROBE_A_IDX2,     TEXT_IDX2,       NODE_PROBE_A,          NODE_NONE,             NODE_NONE,            CTX_PROBE_A_IDX2},

    {NODE_PROBE_B_IDX0,     TEXT_IDX0,       NODE_PROBE_B,          NODE_NONE,             NODE_PROBE_B_IDX1,    CTX_PROBE_B_IDX0},
    {NODE_PROBE_B_IDX1,     TEXT_IDX1,       NODE_PROBE_B,          NODE_NONE,             NODE_PROBE_B_IDX2,    CTX_PROBE_B_IDX1},
    {NODE_PROBE_B_IDX2,     TEXT_IDX2,       NODE_PROBE_B,          NODE_NONE,             NODE_NONE,            CTX_PROBE_B_IDX2},

    {NODE_TRANSMIT_ON,      TEXT_ON,         NODE_TRANSMIT,         NODE_NONE,             NODE_TRANSMIT_OFF,    CTX_TRANSMIT_ON},
    {NODE_TRANSMIT_OFF,     TEXT_OFF,        NODE_TRANSMIT,         NODE_NONE,             NODE_NONE,            CTX_TRANSMIT_OFF},

    {NODE_SAVE,             TEXT_SAVE,       NODE_SETTINGS,         NODE_NONE,             NODE_DISCARD,         CTX_SETTINGS_SAVE},
    {NODE_DISCARD,          TEXT_DISCARD,    NODE_SETTINGS,         NODE_NONE,             NODE_DEFAULT,         CTX_SETTINGS_DISCARD},
    {NODE_DEFAULT,          TEXT_DEFAULT,    NODE_SETTINGS,         NODE_NONE,             NODE_NONE,            CTX_SETTINGS_DEFAULT},

    {NODE_PROBE_EDIT,       TEXT_EDIT,       NODE_PROBE_DB,         NODE_PROBE_CORE,       NODE_PROBE_SAVE,      CTX_NONE},
    {NODE_PROBE_SAVE,       TEXT_SAVE,       NODE_PROBE_DB,         NODE_NONE,             NODE_PROBE_DISCARD,   CTX_PROBE_SAVE},
    {NODE_PROBE_DISCARD,    TEXT_DISCARD,    NODE_PROBE_DB,         NODE_NONE,             NODE_PROBE_DEFAULT,   CTX_PROBE_DISCARD},
    {NODE_PROBE_DEFAULT,    TEXT_DEFAULT,    NODE_PROBE_DB,         NODE_NONE,             NODE_NONE,            CTX_PROBE_DEFAULT},

    {NODE_PROBE_CORE,       TEXT_CORE,       NODE_PROBE_EDIT,       NODE_PROBE_CORE_IDX,   NODE_PROBE_CF,        CTX_PROBE_CORE_IDX},
    {NODE_PROBE_CORE_IDX,   TEXT_BLANK,      NODE_PROBE_CORE,       NODE_PROBE_CORE_PARAM, NODE_NONE,            CTX_PROBE_CORE_PARAM},
    {NODE_PROBE_CORE_PARAM, TEXT_BLANK,      NODE_PROBE_CORE_IDX,   NODE_PROBE_CORE_VALUE, NODE_NONE,            CTX_PROBE_CORE_VALUE},
    {NODE_PROBE_CORE_VALUE, TEXT_BLANK,      NODE_PROBE_CORE_PARAM, NODE_NONE,             NODE_NONE,            CTX_NONE},

    {NODE_PROBE_CF,         TEXT_CF,         NODE_PROBE_EDIT,       NODE_PROBE_CF_IDX,     NODE_NONE,            CTX_PROBE_CF_IDX},
    {NODE_PROBE_CF_IDX,     TEXT_BLANK,      NODE_PROBE_CF,         NODE_PROBE_CF_PARAM,   NODE_NONE,            CTX_PROBE_CF_PARAM},
    {NODE_PROBE_CF_PARAM,   TEXT_BLANK,      NODE_PROBE_CF_IDX,     NODE_PROBE_CF_VALUE,   NODE_NONE,            CTX_PROBE_CF_VALUE},
    {NODE_PROBE_CF_VALUE,   TEXT_BLANK,      NODE_PROBE_CF_PARAM,   NODE_NONE,             NODE_NONE,            CTX_NONE},
    /*  node_id             text_id          parent                 first child            next sibling          action_id */
};

//_Static_assert(sizeof(menu) / sizeof(menu[0]) == NODE_COUNT, "MenuNode menu[] array size mismatch");
STATIC_ASSERT(sizeof(menu) / sizeof(menu[0]) == NODE_COUNT, menu_size_mismatch);

