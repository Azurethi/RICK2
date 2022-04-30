#pragma once
#include <windows.h>
#include <node_api.h>

#include "../helpers.hh"

napi_value method_GetCursorPos(napi_env env, napi_callback_info info);
napi_value method_SetCursorPos(napi_env env, napi_callback_info info);

int win_wrappers_mouse_register(napi_env env, napi_value exports);