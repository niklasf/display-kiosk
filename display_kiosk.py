#!/usr/bin/env python
# -*- coding: utf-8 -*-

__author__ = "Niklas Fiekas"
__copyright__ = "Copyright 2013, Niklas Fiekas"
__license__ = "GPL"
__version__ = "0.0.1"
__maintainer__ = "Niklas Fiekas"
__email__ = "niklas.fiekas@tu-clausthal.de"
__status__ = "Development"


import sys

from PySide.QtCore import *
from PySide.QtGui import *
from PySide.QtWebKit import *


class Kiosk(QWebView):
    def __init__(self):
        super(Kiosk, self).__init__()

        self.settings = QSettings(QSettings.IniFormat, QSettings.UserScope, "display_kiosk", "display_kiosk")

        if not self.settings.value("Url"):
            SettingsDialog(self).exec_()


class SettingsDialog(QDialog):
    def __init__(self, kiosk, parent=None):
        super(SettingsDialog, self).__init__(parent)

        layout = QGridLayout()

        self.fullScreenBox = QCheckBox("Use fullscreen")

        self.setLayout(layout)
        self.setWindowTitle("Display kiosk settings")


if __name__ == "__main__":
    app = QApplication(sys.argv)

    settings = QSettings(QSettings.IniFormat, QSettings.UserScope, "display_kiosk", "display_kiosk")
    if settings.value("Url") or SettingsDialog(settings).exec_():
        kiosk = Kiosk(settings)
        kiosk.show()

        sys.exit(app.exec_())
