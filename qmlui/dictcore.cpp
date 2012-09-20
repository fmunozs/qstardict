/*****************************************************************************
 * dictcore.cpp - QStarDict, a StarDict clone written using Qt               *
 * Copyright (C) 2008 Alexander Rodin                                        *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the GNU General Public License as published by      *
 * the Free Software Foundation; either version 2 of the License, or         *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the             *
 * GNU General Public License for more details.                              *
 *                                                                           *
 * You should have received a copy of the GNU General Public License along   *
 * with this program; if not, write to the Free Software Foundation, Inc.,   *
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.               *
 *****************************************************************************/

#include "dictcore.h"

#include <QFileInfoList>
#include <QFileInfo>
#include <QDir>
#include <QRegExp>
#include <QSettings>
#include <QDebug>
#include <QPluginLoader>
#include "../plugins/dictplugin.h"

namespace QStarDict
{

    DictCore::DictCore(QObject *parent)
        : QObject(parent)
    {
        loadSettings();
    }

    DictCore::~DictCore()
    {
        saveSettings();
        foreach (QPluginLoader *loader, m_plugins)
        {
            delete loader->instance();
            delete loader;
        }
    }

    void DictCore::setThemeStatus(bool m) {
        themeInverted = m;
       // qDebug() << "tema seteado en " << m;

    }

    bool DictCore::getThemeStatus() {
      //  qDebug() << themeInverted;
        return themeInverted;
    }

    bool DictCore::isTranslatable(const QString &word)
    {
        for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
        {
            if (! m_plugins.contains(i->plugin()))
                continue;
            if (qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance())->isTranslatable(i->name(), word))
                return true;
        }
        return false;
    }

    QString DictCore::translate(const QString &word)
    {
        QString simplifiedWord = word.simplified();
        QString result;

        QString tr;
        for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
        {
            if (! m_plugins.contains(i->plugin()))
                continue;
            DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance());
            if (! plugin->isTranslatable(i->name(), simplifiedWord)) {
                continue;
            }
            DictPlugin::Translation translation = plugin->translate(i->name(), simplifiedWord);


            result += "<p>"
                "<font class=\"dict_name\">" + translation.dictName() + "</font><br>"
                "<font class=\"title\">" + translation.title() + "</font><br>"
                + translation.translation() + "</p>";

        }

        if (result.isEmpty())
            return "Not found";

        result =  "<style type='text/css'> .transcription { font-weight: bold;} .dict_name { font-size: small; font-style: italic;} .title { font-size: large; font-weight: bold; } .abbreviature {font-style: italic;}</style>" + result;
        return result;
    }

    QStringList DictCore::findSimilarWords(const QString &word)
    {
        QString simplifiedWord = word.simplified();
        QStringList result;
        for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
        {
            if (! m_plugins.contains(i->plugin()))
                continue;
            DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i->plugin()]->instance());
            if (! plugin->features().testFlag(DictPlugin::SearchSimilar))
                continue;
            QStringList similar = plugin->findSimilarWords(i->name(), simplifiedWord);
            for (QStringList::const_iterator j = similar.begin(); j != similar.end(); ++j)
                if (! result.contains(*j, Qt::CaseInsensitive))
                    result << *j;
        }
        return result;
    }

    QStringList DictCore::availablePlugins() const
    {
        QStringList result;

        QFileInfoList files = QDir(QSTARDICT_PLUGINS_DIR).entryInfoList(QStringList("lib*.so"),
                      QDir::Files | QDir::NoDotAndDotDot);
        for (QFileInfoList::const_iterator i = files.begin(); i != files.end(); ++i)
            result << i->fileName().mid(3, i->fileName().length() - 6);

        return result;
    }

    void DictCore::setLoadedPlugins(const QStringList &loadedPlugins)
    {
        for (QHash <QString, QPluginLoader*>::iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
        {
            delete (*i)->instance();
            delete *i;
        }
        m_plugins.clear();

        for (QStringList::const_iterator i = loadedPlugins.begin(); i != loadedPlugins.end(); ++i)
        {

            QString pluginFilename = static_cast<QString>(QSTARDICT_PLUGINS_DIR) + "/" "lib" + *i + ".so";

            QPluginLoader *plugin = new QPluginLoader(pluginFilename);
            if (! plugin->load())
            {
                qWarning() << plugin->errorString();
                delete plugin;
                continue;
            }
            m_plugins[*i] = plugin;
        }
    }

    QList<DictCore::Dictionary> DictCore::availableDicts() const
    {
        QList<Dictionary> result;

        for (QHash<QString, QPluginLoader*>::const_iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
        {
            DictPlugin *plugin = qobject_cast<DictPlugin*>((*i)->instance());
            QStringList dicts = plugin->availableDicts();
            for (QStringList::const_iterator j = dicts.begin(); j != dicts.end(); ++j) {
                result << Dictionary(i.key(), *j);

            }
        }
        return result;
    }

    void DictCore::setLoadedDicts(const QList<Dictionary> &loadedDicts)
    {
        QHash<QString, QStringList> dicts;
        for (QList<Dictionary>::const_iterator i = loadedDicts.begin(); i != loadedDicts.end(); ++i)
            dicts[i->plugin()] << i->name();
        for (QHash<QString, QStringList>::const_iterator i = dicts.begin(); i != dicts.end(); ++i)
        {
            if (! m_plugins.contains(i.key()))
                continue;
            DictPlugin *plugin = qobject_cast<DictPlugin*>(m_plugins[i.key()]->instance());
            plugin->setLoadedDicts(*i);
            dicts[i.key()] = plugin->loadedDicts();
        }
        m_loadedDicts.clear();
        for (QList<Dictionary>::const_iterator i = loadedDicts.begin(); i != loadedDicts.end(); ++i)
            if (dicts.contains(i->plugin()) && dicts[i->plugin()].contains(i->name()))
                m_loadedDicts << *i;
    }

    void DictCore::saveSettings()
    {
        QSettings config("AppsBeford", "qstardict");
        config.setValue("DictCore/firstTime", false);
        config.setValue("DictCore/themeInverted", themeInverted);
        config.setValue("DictCore/loadedPlugins", loadedPlugins());
        QStringList rawDictsList;
        for (QList<Dictionary>::const_iterator i = m_loadedDicts.begin(); i != m_loadedDicts.end(); ++i)
            rawDictsList << i->plugin() << i->name();
       // config.setValue("DictCore/loadedDicts", rawDictsList);
    }

    void DictCore::loadSettings()
    {
        QSettings config("AppsBeford", "qstardict");
        firstRunVar = config.value("DictCore/firstTime", true).toBool();
        themeInverted = config.value("DictCore/themeInverted", false).toBool();

        setLoadedPlugins(config.value("DictCore/loadedPlugins", availablePlugins()).toStringList());
        QStringList rawDictsList = config.value("DictCore/loadedDicts").toStringList();
        if (rawDictsList.isEmpty())
            setLoadedDicts(availableDicts());
        else
        {
            QList<Dictionary> dicts;
            for (QStringList::const_iterator i = rawDictsList.begin(); i != rawDictsList.end(); i += 2)
                dicts << Dictionary(*i, *(i + 1));
            setLoadedDicts(dicts);
        }
    }

    void DictCore::reloadDicts()
    {
        QList<Dictionary> loaded;
        for (QHash<QString, QPluginLoader*>::const_iterator i = m_plugins.begin(); i != m_plugins.end(); ++i)
        {
            DictPlugin *plugin = qobject_cast<DictPlugin*>((*i)->instance());
            plugin->setLoadedDicts(plugin->loadedDicts());
            QStringList loadedNames = plugin->loadedDicts();
            for (QStringList::const_iterator j = loadedNames.begin(); j != loadedNames.end(); ++j)
                loaded << Dictionary(i.key(), *j);
        }
        QList<Dictionary> oldLoaded = m_loadedDicts;
        m_loadedDicts.clear();
        for (QList<Dictionary>::iterator i = oldLoaded.begin(); i != oldLoaded.end(); ++i)
            if (loaded.contains(*i))
                m_loadedDicts << *i;
    }

}
