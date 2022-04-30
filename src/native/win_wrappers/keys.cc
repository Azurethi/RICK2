#include "keys.hh"

int win_wrappers_keys_register(napi_env env, napi_value exports){
    napi_value fn;

    registerFunction(method_getKeyState, "getKeyState")
    registerFunction(method_getScanCode, "getScanCode")
    
    return 1;
}

napi_value method_getKeyState(napi_env env, napi_callback_info info){
    napi_value args[1];
    size_t argc = 1;

    int32_t key;
    nc(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr), env, "method_getKeyState", "Failed to get argument");
    if(argc != 1) { napi_throw_error(env, "method_getKeyState", "Need one argument"); return nullptr;}
    nc(napi_get_value_int32(env, args[0], &key), env, "method_getKeyState", "Failed to get int32 argument");

    bool pressed = (GetAsyncKeyState(key) == -32767);

    return_int32(env, pressed, "method_getKeyState");
}

napi_value method_getScanCode(napi_env env, napi_callback_info info){
    napi_value args[1];
    size_t argc = 1;

    int32_t key;
    nc(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr), env, "method_getScanCode", "Failed to get argument");
    if(argc != 1) { napi_throw_error(env, "method_getScanCode", "Need one argument"); return nullptr;}
    nc(napi_get_value_int32(env, args[0], &key), env, "method_getScanCode", "Failed to get int32 argument");

    int scancode = MapVirtualKey(LOBYTE(VkKeyScan(key)),0);

    return_int32(env, scancode, "method_getScanCode");
}

