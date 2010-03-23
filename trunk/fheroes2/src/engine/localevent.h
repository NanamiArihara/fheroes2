/***************************************************************************
 *   Copyright (C) 2006 by Andrey Afletdinov <fheroes2@gmail.com>          *
 *   Copyright (C) 2008 by Josh Matthews <josh@joshmatthews.net>           *
 *                                                                         *
 *   Part of the Free Heroes2 Engine:                                      *
 *   http://sourceforge.net/projects/fheroes2                              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef H2LOCALEVENT_H
#define H2LOCALEVENT_H

#include "rect.h"
#include "thread.h"
#include "types.h"

class Point;
class Rect;

struct SDL_MouseMotionEvent;
struct SDL_MouseButtonEvent;
union SDL_Event;
struct SDL_keysym;

enum KeyMod { MOD_NONE = KMOD_NONE, MOD_CTRL = KMOD_CTRL, MOD_SHIFT = KMOD_SHIFT, MOD_ALT = KMOD_ALT, MOD_CAPS = KMOD_CAPS };

enum KeySym
{
    KEY_NONE		= SDLK_UNKNOWN,

    KEY_BACKSPACE	= SDLK_BACKSPACE,
    KEY_RETURN		= SDLK_RETURN,
    KEY_ESCAPE		= SDLK_ESCAPE,
    KEY_SPACE		= SDLK_SPACE,
    KEY_EXCLAIM		= SDLK_EXCLAIM,
    KEY_QUOTEDBL	= SDLK_QUOTEDBL,
    KEY_HASH		= SDLK_HASH,
    KEY_DOLLAR		= SDLK_DOLLAR,
    KEY_AMPERSAND	= SDLK_AMPERSAND,
    KEY_QUOTE		= SDLK_QUOTE,
    KEY_LEFTPAREN	= SDLK_LEFTPAREN,
    KEY_RIGHTPAREN	= SDLK_RIGHTPAREN,
    KEY_ASTERISK	= SDLK_ASTERISK,
    KEY_PLUS		= SDLK_PLUS,
    KEY_COMMA		= SDLK_COMMA,
    KEY_MINUS		= SDLK_MINUS,
    KEY_PERIOD		= SDLK_PERIOD,
    KEY_SLASH		= SDLK_SLASH,
    KEY_COLON		= SDLK_COLON,
    KEY_SEMICOLON	= SDLK_SEMICOLON,
    KEY_LESS		= SDLK_LESS,
    KEY_EQUALS		= SDLK_EQUALS,
    KEY_GREATER		= SDLK_GREATER,
    KEY_QUESTION	= SDLK_QUESTION,
    KEY_AT		= SDLK_AT,
    KEY_LEFTBRACKET	= SDLK_LEFTBRACKET,
    KEY_BACKSLASH	= SDLK_BACKSLASH,
    KEY_RIGHTBRACKET	= SDLK_RIGHTBRACKET,
    KEY_CARET		= SDLK_CARET,
    KEY_UNDERSCORE	= SDLK_UNDERSCORE,
    KEY_ALT		= SDLK_LALT,
    KEY_CONTROL		= SDLK_LCTRL,
    KEY_SHIFT		= SDLK_LSHIFT,
    KEY_DELETE		= SDLK_DELETE,
    KEY_PAGEUP		= SDLK_PAGEUP,
    KEY_PAGEDOWN	= SDLK_PAGEDOWN,
    KEY_F1		= SDLK_F1,
    KEY_F2		= SDLK_F2,
    KEY_F3		= SDLK_F3,
    KEY_F4		= SDLK_F4,
    KEY_F5		= SDLK_F5,
    KEY_F6		= SDLK_F6,
    KEY_F7		= SDLK_F7,
    KEY_F8		= SDLK_F8,
    KEY_F9		= SDLK_F9,
    KEY_F10		= SDLK_F10,
    KEY_F11		= SDLK_F11,
    KEY_F12		= SDLK_F12,
    KEY_PRINT		= SDLK_PRINT,
    KEY_LEFT		= SDLK_LEFT,
    KEY_RIGHT		= SDLK_RIGHT,
    KEY_UP		= SDLK_UP,
    KEY_DOWN		= SDLK_DOWN,
    KEY_0		= SDLK_0,
    KEY_1		= SDLK_1,
    KEY_2		= SDLK_2,
    KEY_3		= SDLK_3,
    KEY_4		= SDLK_4,
    KEY_5		= SDLK_5,
    KEY_6		= SDLK_6,
    KEY_7		= SDLK_7,
    KEY_8		= SDLK_8,
    KEY_9		= SDLK_9,
    KEY_a		= SDLK_a,
    KEY_b		= SDLK_b,
    KEY_c		= SDLK_c,
    KEY_d		= SDLK_d,
    KEY_e		= SDLK_e,
    KEY_f		= SDLK_f,
    KEY_g		= SDLK_g,
    KEY_h		= SDLK_h,
    KEY_i		= SDLK_i,
    KEY_j		= SDLK_j,
    KEY_k		= SDLK_k,
    KEY_l		= SDLK_l,
    KEY_m		= SDLK_m,
    KEY_n		= SDLK_n,
    KEY_o		= SDLK_o,
    KEY_p		= SDLK_p,
    KEY_q		= SDLK_q,
    KEY_r		= SDLK_r,
    KEY_s		= SDLK_s,
    KEY_t		= SDLK_t,
    KEY_u		= SDLK_u,
    KEY_v		= SDLK_v,
    KEY_w		= SDLK_w,
    KEY_x		= SDLK_x,
    KEY_y		= SDLK_y,
    KEY_z		= SDLK_z,

#ifdef _WIN32_WCE
    KEY_APP01		= 0xC1,
    KEY_APP02		= 0xC2,
    KEY_APP03		= 0xC3,
    KEY_APP04		= 0xC4,
    KEY_APP05		= 0xC5,
    KEY_APP06		= 0xC6,
    KEY_APP07		= 0xC7,
    KEY_APP08		= 0xC8,
    KEY_APP09		= 0xC9,
    KEY_APP10		= 0xCA,
    KEY_APP11		= 0xCB,
    KEY_APP12		= 0xCC,
    KEY_APP13		= 0xCD,
    KEY_APP14		= 0xCE,
    KEY_APP15		= 0xCF,
#endif
};

#ifdef WITH_KEYMAPPING
#include <vector>
#include <utility>

struct KeyMap : std::pair<int, KeySym>
{
    KeyMap(int i = 0, KeySym s = KEY_NONE) : std::pair<int, KeySym>(i, s){};
    bool isKey(int k) const{ return first == k; };
};
#endif

class LocalEvent
{
public:
    static LocalEvent & Get(void);

    void SetGlobalFilterMouseEvents(void (*pf)(u16, u16));
    void SetGlobalFilterKeysEvents(void (*pf)(u32, u16));
    void SetGlobalFilter(bool);
    void SetTapMode(bool);
    void SetTapDelayForRightClickEmulation(double);
    void SetMouseOffsetX(s16);
    void SetMouseOffsetY(s16);

#ifdef WITH_KEYMAPPING
    void   SetVirtualKey(int, KeySym);
    KeySym GetVirtualKey(KeySym) const;
#endif

    static void SetStateDefaults(void);
    static void SetState(u8 type, bool enable);
    static u8   GetState(u8 type);

    bool HandleEvents(bool delay = true);

    bool MouseMotion(void) const;
    bool MouseMotion(const Rect &rt) const;

    const Point & GetMouseCursor(void);
    const Point & GetMousePressLeft(void) const{ return mouse_pl; };
    const Point & GetMousePressMiddle(void) const{ return mouse_pm; };
    const Point & GetMousePressRight(void) const{ return mouse_pr; };

    const Point & GetMouseReleaseLeft(void) const{ return mouse_rl; };
    const Point & GetMouseReleaseMiddle(void) const{ return mouse_rm; };
    const Point & GetMouseReleaseRight(void) const{ return mouse_rr; };

    void ResetPressLeft(void);
    void ResetPressRight(void);
    void ResetPressMiddle(void);

    void ResetReleaseLeft(void);
    void ResetReleaseRight(void);
    void ResetReleaseMiddle(void);
    
    bool MouseClickLeft(void);
    bool MouseClickMiddle(void);
    bool MouseClickRight(void);

    bool MouseClickLeft(const Rect &rt);
    bool MouseClickMiddle(const Rect &rt);
    bool MouseClickRight(const Rect &rt);

    bool MouseWheelUp(void) const;
    bool MouseWheelDn(void) const;

    bool MousePressLeft(void) const;
    bool MousePressLeft(const Rect &rt) const;
    bool MousePressLeft(const Point &pt, u16 w, u16 h) const;
    bool MousePressMiddle(void) const;
    bool MousePressMiddle(const Rect &rt) const;
    bool MousePressRight(void) const;
    bool MousePressRight(const Rect &rt) const;

    bool MouseReleaseLeft(void) const;
    bool MouseReleaseLeft(const Rect &rt) const;
    bool MouseReleaseMiddle(void) const;
    bool MouseReleaseMiddle(const Rect &rt) const;
    bool MouseReleaseRight(void) const;
    bool MouseReleaseRight(const Rect &rt) const;

    bool MouseWheelUp(const Rect &rt) const;
    bool MouseWheelDn(const Rect &rt) const;

    bool MouseCursor(const Rect &rt) const;

    bool KeyPress(void) const;
    bool KeyPress(KeySym key) const;
    KeySym KeyValue(void) const;
    u16	   KeyMod(void) const;

private:
    LocalEvent();

    void HandleMouseMotionEvent(const SDL_MouseMotionEvent & motion);
    void HandleMouseButtonEvent(const SDL_MouseButtonEvent & button);
    void HandleKeyboardEvent(SDL_KeyboardEvent &);

#if SDL_VERSION_ATLEAST(1, 3, 0)
    static int GlobalFilterEvents(void *userdata, SDL_Event *event);
#else
    static int GlobalFilterEvents(const SDL_Event *event);
#endif

    enum flag_t
    {
	KEY_PRESSED	= 0x0001,
	MOUSE_MOTION	= 0x0002,
	MOUSE_PRESSED	= 0x0004,
	GLOBAL_FILTER	= 0x0008,
	CLICK_LEFT	= 0x0010,
	CLICK_RIGHT	= 0x0020,
	CLICK_MIDDLE	= 0x0040,
	TAP_MODE	= 0x0080,
	MOUSE_OFFSET	= 0x0100,
	CLOCK_ON	= 0x0200,
    };

    void SetModes(flag_t);
    void ResetModes(flag_t);

    u16    modes;
    KeySym key_value;
    u8     mouse_state;
    u8     mouse_button;

    Point mouse_st;	// mouse offset for pocketpc

    Point mouse_pl;	// press left
    Point mouse_pm;	// press middle
    Point mouse_pr;	// press right

    Point mouse_rl;	// release left
    Point mouse_rm;	// release middle
    Point mouse_rr;	// release right

    Point mouse_cu;	// point cursor

    void (*redraw_cursor_func)(u16, u16);
    void (*keyboard_filter_func)(u32, u16);

    SDL::Time clock;
    double clock_delay;

#ifdef WITH_KEYMAPPING
    std::vector<KeyMap> vkey;
#endif
};

#endif