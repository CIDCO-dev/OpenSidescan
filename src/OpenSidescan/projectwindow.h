#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QDockWidget>

//#include <QListWidget>
//#include <QStringListModel>

#include <QTreeView>


#include <QVariant>

#include "project.h"
#include "sidescanfile.h"

#include "treemodel.h"


class ProjectWindow : public QDockWidget
{
    Q_OBJECT

public:

    explicit    ProjectWindow(QWidget *parent);

    Project *   getProject(){ return project;}
    void        setProject(Project * project);

    void        addFile(SidescanFile * file);
    void        refresh();

    bool        containsFile(std::string & filename);

    //FIXME: this will be obsolete with the use of a proper listview model
    SidescanFile * getSelectedFile(){
        if(files && project){
            QModelIndex index = files->currentIndex();
            QString itemText = index.data(Qt::DisplayRole).toString();

            for(auto i = project->getFiles().begin();i != project->getFiles().end();i++){
                if((*i)->getFilename()==itemText.toStdString()){
                    return (*i);
                }
            }
        }

        return NULL;
    }

signals:

public slots:

protected:
    Project *   project            = NULL;

//    QListView * files              = NULL;
//    QStringListModel * model       = NULL;


    QTreeView * tree                = NULL;
    TreeModel * model               = NULL;

};


#endif // PROJECTWINDOW_H
