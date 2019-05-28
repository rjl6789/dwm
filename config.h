/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 2;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const unsigned int gappih    = 10;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 10;       /* vert outer gap between windows and screen edge */
static const int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const unsigned int barsize   = 6;       /* size of the dwm bar */
static const char *fonts[]          = { //"monospace:size=12",
					"UbuntuMono Nerd Font:size=12",
					"icomoon:size=11",
					//"Symbola:style=Regular:size=10",
};
static const char dmenufont[]       = "monospace:size=12";

static char col_red[]         = "#cc241d";
static char col_yellow[]      = "#fabd2f";
static char col_white[]       = "#ebdbb2";
static char col_black[]       = "#282828";

static char col_1[]           = "#16a085";
static char col_2[]           = "#b07f1c";
static char col_3[]           = "#458588";
static char col_4[]           = "#8ec07c";
static char col_5[]           = "#d65d0e";
static char col_6[]           = "#444444";
static char col_7[]           = "#005577";
static char col_8[]           = "#bbbbbb";
static char col_9[]           = "#222222";

static char normbgcolor[]     = "#222222";
static char normbordercolor[] = "#444444";
static char normfgcolor[]     = "#bbbbbb";
static char selfgcolor[]      = "#eeeeee";
static char selbordercolor[]  = "#005577";
static char selbgcolor[]      = "#005577";
static char *colors[][3]            = {		/* when referencing for status colors use \x01..9..A..F etc i.e. Hex */
	/*                        fg           bg           border   */
	[SchemeNorm]	=	{ normfgcolor, normbgcolor, normbordercolor },
	[SchemeSel]	=	{ selfgcolor,  selbgcolor,  selbordercolor  },
	[SchemeWarn]	=	{ col_black,   col_yellow,  col_red },
	[SchemeUrgent]	=	{ col_white,   col_red,     col_red },
	[SchemeCol1]	=	{ col_black,   col_1,       col_red },
	[SchemeCol2]	=	{ col_black,   col_2,       col_red },
	[SchemeCol3]	=	{ col_black,   col_3,       col_red },
	[SchemeCol4]	=	{ col_black,   col_4,       col_red },
	[SchemeCol5]	=	{ col_black,   col_5,       col_red },
	[SchemeCol6]	=	{ col_black,   col_6,       col_red },
	[SchemeCol7]	=	{ col_black,   col_7,       col_red },
	[SchemeCol8]	=	{ col_black,   col_8,       col_red },
	[SchemeCol9]	=	{ col_black,   col_9,       col_red },
};

/* Xresources location */
static const char *xres = "/home/rob/.Xresources";

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class           instance    title       tags mask     isfloating   monitor */
	{ "Gimp",          NULL,       NULL,       0,            0,           -1 },
	//{ "Firefox",       NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Chromium",      NULL,       NULL,       1 << 8,       0,           -1 },
	{ "Google-chrome", NULL,       NULL,       1 << 7,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.53; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#include "fibonacci.c"
#include "centeredmaster.c"
#include "grid.c"
#include "bottomstack.c"
static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "|M|",      centeredmaster },
	{ ">M>",      centeredfloatingmaster },
        { "HHH",      grid },
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
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
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

static Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_d,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.02} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.02} },
	{ MODKEY|Mod1Mask,              XK_k,      incrgaps,       {.i = +1 } },
	{ MODKEY|Mod1Mask,              XK_j,      incrgaps,       {.i = -1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_k,      incrogaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_j,      incrogaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_k,      incrigaps,      {.i = +1 } },
	{ MODKEY|Mod1Mask|ControlMask,  XK_j,      incrigaps,      {.i = -1 } },
	{ MODKEY|Mod1Mask,              XK_0,      togglegaps,     {0} },
	{ MODKEY|Mod1Mask|ShiftMask,    XK_0,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[5]} },
	{ MODKEY|ShiftMask,             XK_o,      setlayout,      {.v = &layouts[6]} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[7]} },
        { MODKEY|ShiftMask,             XK_b,      setlayout,      {.v = &layouts[8]} },
        { MODKEY|Mod1Mask,              XK_b,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	//{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
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

