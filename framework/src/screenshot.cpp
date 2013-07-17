#include "screenshot.h"
#include "ui_screenshot.h"

#include <QFileDialog>

Screenshot::Screenshot(MultiView* multiview, QWidget *parent) :
    multiview(multiview),
    QDialog(parent),
    ui(new Ui::Screenshot)
{
    ui->setupUi(this);
    setWindowTitle("Take Screenshots");
}

Screenshot::~Screenshot()
{
    delete ui;
}

void Screenshot::on_pushButtonBrowse_clicked()
{
    QFileDialog dialog(NULL, "Save Screenshot to File");
    dialog.setFileMode(QFileDialog::AnyFile);
    dialog.setNameFilter(key::png_filter);
    dialog.setDirectory(QDir::currentPath());
    dialog.exec();

    QString filename = dialog.selectedFiles().at(0);
    QString filter = dialog.selectedNameFilter();


    if(filename == QString("")) return; // if the cancel button has been clicked ..
    else {
        QString suffix = QFileInfo(filename).suffix();

        // if the user just entered a filename without a suffix,
        // use the chosen filter to add it automatically ..
        if(suffix.isEmpty() && (filter == key::png_filter))
        {
            filename += "." + key::png_suffix;
        }

        ui->lineEdit->setText(filename);
    }
}

void Screenshot::on_pushButtonClose_clicked()
{
    this->close();
}

void Screenshot::on_pushButtonTakeScreen_clicked()
{
    multiview->takeScreenshotOfActiveView(ui->lineEdit->text());
}
