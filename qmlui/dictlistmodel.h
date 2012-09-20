#ifndef DICTLISTMODEL_H
#define DICTLISTMODEL_H

#include "dictcore.h"

using namespace QStarDict;

class                                           DictListModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum AnimalRoles {
        TypeRole = Qt::UserRole + 1,
        SizeRole,
        IsTicked
    };

    DictListModel(QObject *parent = 0);

    void addDict(DictCore::Dictionary  *dict);
    void clearDicts();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

private:
    QList<DictCore::Dictionary> m_dicts;
};

#endif // DICTLISTMODEL_H
