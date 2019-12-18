// Modified from example installed with Qt
// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html

#include <QtWidgets>

#include "projecttreeitem.h"
#include "projecttreemodel.h"

//! [0]
ProjectTreeModel::ProjectTreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{

    QVariant rootData( "root" );
    rootItem = new ProjectTreeItem(rootData);

    // Insert a ProjectTreeItem for node "Files"
    rootItem->insertChildren( rootItem->childCount(), 1, 1 );

    // Get a pointer to the node "Files"
    fileNode = rootItem->child( rootItem->childCount() - 1 );

    // Set the node "Files"'s data to "Files"
    QVariant data( tr("Files") );
    fileNode->setData(0, data);
    fileNode->setSidescanFile( nullptr ); // Node those not have an associated sidescan file

}


ProjectTreeModel::~ProjectTreeModel()
{
    delete rootItem;
}



void ProjectTreeModel::appendFile( QString &filename, SidescanFile *sidescanFile )
{
    fileNode->insertChildren( fileNode->childCount(), 1, 1 );

    QVariant data( filename );

    fileNode->child(fileNode->childCount() - 1)->setData(0, data);

    fileNode->child(fileNode->childCount() - 1)->setSidescanFile( sidescanFile );
}


SidescanFile * ProjectTreeModel::getSidescanFile(const QModelIndex &index) const
{

    qDebug() << tr( "ProjectTreeModel::getSidescanFile" );

    if (index.isValid() == false)
        return nullptr;


    ProjectTreeItem *item = static_cast<ProjectTreeItem*>(index.internalPointer());

    if (item) {

        // If it is not the file node
        if (item != fileNode) {
            qDebug() << tr( "item != fileNode" );
            return item->getSidescanFile();
        }
        else {
            qDebug() << tr( "item == fileNode" );
            return nullptr;
        }
    }
    else {
        return nullptr;
    }
}



//ProjectTreeModel * ProjectTreeModel::getFileNode()
//{
//    return fileNode;
//}



int ProjectTreeModel::columnCount(const QModelIndex & /* parent */) const
{
//    return rootItem->columnCount();

    return nbColumns;

}


QVariant ProjectTreeModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << tr( "QVariant ProjectTreeModel::data, role: " ) << role;

    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    ProjectTreeItem *item = getItem(index);

    return item->data(index.column());
}



Qt::ItemFlags ProjectTreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}


ProjectTreeItem *ProjectTreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        ProjectTreeItem *item = static_cast<ProjectTreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}


bool ProjectTreeModel::isFilesNode( const QModelIndex &index ) const
{
    ProjectTreeItem *item = static_cast<ProjectTreeItem*>(index.internalPointer());

    if( item == fileNode )
        return true;
    else
        return false;

}


QVariant ProjectTreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex ProjectTreeModel::index(int row, int column, const QModelIndex &parent) const
{

//    qDebug() << QObject::tr("QModelIndex ProjectTreeModel::index(), row ") << row
//              << QObject::tr(", column ") << column
//              << QObject::tr(", parent.isValid() ") << parent.isValid()
//              << QObject::tr(", parent.column() ") << parent.column();


    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    ProjectTreeItem *parentItem = getItem(parent);

    ProjectTreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


bool ProjectTreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
    // Only one columns, cannot insert new columns
    return false;
}

bool ProjectTreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    ProjectTreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}


QModelIndex ProjectTreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    ProjectTreeItem *childItem = getItem(index);
    ProjectTreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool ProjectTreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
    // Only one columns, cannot delete column(s)
    return false;
}

bool ProjectTreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    ProjectTreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


int ProjectTreeModel::rowCount(const QModelIndex &parent) const
{
    ProjectTreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}


bool ProjectTreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    ProjectTreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {role});

    return result;
}

bool ProjectTreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

