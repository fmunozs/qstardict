#############################################################################
# qstardict.pri - QStarDict, a StarDict clone written with using Qt         #
# Copyright (C) 2008 Alexander Rodin                                        #
#                                                                           #
# This program is free software; you can redistribute it and/or modify      #
# it under the terms of the GNU General Public License as published by      #
# the Free Software Foundation; either version 2 of the License, or         #
# (at your option) any later version.                                       #
#                                                                           #
# This program is distributed in the hope that it will be useful,           #
# but WITHOUT ANY WARRANTY; without even the implied warranty of            #
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             #
# GNU General Public License for more details.                              #
#                                                                           #
# You should have received a copy of the GNU General Public License along   #
# with this program; if not, write to the Free Software Foundation, Inc.,   #
# 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               #
#############################################################################

VERSION = 1.0

#QT =     core     gui     network     xml
QT= core
CONFIG +=  qt warn_on release libtuiclient

CONFIG += link_pkgconfig
PKGCONFIG += glib-2.0
LIBS += -lz
#-lX11

unix:DEFINES += HAVE_MMAP
unix:isEmpty(NO_DBUS):!contains(QT_CONFIG, dbus): NO_DBUS = 1
unix:isEmpty(NO_DBUS):CONFIG += qdbus
unix:isEmpty(NO_DBUS):DEFINES += QSTARDICT_WITH_DBUS

isEmpty(ENABLED_PLUGINS):ENABLED_PLUGINS="stardict"

unix {
        isEmpty(INSTALL_PREFIX):INSTALL_PREFIX=/opt
        isEmpty(BIN_DIR):BIN_DIR=$$INSTALL_PREFIX/bin
      #  isEmpty(DATA_DIR):DATA_DIR=$$INSTALL_PREFIX/share/qstardict
      #  isEmpty(NO_TRANSLATIONS):isEmpty(TRANSLATIONS_DIR):TRANSLATIONS_DIR=$$DATA_DIR/translations
        isEmpty(LIB_DIR):LIB_DIR=$$INSTALL_PREFIX/lib/qstardict
        isEmpty(PLUGINS_DIR):PLUGINS_DIR=$$LIB_DIR/plugins
       # isEmpty(DOCS_DIR):DOCS_DIR=$$INSTALL_PREFIX/share/doc/qstardict

        DEFINES += QSTARDICT_VERSION=\\\"$$VERSION\\\"
        DEFINES += QSTARDICT_INSTALL_PREFIX=\\\"$$INSTALL_PREFIX\\\"
        DEFINES += QSTARDICT_BIN_DIR=\\\"$$BIN_DIR\\\"
        DEFINES += QSTARDICT_DATA_DIR=\\\"$$DATA_DIR\\\"
        isEmpty(NO_TRANSLATIONS):DEFINES += QSTARDICT_TRANSLATIONS_DIR=\\\"$$TRANSLATIONS_DIR\\\"
        DEFINES += QSTARDICT_PLUGINS_DIR=\\\"$$PLUGINS_DIR\\\"

} 

#system(echo "ENABLED_PLUGINS = $$ENABLED_PLUGINS" >plugins/plugins.pri)
