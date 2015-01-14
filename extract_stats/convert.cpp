#include <gdal/ogr_spatialref.h>
#include <iostream>

int main()
{
  // Google Earth uses Simple Cylindrical projection with a WGS84 datum for its imagery base.
  // Lat Long
  OGRSpatialReference oSourceSRS;
  oSourceSRS.SetWellKnownGeogCS( "WGS84" );

  // UTM
  OGRSpatialReference oTargetSRS;
  oTargetSRS.SetWellKnownGeogCS("NAD27");
  oTargetSRS.SetUTM( 10, TRUE );

  OGRCoordinateTransformation *poCT;
  poCT = OGRCreateCoordinateTransformation( &oSourceSRS,
					    &oTargetSRS );

  double                  x, y;
  x = -120.4477801704399;
  y = 44.21157786092454;

  if( poCT == NULL || !poCT->Transform( 1, &x, &y ) )
    printf( "Transformation failed.\n" );
  else
    printf("(%f,%f)\n", 
	    x, y );
  return 0;
}
