display-kiosk
=============
Display a website in a stripped down kiosk.

Features
--------

 - **Hide the cursor**.
 - **Scroll automatically** at configurable speed.
 - **Reload automatically** at a configurable interval. If possible this is
   done **at a good moment**, i.e. when the application is idle or autoscrolling
   has reached the end.
 - **Configurable** via UI and config file.
 - Respects system **proxy** settings.

Installation
------------
Add the [public key for the package archive](https://iserv-trg-oha.de/apt/apt.gpg.key).

```
$ wget https://iserv-trg-oha.de/apt/apt.gpg.key
$ sudo apt-key add apt.gpg.key
```

Append the following APT lines to `/etc/apt/sources.list` or a file in
`/etc/apt/sources.list.d/`.

```
deb http://iserv-trg-oha.de/apt/ squeeze main
deb-src http://iserv-trg-oha.de/apt/ squeeze main
```

Install the package.

```
$ sudo apt-get update
$ sudo apt-get install display-kiosk
```

Configuring stand alone
-----------------------

See `/usr/share/doc/display-kiosk/examples/xinitrc` for an example `~/.xinitrc`
file. Do not forget to make it executable.

Put `su - username -c startx` in `/etc/rc.local` (usually before `exit 0`) and
`update-rc.d rc.local defaults`.

Configuring LXDE (used as the default desktop environment of the Raspberry Pi)
----------------------------------------------------------------------------------
Configure the Raspberry to automatically start a GUI session on startup. Then
you can autostart display-kiosk via

```
$ mkdir -p ~/.config/autostart/
$ ln -s /usr/share/applications/display-kiosk.desktop ~/.config/autostart/display-kiosk.desktop
```

X11 might blank the display after a while of inactivity. Put the following in
`~/.xsessionrc`:

```
# Disables screen-saver and display blanking.
xset s off
xset s noblank
xset -dpms
```

See `~/.config/openbox/lxde-rc.xml` for keyboard shortcuts you might want
to disable.

License
-------
display-kiosk is licensed under the GPL-3. See the
[debian/copyright](/debian/copyright) file for details. Feel free to use, drop
an issue or submit a pull request.
