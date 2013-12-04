#ifndef CSGGENERATOR_H
#define CSGGENERATOR_H

#include <QDialog>


namespace Ui {
class CSGGenerator;
}

class CSGGenerator : public QDialog
{
    Q_OBJECT

public:
    explicit CSGGenerator(QWidget *parent = 0);
    ~CSGGenerator();
    void reset_csg();


signals:
  void newCSGAvailable(QString const& csg_string);

private slots:

  void on_pushButtonGenerate_clicked();

  void on_pushButtonClose_clicked();

  void on_pushButtonSave_clicked();

  void on_pushButtonLoad_clicked();

  void on_pushButtonReset_clicked();

private:
    Ui::CSGGenerator *ui;
};

#endif // CSGGENERATOR_H
