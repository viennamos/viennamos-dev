#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <QDialog>
#include <QtCore>
#include <QtGui>

#include "xmlstreamio.h"

//#include "viennamaterials/library.hpp"
//#include "viennamaterials/kernels/pugixml.hpp"

namespace Ui {
class MaterialManager;
}

class MaterialManager : public QDialog
{
    Q_OBJECT
    
public:
//    typedef vmat::Library<vmat::tag::pugixml>::type  Library;

    explicit MaterialManager(QWidget *parent = 0);
    ~MaterialManager();
    
    void readXMLFile(QString const& filename);
    void saveXMLFile(QString const& filename);
//    Library& getLibrary();

private slots:
    void on_pushButtonClose_clicked();
    void on_pushButtonLoad_clicked();
    void on_pushButtonSave_clicked();

private:
    Ui::MaterialManager     *ui;
    QString                  material_file_str;
//    Library                  library;
};

#endif // MATERIALMANAGER_H
