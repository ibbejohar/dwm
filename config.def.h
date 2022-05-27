/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx       = 4;        /* border pixel of windows */
static const unsigned int snap           = 15;       /* snap pixel */
static const int showbar                 = 1;        /* 0 means no bar */
static const int topbar                  = 1;        /* 0 means bottom bar */
static const char *fonts[]               = { "JetBrains Mono Nerd Font:pixelsize=19:antialias=true:autohint=true" };
static const char dmenufont[]            = "monospace:size=10";
static const char col_darkPurple[]       = "#0f0f17";
static const char col_lightDarkPurple[]  = "#171723";
static const char col_lightPurple[]      = "#7070a2";
static const char col_gray4[]            = "#eeeeee";
static const char col_cyan[]             = "#314f57";
static const char *colors[][3]           = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray4, col_lightDarkPurple, col_darkPurple},
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5"};

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ " ",      tile },    /* first entry is default */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

#define STATUSBAR "dwmblocks"

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-modi", "drun", "-show", "drun", NULL};
static const char *termcmd[]  = { "alacritty", NULL };
static const char *mpdplay[]  = { "mpc", "toggle", NULL };
static const char *mpdnext[]  = { "mpc", "next", NULL };
static const char *mpdprev[]  = { "mpc", "prev", NULL };

#include "movestack.c"
#include <X11/XF86keysym.h>
static Key keys[] = {
	/* modifier                     key        function        argument */

	// BAR MANIPULATION
	{ MODKEY,                       XK_b,      togglebar,      {0} },

	// LAYOUTS
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },

	// TILING MANIPULATION
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_comma,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_period, incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_e,      togglefullscr,  {0} },

	// FLOATING MANIPULATION
	{ MODKEY,                       XK_f,      togglefloating, {0} },

	// QUIT
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },

	// TERMINAL
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },

	// LAUNCHER
	{ MODKEY,                       XK_s,      spawn,          {.v = dmenucmd } },

	// MPDCONTROL
	{ MODKEY,                       XK_Escape, spawn,          {.v = mpdplay } },
	{ MODKEY,                       XK_F1,     spawn,          {.v = mpdprev } },
	{ MODKEY,                       XK_F2,     spawn,          {.v = mpdnext } },

	// TAGS
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
    TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)

	// MEDIA
	{0, XF86XK_AudioRaiseVolume, spawn, SHCMD("pulsemixer --change-volume +5 | kill -39 $(pidof dwmblocks)") },
	{0, XF86XK_AudioLowerVolume, spawn, SHCMD("pulsemixer --change-volume -5 | kill -39 $(pidof dwmblocks)") },

	// USELESS
	{ ControlMask,                  XK_comma,  focusmon,       {.i = -1 } },
	{ ControlMask,                  XK_period, focusmon,       {.i = +1 } },
	{ ControlMask|ShiftMask,        XK_comma,  tagmon,         {.i = -1 } },
	{ ControlMask|ShiftMask,        XK_period, tagmon,         {.i = +1 } },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkStatusText,        0,              Button1,        sigstatusbar,          {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigstatusbar,          {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigstatusbar,          {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

