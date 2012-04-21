#ifndef GAMMA_CORR_UI_H
#define GAMMA_CORR_UI_H

#include <QtGUi/QDialog>

namespace Ui {
    class gamma_corr_ui;
}

class gamma_corr_ui : public QDialog
{
    Q_OBJECT

public:
    explicit gamma_corr_ui(QWidget *parent = 0);
    ~gamma_corr_ui();
	double getGammaValue();

private slots:
    void on_gammaSlider_valueChanged(int value);

    void on_ok_clicked();

    void on_cancel_clicked();

private:
    Ui::gamma_corr_ui *ui;
	double gammaValue;
};

#endif // GAMMA_CORR_UI_H
