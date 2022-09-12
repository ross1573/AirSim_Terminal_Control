#include "KeyboardInput.h"


KeyboardInput* KeyboardInput::__i_ = nullptr;
KeyboardInput::_Fp KeyboardInput::__p_ = nullptr;
KeyboardInput::_Fp KeyboardInput::__r_ = nullptr;
KeyboardInput::_Kt KeyboardInput::__k_;
bool KeyboardInput::__s_ = false;


KeyboardInput::KeyboardInput() {
    assert(!__i_);
    __k_.fill(false);
}

KeyboardInput::~KeyboardInput() {
    if (__i_) {
        delete __i_;
        __i_ = nullptr;
    }
}

KeyboardInput* KeyboardInput::getInstance() {
    if (!__i_) __i_ = new KeyboardInput();
    return __i_;
}


KeyboardInput* KeyboardInput::runCallback(bool __det) {
#ifdef __APPLE__
    __t = std::thread(&KeyboardInput::__init_darwin, __i_);
#elif _WIN32
    __t = std::thread(&KeyboardInput::__init_win32, __i_);
#endif
    __s_ = true;
    if (__det) __t.detach();
    else __t.join();
    return __i_;
}

KeyboardInput* KeyboardInput::setPressCallback(_Fp __p) {
    getInstance()->__p_ = __p;
    return __i_;
}

KeyboardInput* KeyboardInput::setReleaseCallback(_Fp __f) {
    getInstance()->__r_ = __f;
    return __i_;
}


KeyboardInput* KeyboardInput::stopCallback() {
#ifdef __APPLE__
    CFRunLoopStop(CFRunLoopGetCurrent());
    pthread_cancel(__t.native_handle());
#endif
    __s_ = false;
    return getInstance();
}


bool KeyboardInput::isRunning() {
    return __s_;
}

bool KeyboardInput::isKeyPressed(KEY_CODE __key) {
    return __k_[__key];
}

bool KeyboardInput::isKeyReleased(KEY_CODE __key) {
    return !__k_[__key];
}


#ifdef __APPLE__
void KeyboardInput::__init_darwin() {
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, nullptr);
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, nullptr);
    CFMachPortRef __e = CGEventTapCreate(kCGSessionEventTap,
                                         kCGHeadInsertEventTap,
                                         kCGEventTapOptionDefault,
                                         CGEventMaskBit(kCGEventKeyDown) | CGEventMaskBit(kCGEventKeyUp),
                                         &KeyboardInput::__callback_darwin,
                                         NULL);
    
    if (!__e) {
        fprintf(stderr, "Failed to create event tap\n");
        exit(1);
    }
    
    CFRunLoopSourceRef __l = CFMachPortCreateRunLoopSource(kCFAllocatorDefault, __e, 0);
    CFRunLoopAddSource(CFRunLoopGetCurrent(), __l, kCFRunLoopCommonModes);
    CGEventTapEnable(__e, true);
    CFRunLoopRun();
}

CGEventRef KeyboardInput::__callback_darwin(CGEventTapProxy __p, CGEventType __t, CGEventRef __e, void* __r) {
    if ((__t != kCGEventKeyDown) && (__t != kCGEventKeyUp)) return __e;
    CGKeyCode __k = (CGKeyCode)CGEventGetIntegerValueField(__e, kCGKeyboardEventKeycode);
    if (__t == kCGEventKeyDown) {
        __k_[__k] = true;
        if (__p_) __p_((KEY_CODE)__k);
    }
    else if (__t == kCGEventKeyUp) {
        __k_[__k] = false;
        if (__r_) __r_((KEY_CODE)__k);
    }
    return __e;
}
#elif _WIN32
void KeyboardInput::__init_win32() {
    __h_ = SetWindowsHookEx(WH_KEYBOARD_LL, &KeyboardInput::__callback_win32, NULL, 0);
    if (!__h_) {
        std::cout << "Failed to create hook : " << GetLastError();
        return;
    }
    while (!GetMessage(&__m_, NULL, 0, 0) && __s_) {
        TranslateMessage(&__m_);
        DispatchMessage(&__m_);
    }
    UnhookWindowsHookEx(__h_);
}

LRESULT CALLBACK KeyboardInput::__callback_win32(int __c, WPARAM __w, LPARAM __l) {
    if (__c != HC_ACTION) return CallNextHookEx(NULL, __c, __w, __l);
    auto __k = ((PKBDLLHOOKSTRUCT)__l)->vkCode;
    if (__w == WM_KEYDOWN) {
        __k_[__k] = true;
        if (__p_) __p_((KEY_CODE)__k);
    }
    else if (__w == WM_KEYUP) {
        __k_[__k] = false;
        if (__r_) __r_((KEY_CODE)__k);
    }
    
    return CallNextHookEx(NULL, __c, __w,  __l);
}
#endif
