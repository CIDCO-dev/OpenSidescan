// Modified from example installed with Qt
// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html

#include <QDebug>
#include <QStringList>

#include "treeitem.h"


//TreeItem::TreeItem(const QVariant &data, SidescanFile *sidescanFile, TreeItem *parent)
TreeItem::TreeItem(const QVariant &data, TreeItem *parent)
    : parentItem( parent ), itemData( data ), sidescanFile( nullptr )
{
}


TreeItem::~TreeItem()
{
    qDeleteAll(childItems);
}





TreeItem *TreeItem::child(int number)
{
    return childItems.value(number);
}


int TreeItem::childCount() const
{
    return childItems.count();
}


int TreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<TreeItem*>(this));

    return 0;
}


int TreeItem::columnCount() const
{
    return nbColumns;
}


QVariant TreeItem::data(int column) const
{
    return itemData;
}


bool TreeItem::insertChildren(int position, int count, int columns)
{
    qDebug() << QObject::tr("bool TreeItem::insertChildren, position ") << position
              << QObject::tr(", count ") << count
              << QObject::tr(", columns ") << columns;

//    qDebug() << QObject::tr("bool TreeItem::insertChildren, position ");


    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVariant data;
        TreeItem *item = new TreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}


bool TreeItem::insertColumns(int position, int columns)
{
    // Only one columns, cannot insert new columns
    return false;
}


TreeItem *TreeItem::parent()
{
    return parentItem;
}


bool TreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}



bool TreeItem::removeColumns(int position, int columns)
{
    // Only one columns, cannot delete column(s)
    return false;
}



bool TreeItem::setData(int column, const QVariant &value)
{
    // Only one column

    itemData = value;
    return true;
}

void TreeItem::setSidescanFile( SidescanFile *sidescanFileIN )
{
    sidescanFile = sidescanFileIN;
}

SidescanFile * TreeItem::getSidescanFile() const
{
    return sidescanFile;
}
