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

Building from source
--------------------

```
sudo apt-get installl ibqt4-dev libqtwebkit-dev
qmake display-kiosk.pro
make
```

Configuring
-----------

See `/usr/share/doc/display-kiosk/examples/xinitrc` for an example `~/.xinitrc`
file. Do not forget to make it executable.

Put `su - username -c startx` in `/etc/rc.local` (usually before `exit 0`) and
`update-rc.d rc.local defaults`.

License
-------
display-kiosk is licensed under the GPL-3. See the
[debian/copyright](/debian/copyright) file for details. Feel free to use, drop
an issue or submit a pull request.
