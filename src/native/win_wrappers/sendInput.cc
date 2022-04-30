#include "sendInput.hh"

int win_wrappers_sendInput_register(napi_env env, napi_value exports){

    napi_value fn;
    registerFunction(method_sendInputs, "sendInputs")


    //input types
    registerConstant(env, fn, INPUT_KEYBOARD,           "TYPE_KEYBOARD"     );
    registerConstant(env, fn, INPUT_MOUSE,              "TYPE_MOUSE"        );

    //keyboard flags
    registerConstant(env, fn, KEYEVENTF_EXTENDEDKEY,    "KFLAG_EXTENDEDKEY" );
    registerConstant(env, fn, KEYEVENTF_KEYUP,          "KFLAG_KEYUP"       );
    registerConstant(env, fn, KEYEVENTF_UNICODE,        "KFLAG_UNICODE"     );
    registerConstant(env, fn, KEYEVENTF_SCANCODE,       "KFLAG_SCANCODE"    );

    //mouse flags
    registerConstant(env, fn, MOUSEEVENTF_MOVE,         "MFLAG_MOVE"        );
    registerConstant(env, fn, MOUSEEVENTF_ABSOLUTE,     "MFLAG_ABSOLUTE"    );
    registerConstant(env, fn, MOUSEEVENTF_LEFTDOWN,     "MFLAG_LEFTDOWN"    );
    registerConstant(env, fn, MOUSEEVENTF_LEFTUP,       "MFLAG_LEFTUP"      );
    registerConstant(env, fn, MOUSEEVENTF_RIGHTDOWN,    "MFLAG_RIGHTDOWN"   );
    registerConstant(env, fn, MOUSEEVENTF_RIGHTUP,      "MFLAG_RIGHTUP"     );
    registerConstant(env, fn, MOUSEEVENTF_MIDDLEDOWN,   "MFLAG_MIDDLEDOWN"  );
    registerConstant(env, fn, MOUSEEVENTF_MIDDLEUP,     "MFLAG_MIDDLEUP"    );

    return 1;
}

napi_value method_sendInputs(napi_env env, napi_callback_info info){
    napi_value args[1];
    size_t argc = 1;
    nc(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr), env, "method_sendInputs", "Failed to get argument");
    if(argc != 1) { napi_throw_error(env, "method_sendInputs", "Need one argument"); return nullptr;}

    bool gotArray;
    nc(napi_is_array(env, args[0], &gotArray), env, "method_sendInputs", "Failed to check if array");

    if(!gotArray){
        napi_throw_error(env, "method_sendInputs", "Need one argument to be array");
        return nullptr;
    }

    uint32_t arrlen;
    napi_get_array_length(env, args[0], &arrlen);

    UINT totsent = 0;

    for(uint32_t i=0; i<arrlen; i++){
        INPUT ip = { 0 };

        napi_value elem;
        napi_get_element(env, args[0], i, &elem);

        //assign properties for keyboard or mouse events
        bool hadProp;
        assignPropIfHas(env, hadProp, elem, "type",      ip.type,        "method_sendInputs");
        if(ip.type == INPUT_KEYBOARD){
            assignPropIfHas(env, hadProp, elem, "flags",     ip.ki.dwFlags,  "method_sendInputs");
            assignPropIfHas(env, hadProp, elem, "keycode",   ip.ki.wVk,      "method_sendInputs");
            assignPropIfHas(env, hadProp, elem, "scancode",  ip.ki.wScan,    "method_sendInputs");
        } else if(ip.type == INPUT_MOUSE){
            assignPropIfHas(env, hadProp, elem, "dx",        ip.mi.dx,       "method_sendInputs");
            assignPropIfHas(env, hadProp, elem, "dy",        ip.mi.dy,       "method_sendInputs");
            assignPropIfHas(env, hadProp, elem, "flags",     ip.mi.dwFlags,  "method_sendInputs");
        }
        
        totsent+=SendInput(1, &ip, sizeof(INPUT));
    }

    return_int32(env, totsent, "method_sendInputs");
}

