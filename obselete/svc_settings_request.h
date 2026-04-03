/**
 * @file svc_settings_request.h
 * @brief
 */

#ifndef SVC_SETTINGS_REQUEST_H
#define SVC_SETTINGS_REQUEST_H

#include "sys_settings_types.h"

void svc_handle_settings_requests(void);
const Settings * get_requested_settings(void);
Settings * get_requested_settings_mutable(void);

#endif // SVC_SETTINGS_REQUEST_H

