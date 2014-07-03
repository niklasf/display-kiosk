display-kiosk
=============

Display a website in a stripped down kiosk. Display kiosk can:

 - **Hide the cursor**.
 - **Hide the toolbar**, **hide the statusbar**.
 - **Scroll automatically** at configurable speed.
 - **Reload automatically** at a configurable interval. If possible this is
   done **at a good moment**, i.e. when the application is idle or autoscrolling
   has reached the end.
 - Respect system **proxy** settings.

Synopsis
--------

    display-kiosk <url>
        [--hide-cursor]
        [--hide-status] [--hide-toolbar]
        [--prevent-close]
        [--auto-scroll
            [--auto-scroll-delta <pixels>]
            [--auto-scroll-interval <milliseconds>] ]
        [--auto-reload <seconds>]
        [--window]
        [--reset-text <reset-text>]

Have a look at the manpage or `display-kiosk --help` for details.

Installation
------------

Add the
[public key for the package archive](https://iserv-trg-oha.de/apt/apt.gpg.key).

    $ wget https://iserv-trg-oha.de/apt/apt.gpg.key
    $ sudo apt-key add apt.gpg.key

Append the following APT lines to `/etc/apt/sources.list` or a file in
`/etc/apt/sources.list.d/`.

    deb http://iserv-trg-oha.de/apt/ wheezy main
    deb-src http://iserv-trg-oha.de/apt/ wheezy main

Install the package.

    $ sudo apt-get update
    $ sudo apt-get install display-kiosk

Building from source
--------------------

    $ sudo apt-get install libqt4-dev libqtwebkit-dev
    $ qmake display-kiosk.pro
    $ make

Configuring
-----------

See `/usr/share/doc/display-kiosk/examples/xsession` for an example
`~/.xsession` or `~/.xinitrc` file. A suggested minimal window manager is
[nodm](http://www.enricozini.org/sw/nodm/). It just automatically starts the
session for the user configured in `/etc/defaults/nodm`.

A poor mans solution can be `su - username -c startx` in `/etc/rc.local`
(usually before `exit 0`) and `update-rc.d rc.local defaults`.

License
-------

display-kiosk is licensed under the GPL-3. Feel free to use, drop an issue or
submit a pull request.
