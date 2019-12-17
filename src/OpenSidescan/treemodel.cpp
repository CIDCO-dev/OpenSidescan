// Modified from example installed with Qt
// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html

#include <QtWidgets>

#include "treeitem.h"
#include "treemodel.h"

//! [0]
TreeModel::TreeModel(QObject *parent)
    : QAbstractItemModel(parent)
{

//    QVector<QVariant> rootData;
//    foreach (QString header, headers)
//        rootData << header;

//    rootItem = new TreeItem(rootData);
//    setupModelData(data.split(QString("\n")), rootItem);


    QVariant rootData( "root" );
    rootItem = new TreeItem(rootData);

    // Insert a TreeItem for node "Files"
    rootItem->insertChildren( rootItem->childCount(), 1, 1 );

    // Get a pointer to the node "Files"
    fileNode = rootItem->child( rootItem->childCount() - 1 );

    // Set the node "Files"'s data to "Files"
    QVariant data( tr("Files") );
    fileNode->setData(0, data);
    fileNode->setSidescanFile( nullptr ); // Node those not have an associated sidescan file

}


TreeModel::~TreeModel()
{
    delete rootItem;
}
//! [1]

void TreeModel::populateTree( const QStringList &list )
{


//    for (int count = 0; count < list.count(); ++count)
//    {
//        rootItem->insertChildren( rootItem->childCount(), 1, 1 );

//        QVariant data( list[ count ]);

//        rootItem->child(rootItem->childCount() - 1)->setData(0, data);
//    }

    for (int count = 0; count < list.count(); ++count)
    {
        fileNode->insertChildren( fileNode->childCount(), 1, 1 );

        QVariant data( list[ count ]);

        fileNode->child(fileNode->childCount() - 1)->setData(0, data);
    }



}

//TreeModel * TreeModel::getFileNode()
//{
//    return fileNode;
//}




int TreeModel::columnCount(const QModelIndex & /* parent */) const
{
//    return rootItem->columnCount();

    return nbColumns;

}



QVariant TreeModel::data(const QModelIndex &index, int role) const
{
//    qDebug() << tr( "QVariant TreeModel::data, role: " ) << role;

    if (!index.isValid())
        return QVariant();

    if (role != Qt::DisplayRole && role != Qt::EditRole)
        return QVariant();

    TreeItem *item = getItem(index);

    return item->data(index.column());
}



Qt::ItemFlags TreeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;

    return Qt::ItemIsEditable | QAbstractItemModel::flags(index);
}


TreeItem *TreeModel::getItem(const QModelIndex &index) const
{
    if (index.isValid()) {
        TreeItem *item = static_cast<TreeItem*>(index.internalPointer());
        if (item)
            return item;
    }
    return rootItem;
}




bool TreeModel::isFilesNode( const QModelIndex &index ) const
{
    TreeItem *item = static_cast<TreeItem*>(index.internalPointer());

    if( item == fileNode )
        return true;
    else
        return false;

}




QVariant TreeModel::headerData(int section, Qt::Orientation orientation,
                               int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

//! [5]
QModelIndex TreeModel::index(int row, int column, const QModelIndex &parent) const
{

//    qDebug() << QObject::tr("QModelIndex TreeModel::index(), row ") << row
//              << QObject::tr(", column ") << column
//              << QObject::tr(", parent.isValid() ") << parent.isValid()
//              << QObject::tr(", parent.column() ") << parent.column();


    if (parent.isValid() && parent.column() != 0)
        return QModelIndex();

    TreeItem *parentItem = getItem(parent);

    TreeItem *childItem = parentItem->child(row);
    if (childItem)
        return createIndex(row, column, childItem);
    else
        return QModelIndex();
}


bool TreeModel::insertColumns(int position, int columns, const QModelIndex &parent)
{
//    bool success;

//    beginInsertColumns(parent, position, position + columns - 1);
//    success = rootItem->insertColumns(position, columns);
//    endInsertColumns();

//    return success;

    // Only one columns, cannot insert new columns
    return false;

}

bool TreeModel::insertRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success;

    beginInsertRows(parent, position, position + rows - 1);
    success = parentItem->insertChildren(position, rows, rootItem->columnCount());
    endInsertRows();

    return success;
}


QModelIndex TreeModel::parent(const QModelIndex &index) const
{
    if (!index.isValid())
        return QModelIndex();

    TreeItem *childItem = getItem(index);
    TreeItem *parentItem = childItem->parent();

    if (parentItem == rootItem)
        return QModelIndex();

    return createIndex(parentItem->childNumber(), 0, parentItem);
}

bool TreeModel::removeColumns(int position, int columns, const QModelIndex &parent)
{
//    bool success;

//    beginRemoveColumns(parent, position, position + columns - 1);
//    success = rootItem->removeColumns(position, columns);
//    endRemoveColumns();

//    if (rootItem->columnCount() == 0)
//        removeRows(0, rowCount());

//    return success;


    // Only one columns, cannot delete column(s)
    return false;
}

bool TreeModel::removeRows(int position, int rows, const QModelIndex &parent)
{
    TreeItem *parentItem = getItem(parent);
    bool success = true;

    beginRemoveRows(parent, position, position + rows - 1);
    success = parentItem->removeChildren(position, rows);
    endRemoveRows();

    return success;
}


int TreeModel::rowCount(const QModelIndex &parent) const
{
    TreeItem *parentItem = getItem(parent);

    return parentItem->childCount();
}


bool TreeModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (role != Qt::EditRole)
        return false;

    TreeItem *item = getItem(index);
    bool result = item->setData(index.column(), value);

    if (result)
        emit dataChanged(index, index, {role});

    return result;
}

bool TreeModel::setHeaderData(int section, Qt::Orientation orientation,
                              const QVariant &value, int role)
{
    if (role != Qt::EditRole || orientation != Qt::Horizontal)
        return false;

    bool result = rootItem->setData(section, value);

    if (result)
        emit headerDataChanged(orientation, section, section);

    return result;
}

//void TreeModel::setupModelData(const QStringList &lines, TreeItem *parent)
//{
//    QList<TreeItem*> parents;
//    QList<int> indentations;
//    parents << parent;
//    indentations << 0;

//    int number = 0;

//    while (number < lines.count()) {
//        int position = 0;
//        while (position < lines[number].length()) {
//            if (lines[number].at(position) != ' ')
//                break;
//            ++position;
//        }

//        QString lineData = lines[number].mid(position).trimmed();

//        if (!lineData.isEmpty()) {
//            // Read the column data from the rest of the line.
//            QStringList columnStrings = lineData.split("\t", QString::SkipEmptyParts);
//            QVector<QVariant> columnData;
//            for (int column = 0; column < columnStrings.count(); ++column)
//                columnData << columnStrings[column];

//            if (position > indentations.last()) {
//                // The last child of the current parent is now the new parent
//                // unless the current parent has no children.

//                if (parents.last()->childCount() > 0) {
//                    parents << parents.last()->child(parents.last()->childCount()-1);
//                    indentations << position;
//                }
//            } else {
//                while (position < indentations.last() && parents.count() > 0) {
//                    parents.pop_back();
//                    indentations.pop_back();
//                }
//            }

//            // Append a new item to the current parent's list of children.
//            TreeItem *parent = parents.last();
//            parent->insertChildren(parent->childCount(), 1, rootItem->columnCount());
//            for (int column = 0; column < columnData.size(); ++column)
//                parent->child(parent->childCount() - 1)->setData(column, columnData[column]);
//        }

//        ++number;
//    }
//}
