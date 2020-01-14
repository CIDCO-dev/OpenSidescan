// Modified from example installed with Qt
// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html

#ifndef PROJECTTREEMODEL_H
#define PROJECTTREEMODEL_H

#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

#include "sidescanfile.h"


class ProjectTreeItem;


class ProjectTreeModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ProjectTreeModel( QObject *parent = 0);
    ~ProjectTreeModel();

    void appendFile( QString &filename, SidescanFile *sidescanFile );

    SidescanFile * getSidescanFile(const QModelIndex &index) const;


//    ProjectTreeModel * getFileNode();

    bool isFilesNode( const QModelIndex &index ) const;


    QVariant data(const QModelIndex &index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    QModelIndex index(int row, int column,
                      const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &index) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;


    Qt::ItemFlags flags(const QModelIndex &index) const override;
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;
    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    bool insertColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool removeColumns(int position, int columns,
                       const QModelIndex &parent = QModelIndex()) override;
    bool insertRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;
    bool removeRows(int position, int rows,
                    const QModelIndex &parent = QModelIndex()) override;

    int getNbFiles() const;

private:
    void setupModelData(const QStringList &lines, ProjectTreeItem *parent);
    ProjectTreeItem *getItem(const QModelIndex &index) const;


    ProjectTreeItem *rootItem;
    ProjectTreeItem *fileNode;


    const int nbColumns = 1;
};


#endif // PROJECTTREEMODEL_H
