/* See LICENSE file for copyright and license details. */

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 10;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char statussep         = ';';      /* separator between status bars */
static const char *fonts[]          = { "Jetbrains Mono Nerd Font:size=15" };
static const char dmenufont[]       = "monospace:size=10";
static const char col_dPurple[]     = "#0f0f17";
static const char col_lDPurple[]    = "#171723";
static const char col_lPurple[]     = "#7070a1";
static const char col_white[]       = "#eeeeee";
static const char col_cyan[]        = "#314f57";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_white, col_lDPurple, col_dPurple },
	[SchemeSel]  = { col_white, col_cyan,  col_cyan  },
};

static const char *const autostart[] = {
	"dwmblocks", "&", NULL,
	"nitrogen", "--restore", NULL,
	"setxkbmap", "-option", "caps:escape_shifted_capslock", NULL,
	"unclutter", "--timeout", "3", "&", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor  scratch key*/
	{ "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1, 0 },
	{ "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1, 0 },
	{ "Alacritty", "Alacritty", NULL,      0,         0,          1,           0,        -1, 0 },
	{ NULL,      NULL,     "Event Tester", 0,         0,          0,           1,        -1, 0 }, /* xev */
	{ NULL,      NULL,     "scratchpad",   0,         1,          1,           0,        -1, 's' },
	{ "Bitwarden", "bitwarden",     NULL,  0,         1,          0,           1,        -1, 'b' }, 
	{ "firefox", "Toolkit", "Picture-in-Picture", 1 << 8,    1,          0,    1,        -1, 0 },
};

/* Bar rules allow you to configure what is shown where on the bar, as well as
 * introducing your own bar modules.
 *
 *    monitor:
 *      -1  show on all monitors
 *       0  show on monitor 0
 *      'A' show on active monitor (i.e. focused / selected) (or just -1 for active?)
 *    bar - bar index, 0 is default, 1 is extrabar
 *    alignment - how the module is aligned compared to other modules
 *    widthfunc, drawfunc, clickfunc - providing bar module width, draw and click functions
 *    name - does nothing, intended for visual clue and for logging / debugging
 */
static const BarRule barrules[] = {
	/* monitor  bar    alignment         widthfunc              drawfunc              clickfunc           name */
	{ -1,       0,     BAR_ALIGN_CENTER, width_status2d,     draw_status2d,          click_statuscmd, "status2d" },
	{ -1,       0,     BAR_ALIGN_LEFT_LEFT,   width_tags,            draw_tags,            click_tags,         "tags" },
	{ -1,       0,     BAR_ALIGN_RIGHT_RIGHT,   width_ltsymbol,        draw_ltsymbol,        click_ltsymbol,     "layout" },
	{ -1,       0,     BAR_ALIGN_RIGHT_RIGHT, width_status2d_es,     draw_status2d_es,     click_statuscmd_es, "status2d_es" },

};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask

#define TAGKEYS(KEY,TAG)								    \
	{1, {{MODKEY, KEY}},				view,           {.ui = 1 << TAG} }, \
	{1, {{MODKEY|ControlMask, KEY}},		toggleview,     {.ui = 1 << TAG} }, \
	{1, {{MODKEY|ShiftMask, KEY}},			tag,            {.ui = 1 << TAG} }, \
	{1, {{MODKEY|ControlMask|ShiftMask, KEY}},	toggletag,      {.ui = 1 << TAG} },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL };
static const char *bookcmd[] = { "bookmark", NULL };
static const char *confcmd[] = { "conf", NULL };
static const char *searencmd[] = { "searen", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *scratchpadcmd[] = {"s", "alacritty", "-t", "scratchpad", NULL};
static const char *bitwardencmd[] = {"b", "bitwarden-desktop", NULL};
static const char *mpdplay[]  = { "mpc", "toggle", NULL };
static const char *mpdstop[]  = { "mpc", "stop", NULL };
static const char *mpdnext[]  = { "mpc", "next", NULL };
static const char *mpdprev[]  = { "mpc", "prev", NULL };

#include <X11/XF86keysym.h>

static Keychord keychords[] = {
	/* modifier                     key        function        argument */
	// BAR
	{1, {{MODKEY,                       XK_b}},      togglebar,      {0} },

	// LAYOUT
 	{1, {{MODKEY,                       XK_comma}},      incnmaster,     {.i = +1 } },
	{1, {{MODKEY,                       XK_period}},      incnmaster,     {.i = -1 } },
	{1, {{MODKEY,                       XK_t}},      setlayout,      {.v = &layouts[0]} },
	{1, {{MODKEY|ShiftMask,             XK_f}},      setlayout,      {.v = &layouts[1]} },
	{1, {{MODKEY,                       XK_m}},      setlayout,      {.v = &layouts[2]} },
	{1, {{MODKEY,                       XK_space}},  setlayout,      {0} },
	{1, {{MODKEY,             	    XK_f}},      togglefloating, {0} },
	{1, {{MODKEY,                       XK_e}},      togglefullscr,  {0} },

	// WINDOW
//	{1, {{MODKEY,                       XK_j}},      focusstack,     {.i = +1 } },
//	{1, {{MODKEY,                       XK_k}},      focusstack,     {.i = -1 } },
	{1, {{MODKEY,                       XK_l}},      focusdir,       {.i = 0 } }, // left
	{1, {{MODKEY,                       XK_h}},      focusdir,       {.i = 1 } }, // right
	{1, {{MODKEY,                       XK_k}},      focusdir,       {.i = 2 } }, // up
	{1, {{MODKEY,                       XK_j}},      focusdir,       {.i = 3 } }, // down
	{1, {{MODKEY|ControlMask,           XK_l}},      placedir,       {.i = 0 } }, // left
	{1, {{MODKEY|ControlMask,           XK_h}},      placedir,       {.i = 1 } }, // right
	{1, {{MODKEY|ControlMask,           XK_k}},      placedir,       {.i = 2 } }, // up
	{1, {{MODKEY|ControlMask,           XK_j}},      placedir,       {.i = 3 } }, // down
	{1, {{MODKEY|ShiftMask,             XK_h}},      setmfact,       {.f = -0.05} },
	{1, {{MODKEY|ShiftMask,             XK_l}},      setmfact,       {.f = +0.05} },
	{1, {{MODKEY|ShiftMask,             XK_Return}}, zoom,           {0} },

	// TERMINAL
	{1, {{MODKEY,                       XK_Return}}, spawn,          {.v = termcmd } },

	// LAUNCHER
	{1, {{MODKEY,                       XK_s}},      spawn,          {.v = dmenucmd } },
	{2, {{MODKEY, XK_d},                {0, XK_b}},  spawn,          {.v = bookcmd } },
	{2, {{MODKEY, XK_d},                {0, XK_s}},  spawn,          {.v = searencmd } },
	{2, {{MODKEY, XK_d},                {0, XK_c}},  spawn,          {.v = confcmd } },

	// SCRATCHPAD
	{2, {{MODKEY,             XK_y},      {0, XK_y}}, togglescratch,  {.v = scratchpadcmd } },
	{2, {{MODKEY,             XK_y},      {0, XK_b}}, togglescratch,  {.v = bitwardencmd } },

	// QUIT
	{1, {{MODKEY,                       XK_q}},      killclient,     {0} },
	{1, {{MODKEY|ShiftMask,             XK_q}},      quit,           {0} },
	{1, {{MODKEY|ShiftMask,             XK_r}},      quit,           {1} }, 

	// MEDIA CONTROLL
	{1, {{0, XF86XK_AudioRaiseVolume}},	spawn,	   SHCMD("pulsemixer --change-volume +5 && pulsemixer --max-volume 100 && kill -39 $(pidof dwmblocks)") },
	{1, {{0, XF86XK_AudioLowerVolume}},	spawn,	   SHCMD("pulsemixer --change-volume -5 && kill -39 $(pidof dwmblocks)") },

	// MPD CONTROLL
	{1, {{MODKEY,                   XK_p}},      spawn,          {.v = mpdplay } },
	{1, {{MODKEY,                   XK_o}},      spawn,          {.v = mpdstop } },
	{1, {{MODKEY|ShiftMask,         XK_n}},      spawn,          {.v = mpdprev } },
	{1, {{MODKEY,                   XK_n}},      spawn,          {.v = mpdnext } },

	// MULTI MONITOR
//	{1, {{MODKEY,                       XK_comma}},  focusmon,       {.i = -1 } },
//	{1, {{MODKEY,                       XK_period}}, focusmon,       {.i = +1 } },
//	{1, {{MODKEY|ShiftMask,             XK_comma}},  tagmon,         {.i = -1 } },
//	{1, {{MODKEY|ShiftMask,             XK_period}}, tagmon,         {.i = +1 } },

	// TAGS
	{1, {{MODKEY,                       XK_Tab}},    view,           {0} },
	{1, {{MODKEY,                       XK_0}},      view,           {.ui = ~0 } },
	{1, {{MODKEY|ShiftMask,             XK_0}},      tag,            {.ui = ~0 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1 } },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2 } },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3 } },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4 } },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5 } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

