#pragma once
#include <windows.h>
#include <node_api.h>

#include "../helpers.hh"

napi_value method_sendInputs(napi_env env, napi_callback_info info);

int win_wrappers_sendInput_register(napi_env env, napi_value exports);