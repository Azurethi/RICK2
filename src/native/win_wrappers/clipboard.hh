#pragma once
#include <windows.h>
#include <node_api.h>

#include "../helpers.hh"

napi_value method_setClipboard(napi_env env, napi_callback_info info);

int win_wrappers_clipboard_register(napi_env env, napi_value exports);