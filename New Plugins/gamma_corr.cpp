/* 
 * The information in this file is
 * Copyright (C) 2012, Vijesh M <mv.vijesh@gmail.com>
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
#include "StringUtilities.h"

#include "gamma_corr.h"
#include "gamma_corr_ui.h"

#include <limits>
#include <cmath>

REGISTER_PLUGIN_BASIC(PhotographyProcessingTools, gamma_corr);

#define MAX_SIZE 15

namespace
{
	template < typename T >
	void correctGamma(T * pData, DataAccessor srcDataAcc, int curRow, int curCol, double gamma)
	{
		srcDataAcc->toPixel(curRow, curCol);
		VERIFYNRV(srcDataAcc.isValid());
		int pixelValue = *reinterpret_cast<T*>(srcDataAcc->getColumn());

		*pData = static_cast<T>( int(255 * pow(double(pixelValue) / 255, gamma)) );
	}
};

bool analyzeChannel(RasterElement *pRaster, RasterElement *dRaster, int channel, Progress *pProgress, double gamma)
{
	// int flag = 0;
	// int size = 3; //Pratik has fixed the radius to 3.
	// int sizeMax = MAX_SIZE;

	VERIFY(pRaster != NULL);
	RasterDataDescriptor *pDesc = dynamic_cast < RasterDataDescriptor * >(pRaster->getDataDescriptor());

	VERIFY(dRaster != NULL);
	RasterDataDescriptor *rDesc = dynamic_cast < RasterDataDescriptor * >(dRaster->getDataDescriptor());

	// source
	DimensionDescriptor activeBand = pDesc->getActiveBand(channel);	// get active band
	FactoryResource < DataRequest > pSourceRequest;
	pSourceRequest->setInterleaveFormat(BSQ);
	pSourceRequest->setBands(activeBand, activeBand);
	DataAccessor srcDataAcc = pRaster->getDataAccessor(pSourceRequest.release());

	// destination
	activeBand = rDesc->getActiveBand(channel);
	FactoryResource < DataRequest > pResultRequest;
	pResultRequest->setWritable(true);
	pResultRequest->setInterleaveFormat(BSQ); //This was initially pRequest. Isnt that wrong?
	pResultRequest->setBands(activeBand, activeBand);
	DataAccessor destDataAcc = dRaster->getDataAccessor(pResultRequest.release());

	VERIFY(srcDataAcc.isValid());
	VERIFY(destDataAcc.isValid());

	for (unsigned int curRow = 0; curRow < pDesc->getRowCount(); ++curRow)
	{
		for (unsigned int curCol = 0; curCol < pDesc->getColumnCount(); ++curCol)
		{
			VERIFY(destDataAcc.isValid());
			switchOnEncoding(pDesc->getDataType(), correctGamma, destDataAcc->getColumn(), srcDataAcc, curRow, curCol, gamma);
		}
		destDataAcc->nextRow();
	}

	return true;
}


gamma_corr::gamma_corr()
{
	setDescriptorId("{FF1EFA03-0888-4199-AB40-0503C9FABC80}");
	setName("gamma_corr");
	setDescription("Perform Gamma Correction on the raster data");
	setCreator("Vijesh M");
	setVersion("0.1");
	setCopyright("Copyright (C) 2012, Vijesh M <mv.vijesh@gmail.com>");
	setProductionStatus(true);
	setType("Algorithm");
	setSubtype("Gamma Correction");
	setMenuLocation("[Photography]/Gamma Correction");
	setAbortSupported(false);
}

gamma_corr::~gamma_corr()
{
}

bool gamma_corr::getInputSpecification(PlugInArgList * &pInArgList)
{
	VERIFY(pInArgList = Service < PlugInManagerServices > ()->getPlugInArgList());
	pInArgList->addArg < Progress > (Executable::ProgressArg(), NULL, "Progress reporter");
	pInArgList->addArg < RasterElement > (Executable::DataElementArg(), "Perform Gamma Correction on this Raster Element");
	return true;
}

bool gamma_corr::getOutputSpecification(PlugInArgList * &pOutArgList)
{
	VERIFY(pOutArgList = Service < PlugInManagerServices > ()->getPlugInArgList());
	pOutArgList->addArg < RasterElement > ("Gamma Correction Result", NULL);
	return true;
}

bool gamma_corr::execute(PlugInArgList *pInArgList, PlugInArgList *pOutArgList)
{
	StepResource pStep("gamma", "corr", "5EA0CC75-9E0B-4c3d-BA23-6DB7157BBD56");

	if (pInArgList == NULL || pOutArgList == NULL)
	{
		return false;
	}

	Service <DesktopServices> pDesktop;
	gamma_corr_ui dialog(pDesktop->getMainWidget());
   	int status = dialog.exec();
   	if (status == QDialog::Accepted)
	{	
		int gamma = dialog.getGammaValue();
		std::string msg = "Gamma Correction";
		Progress *pProgress = pInArgList->getPlugInArgValue < Progress > (Executable::ProgressArg());
		RasterElement *pCube = pInArgList->getPlugInArgValue < RasterElement >(Executable::DataElementArg());
	
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
	
		RasterDataDescriptor *pDesc = static_cast < RasterDataDescriptor * >(pCube->getDataDescriptor());
		VERIFY(pDesc != NULL);
		if (pDesc->getDataType() == INT4SCOMPLEX || pDesc->getDataType() == FLT8COMPLEX)
		{
			std::string msg = "Gamma Correction cannot be performed on complex types";
			pStep->finalize(Message::Failure, msg);
			if (pProgress != NULL)
			{
				pProgress->updateProgress(msg, 0, ERRORS);
			}
			return false;
		}
	
		FactoryResource < DataRequest > pRequest;
		pRequest->setInterleaveFormat(BSQ);
		DataAccessor pSrcAcc = pCube->getDataAccessor(pRequest.release());
	
		RasterElement *dRas = RasterUtilities::createRasterElement(pCube->getName() + "Gamma_Correction_filter", pDesc->getRowCount(), pDesc->getColumnCount(), 3, pDesc->getDataType(), BSQ);
	
		pProgress->updateProgress(msg, 50, NORMAL);
	
		analyzeChannel(pCube, dRas, 0, pProgress, gamma);
		pProgress->updateProgress(msg + " RED complete", 60, NORMAL);
	
		analyzeChannel(pCube, dRas, 1, pProgress, gamma);
		pProgress->updateProgress(msg + " GREEN complete", 70, NORMAL);
	
		analyzeChannel(pCube, dRas, 2, pProgress, gamma);
		pProgress->updateProgress(msg + " BLUE complete", 80, NORMAL);
	
		// new model resource
		RasterDataDescriptor *rDesc = dynamic_cast < RasterDataDescriptor * >(dRas->getDataDescriptor());
		rDesc->setDisplayMode(RGB_MODE);	// enable color mode
		rDesc->setDisplayBand(RED, rDesc->getActiveBand(0));
		rDesc->setDisplayBand(GREEN, rDesc->getActiveBand(1));
		rDesc->setDisplayBand(BLUE, rDesc->getActiveBand(2));
	
		ModelResource < RasterElement > pResultCube(dRas);
	
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
	
		pProgress->updateProgress("Final", 100, NORMAL);
	
		pProgress->updateProgress(msg, 100, NORMAL);
	
		if (!isBatch())
		{
			Service < DesktopServices > pDesktop;
	
			SpatialDataWindow *pWindow = static_cast <SpatialDataWindow *>(pDesktop->createWindow(pResultCube->getName(), SPATIAL_DATA_WINDOW));
	
			SpatialDataView *pView = (pWindow == NULL) ? NULL : pWindow->getSpatialDataView();
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
			pProgress->updateProgress("gamma_corr is compete.", 100, NORMAL);
		}
	
		pOutArgList->setPlugInArgValue("gamma_corr_Result", pResultCube.release());
		pStep->finalize();
	}
	return true;
}