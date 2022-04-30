#include "mouse.hh"


int win_wrappers_mouse_register(napi_env env, napi_value exports){
    napi_value fn;

    registerFunction(method_GetCursorPos, "getCursorPos")
    registerFunction(method_SetCursorPos, "setCursorPos")
    
    return 1;
}

napi_value method_GetCursorPos(napi_env env, napi_callback_info info){

    POINT pt = { 0 };
    BOOL success = GetCursorPos(&pt);

    napi_value returnObj;
    nc(napi_create_object(env, &returnObj), env, "method_GetCursorPos", "failed to create object");

    napi_value tmp;
    nc(napi_create_int32(env, pt.x, &tmp), env, "method_GetCursorPos", "Failed to create i32");
    nc(napi_set_named_property(env, returnObj, "x", tmp), env, "method_GetCursorPos", "failed to set obj property");
    nc(napi_create_int32(env, pt.y, &tmp), env, "method_GetCursorPos", "Failed to create i32");
    nc(napi_set_named_property(env, returnObj, "y", tmp), env, "method_GetCursorPos", "failed to set obj property");
    nc(napi_create_int32(env, success, &tmp), env, "method_GetCursorPos", "Failed to create i32");
    nc(napi_set_named_property(env, returnObj, "success", tmp), env, "method_GetCursorPos", "failed to set obj property");

    return returnObj;
}

napi_value method_SetCursorPos(napi_env env, napi_callback_info info){
    napi_value args[2];
    size_t argc = 2;

    int32_t xpos, ypos;
    nc(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr), env, "method_SetCursorPos", "Failed to get argument");
    if(argc != 2) { napi_throw_error(env, "method_SetCursorPos", "Need two arguments"); return nullptr;}
    nc(napi_get_value_int32(env, args[0], &xpos), env, "method_SetCursorPos", "Failed to get int32 argument");
    nc(napi_get_value_int32(env, args[1], &ypos), env, "method_SetCursorPos", "Failed to get int32 argument");

    BOOL success = SetCursorPos(xpos, ypos);

    return_int32(env, success, "method_SetCursorPos");
}