#include "csggenerator.h"
#include "ui_csggenerator.h"

#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>


CSGGenerator::CSGGenerator(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CSGGenerator)
{
    ui->setupUi(this);
    this->setWindowTitle("CSG Mesh Generator");
    QString default_csg_device =
        "algebraic3d \n"
        "solid cube = plane (0, 0, 0; 0, 0, -1) \n"
        "       and plane (0, 0, 0; 0, -1, 0) \n"
        "       and plane (0, 0, 0; -1, 0, 0) \n"
        "       and plane (1, 1, 1; 0, 0, 1) \n"
        "       and plane (1, 1, 1; 0, 1, 0) \n"
        "       and plane (1, 1, 1; 1, 0, 0); \n"
        "tlo cube;\n";

    ui->textEditCSG->setPlainText(default_csg_device);
}

CSGGenerator::~CSGGenerator()
{
    delete ui;
}

void CSGGenerator::on_pushButtonGenerate_clicked()
{
    emit newCSGAvailable(ui->textEditCSG->toPlainText());
}

void CSGGenerator::on_pushButtonClose_clicked()
{
  this->close();
}

void CSGGenerator::on_pushButtonSave_clicked()
{

}

void CSGGenerator::on_pushButtonLoad_clicked()
{
  QDir dir(QCoreApplication::applicationDirPath());

  QString csg_file = dir.relativeFilePath(
        QFileDialog::getOpenFileName(this, tr("Open Mesh File"), QDir::currentPath(),
                                    "Netgen CSG files (*.geo);;All files (*.*)")
  );

  if(csg_file.isEmpty()) return; // if the cancel button has been clicked ..

  QFile file(csg_file);
  if(!file.open(QFile::ReadOnly | QFile::Text))
  {
      QMessageBox::warning(this, tr("CSG Generator"),
                           tr("Cannot read file %1:\n%2")
                           .arg(csg_file)
                           .arg(file.errorString()));
      return;
  }

  ui->textEditCSG->clear();
  QTextStream in(&file);
  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->textEditCSG->setPlainText(in.readAll());
  QApplication::restoreOverrideCursor();
}
