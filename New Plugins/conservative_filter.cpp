/*
 * The information in this file is
 * Copyright(c) 2007 Ball Aerospace & Technologies Corporation
 * and is subject to the terms and conditions of the
 * GNU Lesser General Public License Version 2.1
 * The license text is available from   
 * http://www.gnu.org/licenses/lgpl.html
 */

#include "DataAccessor.h"
#include "DataAccessorImpl.h"
#include "DataRequest.h"
#include "DesktopServices.h"
#include "MessageLogResource.h"
#include "ObjectResource.h"
#include "PlugInArgList.h"
#include "PlugInManagerServices.h"
#include "PlugInRegistration.h"
#include "Progress.h"
#include "RasterDataDescriptor.h"
#include "RasterElement.h"
#include "RasterUtilities.h"
#include "SpatialDataView.h"
#include "SpatialDataWindow.h"
#include "switchOnEncoding.h"
#include "conservative_filter.h"
#include <limits>

#include <cmath>
#include <vector>
#include <algorithm>
#include "conservative_filter_ui.h"

REGISTER_PLUGIN_BASIC(PhotographyProcessingTools, conservative_filter);

namespace
{
   template<typename T>
   void verifyRange(T* pData, DataAccessor pSrcAcc, int row, int col, int rowSize, int colSize, int radius)
   {
	   int windowSize = (2*radius + 1);
	   std::vector<int> neighborhood;

	   for (int i = row - radius; i <= row + radius; i++)
	   {
		   if(  i >= 0 && i <= rowSize - 1)
		   {
			   for (int j = col - radius; j <= col + radius; j++ )
			   {
				   if (j >= 0 && j <= colSize - 1)
				   {
						if (i != row && j != col)
						{
							pSrcAcc->toPixel(i, j);
							VERIFYNRV(pSrcAcc.isValid());
							neighborhood.push_back( *reinterpret_cast<T*>(pSrcAcc->getColumn()));
						}
				   }
			   }
		   }
	   }

	   int min = *std::min_element(neighborhood.begin(), neighborhood.end());
	   int max = *std::max_element(neighborhood.begin(), neighborhood.end());

	   if(*pData < min)
		   *pData = static_cast<T>(min);

	   else if (*pData > max)
		   *pData = static_cast<T>(max);
   }
};

conservative_filter::conservative_filter()
{
   setDescriptorId("{BE00BBC3-A1E3-4b0d-8780-1B5D9A8620CD}"); //set this later!!
   setName("Conservative Filter");
   setVersion("1.0");
   setDescription("Apply the conservative filter algorithm with a given radius to the raster data");
   setCreator("Vijesh");
   setCopyright("Copyright (C) 2012, Vijesh M <mv.vijesh@gmail.com>");
   setProductionStatus(true);
   setType("Algorithm");
   setSubtype("Conservative Filter");
   setMenuLocation("[Photography]/[Hello There]/Conservative Filter");
   setAbortSupported(false);
}

conservative_filter::~conservative_filter()
{
}

bool conservative_filter::getInputSpecification(PlugInArgList*& pInArgList)
{
   VERIFY(pInArgList = Service<PlugInManagerServices>()->getPlugInArgList());
   pInArgList->addArg<Progress>(Executable::ProgressArg(), NULL, "Progress reporter");
   pInArgList->addArg<RasterElement>(Executable::DataElementArg(), "Apply conservative filter algorithm to the raster data");
   return true;
}

bool conservative_filter::getOutputSpecification(PlugInArgList*& pOutArgList)
{
   VERIFY(pOutArgList = Service<PlugInManagerServices>()->getPlugInArgList());
   pOutArgList->addArg<RasterElement>("Result", NULL);
   return true;
}

bool conservative_filter::execute(PlugInArgList* pInArgList, PlugInArgList* pOutArgList)
{
   StepResource pStep("Conservative", "Filter", "5EA0CC75-9E0B-4c3d-BA23-6DB7157BBD55"); //what is this?
   if (pInArgList == NULL || pOutArgList == NULL)
   {
      return false;
   }

   Service <DesktopServices> pDesktop;
   conservative_filter_ui dialog(pDesktop->getMainWidget());
   int status = dialog.exec();
   if (status == QDialog::Accepted)
   {
	   int radius = dialog.getRadiusValue();

   Progress* pProgress = pInArgList->getPlugInArgValue<Progress>(Executable::ProgressArg());
   RasterElement* pCube = pInArgList->getPlugInArgValue<RasterElement>(Executable::DataElementArg());
   if (pCube == NULL)
   {
      std::string msg = "A raster cube must be specified.";
      pStep->finalize(Message::Failure, msg);
      if (pProgress != NULL) 
      {
         pProgress->updateProgress(msg, 0, ERRORS);
      }
      return false;
   }
   RasterDataDescriptor* pDesc = static_cast<RasterDataDescriptor*>(pCube->getDataDescriptor());
   
	VERIFY(pDesc != NULL);

   if (pDesc->getDataType() == INT4SCOMPLEX || pDesc->getDataType() == FLT8COMPLEX)
   {
      std::string msg = "Conservative Filter cannot be performed on complex types.";
      pStep->finalize(Message::Failure, msg);
      if (pProgress != NULL) 
      {
         pProgress->updateProgress(msg, 0, ERRORS);
      }
      return false;
   }

   FactoryResource<DataRequest> pRequest;
   pRequest->setInterleaveFormat(BSQ);
   DataAccessor pSrcAcc = pCube->getDataAccessor(pRequest.release());

   ModelResource<RasterElement> pResultCube(RasterUtilities::createRasterElement(pCube->getName() + "_Conservative_Filter_Result", pDesc->getRowCount(), pDesc->getColumnCount(), pDesc->getDataType()));
   if (pResultCube.get() == NULL)
   {
      std::string msg = "A raster cube could not be created.";
      pStep->finalize(Message::Failure, msg);
      if (pProgress != NULL) 
      {
         pProgress->updateProgress(msg, 0, ERRORS);
      }
      return false;
   }
   FactoryResource<DataRequest> pResultRequest;
   pResultRequest->setWritable(true);
   DataAccessor pDestAcc = pResultCube->getDataAccessor(pResultRequest.release());

   for (unsigned int row = 0; row < pDesc->getRowCount(); ++row)
   {
      if (pProgress != NULL)
      {
         pProgress->updateProgress("Applying Conservative Filter", row * 100 / pDesc->getRowCount(), NORMAL);
      }
      if (isAborted())
      {
         std::string msg = getName() + " has been aborted.";
         pStep->finalize(Message::Abort, msg);
         if (pProgress != NULL)
         {
            pProgress->updateProgress(msg, 0, ABORT);
         }
         return false;
      }
      if (!pDestAcc.isValid())
      {
         std::string msg = "Unable to access the cube data.";
         pStep->finalize(Message::Failure, msg);
         if (pProgress != NULL) 
         {
            pProgress->updateProgress(msg, 0, ERRORS);
         }
         return false;
      }
      for (unsigned int col = 0; col < pDesc->getColumnCount(); ++col)
      {
         switchOnEncoding(pDesc->getDataType(), verifyRange, pDestAcc->getColumn(), pSrcAcc, row, col, pDesc->getRowCount(), pDesc->getColumnCount(), radius);
         pDestAcc->nextColumn();
      }
      pDestAcc->nextRow();
   }

   if (!isBatch())
   {
      Service<DesktopServices> pDesktop;

      SpatialDataWindow* pWindow = static_cast<SpatialDataWindow*>(pDesktop->createWindow(pResultCube->getName(),
         SPATIAL_DATA_WINDOW));

      SpatialDataView* pView = (pWindow == NULL) ? NULL : pWindow->getSpatialDataView();
      if (pView == NULL)
      {
         std::string msg = "Unable to create view.";
         pStep->finalize(Message::Failure, msg);
         if (pProgress != NULL) 
         {
            pProgress->updateProgress(msg, 0, ERRORS);
         }
         return false;
      }

      pView->setPrimaryRasterElement(pResultCube.get());
      pView->createLayer(RASTER, pResultCube.get());
   }

   if (pProgress != NULL)
   {
      pProgress->updateProgress("COnservative Filter is complete", 100, NORMAL);
   }

   pOutArgList->setPlugInArgValue("conservative_filter_result", pResultCube.release());

   pStep->finalize();
   }
   return true;
}