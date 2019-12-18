#include "projectwindow.h"

//#include <QStringListModel>

#include <QFileInfo>

#include <QMenu>

#include <QDebug>

#include <QMessageBox>

#include "mainwindow.h"

//ProjectWindow::ProjectWindow(QWidget *parent) : QDockWidget(tr("Files"),parent),project(NULL),model(NULL)
ProjectWindow::ProjectWindow(QWidget *parent)
    : QDockWidget(tr("Project"),parent),project(NULL),model(NULL), parent(parent)

{
    //TODO: use model to deal with SidescanFile * objects instead
    tree = new QTreeView(this);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);

    model = new ProjectTreeModel(this);
    tree->setModel(model);

    qDebug() << tr("ProjectWindow::ProjectWindow() right before connect");

    connect(tree->selectionModel(),&QItemSelectionModel::selectionChanged,(MainWindow*)parent,&MainWindow::fileSelected );

    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tree, &QWidget::customContextMenuRequested,
                this, &ProjectWindow::customContextMenu);


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

        model = new ProjectTreeModel(this);


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


void ProjectWindow::customContextMenu(QPoint pos)
{
    // Based on
    // https://www.qtcentre.org/threads/49656-ContextMenu-in-QTreeView
    // https://github.com/qt-creator/qt-creator/blob/master/src/libs/modelinglib/qmt/model_widgets_ui/modeltreeview.cpp

    QModelIndex index=tree->indexAt(pos);

    if (index.isValid()) {

        if ( model->isFilesNode( index ) ) {
            // No right-click menu

            qDebug() << tr("MainWindow::customContextMenu() Selected node is file node");
            return;
        }

        QMenu menu;

        menu.addAction( QString("Remove..."), this, &ProjectWindow::removeFileFromProject );

        menu.exec( QCursor::pos() );
    }

}


void ProjectWindow::removeFileFromProject()
{
    qDebug() << tr("Inside 'ProjectWindow::removeFileFromProject()'");

    QModelIndex index = tree->selectionModel()->currentIndex();

    if ( model->isFilesNode( index ) ) {
        return;
    }

    SidescanFile * sidescanFile = model->getSidescanFile( index );
    std::string filename = sidescanFile->getFilename();

    QMessageBox msgBox;
    msgBox.setWindowTitle("Remove File");
    msgBox.setText("The following file will be removed from the project (cannot be undone):");

    msgBox.setInformativeText( QString::fromStdString(filename) );

    msgBox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Cancel)
        return;

    emit removeFileFromProjectRequest( sidescanFile );

    if (model->removeRow(index.row(), index.parent())) {

        // Update selection

        bool hasCurrent = tree->selectionModel()->currentIndex().isValid();

        if (hasCurrent) {
            tree->closePersistentEditor(tree->selectionModel()->currentIndex());
        }

    }

}
