#ifndef __KEYBOARD_INPUT_H__
#define __KEYBOARD_INPUT_H__

#include <functional>
#include <thread>
#include <array>
#include <assert.h>
#include <iostream>

#include "KeyCode.h"

#ifdef  __APPLE__
#include <ApplicationServices/ApplicationServices.h>
typedef __darwin_::KEY_CODE KEY_CODE;
#elif _WIN32
#define NOMINMAX
#include <windows.h>
typedef __win32_::KEY_CODE KEY_CODE;
#endif


class KeyboardInput {
    typedef KeyboardInput* _Ip;
    typedef KEY_CODE _Key;
    typedef std::function<void(_Key)> _Fp;
    typedef std::array<bool, 256> _Arr;
    
private:
    static _Ip __i_;
    static _Fp __p_;
    static _Fp __r_;
    static _Arr __k_;
    static bool __s_;
    std::thread __t_;
#ifdef __APPLE__
    CFRunLoopRef __l_;
#elif _WIN32
    HHOOK __h_;
    MSG __m_;
#endif
    
private:
    KeyboardInput();
    ~KeyboardInput();

public:
    static _Ip getInstance();
    
    static _Ip setPressCallback(_Fp __f);
    static _Ip setReleaseCallback(_Fp __f);
    _Ip runCallback(bool __det = true);
    _Ip stopCallback();
    
    static bool isRunning();
    static bool isKeyPressed(_Key __key);
    static bool isKeyReleased(_Key __key);
    
private:
#ifdef __APPLE__
    void __init_darwin();
    static CGEventRef __callback_darwin(CGEventTapProxy __p, CGEventType __t, CGEventRef __e, void* __r);
#elif _WIN32
    void __init_win32();
    static LRESULT CALLBACK __callback_win32(int __c, WPARAM __w, LPARAM __l);
#endif
};

#endif /* __KEYBOARD_INPUT_H__ */
