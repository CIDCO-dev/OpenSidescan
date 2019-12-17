#include "projectwindow.h"

#include <QStringListModel>
#include "mainwindow.h"

ProjectWindow::ProjectWindow(QWidget *parent) : QDockWidget(tr("Files"),parent),project(NULL),model(NULL)
{
    //TODO: use model to deal with SidescanFile * objects instead
    tree = new QTreeView(this);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);

    model = new TreeModel(this);
    tree->setModel(model);

    connect(tree->selectionModel(),&QItemSelectionModel::selectionChanged,(MainWindow*)parent,&MainWindow::fileSelected );

    this->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    this->setWidget(tree);
    this->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    this->show();
}

void ProjectWindow::setProject(Project * project){
    this->project = project;

    refresh();
}

void ProjectWindow::refresh(){
    if(project){
        QStringList filenames;

        for(auto i=project->getFiles().begin();i!=project->getFiles().end();i++){
            filenames.push_back( QString::fromStdString( (*i)->getFilename() ) );
        }

        model->setStringList(filenames);
    }
}

void ProjectWindow::addFile(SidescanFile * file){
    if(project){
        //TODO: avoid duplicates
        project->getFiles().push_back(file);
        refresh();
    }
    else{
        //TODO: throw exception or msgbox
    }
}

bool ProjectWindow::containsFile(std::string & filename){
    if(project){
        for(auto i=project->getFiles().begin();i!=project->getFiles().end();i++){
            if(strcmp((*i)->getFilename().c_str(),filename.c_str()) == 0){
                return true;
            }
        }
    }

    return false;
}
