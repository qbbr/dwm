/* See LICENSE file for copyright and license details. */
#include "gaplessgrid.c"
#include "bstack.c"
#include "movestack.c"

/* appearance */
static const unsigned int borderpx       = 2;   /* border pixel of windows */
static const unsigned int snap           = 32;  /* snap pixel */
static const unsigned int systraypinning = 0;   /* 0: sloppy systray follows selected monitor, >0: pin systray to monitor X */
static const unsigned int systrayonleft  = 0;   /* 0: systray in the right corner, >0: systray on left of status text */
static const unsigned int systrayspacing = 4;   /* systray spacing */
static const int systraypinningfailfirst = 1;   /* 1: if pinning fails, display systray on the first monitor, False: display systray on the last monitor*/
static const int showsystray             = 1;   /* 0 means no systray */
static const int showbar                 = 1;   /* 0 means no bar */
static const int topbar                  = 1;   /* 0 means bottom bar */
static const unsigned int bottommargin   = 20;  /* Margin at the bottom for another bar */
static const char *fonts[]               = { "Hack:size=10", "FontAwesome:size=10" };
static const char dmenufont[]            = "Hack:size=10";
static const char col_gray1[]            = "#002B36";
static const char col_gray2[]            = "#073642";
static const char col_gray3[]            = "#657B83";
static const char col_gray4[]            = "#EEE8D5";
static const char col_cyan[]             = "#268BD2";
static const char col_yellow[]           = "#B58900";
static const char *colors[][3]           = {
	/*                 fg         bg          border     */
	[SchemeNorm]   = { col_gray3, col_gray1,  col_gray2  },
	[SchemeSel]    = { col_gray4, col_cyan,   col_cyan   },
	[SchemeUrgent] = { col_gray4, col_yellow, col_yellow },
};

/* tagging */
static const char *tags[] = { "1:term", "2:dev", "3:web", "q:mail", "w:chat", "e:mdia", "a:gile", "s:ss", "d:bug" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class                instance    title            tags mask     isfloating   monitor */
	{ "xterm-float",        NULL,       NULL,            0,            True,        -1 },
	{ "MPlayer",            NULL,       NULL,            0,            True,        -1 },
	{ "mpv",                NULL,       NULL,            0,            True,        -1 },
	{ "feh",                NULL,       NULL,            0,            True,        -1 },
	{ "Osmo",               NULL,       NULL,            0,            True,        -1 },
	{ "Lxappearance",       NULL,       NULL,            0,            True,        -1 },
	{ "Gimp",               NULL,       NULL,            0,            True,        -1 },
	{ "qemu",               NULL,       NULL,            0,            True,        -1 },
	{ "mupdf",              NULL,       NULL,            0,            True,        -1 },
	{ "Pgadmin3",           NULL,       NULL,            0,            True,        -1 },
	{ "Gcolor3",            NULL,       NULL,            0,            True,        -1 },
	// 1:term
	{ "htop",               NULL,       NULL,            0,            False,        2 },
	// 2:dev
	{ "jetbrains-idea",     NULL,       NULL,            1 << 1,       False,       1  },
	{ "jetbrains-phpstorm", NULL,       NULL,            1 << 1,       False,       1  },
	{ "jetbrains-pycharm",  NULL,       NULL,            1 << 1,       False,       1  },
	{ "Fritzing",           NULL,       NULL,            1 << 1,       False,       1  },
	{ "java-lang-Thread",   NULL,       NULL,            1 << 1,       True,        1  },
	// 3:web
	{ "firefox",            NULL,       NULL,            1 << 2,       False,       -1 },
	{ "Tor Browser",        NULL,       NULL,            1 << 2,       False,       -1 },
	{ "Firefox-esr",        NULL,       NULL,            1 << 2,       False,       -1 },
	{ "firefox-aurora",     NULL,       NULL,            1 << 2,       False,       0  },
	{ "Chromium",           NULL,       NULL,            1 << 2,       False,       -1 },
	// q:mail
	{ "Icedove",            NULL,       NULL,            1 << 3,       False,       -1 },
	{ "Thunderbird",        NULL,       NULL,            1 << 3,       False,       -1 },
	{ "Evolution",          NULL,       NULL,            1 << 3,       False,       -1 },
	{ NULL,                 NULL,       "mutt",          1 << 3,       False,       1  },
	// w:chat
	{ NULL,                 NULL,       "weechat",       1 << 4,       False,       -1 },
	{ NULL,                 NULL,       "mcabber",       1 << 4,       False,       1  },
	{ "Pidgin",             NULL,       NULL,            1 << 4,       False,       -1 },
	{ "TelegramDesktop" ,   NULL,       NULL,            1 << 4,       False,       -1 },
	{ "Skype",              NULL,       NULL,            1 << 4,       False,       -1 },
	{ "discord",            NULL,       NULL,            1 << 4,       False,       -1 },
	// e:mdia
	{ NULL,                 NULL,       "newsbeuter",    1 << 5,       False,       -1 },
	{ NULL,                 NULL,       "ncmpcpp",       1 << 5,       False,       1  },
	// a:gile
	// s:ss
	{ "Fpm2",               NULL,       NULL,            1 << 7,       False,       -1 },
	{ "keepassx",           NULL,       NULL,            1 << 7,       False,       -1 },
	{ "KeePassXC",          NULL,       NULL,            1 << 7,       False,       -1 },
	{ NULL,                 NULL,       "rtorrent",      1 << 7,       False,       -1 },
	// d:bug
	{ "dmesg",              NULL,       NULL,            1 << 8,       False,        1 },
};

/* layout(s) */
static const float mfact     = 0.65; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */
static const int lockfullscreen = 1; /* 1 will force focus on the fullscreen window */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "###",      gaplessgrid }, /* first entry is default */
	{ "[]=",      tile },
	{ "><>",      NULL },        /* no layout function means floating behavior */
	{ "[M]",      monocle },
	{ "TTT",      bstack },
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
static const char *dmenucmd[]       = { "dmenu_run", "-p", "cmd:", "-b", "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL };
static const char *termcmd[]        = { "uxterm", NULL };
static const char *roficmd[]        = { "rofi", "-show", "combi", NULL };
static const char *dmenumpccmd[]    = { "dmenu-mpc-playlist", NULL };
static const char *slockcmd[]       = { "slock", NULL };
static const char *screenshotcmd[]  = { "scrot", NULL };
static const char *volupcmd[]       = { "volume-control", "-i", "5", "--dzen", NULL };
static const char *voldowncmd[]     = { "volume-control", "-d", "5", "--dzen", NULL };
static const char *voltogglecmd[]   = { "volume-control", "-t", "--dzen", NULL };
static const char *mixertogglecmd[] = { "amixer", "-c", "0", "set", "Headphone", "toggle", "&&", "amixer", "-c", "0", "set", "Front", "toggle", NULL };
static const char *mpdnextcmd[]     = { "mpc", "next", NULL };
static const char *mpdprevcmd[]     = { "mpc", "prev", NULL };
static const char *mpdtogglecmd[]   = { "mpc", "toggle", NULL };
static const char *mpdseekpcmd[]    = { "mpc", "seek", "+5", NULL };
static const char *mpdseekmcmd[]    = { "mpc", "seek", "-5", NULL };

static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } },
	{ MODKEY,                       XK_F2,     spawn,          {.v = roficmd } },
	{ MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } },
	{ MODKEY,                       XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY,                       XK_i,      incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_y,      incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_u,      resetnmaster,   {0} },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_Return, zoom,           {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY|ShiftMask,             XK_c,      killclient,     {0} },
	{ MODKEY,                       XK_g,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY,                       XK_m,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_o,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY,                       XK_space,  setlayout,      {0} },
	{ MODKEY|ShiftMask,             XK_space,  togglefloating, {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_comma,  focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_period, focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_comma,  tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_period, tagmon,         {.i = +1 } },
	{ ControlMask,                  0xff13,    spawn,          {.v = slockcmd } },
	{ ControlMask,                  0xff61,    spawn,          {.v = screenshotcmd } },
	{ ControlMask,                  0xffaa,    spawn,          {.v = volupcmd } },
	{ ControlMask,                  0xffaf,    spawn,          {.v = voldowncmd } },
	{ ControlMask,                  0xffad,    spawn,          {.v = voltogglecmd } },
	{ ControlMask,                  0xffab,    spawn,          {.v = mixertogglecmd } },
	{ ControlMask,                  0xff98,    spawn,          {.v = mpdnextcmd } },
	{ ControlMask,                  0xff96,    spawn,          {.v = mpdprevcmd } },
	{ ControlMask,                  0xff9d,    spawn,          {.v = mpdtogglecmd } },
	{ ControlMask,                  0xff95,    spawn,          {.v = mpdseekmcmd } },
	{ ControlMask,                  0xff9a,    spawn,          {.v = mpdseekpcmd } },
	{ ControlMask,                  0xff9e,    spawn,          {.v = dmenumpccmd } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_q,                      3)
	TAGKEYS(                        XK_w,                      4)
	TAGKEYS(                        XK_e,                      5)
	TAGKEYS(                        XK_a,                      6)
	TAGKEYS(                        XK_s,                      7)
	TAGKEYS(                        XK_d,                      8)
	{ MODKEY|ShiftMask|ControlMask, XK_q,      quit,           {0} },
	{ MODKEY|ShiftMask|ControlMask, XK_r,      quit,           {1} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkMonNum,            0,              Button1,        focusmon,       {.i = +1} },
	{ ClkMonNum,            0,              Button3,        focusmon,       {.i = -1} },
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

