display-kiosk
=============
Display a website in a stripped down kiosk.

Screenshot
----------
![Screenshot of the display-kiosk application](http://niklasf.github.io/display-kiosk/screenshot.png)

Features
--------

 - **Password check** with the current user, before the window can be closed or
   settings changed.
 - Switch between **full screen mode and window mode**.
 - **Hide the cursor**.
 - **Scroll automatically** at configurable speed.
 - **Configurable** via UI and config file.
 - Respects system **proxy** settings.

Installation
------------
Add my public key.

```
$ wget http://niklasf.github.io/display-kiosk/niklas.fiekas@tu-clausthal.de.gpg.key
$ sudo apt-key add niklas.fiekas@tu-clausthal.de.gpg.key
```

Append the following APT lines to `/etc/apt/sources.list` or a file in
`/etc/apt/sources.list.d/`.

```
deb http://niklasf.github.io/display-kiosk squeeze main
deb-src http://niklasf.github.io/display-kiosk squeeze main
```

Install the package.

```
$ sudo apt-get update
$ sudo apt-get install display-kiosk
```

Launch for the first time and configure. If you run in fullscreen mode most
window managers allow closing the kiosk with `ALT + F4` or `CTRL + Q`. The
password prompted for is the current users password.

```
$ display-kiosk
```

Autostarting on LXDE (used as the default desktop environment of the Raspberry Pi)
----------------------------------------------------------------------------------
One practical use of display-kiosk is to display news on a public monitor
connected to a [Raspberry Pi](http://www.raspberrypi.org/). Configure the
Raspberry to automatically start a GUI session on startup. Then you can
autostart display-kiosk via

```
$ mkdir -p ~/.config/autostart/
$ ln -s /usr/share/applications/display-kiosk.desktop ~/.config/autostart/display-kiosk.desktop
```

License
-------
display-kiosk is licensed under the GPL-3. See the
[debian/copyright](/debian/copyright) file for details. Feel free to use, drop
an issue or submit a pull request.
