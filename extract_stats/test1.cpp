#include "gdal_priv.h"
#include "cpl_conv.h" // for CPLMalloc()


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
  poBand->RasterIO( GF_Read, 0, 0, nXSize, 1, 
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
  
}
