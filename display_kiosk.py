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
import PAM
import getpass

from PySide.QtCore import *
from PySide.QtGui import *
from PySide.QtWebKit import *
from PySide.QtNetwork import *


class Kiosk(QWebView):
    def __init__(self, settings):
        super(Kiosk, self).__init__()

        self.settings = settings

        self.timer = QTimer()
        self.timer.timeout.connect(self.onTimeout)

        self.reconfigure()

    def reconfigure(self):
        if self.settings.value("FullScreen") == "true":
            self.showFullScreen()
        else:
            self.showNormal()

        if self.settings.value("HideCursor") == "true":
            self.setCursor(Qt.BlankCursor)
        else:
            self.unsetCursor()

        if self.settings.value("AutoScroll") == "true":
            self.timer.start(int(self.settings.value("AutoScrollInterval")))
        else:
            self.timer.stop()

        proxy = QNetworkProxyFactory.systemProxyForQuery(QNetworkProxyQuery(QUrl(self.settings.value("Url"))))
        if proxy:
            QNetworkProxy.setApplicationProxy(proxy[0])
        self.reloadPage()

    def reloadPage(self):
        self.load(QUrl(self.settings.value("Url")))

    def onTimeout(self):
        delta = int(self.settings.value("AutoScrollDelta"))
        frame = self.page().mainFrame()
        if frame.scrollBarValue(Qt.Vertical) >= frame.scrollBarMaximum(Qt.Vertical):
            self.reloadPage()
        else:
            frame.setScrollBarValue(Qt.Vertical, frame.scrollBarValue(Qt.Vertical) + delta)

    def closeEvent(self, event):
        unlockDialog = UnlockDialog(self.settings, self)
        if not unlockDialog.exec_():
            if unlockDialog.settingsChanged:
                self.reconfigure()
            event.ignore()


class UnlockDialog(QDialog):
    def __init__(self, settings, parent):
        super(UnlockDialog, self).__init__(parent)
        self.settings = settings
        self.settingsChanged = False

        layout = QGridLayout()

        layout.addWidget(QLabel("User password:"), 0, 0)
        self.passwordBox = QLineEdit()
        self.passwordBox.setEchoMode(QLineEdit.Password)
        layout.addWidget(self.passwordBox, 0, 1)

        settingsButton = QPushButton("Settings")
        settingsButton.clicked.connect(self.onSettingsButtonClicked)
        layout.addWidget(settingsButton, 1, 0)

        buttonBox = QHBoxLayout()
        cancelButton = QPushButton("Cancel")
        cancelButton.clicked.connect(self.reject)
        buttonBox.addWidget(cancelButton)
        closeButton = QPushButton("Close")
        closeButton.clicked.connect(self.onCloseButtonClicked)
        closeButton.setDefault(True)
        buttonBox.addWidget(closeButton)
        layout.addLayout(buttonBox, 1, 1, 1, 1, Qt.AlignRight)

        self.setLayout(layout)
        self.setWindowTitle("Unlock to close")

    def pamConv(self, auth, queryList, userData):
        resp = []

        for i in range(len(queryList)):
            query, mode = queryList[i]
            if mode == PAM.PAM_PROMPT_ECHO_ON:
                resp.append((getpass.getuser(), 0))
            elif mode == PAM.PAM_PROMPT_ECHO_OFF:
                resp.append((self.passwordBox.text(), 0))
            elif mode == PAM.PAM_PROMPT_ERROR_MSG or mode == PAM.PAM_PROMPT_TEXT_INFO:
                resp.append(("", 0))
            else:
                return None

        return resp

    def checkPassword(self):
        try:
            auth = PAM.pam()
            auth.start("passwd")
            auth.set_item(PAM.PAM_CONV, self.pamConv)

            auth.authenticate()
            auth.acct_mgmt()
        except PAM.error, resp:
            QMessageBox.warning(self, "Did not unlock", str(resp))
            self.passwordBox.selectAll()
            return False
        else:
            return True

    def onCloseButtonClicked(self):
        if self.checkPassword():
            self.accept()

    def onSettingsButtonClicked(self):
        if self.checkPassword():
            self.settingsChanged = SettingsDialog(self.settings, self).exec_()
            self.reject()

    def sizeHint(self):
        # Make the dialog a little wider than nescessary.
        normalSize = super(UnlockDialog, self).sizeHint()
        return QSize(normalSize.width() + 50, normalSize.height())


class SettingsDialog(QDialog):
    def __init__(self, settings, parent=None):
        super(SettingsDialog, self).__init__(parent)
        self.settings = settings

        layout = QVBoxLayout()

        box = QHBoxLayout()
        box.addWidget(QLabel("URL:"))
        self.urlBox = QLineEdit()
        box.addWidget(self.urlBox)
        layout.addLayout(box)

        self.fullScreenBox = QCheckBox("Use full screen mode\n(exit with ALT+F4 for most window managers)")
        layout.addWidget(self.fullScreenBox)

        self.hideCursorBox = QCheckBox("Hide cursor")
        layout.addWidget(self.hideCursorBox)

        box = QHBoxLayout()
        self.autoScrollBox = QCheckBox("Automatically scroll")
        self.autoScrollBox.stateChanged.connect(self.onAutoScrollStateChanged)
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
        self.reset()

    def reset(self):
        self.urlBox.setText(self.settings.value("Url", "http://"))
        self.fullScreenBox.setCheckState(Qt.Checked if self.settings.value("FullScreen", "true") == "true" else Qt.Unchecked)
        self.hideCursorBox.setCheckState(Qt.Checked if self.settings.value("HideCursor", "false") == "true" else Qt.Unchecked)
        self.autoScrollBox.setCheckState(Qt.Checked if self.settings.value("AutoScroll", "false") == "true" else Qt.Unchecked)
        self.autoScrollDeltaBox.setValue(int(self.settings.value("AutoScrollDelta", 300)))
        self.autoScrollIntervalBox.setValue(int(self.settings.value("AutoScrollInterval", 5000)))
        self.onAutoScrollStateChanged(self.autoScrollBox.checkState())

    def onAccepted(self):
        url = QUrl.fromUserInput(self.urlBox.text())
        if not url.isValid():
            QMessageBox.warning(self, self.windowTitle(), "The given URL is invalid.")
            self.urlBox.selectAll()
            return

        self.settings.setValue("Url", url.toString())
        self.settings.setValue("FullScreen", "true" if self.fullScreenBox.isChecked() else "false")
        self.settings.setValue("HideCursor", "true" if self.hideCursorBox.isChecked() else "false")
        self.settings.setValue("AutoScroll", "true" if self.autoScrollBox.isChecked() else "false")
        self.settings.setValue("AutoScrollDelta", self.autoScrollDeltaBox.value())
        self.settings.setValue("AutoScrollInterval", self.autoScrollIntervalBox.value())
        self.accept()

    def onAutoScrollStateChanged(self, state):
        self.autoScrollDeltaBox.setEnabled(state == Qt.Checked)
        self.autoScrollIntervalBox.setEnabled(state == Qt.Checked)


if __name__ == "__main__":
    app = QApplication(sys.argv)

    settings = QSettings(QSettings.IniFormat, QSettings.UserScope, "display_kiosk", "display_kiosk")
    if settings.value("Url") or SettingsDialog(settings).exec_():

        kiosk = Kiosk(settings)

        sys.exit(app.exec_())
