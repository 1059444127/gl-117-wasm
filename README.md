![GL-117](title.png)

[![License: GPL 2.0 or later][license-img]][license-spdx]

GL-117 is an arcade action flight simulator written in C++ and OpenGL. It was
originally developed from 2002-2005 by Thomas A. Drexl and others. It is
distributed under the terms of the GNU General Public License v2. This is an
import of the original sourceforge CVS repository with minor fixes and an
update to modern tooling (meson, SDL2).  The original project website with
screenshots is gone, but an archive can be seen [here][gl-117-old].

## Building

Requirements are:

- meson
- SDL2
- SDL2_Mixer
- SDL2_Net
- OpenGL
- GLU

Steps to build:

```sh
meson build
ninja -C build
sudo ninja -C build install
```

## Credits

The original credits are as follows:

- Thomas Drexl (developer)
- Jean-Marc Le Peuvedic (developer)
- Eugene Andreeschev (developer and designer)
- Eric Cheung (texture designer)
- Piotr Pawlow (lens flares, bugfixing)
- Louens Veen (mouse interface, landscape improvements)
- Arne Reiners (action physics model, cockpit improvements)
- Norbert Drexl (moon terrain)
- James Stone (cool soundtrack => version 2.0)
- Bernhard Kaindl (SuSE)
- Cedric Delfosse (Debian)

Additional credits:

- Jason Francis (modernization, bugfixing)

## See Also

- [Linux Air Combat][lac] - A rewrite of GL-117 but with a realistic WWII flight sim theme

[gl-117-old]: https://web.archive.org/web/20080904212658/http://www.heptargon.de/gl-117/gl-117.html
[lac]: https://askmisterwizard.com/2019/LinuxAirCombat/LinuxAirCombat.htm

[license-img]:  https://img.shields.io/badge/License-GPL%202.0%20or%20later-blue.svg?logo=gnu
[license-spdx]: https://spdx.org/licenses/GPL-2.0-or-later.html
