/* See LICENSE file for copyright and license details. */
#include <X11/XF86keysym.h>
/* appearance */
static const unsigned int borderpx  = 1;        /* border pixel of windows */
static const int gappx     = 5;                 /* gaps between windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const char *fonts[]          = {
	"monospace:size=11",
	"JoyPixels:pixelsize=14:antialias=true:autohint=true",
};
static const char dmenufont[]       = "monospace:size=11";
static const char col_gray1[]       = "#222222";
static const char col_gray2[]       = "#444444";
static const char col_gray3[]       = "#bbbbbb";
static const char col_gray4[]       = "#eeeeee";
static const char col_cyan[]        = "#005577";
static const char col_cursor[]      = "#d65d0e";
static const char col_fg[]          = "#ebdbb2";
static const char col_bg[]          = "#282828";
static const char col_0[]           = "#1d2021";
static const char col_1[]           = "#cc241d";
static const char col_2[]           = "#98971a";
static const char col_3[]           = "#d79921";
static const char col_4[]           = "#458588";
static const char col_5[]           = "#b16286";
static const char col_6[]           = "#689d6a";
static const char col_7[]           = "#a89984";
static const char col_8[]           = "#928374";
static const char col_9[]           = "#fb4934";
static const char col_10[]          = "#b8bb26";
static const char col_11[]          = "#fabd2f";
static const char col_12[]          = "#83a598";
static const char col_13[]          = "#d3869b";
static const char col_14[]          = "#8ec07c";
static const char col_15[]          = "#ebdbb2";
static const char *colors[][3]      = {
	/*                   fg         bg          border   */
	[SchemeNorm]     = { col_fg,    col_bg,     col_8 },
	[SchemeSel]      = { col_bg,    col_cursor, col_cursor },
	[SchemeStatus]   = { col_fg,    col_bg,     col_8 },
	[SchemeTagsNorm] = { col_fg,    col_bg,     col_8 },
	[SchemeTagsSel]  = { col_bg,    col_cursor, col_cursor },
	[SchemeInfoNorm] = { col_fg,    col_bg,     col_8 },
	[SchemeInfoSel]  = { col_bg,    col_4,      col_4 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

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
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
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

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_bg, "-nf", col_fg, "-sb", col_cursor, "-sf", col_bg, NULL };
static const char *termcmd[]  = { "alacritty", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_u,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ShiftMask,             XK_u,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_y,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_y,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_minus,  setgaps,        {.i = -5 } },
	{ MODKEY,                       XK_equal,  setgaps,        {.i = +5 } },
	{ MODKEY|ShiftMask,             XK_equal,  setgaps,        {.i = 0  } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
	{ 0, XF86XK_AudioMute,		spawn,		SHCMD("volumeControl togmute; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioRaiseVolume,	spawn,		SHCMD("volumeControl up; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioLowerVolume,	spawn,		SHCMD("volumeControl down; kill -44 $(pidof dwmblocks)") },
	{ 0, XF86XK_AudioPrev,		spawn,		SHCMD("mpc prev") },
	{ 0, XF86XK_AudioNext,		spawn,		SHCMD("mpc next") },
	{ 0, XF86XK_AudioPause,		spawn,		SHCMD("mpc pause") },
	{ 0, XF86XK_AudioPlay,		spawn,		SHCMD("mpc play") },
	{ 0, XF86XK_AudioStop,		spawn,		SHCMD("mpc stop") },
	{ 0, XF86XK_AudioRewind,	spawn,		SHCMD("mpc seek -10") },
	{ 0, XF86XK_AudioForward,	spawn,		SHCMD("mpc seek +10") },
	{ 0, XF86XK_AudioMedia,		spawn,		SHCMD("alacritty -e ncmpcpp") },
	{ 0, XF86XK_Calculator,		spawn,		SHCMD("alacritty -e bc -l") },
	{ 0, XF86XK_WWW,		spawn,		SHCMD("$BROWSER") },
	{ 0, XF86XK_Mail,		spawn,		SHCMD("alacritty -e neomutt ; pkill -RTMIN+12 dwmblocks") },
	{ 0, XF86XK_MonBrightnessUp,	spawn,		SHCMD("brightnessControl up") },
	{ 0, XF86XK_MonBrightnessDown,	spawn,		SHCMD("brightnessControl down") },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkStatusText,        0,              Button4,        sigdwmblocks,   {.i = 4} },
	{ ClkStatusText,        0,              Button5,        sigdwmblocks,   {.i = 5} },
	{ ClkStatusText,        ShiftMask,      Button1,        sigdwmblocks,   {.i = 6} },
	{ ClkStatusText,        ShiftMask,      Button3,        spawn,          SHCMD("alacritty -e nvim ~/src/dwmblocks-lukesmith/config.h") },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

