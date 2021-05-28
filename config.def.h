/*
   ______        ____  __
  |  _ \ \      / /  \/  |
  | | | \ \ /\ / /| |\/| |
  | |_| |\ V  V / | |  | |
  |____/  \_/\_/  |_|  |_|

   See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx        = 4;        /* border pixel of windows */
static unsigned int snap            = 16;       /* snap pixel */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 20;       /* vert inner gap between windows */
static const unsigned int gappoh    = 20;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 20;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static int showbar                  = 1;        /* 0 means no bar */
static int topbar                   = 0;        /* 0 means bottom bar */
static const int user_bh            = 35;        /* 0 means that dwm will calculate bar height, >= 1 means dwm will user_bh as bar height */
static const char *fonts[]          = { "SourceCodePro-Regular:size=12", "fontawesome:size=10" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selbgcolor[]            = "#eeeeee";
static char selfgcolor[]            = "#FF0000";
static char selbordercolor[]        = "#FF0000";
static char col1[]                  = "#ffffff";
static char col2[]                  = "#ffffff";
static char col3[]                  = "#ffffff";
static char col4[]                  = "#ffffff";
static char col5[]                  = "#ffffff";
static char col6[]                  = "#ffffff";

enum { SchemeNorm, SchemeCol1, SchemeCol2, SchemeCol3, SchemeCol4,
       SchemeCol5, SchemeCol6, SchemeSel }; /* color schemes */

static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm]  = { normfgcolor, normbgcolor, normbordercolor },
	[SchemeCol1]  = { col1,      normbgcolor, normbordercolor },
	[SchemeCol2]  = { col2,      normbgcolor, normbordercolor },
	[SchemeCol3]  = { col3,      normbgcolor, normbordercolor },
	[SchemeCol4]  = { col4,      normbgcolor, normbordercolor },
	[SchemeCol5]  = { col5,      normbgcolor, normbordercolor },
	[SchemeCol6]  = { col6,      normbgcolor, normbordercolor },
	[SchemeSel]   = { selfgcolor, selbgcolor,  selbordercolor },
};

static const char *const autostart[] = {
	"dwmblocks", NULL,
	"climenud", NULL,
	"lxsession", NULL,
	"picom", "--config", "/home/simen/.config/picom.conf", NULL,
	"dunst", NULL,
	"light-locker", NULL,
	"redshift", "-c", "/home/simen/.config/redshift/redshift.conf", NULL,
	"keepassxc", NULL,
	"/home/simen/.local/bin/setwp", "-R", NULL,
	NULL /* terminate */
};

/* tagging */
static const char *tags[] = {"", "", "", "", "", "", "", "", ""};
static const char *tagsalt[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class            instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	{ "Gimp",           NULL,     NULL,           0,         1,          0,           0,        -1 },
	{ "Firefox",        NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 },
	{ "St",             NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ "Alacritty",      NULL,     NULL,           0,         0,          1,           0,        -1 },
	{ NULL,             NULL,     "Event Tester", 0,         0,          0,           1,        -1 }, /* xev */
};

/* layout(s) */
static float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static int nmaster     = 1;    /* number of clients in master area */
static int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "vanitygaps.c"
#include "focusurgent.c"
#include <X11/XF86keysym.h>

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
    { "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
    { "|M|",      centeredmaster },
    { "TTT",      bstack },
    { ":::",      gaplessgrid },
    { "H[]",      deck },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ">M>",      centeredfloatingmaster },
	{ NULL,       NULL },
};

/*
 * Xresources preferences to load at startup
 */

/* commands */
ResourcePref resources[] = {
		{ "background",        STRING,  &normbgcolor },
		{ "normbordercolor",    STRING,  &normbordercolor },
		{ "foreground",        STRING,  &normfgcolor },
		{ "selbackground",         STRING,  &selbgcolor },
		{ "selbordercolor",     STRING,  &selbordercolor },
		{ "selforeground",         STRING,  &selfgcolor },
		{ "color1",        STRING,  &col1 },
		{ "color2",        STRING,  &col2 },
		{ "color3",        STRING,  &col3 },
		{ "color4",        STRING,  &col4 },
		{ "color5",        STRING,  &col5 },
		{ "color6",        STRING,  &col6 },
		{ "borderpx",          	INTEGER, &borderpx },
		{ "snap",          		INTEGER, &snap },
		{ "showbar",          	INTEGER, &showbar },
		{ "topbar",          	INTEGER, &topbar },
		{ "nmaster",          	INTEGER, &nmaster },
        { "resizehints",       	INTEGER, &resizehints },
		{ "mfact",      	 	FLOAT,   &mfact },
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

/* Some programs */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-c", "-l", "20", NULL };
static const char *termcmd[]  = { "alacritty", NULL };
static const char *browsercmd[]  = { "firefox", NULL };
static const char *explorercmd[]  = { "nautilus", NULL };
static const char *lockcmd[]  = { "light-locker-command", "-l", NULL };
static const char *calccmd[]  = { "gnome-calculator", NULL };
static const char *layoutmenu_cmd = "layoutmenu.sh";

static Key on_empty_keys[] = {
    /* modifier key            function                argument */
    { 0,        XK_w,          spawn,                  {.v = browsercmd } },
    { 0,        XK_t,          spawn,                  {.v = termcmd } },
    { 0,        XK_e,          spawn,                  {.v = explorercmd } },
    { 0,        XK_c,          spawn,                  {.v = calccmd } },
    { 0,        XK_v,          spawn,                  SHCMD("alacritty -e nvim")},
};

static Key keys[] = {
	/* modifier                     key                  function             argument */
	{ Mod1Mask,                     XK_space,            spawn,               {.v = dmenucmd } },
	{ MODKEY|ShiftMask,             XK_Return,           spawn,               {.v = termcmd } },
	{ MODKEY,                       XK_w,                spawn,               {.v = browsercmd } },
	{ MODKEY,                       XK_o,                spawn,               {.v = lockcmd } },
	{ MODKEY,                       XK_e,                spawn,               {.v = explorercmd } },
	{ MODKEY,                       XK_z,                spawn,               SHCMD("powermenu.sh")},
	{ MODKEY,                       XK_x,                spawn,               SHCMD("xkill")},
	{ MODKEY|ShiftMask,             XK_w,                spawn,               SHCMD("searchmenu.sh")},
    { 0,                            XF86XK_Calculator,   spawn,               {.v = calccmd } },
    { 0,                            XF86XK_AudioMute,    spawn,               SHCMD("pactl set-sink-mute @DEFAULT_SINK@ toggle && notify-send Mute 'Toggled mute'")},
	{ MODKEY,                       XK_b,                togglebar,           {0} },
	{ MODKEY|ShiftMask,             XK_j,                rotatestack,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,                rotatestack,         {.i = -1 } },
	{ MODKEY,                       XK_j,                focusstack,          {.i = +1 } },
	{ MODKEY,                       XK_k,                focusstack,          {.i = -1 } },
	{ MODKEY,                       XK_i,                incnmaster,          {.i = +1 } },
	{ MODKEY,                       XK_d,                incnmaster,          {.i = -1 } },
	{ MODKEY,                       XK_h,                setmfact,            {.f = -0.05} },
	{ MODKEY,                       XK_l,                setmfact,            {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,                setcfact,            {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,                setcfact,            {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,                setcfact,            {.f =  0.00} },
	{ MODKEY,                       XK_Return,           zoom,                {0} },
	{ MODKEY,                       XK_y,                incrgaps,            {.i = +10 } },
	{ MODKEY,                       XK_u,                incrgaps,            {.i = -10 } },
	{ MODKEY|ShiftMask,             XK_y,                incrigaps,           {.i = +10 } },
	{ MODKEY|ShiftMask,             XK_u,                incrigaps,           {.i = -10 } },
	{ MODKEY|ShiftMask,             XK_i,                incrogaps,           {.i = +10 } },
	{ MODKEY|ShiftMask,             XK_t,                incrogaps,           {.i = -10 } },
	{ MODKEY|Mod1Mask,              XK_y,                incrihgaps,          {.i = +10 } },
	{ MODKEY|Mod1Mask,              XK_u,                incrihgaps,          {.i = -10 } },
	{ MODKEY|ControlMask,           XK_y,                incrivgaps,          {.i = +10 } },
	{ MODKEY|ControlMask,           XK_u,                incrivgaps,          {.i = -10 } },
	{ MODKEY|Mod1Mask,              XK_i,                incrohgaps,          {.i = +10 } },
	{ MODKEY|Mod1Mask,              XK_t,                incrohgaps,          {.i = -10 } },
	{ MODKEY|ControlMask,           XK_i,                incrovgaps,          {.i = +10 } },
	{ MODKEY|ControlMask,           XK_t,                incrovgaps,          {.i = -10 } },
	{ MODKEY,                       XK_space,            togglegaps,          {0} },
	{ MODKEY,                       XK_n,                defaultgaps,         {0} },
	{ MODKEY,                       XK_Tab,              view,                {0} },
	{ MODKEY,                       XK_q,                killclient,          {0} },
	{ MODKEY,                       XK_t,                setlayout,           {.v = &layouts[0]} },
	{ MODKEY,                       XK_f,                setlayout,           {.v = &layouts[1]} },
	{ MODKEY,                       XK_m,                setlayout,           {.v = &layouts[2]} },
	{ MODKEY,                       XK_c,                setlayout,           {.v = &layouts[3]} },
	{ MODKEY|ControlMask,		    XK_comma,            cyclelayout,         {.i = -1 } },
	{ MODKEY|ControlMask,           XK_period,           cyclelayout,         {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_space,            setlayout,           {0} },
	{ MODKEY,                       XK_Down,             moveresize,          {.v = "0x 50y 0w 0h" } },
	{ MODKEY,                       XK_Up,               moveresize,          {.v = "0x -50y 0w 0h" } },
	{ MODKEY,                       XK_Right,            moveresize,          {.v = "50x 0y 0w 0h" } },
	{ MODKEY,                       XK_Left,             moveresize,          {.v = "-50x 0y 0w 0h" } },
	{ MODKEY|ShiftMask,             XK_Down,             moveresize,          {.v = "0x 0y 0w 50h" } },
	{ MODKEY|ShiftMask,             XK_Up,               moveresize,          {.v = "0x 0y 0w -50h" } },
	{ MODKEY|ShiftMask,             XK_Right,            moveresize,          {.v = "0x 0y 50w 0h" } },
	{ MODKEY|ShiftMask,             XK_Left,             moveresize,          {.v = "0x 0y -50w 0h" } },
	{ MODKEY|ControlMask,           XK_Up,               moveresizeedge,      {.v = "t"} },
	{ MODKEY|ControlMask,           XK_Down,             moveresizeedge,      {.v = "b"} },
	{ MODKEY|ControlMask,           XK_Left,             moveresizeedge,      {.v = "l"} },
	{ MODKEY|ControlMask,           XK_Right,            moveresizeedge,      {.v = "r"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Up,               moveresizeedge,      {.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Down,             moveresizeedge,      {.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Left,             moveresizeedge,      {.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask, XK_Right,            moveresizeedge,      {.v = "R"} },
	{ MODKEY,                       XK_0,                view,                {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,                tag,                 {.ui = ~0 } },
	{ MODKEY,                       XK_comma,            focusmon,            {.i = -1 } },
	{ MODKEY,                       XK_period,           focusmon,            {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,            tagmon,              {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period,           tagmon,              {.i = +1 } },
	{ MODKEY,                       XK_a,                togglealttag,        {0} },
	TAGKEYS(                        XK_1,                                     0)
	TAGKEYS(                        XK_2,                                     1)
	TAGKEYS(                        XK_3,                                     2)
	TAGKEYS(                        XK_4,                                     3)
	TAGKEYS(                        XK_5,                                     4)
	TAGKEYS(                        XK_6,                                     5)
	TAGKEYS(                        XK_7,                                     6)
	TAGKEYS(                        XK_8,                                     7)
	TAGKEYS(                        XK_9,                                     8)
	{ MODKEY|ShiftMask,             XK_q,                quit,                {1} },
	{ MODKEY|ControlMask|ShiftMask, XK_q,                quit,                {0} },
	{ MODKEY,                       XK_s,                focusurgent,         {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        layoutmenu,     {0} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkRootWin,           0,              Button3,        spawn,          SHCMD("xmenu.sh")},
	{ ClkStatusText,        0,              Button1,        sigdwmblocks,   {.i = 1} },
	{ ClkStatusText,        0,              Button2,        sigdwmblocks,   {.i = 2} },
	{ ClkStatusText,        0,              Button3,        sigdwmblocks,   {.i = 3} },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};


