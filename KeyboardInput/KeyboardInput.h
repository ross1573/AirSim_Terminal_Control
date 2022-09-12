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
#include <windows.h>
typedef __win32_::KEY_CODE KEY_CODE;
#endif


class KeyboardInput {
    typedef std::function<void(KEY_CODE)> _Fp;
    typedef std::array<bool, 256> _Kt;
    
private:
    static KeyboardInput* __i_;
    static _Fp __p_;
    static _Fp __r_;
    static _Kt __k_;
    static bool __s_;
    std::thread __t;
#ifdef _WIN32
    HHOOK __h_;
    MSG __m_;
#endif
    
private:
    KeyboardInput();
    ~KeyboardInput();

public:
    static KeyboardInput* getInstance();
    
    static KeyboardInput* setPressCallback(_Fp __f);
    static KeyboardInput* setReleaseCallback(_Fp __f);
    KeyboardInput* runCallback(bool __det = true);
    KeyboardInput* stopCallback();
    
    static bool isRunning();
    static bool isKeyPressed(KEY_CODE __key);
    static bool isKeyReleased(KEY_CODE __key);
    
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
