/***************************************

    Game Application startup class

    Copyright (c) 1995-2017 by Rebecca Ann Heineman <becky@burgerbecky.com>

    It is released under an MIT Open Source license. Please see LICENSE for
    license details. Yes, you can use it in a commercial title without paying
    anything, just give me a credit.

    Please? It's not like I'm asking you for money!

***************************************/

#ifndef __BRGAMEAPP_H__
#define __BRGAMEAPP_H__

#ifndef __BRTYPES_H__
#include "brtypes.h"
#endif

#ifndef __BRBASE_H__
#include "brbase.h"
#endif

#ifndef __BRMEMORYHANDLE_H__
#include "brmemoryhandle.h"
#endif

#ifndef __BRRUNQUEUE_H__
#include "brrunqueue.h"
#endif

#ifndef __BRRECT_H__
#include "brrect.h"
#endif

#if defined(BURGER_WINDOWS) && !defined(__BRWINDOWSTYPES_H__)
#include "brwindowstypes.h"
#endif

#if defined(BURGER_IOS) && !defined(__BRIOSTYPES_H__)
#include "briostypes.h"
#endif

#if defined(BURGER_MACOSX) && !defined(__BRMACOSXTYPES_H__)
#include "brmacosxtypes.h"
#endif

#if defined(BURGER_XBOX360) && !defined(__BRXBOX360TYPES_H__)
#include "brxbox360types.h"
#endif

/* BEGIN */
namespace Burger {
class Keyboard;
class Joypad;
class Mouse;
class SoundManager;
class Display;
class Renderer;

enum eEvent {
    EVENT_NULL,          ///< Keyboard null event
    EVENT_TIMER,         ///< Keyboard timer event
    EVENT_KEYDOWN,       ///< Keyboard keypress event
    EVENT_KEYUP,         ///< Keyboard key up event
    EVENT_KEYAUTO,       ///< Keyboard repeated key event
    EVENT_MOUSEDOWN,     ///< Mouse button down event
    EVENT_MOUSEUP,       ///< Mouse button up event
    EVENT_MOUSEMOVE,     ///< Mouse motion event
    EVENT_MOUSEPOSITION, ///< Mouse position event
    EVENT_MOUSEWHEEL,    ///< Mouse wheel event
    EVENT_JOYPADDOWN,    ///< Joypad button press
    EVENT_JOYPADUP,      ///< Joypad button release
    EVENT_JOYPADAXIS,    ///< Joypad axis motion
    EVENT_COUNT          ///< Maximum event code
};

struct EventHeader_t {
    eEvent m_uEvent;         ///< Event type
    uint_t m_uWhich;         ///< Which device generated the event
    uint32_t m_uMSTimeStamp; ///< Time when the event occurred
};

struct KeyEvent_t: public EventHeader_t {
    uint32_t m_uAscii;    ///< Unicode ASCII value
    uint32_t m_uFlags;    ///< Flags for key modifiers
    uint32_t m_uScanCode; ///< Scan code of the key
};

struct MouseButtonEvent_t: public EventHeader_t {
    uint32_t m_uX;       ///< Absolute X position on the screen
    uint32_t m_uY;       ///< Absolute Y position on the screen
    uint32_t m_uButtons; ///< Mouse buttons
};

struct MouseWheelEvent_t: public EventHeader_t {
    /** Horizontal mouse wheel motion (Apple ball mice support this) */
    int32_t m_iMouseWheelX;
    /** Vertical mouse wheel motion (Most mice support this) */
    int32_t m_iMouseWheelY;
};

struct MouseMotionEvent_t: public EventHeader_t {
    int32_t m_iDeltaX; ///< Relative X motion
    int32_t m_iDeltaY; ///< Relative Y motion
};

struct MousePositionEvent_t: public EventHeader_t {
    uint32_t m_uX; ///< Absolute X position on the screen
    uint32_t m_uY; ///< Absolute Y position on the screen
};

struct JoypadButtonEvent_t: public EventHeader_t {
    uint_t m_uButtons; ///< Pressed or released joypad buttons
};

class GameApp: public Base {
    BURGER_DISABLE_COPY(GameApp);
    BURGER_RTTI_IN_CLASS();

protected:
    Keyboard* m_pKeyboard; ///< Pointer to the active keyboard instance
    Mouse* m_pMouse;       ///< Pointer to the active mouse instance
    Joypad* m_pJoypad;     ///< Pointer to the active joypad instance
    /** Pointer to the active sound manager instance */
    SoundManager* m_pSoundManager;
    Display* m_pDisplay;   ///< Pointer to the active display instance
    const char** m_ppArgv; ///< Arguments passed to the console
    /** Handle based memory manager for the app */
    MemoryManagerGlobalHandle m_MemoryManagerHandle;
    RunQueue m_RunQueue; ///< Internal run queue

#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
public:
    typedef uint_t(BURGER_API* MainWindowProc)(GameApp* pThis, HWND__* hWnd,
        uint_t uMessage, uintptr_t wParam, uintptr_t lParam,
        uintptr_t* pOutput); ///< Callback custom window message handler
protected:
    /** (Windows only) Current instance of the application */
    HINSTANCE__* m_hInstance;
    HWND__* m_hWindow; ///< (Windows only) Main window for the application
    HMENU__* m_hMenu;  ///< (Windows only) Menu bar for the application
    /** (Windows only) Default cursor when not in the game area */
    HICON__* m_pDefaultCursor;
    /** (Windows only) Saved pointer to the global __argv for restoring on exit
     */
    const char** m_ppOldArgv;
    /** (Windows only) Custom application window proc */
    MainWindowProc m_pCallBack;
    Rect_t m_WindowRect; ///< (Windows only) Bounds rect of the window
    uint_t m_uErrorMode; ///< (Windows only) Initial GetErrorMode() value
    /** (Windows only) \ref TRUE if CoInitializeEx() was initialized */
    uint8_t m_bCoCreateInstanceInit;
    /** (Windows only) \ref TRUE if the window is being resized */
    uint8_t m_bInSizeMove;

#endif

#if defined(BURGER_IOS) || defined(DOXYGEN)
    /** (iOS only) Pointer to the UIWindow for the iOS application */
    UIWindow* m_pWindow;
    /** (iOS only) Pointer to the UIViewController to the window */
    UIViewController* m_pViewController;
    /** (iOS only) Pointer to the UIView being used */
    UIView* m_pView;
    /** (iOS only) Global singular application instance */
    static GameApp* g_piOSApp;
#endif

#if defined(BURGER_MACOSX) || defined(DOXYGEN)
    /** Pointer to the current application context (Copy of NSApp) */
    NSApplication* m_pApplication;
    /** Pointer to the current application delegate */
    NSApplicationDelegate* m_pApplicationDelegate;
    NSWindow* m_pWindow;      ///< Game window
    NSResponder* m_pListener; ///< Main listener for window events
    uint_t m_bCenterWindow;   ///< \ref TRUE if the window needs to be centered
    static RunQueue::eReturnCode BURGER_API EventPoll(void* pData);
#endif

    int m_iArgc;     ///< Number of valid arguments
    uint8_t m_bQuit; ///< Set to \ref TRUE when the application has to shut down
    /** Set to \ref TRUE if the game should be paused due to another window
     being in front */
    uint8_t m_bInBackground;
    /** Set to \ref TRUE if the game is minimized (PC/Mac only, always false on
     consoles) */
    uint8_t m_bMinimized;
    /* Set to \ref TRUE if the game allows switching to windowed mode via
     Alt-Enter on desktop/laptops */
    uint8_t m_bAllowWindowSwitching;
    /** \ref TRUE if the mouse cursor is on the screen in windows systems */
    uint8_t m_bMouseOnScreen;
    /** \ref TRUE if the window resolution switch key was pressed */
    uint8_t m_bWindowSwitchRequested;
    /** \ref TRUE if a \ref Keyboard class was started with a call to
     StartupKeyboard(void) */
    uint8_t m_bKeyboardStarted;
    /** \ref TRUE if a \ref Mouse class was started with a call to
     StartupMouse(void) */
    uint8_t m_bMouseStarted;
    /** \ref TRUE if a \ref Joypad class was started with a call to
      StartupJoypad(void) */
    uint8_t m_bJoypadStarted;
    /** \ref TRUE if a \ref Display class was started with a call to
      StartupDisplay(void) */
    uint8_t m_bDisplayStarted;
    /** \ref TRUE if a \ref SoundManager class was started with a call to
     StartupSoundManager(void) */
    uint8_t m_bSoundManagerStarted;

    void BURGER_API InitDefaults(void);
    void BURGER_API ShutdownDefaults(void);

public:
    GameApp(uintptr_t uDefaultMemorySize =
                Burger::MemoryManagerHandle::DEFAULTMEMORYCHUNK,
        uint_t uDefaultHandleCount =
            Burger::MemoryManagerHandle::DEFAULTHANDLECOUNT,
        uintptr_t uMinReserveSize =
            Burger::MemoryManagerHandle::DEFAULTMINIMUMRESERVE);
    virtual ~GameApp();

    BURGER_INLINE void Poll(void)
    {
        m_RunQueue.Call();
    };
    BURGER_INLINE void AddRoutine(RunQueue::CallbackProc Proc,
        RunQueue::CallbackProc pShutdown, void* pData,
        uint_t uPriority = RunQueue::PRIORITY_MEDIUM)
    {
        m_RunQueue.Add(Proc, pShutdown, pData, uPriority);
    }
    BURGER_INLINE void RemoveRoutine(RunQueue::CallbackProc Proc, void* pData)
    {
        m_RunQueue.Remove(Proc, pData);
    }
    BURGER_INLINE RunQueue* GetRunQueue(void) BURGER_NOEXCEPT
    {
        return &m_RunQueue;
    }

    Keyboard* BURGER_API StartupKeyboard(void);
    Mouse* BURGER_API StartupMouse(void);
    Joypad* BURGER_API StartupJoypad(void);
    Display* BURGER_API StartupDisplay(void);
    SoundManager* BURGER_API StartupSoundManager(void);
    uint_t BURGER_API StartupEverything(void);

    BURGER_INLINE MemoryManagerGlobalHandle* GetMemoryManager(
        void) BURGER_NOEXCEPT
    {
        return &m_MemoryManagerHandle;
    }

    BURGER_INLINE void SetArgv(const char** ppArgv) BURGER_NOEXCEPT
    {
        m_ppArgv = ppArgv;
    }
    BURGER_INLINE const char** GetArgv(void) const BURGER_NOEXCEPT
    {
        return m_ppArgv;
    }
    BURGER_INLINE void SetArgc(int iArgc) BURGER_NOEXCEPT
    {
        m_iArgc = iArgc;
    }
    BURGER_INLINE int GetArgc(void) const BURGER_NOEXCEPT
    {
        return m_iArgc;
    }

    BURGER_INLINE void SetQuitCode(void) BURGER_NOEXCEPT
    {
        m_bQuit = TRUE;
    }
    BURGER_INLINE void ClearQuitCode(void) BURGER_NOEXCEPT
    {
        m_bQuit = FALSE;
    }
    BURGER_INLINE uint_t GetQuitCode(void) const BURGER_NOEXCEPT
    {
        return m_bQuit;
    }

    BURGER_INLINE void SetInBackground(uint_t bInBackground) BURGER_NOEXCEPT
    {
        m_bInBackground = (bInBackground != 0);
    }
    BURGER_INLINE uint_t IsInBackground(void) const BURGER_NOEXCEPT
    {
        return m_bInBackground;
    }

    void BURGER_API SetKeyboard(Keyboard* pKeyboard);
    BURGER_INLINE Keyboard* GetKeyboard(void) const BURGER_NOEXCEPT
    {
        return m_pKeyboard;
    }

    void BURGER_API SetMouse(Mouse* pMouse);
    BURGER_INLINE Mouse* GetMouse(void) const BURGER_NOEXCEPT
    {
        return m_pMouse;
    }

    void BURGER_API SetJoypad(Joypad* pJoypad);
    BURGER_INLINE Joypad* GetJoypad(void) const BURGER_NOEXCEPT
    {
        return m_pJoypad;
    }

    void BURGER_API SetSoundManager(SoundManager* pSoundManager);
    BURGER_INLINE SoundManager* GetSoundManager(void) const BURGER_NOEXCEPT
    {
        return m_pSoundManager;
    }

    void BURGER_API SetDisplay(Display* pDisplay);
    BURGER_INLINE Display* GetDisplay(void) const BURGER_NOEXCEPT
    {
        return m_pDisplay;
    }

    BURGER_INLINE void SetWindowSwitching(
        uint_t bAllowWindowSwitching) BURGER_NOEXCEPT
    {
        m_bAllowWindowSwitching = (bAllowWindowSwitching != 0);
    }
    BURGER_INLINE uint_t IsWindowSwitchingAllowed(void) const BURGER_NOEXCEPT
    {
        return m_bAllowWindowSwitching;
    }
    uint_t BURGER_API SwitchVideo(void);

    BURGER_INLINE uint_t IsMinimized(void) const BURGER_NOEXCEPT
    {
        return m_bMinimized;
    }
    BURGER_INLINE void SetMinimized(uint_t bMinimized) BURGER_NOEXCEPT
    {
        m_bMinimized = (bMinimized != 0);
    }

    uint_t BURGER_API IsAppFullScreen(void) const;
    uint_t BURGER_API IsResizingAllowed(void) const;

    BURGER_INLINE void SetMouseOnScreen(uint_t bMouseOnScreen) BURGER_NOEXCEPT
    {
        m_bMouseOnScreen = (bMouseOnScreen != 0);
    }
    BURGER_INLINE uint_t IsMouseOnScreen(void) const BURGER_NOEXCEPT
    {
        return m_bMouseOnScreen;
    }

    BURGER_INLINE void SetWindowSwitchRequested(
        uint_t bWindowSwitchRequested) BURGER_NOEXCEPT
    {
        m_bWindowSwitchRequested = (bWindowSwitchRequested != 0);
    }
    BURGER_INLINE uint_t IsWindowSwitchRequested(void) const BURGER_NOEXCEPT
    {
        return m_bWindowSwitchRequested;
    }

#if defined(BURGER_WINDOWS) || defined(DOXYGEN)
    BURGER_INLINE HINSTANCE__* GetInstance(void) const BURGER_NOEXCEPT
    {
        return m_hInstance;
    }
    BURGER_INLINE HWND__* GetWindow(void) const BURGER_NOEXCEPT
    {
        return m_hWindow;
    }
    BURGER_INLINE HMENU__* GetMenu(void) const BURGER_NOEXCEPT
    {
        return m_hMenu;
    }
    BURGER_INLINE void SetMenu(HMENU__* hMenu) BURGER_NOEXCEPT
    {
        m_hMenu = hMenu;
    }
    BURGER_INLINE MainWindowProc GetCallBack(void) const BURGER_NOEXCEPT
    {
        return m_pCallBack;
    }
    BURGER_INLINE uint_t GetInSizeMove(void) const BURGER_NOEXCEPT
    {
        return m_bInSizeMove;
    }
    BURGER_INLINE void SetInSizeMove(uint_t bInSizeMode) BURGER_NOEXCEPT
    {
        m_bInSizeMove = (bInSizeMode != 0);
    }
    int BURGER_API InitWindow(
        const char* pGameName, MainWindowProc pCallBack, uint_t uIconResID = 0);
    int BURGER_API SetWindowSize(uint_t uWidth, uint_t uHeight);
    int BURGER_API SetWindowFullScreen(uint_t uWidth, uint_t uHeight);
    void BURGER_API ResetWindowLocation(void);
    void BURGER_API RecordWindowLocation(void);
    uint_t BURGER_API HandleCursor(uint_t uParam);
    void BURGER_API GetInputFocus(void);
    void BURGER_API KillInputFocus(void);
    static RunQueue::eReturnCode BURGER_API Poll(void* pSelf);
#endif

#if defined(BURGER_IOS) || defined(DOXYGEN)
    int BURGER_API Run(void);
    UIWindow* BURGER_API CreateWindow(void);
    void BURGER_API ReleaseWindow(void);
    void BURGER_API SetViewController(
        UIViewController* pViewController, UIView* pView);
    BURGER_INLINE UIWindow* GetWindow(void) const BURGER_NOEXCEPT
    {
        return m_pWindow;
    }
    BURGER_INLINE UIViewController* GetViewController(
        void) const BURGER_NOEXCEPT
    {
        return m_pViewController;
    }
    BURGER_INLINE UIView* GetView(void) const BURGER_NOEXCEPT
    {
        return m_pView;
    }
    BURGER_INLINE static GameApp* GetApp(void) BURGER_NOEXCEPT
    {
        return g_piOSApp;
    }
    static RunQueue::eReturnCode BURGER_API Poll(void* pSelf);
#endif

#if defined(BURGER_MACOSX) || defined(DOXYGEN)
    void BURGER_API FocusWindow(void);
    uint_t BURGER_API SetWindowSize(uint_t uWidth, uint_t uHeight);
    BURGER_INLINE NSApplication* GetApplication(void) const BURGER_NOEXCEPT
    {
        return m_pApplication;
    }
    BURGER_INLINE NSApplicationDelegate* GetDelegate(void) const BURGER_NOEXCEPT
    {
        return m_pApplicationDelegate;
    }
    BURGER_INLINE NSWindow* GetWindow(void) const BURGER_NOEXCEPT
    {
        return m_pWindow;
    }
#endif
};
}

extern int BURGER_API CodeEntry(Burger::GameApp* pGameApp);

/* END */

#endif
