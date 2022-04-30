#pragma once
#include <windows.h>
#include <node_api.h>

#include "../helpers.hh"

napi_value method_getKeyState(napi_env env, napi_callback_info info);
napi_value method_getScanCode(napi_env env, napi_callback_info info);

int win_wrappers_keys_register(napi_env env, napi_value exports);