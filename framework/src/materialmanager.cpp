#include <QResource>

#include "materialmanager.h"
#include "ui_materialmanager.h"

#include <cstdlib>

MaterialManager::MaterialManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MaterialManager)
{
    ui->setupUi(this);
    setWindowTitle("Material Manager");

    ui->pushButtonAdd->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);

    ui->treeWidget->setColumnCount(4);
    QTreeWidgetItem* header = ui->treeWidget->headerItem();
    header->setText(0, "Material");
    header->setText(1, "Parameter");
    header->setText(2, "Value");
    header->setText(3, "Unit");
    ui->treeWidget->setStyleSheet("font-size: 16px;");

    // default material file is in the resources
    QResource resource("://resources/materials.xml");
    readXMLFile(resource.fileName());
}

MaterialManager::~MaterialManager()
{
    delete ui;
}

void MaterialManager::readXMLFile(QString const& filename)
{
    // populate the user interface
    //
    XMLStreamIO xmlstream(ui->treeWidget);
    xmlstream.load(filename);
    for(int i = 0; i < ui->treeWidget->columnCount(); i++)
        ui->treeWidget->resizeColumnToContents(i);

    // transfer the resource file material xml file into a stringstream
    //
    QString result;
    QFile file( filename );
    if( file.open( QFile::ReadOnly ) ) {
       QTextStream ts( &file );
       result = ts.readAll();
    }

    // load the viennamaterial library
    //
//    std::stringstream stream(result.toStdString());
//    library.load(stream);
}

void MaterialManager::saveXMLFile(QString const& filename)
{
    filename.size(); // silence the compiler for now ..
}

void MaterialManager::on_pushButtonClose_clicked()
{
    close();
}

void MaterialManager::on_pushButtonLoad_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this, tr("Open XML Material Database File"), QDir::currentPath(), "Material files (*.xml);;All files (*.*)");
    //QString filename = "/home/weinbub/git/ViennaMaterials/database/materials.xml";
    if(filename.isEmpty()) return; // if the cancel button has been clicked ..
    else {
        ui->treeWidget->clear();
        readXMLFile(filename);
    }
}

void MaterialManager::on_pushButtonSave_clicked()
{

}

//MaterialManager::Library& MaterialManager::getLibrary()
//{
//    return library;
//}
