#ifndef SCREENSHOT_H
#define SCREENSHOT_H

#include <QDialog>

#include "multiview.h"

namespace Ui {
class Screenshot;
}

class Screenshot : public QDialog
{
    Q_OBJECT
    
public:
    explicit Screenshot(MultiView* multiview, QWidget *parent = 0);
    ~Screenshot();
    
private slots:
    void on_pushButtonBrowse_clicked();

    void on_pushButtonClose_clicked();

    void on_pushButtonTakeScreen_clicked();

private:
    Ui::Screenshot *ui;
    MultiView* multiview;
};

#endif // SCREENSHOT_H
