/* 
 * The information in this file is
 * Copyright (C) 2011, Pratik Anand <pratik@pratikanand.com>
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef median_noise_ui_H
#define median_noise_ui_H

// #include "ExecutableShell.h"
#include <QtGui/QDialog>

class QDoubleSpinBox;

class conservative_filter_ui:public QDialog
{
	Q_OBJECT public:
	conservative_filter_ui(QWidget * pParent);
	QDoubleSpinBox *RadiusValueBox;

	double getRadiusValue();
	
	private slots: 
		void setRadiusValue(double t = 0);

	private:
	  double RadiusValue;
};

#endif
