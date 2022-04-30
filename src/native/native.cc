#include "native.hh"

napi_value init(napi_env env, napi_value exports) {

    //win_wrappers > mouse
    win_wrappers_mouse_register(env, exports);

    //win_wrappers > clipboard
    win_wrappers_clipboard_register(env, exports);

    //win_wrappers > keys
    win_wrappers_keys_register(env, exports);

    //win_wrapers > sendInput
    win_wrappers_sendInput_register(env, exports);

    return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, init)
