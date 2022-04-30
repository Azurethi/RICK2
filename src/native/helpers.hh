#pragma once

#define nc(call, env, errcode, errmsg) /*napi call*/    \
    if (call != napi_ok) {                              \
        napi_throw_error(env, errcode, errmsg);         \
        return 0;                                       \
    }

#define registerFunction(method, name)                                                                              \
    nc(napi_create_function(env, nullptr, 0, method, nullptr, &fn), env, name, "Failed to create function");        \
    nc(napi_set_named_property(env, exports, name, fn), env, name, "Failed to set exports property to function");   

#define registerConstant(env, val, value, name)                                                                     \
    nc(napi_create_int32(env, value, &val), env, name, "Failed to create constant int");                            \
    nc(napi_set_named_property(env, exports, name, val), env, name, "Failed to set exports property to function");

#define return_int32(env, value, methodName)                                                    \
    napi_value rv;                                                                              \
    nc(napi_create_int32(env, value, &rv), env, methodName, "Failed to create return value");   \
    return rv;

#define assignPropIfHas(env, hasProp, obj, prop, assign,  methodName)                                               \
        nc(napi_has_named_property(env, obj, prop, &hasProp), env, methodName, "Failed to check property");         \
        if(hasProp){                                                                                                \
            napi_value v;                                                                                           \
            nc(napi_get_named_property(env, obj, prop, &v), env, methodName, "Failed to get property");             \
            int32_t value;                                                                                          \
            nc(napi_get_value_int32(env, v, &value), env, methodName, "failed to get property value");              \
                                                                                                                    \
            assign = value;                                                                                         \
        }