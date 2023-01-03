# dwm 6.4 (patched)

 * [dwm](https://dwm.suckless.org/)
 * original [README](README) file
 * [dzenbar](https://github.com/qbbr/dzenbar)

[![qbbr-dwm](https://i.imgur.com/6XEXVWcl.png)](https://i.imgur.com/6XEXVWc.png)

## Requirements

 * [dmenu](http://tools.suckless.org/dmenu/) (optional)
 * [slock](http://tools.suckless.org/slock) (optional, see `keys[]` in [config](config.h.dist#L125))
 * [skb](https://github.com/polachok/skb) (see [statusdwm](bin/statusdwm))
 * [volume-control](https://github.com/qbbr/dotfiles/blob/master/bin/volume-control) (optional)
 * [dmenu-mpc-playlist](https://github.com/qbbr/dotfiles/blob/master/bin/dmenu-mpc-playlist) (optional)

## Installation

```bash
git clone https://github.com/qbbr/dwm.git
cd dwm/
sudo make install clean
```

add to `.xinitrc`

```bash
/path/to/dwm/bin/startdwm
```

## Overview

status2d with [skb](https://github.com/polachok/skb):

![dwm statusbar skb colored](https://i.imgur.com/7syG1Ni.png)

applied patches:

 * [status2d](https://dwm.suckless.org/patches/status2d/)
 * [systray](https://dwm.suckless.org/patches/systray/)
 * [gaplessgrid](https://dwm.suckless.org/patches/gaplessgrid/)
 * resetnmaster
 * bottommargin

hotkeys:

 * `[Ctrl] + [Pause]`  - lock screen (slock)
 * `[Ctrl] + [PrtScr]` - take screenshot (scrot)
 * `[Ctrl] + [Num /]`  - volume down (volume-control)
 * `[Ctrl] + [Num *]`  - volume up (volume-control)
 * `[Ctrl] + [Num -]`  - volume mute/unmute (volume-control)
 * `[Ctrl] + [Num +]`  - switch Headphone <-> Front output
 * `[Ctrl] + [Num 5]`  - mpd pause/play (mpc)
 * `[Ctrl] + [Num 4]`  - mpd previous track (mpc)
 * `[Ctrl] + [Num 6]`  - mpd next track (mpc)
 * `[Ctrl] + [Num 7]`  - mpd seek backward (mpc)
 * `[Ctrl] + [Num 9]`  - mpd seek forward (mpc)
 * `[Ctrl] + [Num 0]`  - show mpc playlist (dmenu-mpc-playlist)
