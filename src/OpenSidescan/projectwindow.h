#ifndef PROJECTWINDOW_H
#define PROJECTWINDOW_H

#include <QDockWidget>

//#include <QListWidget>
//#include <QStringListModel>

#include <QTreeView>


#include <QVariant>

#include "project.h"
#include "sidescanfile.h"

#include "projecttreemodel.h"


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


    SidescanFile * getSelectedFile();

//    void setSelected( SidescanFile * file );

//    //FIXME: this will be obsolete with the use of a proper listview model
//    SidescanFile * getSelectedFile(){
//        if(files && project){
//            QModelIndex index = files->currentIndex();
//            QString itemText = index.data(Qt::DisplayRole).toString();

//            for(auto i = project->getFiles().begin();i != project->getFiles().end();i++){
//                if((*i)->getFilename()==itemText.toStdString()){
//                    return (*i);
//                }
//            }
//        }

//        return NULL;
//    }

signals:
    void removeFileFromProjectRequest( SidescanFile * file );

public slots:
    void customContextMenu(QPoint pos);
    void removeFileFromProject();

protected:
    Project *   project            = NULL;

//    QListView * files              = NULL;
//    QStringListModel * model       = NULL;


    QTreeView * tree                = NULL;
    ProjectTreeModel * model               = NULL;

    QWidget *parent;

};


#endif // PROJECTWINDOW_H
