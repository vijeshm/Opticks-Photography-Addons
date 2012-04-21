#include "gamma_corr_ui.h"
#include "ui_gamma_corr_ui.h"

gamma_corr_ui::gamma_corr_ui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::gamma_corr_ui)
{
    ui->setupUi(this);
}

gamma_corr_ui::~gamma_corr_ui()
{
    delete ui;
}

void gamma_corr_ui::on_gammaSlider_valueChanged(int value)
{
    QString str;
    str.append("Gamma: " + QString("%1").arg(double(value) / 100));
    ui->gammaValue->setText(str);

	gammaValue = double(value) / 100;
}

void gamma_corr_ui::on_ok_clicked()
{
    accept();
}

void gamma_corr_ui::on_cancel_clicked()
{
    reject();
}

double gamma_corr_ui::getGammaValue()
{
	return gammaValue;
}
