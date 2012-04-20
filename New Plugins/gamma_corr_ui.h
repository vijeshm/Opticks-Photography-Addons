/* 
 * The information in this file is
 * Copyright (C) 2012, Vijesh M <mv.vijesh@gmail.com>
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef gamma_corr_ui_H
#define gamma_corr_ui_H

// #include "ExecutableShell.h"
#include <QtGui/QDialog>

class QDoubleSpinBox;

class gamma_corr_ui:public QDialog
{
	Q_OBJECT public:
	gamma_corr_ui(QWidget * pParent);
	QDoubleSpinBox *GammaValueBox;

	double getGammaValue();
	
	private slots: 
		void setGammaValue(double t = 0);

	private:
	  double GammaValue;
};

#endif
