// Modified from example installed with Qt
// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html

#ifndef TREEITEM_H
#define TREEITEM_H

#include <QList>
#include <QVariant>

#include "sidescanfile.h"



class TreeItem
{
public:
//    explicit TreeItem(const QVariant &data, SidescanFile *sidescanFile, TreeItem *parent = 0);
    explicit TreeItem(const QVariant &data, TreeItem *parent = 0);

    ~TreeItem();

    TreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    TreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    void setSidescanFile( SidescanFile *sidescanFileIN );

private:

    TreeItem *parentItem;
    QList<TreeItem*> childItems;

    QVariant itemData; // Filename, without path, to be displayed in the tree view

    SidescanFile * sidescanFile;

    const int nbColumns = 1;
};

#endif // TREEITEM_H
