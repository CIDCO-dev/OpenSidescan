// Modified from example installed with Qt
// https://doc.qt.io/qt-5/qtwidgets-itemviews-editabletreemodel-example.html

#ifndef PROJECTTREEITEM_H
#define PROJECTTREEITEM_H

#include <QList>
#include <QVariant>

#include "sidescan/sidescanfile.h"



class ProjectTreeItem
{
public:
//    explicit ProjectTreeItem(const QVariant &data, SidescanFile *sidescanFile, ProjectTreeItem *parent = 0);
    explicit ProjectTreeItem(const QVariant &data, ProjectTreeItem *parent = 0);

    ~ProjectTreeItem();

    ProjectTreeItem *child(int number);
    int childCount() const;
    int columnCount() const;
    QVariant data(int column) const;
    bool insertChildren(int position, int count, int columns);
    bool insertColumns(int position, int columns);
    ProjectTreeItem *parent();
    bool removeChildren(int position, int count);
    bool removeColumns(int position, int columns);
    int childNumber() const;
    bool setData(int column, const QVariant &value);

    void setSidescanFile( SidescanFile *sidescanFileIN );

    SidescanFile * getSidescanFile() const;

private:

    ProjectTreeItem *parentItem;
    QList<ProjectTreeItem*> childItems;

    QVariant itemData; // Filename, without path, to be displayed in the tree view

    SidescanFile * sidescanFile;

    const int nbColumns = 1;
};

#endif // PROJECTTREEITEM_H
