display-kiosk
=============
Display a website in a stripped down kiosk.

Screenshot
----------
![Screenshot of the display-kiosk application](/screenshot.png)

Features
--------

 - **Password check** with the current user, before the window can be closed or
   settings changed.
 - Switch between **full screen mode and window mode**.
 - **Hide the cursor**.
 - **Scroll automatically** at configurable speed.
 - **Reload automatically** at a configurable interval.
 - **Configurable** via UI and config file.
 - Respects system **proxy** settings.

Installation
------------
Add my [public key](http://niklasf.github.io/display-kiosk/niklas.fiekas@tu-clausthal.de.gpg.key).

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

Disable display blanking on X11
-------------------------------
X11 might blank the display after a while of inactivity.
`/usr/share/doc/display-kiosk/examples/xsessionrc.gz` is an example for
`~/.xsessionrc` that disables display blanking.

Lock down Openbox-LXDE (used as the default window manager of the Raspberry Pi)
-------------------------------------------------------------------------------
The simple password check in the application can not prevent the user from
switching windows or opening a task manager. Some keyboard bindings have
to be disabled to make sure this does not happen.

Keyboard bindings are configured in `~/.config/openbox/lxde-rc.xml`. Disable
them yourself or use `/usr/share/doc/display-kiosk/examples/lxde-rc.xml.gz` if
you wouldn't overwrite custom settings anyway.

License
-------
display-kiosk is licensed under the GPL-3. See the
[debian/copyright](/debian/copyright) file for details. Feel free to use, drop
an issue or submit a pull request.
