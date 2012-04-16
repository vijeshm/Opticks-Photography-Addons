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
#include "median_filter_ui.h"

#include <QtGui/QDoubleSpinBox>
#include <QtGui/QLabel>
#include <QtGui/QLayout>
#include <QtGui/QPushButton>

using namespace std;

median_filter_ui::median_filter_ui(QWidget * pParent):QDialog(pParent)
{
	setWindowTitle("Median Filter");
	RadiusValue = 3;
	QGridLayout *pLayout = new QGridLayout(this);
	pLayout->setMargin(10);
	pLayout->setSpacing(5);

	QLabel *description = new QLabel("Radius: ", this);
	pLayout->addWidget(description, 0, 0);

	RadiusValueBox = new QDoubleSpinBox(this);
	RadiusValueBox->setRange(1, 100); //I just want to set a minimum but not the maximum. How do I do that?
	RadiusValueBox->setSingleStep(1);
	RadiusValueBox->setValue(RadiusValue);
	pLayout->addWidget(RadiusValueBox, 0, 1, 1, 2);

	/*
	QLabel *pLable2 = new QLabel("Research window: ", this);
	pLayout->addWidget(pLable2, 1, 0);

	ResearchBox = new QDoubleSpinBox(this);
	ResearchBox->setRange(3, 11);
	ResearchBox->setSingleStep(2);
	ResearchBox->setValue(ResearchValue);
	pLayout->addWidget(ResearchBox, 1, 1, 1, 2);

	QLabel *pLable3 = new QLabel("Comparison window: ", this);
	pLable3->hide();
	pLayout->addWidget(pLable3, 2, 0);

	CompareBox = new QDoubleSpinBox(this);
	CompareBox->setRange(3, 11);
	CompareBox->setSingleStep(2);
	CompareBox->setValue(CompareValue);
	CompareBox->hide();
	pLayout->addWidget(CompareBox, 2, 1, 1, 2);
	*/

	QHBoxLayout *pRespLayout = new QHBoxLayout; //Response Layout
	pLayout->addLayout(pRespLayout, 3, 0, 1, 3);

	QPushButton *pAccept = new QPushButton("OK", this);
	pRespLayout->addStretch();
	pRespLayout->addWidget(pAccept);

	QPushButton *pReject = new QPushButton("Cancel", this);
	pRespLayout->addWidget(pReject);

	connect(pAccept, SIGNAL(clicked()), this, SLOT(accept()));
	connect(pReject, SIGNAL(clicked()), this, SLOT(reject()));
	connect(RadiusValueBox, SIGNAL(valueChanged(double)), this, SLOT(setRadiusValue(double)));
}


void median_filter_ui::setRadiusValue(double t)
{
	RadiusValue = t;
}

double median_filter_ui::getRadiusValue()
{
	return RadiusValue;
}