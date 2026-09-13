/* See LICENSE file for copyright and license details. */

#define SESSION_FILE "/tmp/dwm-session"

/* appearance */
static const unsigned int borderpx  = 4;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const int swterminheritfs    = 1;        /* 1 terminal inherits fullscreen on unswallow, 0 otherwise */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int splitstatus        = 1;        /* 1 for split status items */
static const char *splitdelim        = ";";       /* Character used for separating status */
static const char *barlayout        = "t|ls";
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

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title            tags mask  isfloating  isfullscreen isterminal  noswallow  monitor  scratchkey*/
	{ "Gimp",     NULL,       NULL,            0,         1,          0,           0,          0,         -1,      0},
	{ "Firefox",  NULL,       NULL,            1 << 8,    0,          0,           0,          0,         -1,      0},
	{ "Alacritty", "Alacritty", NULL,          0,         0,          0,           1,          0,         -1,      0},
	{ NULL,       NULL,       "Event Tester",  0,         0,          0,           1,          0,         -1,      0}, /* xev */
	{ NULL,       NULL,   "scratchpad",        0,         1,          0,           1,          0,         -1,       's'},
};

/* layout(s) */
static const float mfact     = 0.50; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */
static const int refreshrate = 120;  /* refresh rate (per second) for client move/resize */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod4Mask // Super
#define TAGKEYS(KEY,TAG)                                                                                               \
       &((Keychord){1, {{MODKEY, KEY}},                                        view,           {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask, KEY}},                            toggleview,     {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ShiftMask, KEY}},                              tag,            {.ui = 1 << TAG} }), \
       &((Keychord){1, {{MODKEY|ControlMask|ShiftMask, KEY}},                  toggletag,      {.ui = 1 << TAG} }),

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "drun", NULL};
static const char *termcmd[]  = { "alacritty", NULL };

/*First arg only serves to match against key in rules*/
static const char *scratchpadcmd[] = {"s", "alacritty", "-t", "scratchpad", NULL};

static Keychord *keychords[] = {
	/* keychord  chain modifier            key      chainkey         function        argument */

	// Spawn
	&((Keychord){1, {{ MODKEY,             XK_s}},                   spawn,          {.v = dmenucmd } }),
	&((Keychord){1, {{ MODKEY,             XK_Return }},             spawn,          {.v = termcmd } }),
	
	// Bar
	&((Keychord){1, {{ MODKEY,             XK_b }},                  togglebar,      {0} }),

	// Window
	&((Keychord){1, {{ MODKEY,             XK_q }},                  killclient,     {0} }),
	&((Keychord){1, {{ MODKEY,             XK_l }},                  focusdir,       {.i = 0 } }), // left
	&((Keychord){1, {{ MODKEY,             XK_h }},                  focusdir,       {.i = 1 } }), // right
	&((Keychord){1, {{ MODKEY,             XK_k }},                  focusdir,       {.i = 2 } }), // up
	&((Keychord){1, {{ MODKEY,             XK_j }},                  focusdir,       {.i = 3 } }), // down
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_l }},                  placedir,       {.i = 0 } }), // left
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_h }},                  placedir,       {.i = 1 } }), // right
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_k }},                  placedir,       {.i = 2 } }), // up
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_j }},                  placedir,       {.i = 3 } }), // down
	&((Keychord){1, {{ MODKEY|ControlMask, XK_h }},                  setmfact,       {.f = -0.05} }),
	&((Keychord){1, {{ MODKEY|ControlMask, XK_l }},                  setmfact,       {.f = +0.05} }),
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_Return }},             zoom,           {0} }),
      //&((Keychord){1, {{ MODKEY,             XK_j }},                  focusstack,     {.i = +1 } }),
      //&((Keychord){1, {{ MODKEY,             XK_k }},                  focusstack,     {.i = -1 } }),

	// Layout
	&((Keychord){1, {{ MODKEY,             XK_t }},                  setlayout,      {.v = &layouts[0]} }),
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_f }},                  setlayout,      {.v = &layouts[1]} }),
	&((Keychord){1, {{ MODKEY,             XK_m }},                  setlayout,      {.v = &layouts[2]} }),
	&((Keychord){1, {{ MODKEY,             XK_space }},              setlayout,      {0} }),
	&((Keychord){1, {{ MODKEY,             XK_f }},                  togglefloating, {0} }),
	&((Keychord){1, {{ MODKEY,             XK_e }},                  togglefullscr,  {0} }),
	&((Keychord){1, {{ MODKEY,             XK_comma }},              incnmaster,     {.i = +1 } }),
	&((Keychord){1, {{ MODKEY,             XK_period }},             incnmaster,     {.i = -1 } }),

	// Scratchpad
	&((Keychord){2, {{ MODKEY,             XK_a },  {0, XK_t}},      togglescratch,  {.v = scratchpadcmd } }),


	// Monitor
      //&((Keychord){1, {{ MODKEY,             XK_comma }},              focusmon,       {.i = -1 } }),
      //&((Keychord){1, {{ MODKEY,             XK_period }},             focusmon,       {.i = +1 } }),
      //&((Keychord){1, {{ MODKEY|ShiftMask,   XK_comma }},              tagmon,         {.i = -1 } }),
      //&((Keychord){1, {{ MODKEY|ShiftMask,   XK_period }},             tagmon,         {.i = +1 } }),

	// Quit and Restart
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_q }},                  quit,           {0} }),
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_r }},                  quit,           {1} }), // Restart

	// Tags
	&((Keychord){1, {{ MODKEY,             XK_Tab }},                view,           {0} }),
	&((Keychord){1, {{ MODKEY,             XK_0 }},                  view,           {.ui = ~0 } }),
	&((Keychord){1, {{ MODKEY|ShiftMask,   XK_0 }},                  tag,            {.ui = ~0 } }),

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
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};
