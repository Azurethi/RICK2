#include "clipboard.hh"



int win_wrappers_clipboard_register(napi_env env, napi_value exports){
    napi_value fn;
    registerFunction(method_setClipboard,      "setClipboard");
    
    return 1;
}

napi_value method_setClipboard(napi_env env, napi_callback_info info){
    napi_value args[1];
    size_t argc = 1;

    char16_t buffer[2048] = { 0 };
    size_t size = 0;

    nc(napi_get_cb_info(env, info, &argc, args, nullptr, nullptr), env, "method_setClipboard", "Failed to get argument");
    if(argc != 1) { napi_throw_error(env, "method_setClipboard", "Need one argument"); return nullptr;}
    nc(napi_get_value_string_utf16(env, args[0], buffer, 2048, &size), env, "method_setClipboard", "Failed to get argument value");

    if(!OpenClipboard(NULL)) {
        napi_throw_error(env, "method_setClipboard", "Couldn't open clipboard"); 
        return nullptr;
    }
    
    HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, (size+1) * sizeof(char16_t));
    if (hglbCopy == NULL) {
        CloseClipboard();
        napi_throw_error(env, "method_setClipboard", "Couldn't GlobalAlloc");
        return nullptr; 
    }

    char16_t* globalBuf = (char16_t*) GlobalLock(hglbCopy);
    memcpy(globalBuf, buffer, size * sizeof(char16_t));
    globalBuf[size] = 0;    //null term
    GlobalUnlock(hglbCopy);

    EmptyClipboard();
    SetClipboardData(CF_UNICODETEXT, hglbCopy);
    CloseClipboard();

    return_int32(env, 1, "method_setClipboard");
}