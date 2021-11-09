/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const unsigned int gappx     = 5;        /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = { "menlo:size=10" };
static const char dmenufont[]       = "menlo:size=10";
static const char col_gray1[]       = "#75715e"; // "#222222";
static const char col_gray2[]       = "#49483e"; // "#444444";
static const char col_gray3[]       = "#272822"; // "#bbbbbb";
static const char col_gray4[]       = "#f8f8f2"; // "#eeeeee";
static const char col_cyan[]        = "#ae81ff"; // "#005577";
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                       instance    title       tags mask     isfloating   monitor */
	{ "Microsoft Teams - Preview", NULL,       NULL,       0,            1,           -1 },
	{ "Display-im6.q16",           NULL,       NULL,       0,            1,           -1 },
	{ "Pavucontrol",               NULL,       NULL,       0,            1,           -1 },
	// { "Firefox",                   NULL,       NULL,       1 << 8,       0,           -1 },
};

// Autostart applications
static const char *const autostart[] = {
	"${HOME}/.xinitrc", NULL,                  /* startup script */
	"/usr/bin/bluetoothctl", NULL,             /* Bluetooth */
	"/usr/bin/compton", NULL,                  /* Compton: terminal transparency */
	"/usr/bin/urxvtd", "-q", "-f", "-o", NULL, /* urxvt daemon */
	"compton", NULL,                           /* compton: transparent windows */
	"dunst", NULL,                             /* dunst: notifications */
	"status.sh", NULL,                         /* status script */
	"xmodmap", "${HOME}/.Xmodmap", NULL,
	NULL /* terminate */
};

/* layout(s) */
static const float mfact     = 0.5; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;   /* number of clients in master area */
static const int resizehints = 1;   /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[\\]",     dwindle },                /* decreasing in size right and leftward */
	{ "[]=",      tile },                   /* first entry is default */
	{ "><>",      NULL },                   /* no layout function means floating behavior */
	{ "[M]",      monocle },                /* All windows on top of one another */
	{ "[@]",      spiral },                 /* fibonacci */
	{ "|M|",      centeredmaster },         /* Master in the middle, slaves on sides */
	{ ">M>",      centeredfloatingmaster }, /* Same but master floats */

};

/* key definitions */
#define MODKEY Mod4Mask    /* see xmodmap */
#define ALTKEY Mod1Mask    /* see xmodmap */
#define SHIFTKEY ShiftMask /* see xmodmap */
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask,           KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "rofi", "-show", "combi", NULL };
static const char *dmenussh[] = { "rofi", "-show", "ssh", NULL };
static const char *templatecmd[] = { "templates.sh", "${HOME}/workspace/default_files", NULL };
static const char *termcmd[]  = { "urxvtc", NULL };
static const char *lockcmd[]  = { "lock.sh", NULL };

/* For Keycodes, see xev */
#include "/usr/include/X11/XF86keysym.h"

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_d,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_Pause,  spawn,          {.v = lockcmd } },
	{ MODKEY|ShiftMask,             XK_l,      spawn,          {.v = lockcmd } },
	{ MODKEY,                       XK_t,      spawn,          {.v = templatecmd } },
	{ MODKEY,                       XK_s,      spawn,          {.v = dmenussh } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Right,  focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_Left,   focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_Tab,    focusstack,     {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Tab,    focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_Right,  shiftview,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_Left,   shiftview,      {.i = -1 } },
	{ MODKEY,                       XK_v,      focusstack,     {.i = 0 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_p,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_Return, zoom,           {0} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY|ShiftMask,             XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_i,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_o,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_p,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_f,      togglefullscr,  {0} },
	{ MODKEY,                       XK_space,  spawn,          SHCMD("xmodmap ~/.Xmodmap") },
	{ MODKEY,                       XK_e,      spawn,          SHCMD("nautilus") },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY|ShiftMask,             XK_Print,  spawn,          SHCMD("screenshot.sh --select") },
	{ MODKEY,                       XK_Print,  spawn,          SHCMD("screenshot.sh --full") },
	{ MODKEY,                       XK_agrave, view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_agrave, tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_Insert, spawn,          SHCMD("notify-send \"Clipboard contents:\" \"$(xclip -o -selection clipboard)\"") },
	{ MODKEY|ShiftMask,             XK_b,      spawn,          SHCMD("shutdown now") },
	{ MODKEY|ALTKEY,                XK_Right,  spawn,          SHCMD("xrandr --output $SCREEN --rotate right") },
	{ MODKEY|ALTKEY,                XK_Left,   spawn,          SHCMD("xrandr --output $SCREEN --rotate left") },
	{ MODKEY|ALTKEY,                XK_Up,     spawn,          SHCMD("xrandr --output $SCREEN --rotate normal") },
	{ MODKEY|ALTKEY,                XK_Down,   spawn,          SHCMD("xrandr --output $SCREEN --rotate inverted") },
	TAGKEYS(                        XK_ampersand,              0)
	TAGKEYS(                        XK_eacute,                 1)
	TAGKEYS(                        XK_quotedbl,               2)
	TAGKEYS(                        XK_apostrophe,             3)
	TAGKEYS(                        XK_parenleft,              4)
	TAGKEYS(                        XK_minus,                  5)
	TAGKEYS(                        XK_egrave,                 6)
	TAGKEYS(                        XK_underscore,             7)
	TAGKEYS(                        XK_ccedilla,               8)
	{ MODKEY|ShiftMask,             XK_c,      quit,           {0} },
	{ MODKEY|ShiftMask,             XK_q,      killclient,     {0} },
	{ 0, XF86XK_AudioLowerVolume,   spawn,      SHCMD("sound.sh down") },
	{ 0, XF86XK_AudioMute,          spawn,      SHCMD("sound.sh toggle") },
	{ 0, XF86XK_AudioRaiseVolume,   spawn,      SHCMD("sound.sh up") },
	{ 0, XF86XK_MonBrightnessDown,  spawn,      SHCMD("brightness.sh down") },
	{ 0, XF86XK_MonBrightnessUp,    spawn,      SHCMD("brightness.sh up") },
	{ 0, XF86XK_AudioPlay,          spawn,      SHCMD("FILE=$(mktemp) && xprop WM_CLASS > $FILE && notify-send \"$(cat $FILE)\" \"See $FILE\"") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
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

