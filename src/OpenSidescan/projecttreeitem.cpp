// Modified from example installed with Qt
// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html

#include <QDebug>
#include <QStringList>

#include "projecttreeitem.h"


//ProjectTreeItem::ProjectTreeItem(const QVariant &data, SidescanFile *sidescanFile, ProjectTreeItem *parent)
ProjectTreeItem::ProjectTreeItem(const QVariant &data, ProjectTreeItem *parent)
    : parentItem( parent ), itemData( data ), sidescanFile( nullptr )
{
}


ProjectTreeItem::~ProjectTreeItem()
{
    qDeleteAll(childItems);
}


ProjectTreeItem *ProjectTreeItem::child(int number)
{
    return childItems.value(number);
}


int ProjectTreeItem::childCount() const
{
    return childItems.count();
}


int ProjectTreeItem::childNumber() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<ProjectTreeItem*>(this));

    return 0;
}


int ProjectTreeItem::columnCount() const
{
    return nbColumns;
}


QVariant ProjectTreeItem::data(int column) const
{
    return itemData;
}


bool ProjectTreeItem::insertChildren(int position, int count, int columns)
{
    qDebug() << QObject::tr("bool ProjectTreeItem::insertChildren, position ") << position
              << QObject::tr(", count ") << count
              << QObject::tr(", columns ") << columns;

//    qDebug() << QObject::tr("bool ProjectTreeItem::insertChildren, position ");


    if (position < 0 || position > childItems.size())
        return false;

    for (int row = 0; row < count; ++row) {
        QVariant data;
        ProjectTreeItem *item = new ProjectTreeItem(data, this);
        childItems.insert(position, item);
    }

    return true;
}


bool ProjectTreeItem::insertColumns(int position, int columns)
{
    // Only one columns, cannot insert new columns
    return false;
}


ProjectTreeItem *ProjectTreeItem::parent()
{
    return parentItem;
}


bool ProjectTreeItem::removeChildren(int position, int count)
{
    if (position < 0 || position + count > childItems.size())
        return false;

    for (int row = 0; row < count; ++row)
        delete childItems.takeAt(position);

    return true;
}



bool ProjectTreeItem::removeColumns(int position, int columns)
{
    // Only one columns, cannot delete column(s)
    return false;
}



bool ProjectTreeItem::setData(int column, const QVariant &value)
{
    // Only one column

    itemData = value;
    return true;
}

void ProjectTreeItem::setSidescanFile( SidescanFile *sidescanFileIN )
{
    sidescanFile = sidescanFileIN;
}

SidescanFile * ProjectTreeItem::getSidescanFile() const
{
    return sidescanFile;
}
