/* 
 * The information in this file is
 * Copyright (C) 2011, Pratik Anand <pratik@pratikanand.com>
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "AppAssert.h"
#include "AppVerify.h"
#include "gamma_corr_ui.h"

#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>

using namespace std;

gamma_corr_ui::gamma_corr_ui(QWidget * pParent):QDialog(pParent)
{
	setWindowTitle("Gamma Correction");
	GammaValue = 1;
	QGridLayout *pLayout = new QGridLayout(this);
	pLayout->setMargin(10);
	pLayout->setSpacing(5);

	QLabel *description = new QLabel("Gamma: ", this);
	pLayout->addWidget(description, 0, 0);

	GammaValueBox = new QDoubleSpinBox(this);
	GammaValueBox->setRange(0, 5); //I just want to set a minimum but not the maximum. How do I do that?
	GammaValueBox->setSingleStep(0.01);
	GammaValueBox->setValue(GammaValue);
	pLayout->addWidget(GammaValueBox, 0, 1, 1, 2);

	QHBoxLayout *pRespLayout = new QHBoxLayout; //Response Layout
	pLayout->addLayout(pRespLayout, 3, 0, 1, 3);

	QPushButton *pAccept = new QPushButton("OK", this);
	pRespLayout->addStretch();
	pRespLayout->addWidget(pAccept);

	QPushButton *pReject = new QPushButton("Cancel", this);
	pRespLayout->addWidget(pReject);

	connect(pAccept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(pReject, SIGNAL(clicked()), this, SLOT(reject()));
	connect(GammaValueBox, SIGNAL(valueChanged(double)), this, SLOT(setGammaValue(double)));
}


void gamma_corr_ui::setGammaValue(double t)
{
	GammaValue = t;
}

double gamma_corr_ui::getGammaValue()
{
	return GammaValue;
}
