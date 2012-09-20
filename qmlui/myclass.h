#ifndef MYCLASS_H
#define MYCLASS_H

#include <QProcess>
#include <TransferUI/Client>
#include <TransferUI/Transfer>
#include <QObject>

#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkAccessManager>

#include <QAbstractListModel>
#include <QDebug>
#include <QObject>
#include <QStringList>

#include "dictcore.h"
#include "wordlistmodel.h"
#include "dictlistmodel.h"

using namespace QStarDict;
using namespace TransferUI;

class MyClass : public QObject
{
    Q_OBJECT
public:
    WordListModel model;
    DictListModel dictModel;


    MyClass() : QObject()
    {
        if (!QDir("/home/user/MyDocs/dic").exists()) {
            QDir().mkdir("/home/user/MyDocs/dic");
        }

        m_dictCore = new DictCore();
        qDebug()<< m_dictCore->availablePlugins();
        m_dictCore->setLoadedPlugins(m_dictCore->availablePlugins());
        refreshDicts();

        m_dictCore->setLoadedDicts(dictList);
        client = new TransferUI::Client(this);
        if(!client->init()) {
            delete client;
        }

        connect(client,SIGNAL(cancelTransfer(Transfer*)), this,  SLOT(transferCancelled(Transfer*)));
        connect(client,SIGNAL(pauseTransfer(Transfer*)), this,  SLOT(transferPaused(Transfer*)));
        connect(client,SIGNAL(startTransfer(Transfer*)), this,   SLOT(transferResumed(Transfer*)));
        connect(client,SIGNAL(repairErrorTransfer(Transfer*)), this,   SLOT(transferErrorRepairRequested(Transfer*)));
    }

    void refreshDicts()
    {
        dictList = m_dictCore->availableDicts();
        dictModel.clearDicts();
        foreach(DictCore::Dictionary d, dictList) {
            dictModel.addDict(&d);
        }

        m_dictCore->setLoadedDicts(dictList);
    }

    DictCore *dictCore() const
    {
        return m_dictCore;
    }

    void setDict(DictCore *dict)
    {
        m_dictCore = dict;
    }

    ~MyClass() {
        delete m_dictCore;
        if(client!=0) {
            delete client;
        }
        client = 0;
        downloadTransfer = 0;
    }

    Q_INVOKABLE bool firstRun() {
        return m_dictCore->firstRunVar;
    }

    Q_INVOKABLE void clearWords()
    {
        model.clearWords();
    }

    Q_INVOKABLE bool getThemeStatus()
    {
        return m_dictCore->getThemeStatus();
    }

    Q_INVOKABLE void themeStatus(bool m)
    {
        m_dictCore->setThemeStatus(m);
    }

    Q_INVOKABLE QString translate(const QString &msg)
    {
        QString m = m_dictCore->translate(msg);
        return m;
    }

    Q_INVOKABLE void searchWord(const QString &msg)
    {
        model.clearWords();
        if (msg.length() > 1) {

            QStringList  lst;
            lst = m_dictCore->findSimilarWords(msg);
            foreach (QString str, lst) {
                model.addWord(new Word(str));
             }
        }
    }


    QString saveFileName(const QUrl &url)
    {
        QString path = url.path();
        QString basename = QFileInfo(path).fileName();

        if (basename.isEmpty())
            basename = "download";

        if (QFile::exists(basename)) {
            // already exists, don't overwrite
            int i = 0;
            basename += '.';
            while (QFile::exists(basename + QString::number(i)))
                ++i;

            basename += QString::number(i);
        }

        return basename;
    }

//--------------------



    Q_INVOKABLE bool download(const QString namedict, const QString dicturl)
    {

        if (output.fileName() != "") return false;

        QNetworkAccessManager* mNetworkManager = new QNetworkAccessManager(this);
      //  QObject::connect(mNetworkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(onNetworkReply(QNetworkReply*)));

        //Add a new download transfer in TransferUI
        downloadTransfer = client->registerTransfer(namedict, TransferUI::Client::TRANSFER_TYPES_DOWNLOAD);
        downloadTransfer->setIcon("icon-m-content-dictionary");
        downloadTransfer->setActive();
        QString filename = "/home/user/MyDocs/dic/"+saveFileName(dicturl);
       // qDebug() << filename;
        output.setFileName(filename);

        if (!output.open(QIODevice::WriteOnly)) {
              fprintf(stderr, "Problem opening save file");
              return false;
          }

        QUrl url = QUrl(dicturl);
        reply = mNetworkManager->get(QNetworkRequest(url));
        QObject::connect(reply, SIGNAL(downloadProgress(qint64,qint64)),  SLOT(downloadProgress(qint64,qint64)));
        QObject::connect(reply, SIGNAL(finished()), SLOT(downloadFinished()));
        QObject::connect(reply, SIGNAL(readyRead()),  SLOT(downloadReadyRead()));

        client->showUI();
        return true;

    }



public slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal)
    {
        downloadTransfer->setProgress((float)bytesReceived/bytesTotal);
    }

    void downloadFinished()
    {

       output.close();


       downloadTransfer->markCompleted(true);
       QProcess q;
       q.setWorkingDirectory("/home/user/MyDocs/dic/");
       q.start("tar", QStringList() << "xvjf" << output.fileName());
       if (!q.waitForFinished())
           return;

       output.remove();
       output.setFileName("");
       refreshDicts();
    }

    void downloadReadyRead()
    {
       output.write(reply->readAll());
    }



    void transferCancelled(Transfer * transfer) {
        //User has cancelled the transfer, take neccessary action and mark transfer
        //as cancelled
        //if cancel was sucessful
        transfer->markCancelled();
        QString cancelErrMsg =  ("Download cancelled");
        transfer->markCancelFailed(cancelErrMsg);
        client->removeTransfer(transfer->transferId());

        reply->abort();
    }

    void transferPaused(Transfer * transfer) {
        //User requested transfer to pause, pause transfer and update TransferUI

        transfer->markPaused();
    }

    void transferResumed(Transfer * transfer) {
        //User requested transfer to resume, resume transfer and update TransferUI

        transfer->markResumed();
    }

    void transferErrorRepairRequested(Transfer * transfer) {
        //User has requested for repair the error take some action
    }

private:
    QNetworkReply* reply;
    QFile output;
    DictCore *m_dictCore;
    TransferUI::Client *client;
    TransferUI::Transfer *downloadTransfer;
    QList<DictCore::Dictionary> dictList;
};



#endif // MYCLASS_H
