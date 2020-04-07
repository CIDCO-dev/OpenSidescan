#include "aboutdialog.h"
#include "ui_aboutdialog.h"

#include <QLabel>
#include <QLayout>

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);

    QVBoxLayout * layout = new QVBoxLayout();

    //QLabel * label = new QLabel("Copyright CIDCO bla bla bla bla la bla");
    //label->setPixmap(QPixmap(":/Images/resources/cidco-logo.png"));
    //label->setTextFormat(Qt::RichText);
    //label->setText("<img src=\":/Images/resources/cidco-logo.png\">");
    //layout->setAlignment(Qt::AlignCenter);
    //layout->addWidget(label,Qt::AlignCenter);

    QLabel * label = new QLabel("<h4>Open Sidescan</h4><p>Copyright CIDCO 2019, All Rights Reserved.</p>");
    label->setTextFormat(Qt::RichText);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label,Qt::AlignCenter);

    this->setLayout(layout);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
