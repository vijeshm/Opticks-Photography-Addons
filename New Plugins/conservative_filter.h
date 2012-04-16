/* 
 * The information in this file is
 * Copyright (C) 2012, Vijesh M <mv.vijesh@gmail.com>
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#ifndef conservative_filter_H
#define conservative_filter_H

#include "ExecutableShell.h"

class conservative_filter:public ExecutableShell
{
  public:
	conservative_filter();
	virtual ~conservative_filter();

	virtual bool getInputSpecification(PlugInArgList * &pInArgList);
	virtual bool getOutputSpecification(PlugInArgList * &pOutArgList);
	virtual bool execute(PlugInArgList * pInArgList,PlugInArgList * pOutArgList);
};
#endif
