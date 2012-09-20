
#include "dictlistmodel.h"
#include <QDebug>

using namespace QStarDict;

DictListModel::DictListModel(QObject *parent)
     : QAbstractListModel(parent)
 {
     QHash<int, QByteArray> roles;
     roles[TypeRole] = "title";
     roles[SizeRole] = "subtitle";
     roles[IsTicked] = "isTicked";
     setRoleNames(roles);
 }

 void DictListModel::addDict(DictCore::Dictionary *dict)
 {
     beginInsertRows(QModelIndex(), rowCount(), rowCount());
     m_dicts.append(*dict);
     endInsertRows();
 }

 void DictListModel::clearDicts() {

     beginResetModel();
     m_dicts.clear();
     endResetModel();

 }

 int DictListModel::rowCount(const QModelIndex & parent) const {
     return m_dicts.count();
 }

 QVariant DictListModel::data(const QModelIndex & index, int role) const {
     if (index.row() < 0 || index.row() > m_dicts.count())
         return QVariant();

     const DictCore::Dictionary &dict = m_dicts[index.row()];

     if (role == TypeRole)
         return dict.name();
     else if (role == SizeRole)
         return dict.plugin();

     return QVariant();
 }
