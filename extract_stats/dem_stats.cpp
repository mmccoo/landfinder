#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h" // for CPLMalloc()
#include <gdal/ogr_spatialref.h>

#include <iostream>
#include <limits>

// based on http://www.gdal.org/gdal_tutorial.html

int main()
{
  GDALDataset  *poDataset;

  GDALAllRegister();

  char pszFilename[] = "f2116.dem";
  
  poDataset = (GDALDataset *) GDALOpen( pszFilename, GA_ReadOnly );
  if( poDataset == NULL ) {
    std::cerr << "got an error\n";
    exit(-1);
  } 

  std::cout << "fmin " <<  std::numeric_limits<float>::min() << std::endl;
  std::cout << "imin " <<  std::numeric_limits<int>::min() << std::endl;
  
  std::cerr << "loaded\n";

  double        adfGeoTransform[6];

  printf( "Driver: %s/%s\n",
          poDataset->GetDriver()->GetDescription(), 
          poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );

  // rastercount may be > 1 if you have R, G, and B components or somesort of multilayer thing
  printf( "Size is %dx%dx%d\n", 
          poDataset->GetRasterXSize(), poDataset->GetRasterYSize(),
          poDataset->GetRasterCount() );

  if( poDataset->GetProjectionRef()  != NULL )
    printf( "Projection is `%s'\n", poDataset->GetProjectionRef() );

  if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None ) {
    printf( "Origin = (%.6f,%.6f)\n",
            adfGeoTransform[0], adfGeoTransform[3] );

    // given UTM, the pixel size is in meters.
    // y is negative because we're going north to south
    printf( "Pixel Size = (%.6f,%.6f)\n",
            adfGeoTransform[1], adfGeoTransform[5] );
  }

  double origX = adfGeoTransform[0];
  double origY = adfGeoTransform[3];


  GDALRasterBand  *poBand;
  int             nBlockXSize, nBlockYSize;
  int             bGotMin, bGotMax;
  double          adfMinMax[2];
        
  poBand = poDataset->GetRasterBand( 1 );
  poBand->GetBlockSize( &nBlockXSize, &nBlockYSize );
  printf("Block=%dx%d Type=%s, ColorInterp=%s\n",
         nBlockXSize, nBlockYSize,
         GDALGetDataTypeName(poBand->GetRasterDataType()),
         GDALGetColorInterpretationName(poBand->GetColorInterpretation())
         );

  adfMinMax[0] = poBand->GetMinimum( &bGotMin );
  adfMinMax[1] = poBand->GetMaximum( &bGotMax );
  if( ! (bGotMin && bGotMax) )
    GDALComputeRasterMinMax((GDALRasterBandH)poBand, TRUE, adfMinMax);

  printf( "Min=%.3f, Max=%.3f\n", adfMinMax[0], adfMinMax[1] );
        
  if( poBand->GetOverviewCount() > 0 )
    printf( "Band has %d overviews.\n", poBand->GetOverviewCount() );

  if( poBand->GetColorTable() != NULL )
    printf( "Band has a color table with %d entries.\n", 
            poBand->GetColorTable()->GetColorEntryCount() );


  float *pafScanline;
  int   nXSize = poBand->GetXSize();

  pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);

  /*
  CPLErr GDALRasterBand::RasterIO( GDALRWFlag eRWFlag,
                                 int nXOff, int nYOff, int nXSize, int nYSize,
                                 void * pData, int nBufXSize, int nBufYSize,
                                 GDALDataType eBufType,
                                 GSpacing nPixelSpace,
                                 GSpacing nLineSpace,
                                 GDALRasterIOExtraArg* psExtraArg )
  */
  poBand->RasterIO( GF_Read, 0, 1, nXSize, 1, 
                    pafScanline, nXSize, 1, GDT_Float32, 
                    0, 0 );

  const double nodata = poBand->GetNoDataValue();
  for(int i=0; i<nXSize;i++) {
    if (nodata == pafScanline[i]) {
      printf(".");
    } else {
      printf("%f ", pafScanline[i]);
    }
  } /* end of for(int i=0; i<nXSize;i++) */
  
  // Google Earth uses Simple Cylindrical projection with a WGS84 datum for its imagery base.
  // Lat Long
  OGRSpatialReference oTargetSRS;
  oTargetSRS.SetWellKnownGeogCS( "WGS84" );

  // UTM
  OGRSpatialReference oSourceSRS;
  oSourceSRS.SetWellKnownGeogCS("NAD27");
  oSourceSRS.SetUTM( 10, TRUE );

  OGRCoordinateTransformation *poCT;
  poCT = OGRCreateCoordinateTransformation( &oSourceSRS,
					    &oTargetSRS );

  double coordsX[4];
  double coordsY[4];
  coordsX[0] = origX;
  coordsY[0] = origY;

  coordsX[1] = origX + adfGeoTransform[1]*nBlockXSize ;
  coordsY[1] = origY;

  coordsX[2] = origX + adfGeoTransform[1]*nBlockXSize;
  coordsY[2] = origY + adfGeoTransform[5]*nBlockYSize;

  coordsX[3] = origX;
  coordsY[3] = origY + adfGeoTransform[5]*nBlockYSize;
  
  poCT->Transform( 4, coordsX, coordsY );
  printf("\n");
  for(int i=0; i<4; i++) {
    printf("%f,%f,0 ", coordsX[i], coordsY[i]);
  }
  printf("%f,%f,0 ", coordsX[0], coordsY[0]);
}
