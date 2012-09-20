#include "wordlistmodel.h"
#include <QDebug>

 Word::Word(const QString &type) : m_type(type)
 {

 }

 QString Word::title() const
 {
     return m_type;
 }


 WordListModel::WordListModel(QObject *parent)
     : QAbstractListModel(parent)
 {
     QHash<int, QByteArray> roles;
     roles[TypeRole] = "title";
    // roles[SizeRole] = "subtitle";
     setRoleNames(roles);
 }

 void WordListModel::addWord(Word *animal)
 {
     beginInsertRows(QModelIndex(), rowCount(), rowCount());
     m_words.append(*animal);
     endInsertRows();
 }

 void WordListModel::clearWords() {

     beginResetModel();
     m_words.clear();
     endResetModel();

 }

 int WordListModel::rowCount(const QModelIndex & parent) const {
     return m_words.count();
 }

 QVariant WordListModel::data(const QModelIndex & index, int role) const {
     if (index.row() < 0 || index.row() > m_words.count())
         return QVariant();

     const Word &animal = m_words[index.row()];
     if (role == TypeRole)
         return animal.title();


     return QVariant();
 }
