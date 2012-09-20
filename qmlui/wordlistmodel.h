#ifndef WORDLISTMODEL_H
#define WORDLISTMODEL_H
#include <QAbstractListModel>
#include <QStringList>

class Word
{
public:
    Word(const QString &type);

    QString title() const;
    QString subtitle() const;

private:
    QString m_type;
    QString m_size;
};

class WordListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AnimalRoles {
        TypeRole = Qt::UserRole + 1,
        SizeRole
    };

    WordListModel(QObject *parent = 0);

    void addWord(Word *animal);
    void clearWords();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
    QList<Word> m_words;
};

#endif // WORDLISTMODEL_H
