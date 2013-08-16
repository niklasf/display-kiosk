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
    def __init__(self, settings):
        super(Kiosk, self).__init__()

        self.settings = settings

"""
    def reloadPage(self):
        self.load(QUrl(self.settings.value("Url")))

    def onTimeout(self):
        delta = self.settings.

"""

class SettingsDialog(QDialog):
    def __init__(self, kiosk, parent=None):
        super(SettingsDialog, self).__init__(parent)

        layout = QVBoxLayout()

        box = QHBoxLayout()
        box.addWidget(QLabel("URL:"))
        self.urlBox = QLineEdit()
        box.addWidget(self.urlBox)
        layout.addLayout(box)

        self.fullScreenBox = QCheckBox("Use full screen mode")
        layout.addWidget(self.fullScreenBox)

        self.hideCursorBox = QCheckBox("Hide cursor")
        layout.addWidget(self.hideCursorBox)

        box = QHBoxLayout()
        self.autoScrollBox = QCheckBox("Automatically scroll")
        box.addWidget(self.autoScrollBox)
        self.autoScrollDeltaBox = QSpinBox()
        self.autoScrollDeltaBox.setMinimum(0)
        self.autoScrollDeltaBox.setMaximum(100000)
        self.autoScrollDeltaBox.setSuffix("px")
        box.addWidget(self.autoScrollDeltaBox)
        box.addWidget(QLabel("every"))
        self.autoScrollIntervalBox = QSpinBox()
        self.autoScrollIntervalBox.setMinimum(10)
        self.autoScrollIntervalBox.setMaximum(100000)
        self.autoScrollIntervalBox.setSuffix("ms")
        box.addWidget(self.autoScrollIntervalBox)
        layout.addLayout(box)

        buttons = QDialogButtonBox(QDialogButtonBox.Ok | QDialogButtonBox.Cancel)
        buttons.rejected.connect(self.reject)
        buttons.accepted.connect(self.onAccepted)
        layout.addWidget(buttons)

        self.setLayout(layout)
        self.setWindowTitle("Display kiosk settings")

    def onAccepted(self):
        self.accept()


if __name__ == "__main__":
    app = QApplication(sys.argv)

    settings = QSettings(QSettings.IniFormat, QSettings.UserScope, "display_kiosk", "display_kiosk")
    if settings.value("Url") or SettingsDialog(settings).exec_():
        kiosk = Kiosk(settings)
        kiosk.show()

        sys.exit(app.exec_())
