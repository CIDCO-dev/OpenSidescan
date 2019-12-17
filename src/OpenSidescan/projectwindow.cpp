#include "projectwindow.h"

//#include <QStringListModel>

#include <QFileInfo>

#include <QDebug>

#include "mainwindow.h"

//ProjectWindow::ProjectWindow(QWidget *parent) : QDockWidget(tr("Files"),parent),project(NULL),model(NULL)
ProjectWindow::ProjectWindow(QWidget *parent)
    : QDockWidget(tr("Project"),parent),project(NULL),model(NULL), parent(parent)

{
    //TODO: use model to deal with SidescanFile * objects instead
    tree = new QTreeView(this);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);

    model = new TreeModel(this);
    tree->setModel(model);

    qDebug() << tr("ProjectWindow::ProjectWindow() right before connect");

    connect(tree->selectionModel(),&QItemSelectionModel::selectionChanged,(MainWindow*)parent,&MainWindow::fileSelected );

    this->setAllowedAreas(Qt::LeftDockWidgetArea|Qt::RightDockWidgetArea);
    this->setWidget(tree);
    this->setFeatures(QDockWidget::DockWidgetClosable|QDockWidget::DockWidgetMovable|QDockWidget::DockWidgetFloatable);
    this->show();
}

// TODO: destructor for tree and model?

SidescanFile * ProjectWindow::getSelectedFile() {
    if(tree && project){
        QModelIndex index = tree->selectionModel()->currentIndex();

        if ( model->isFilesNode( index ) ) {
            qDebug() << tr("Selected node is file node");
            return NULL;
        }
        else {
            return model->getSidescanFile(index);
        }



//        QString itemText = index.data(Qt::DisplayRole).toString();

//        for(auto i = project->getFiles().begin();i != project->getFiles().end();i++){
//            if((*i)->getFilename()==itemText.toStdString()){
//                return (*i);
//            }
//        }
    }

    return NULL;
}


void ProjectWindow::setProject(Project * project){
    this->project = project;

    refresh();
}

void ProjectWindow::refresh(){
    if(project){
//        QStringList filenames;

        // Delete previous model
        tree->setModel(nullptr);

        if(model) {
            delete model;
        }

        model = new TreeModel(this);


        for(auto i=project->getFiles().begin();i!=project->getFiles().end();i++){

//            filenames.push_back( QString::fromStdString( (*i)->getFilename() ) );


            QFileInfo fileInfo( tr( (*i)->getFilename().c_str() )  );
            QString filename = fileInfo.fileName(); // Filename without path

            model->appendFile( filename, (*i) );
        }

        tree->setModel(model);

        tree->setHeaderHidden(true);

        connect(tree->selectionModel(),&QItemSelectionModel::selectionChanged,(MainWindow*)parent,&MainWindow::fileSelected );

        tree->expandAll();


//        model->setStringList(filenames);

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
