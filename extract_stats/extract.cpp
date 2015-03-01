
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <set>
#include <list>

#include <sqlite3.h> 
#include <SQLiteCpp/SQLiteCpp.h>
#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/foreach.hpp>
#include <boost/foreach.hpp>
#include <boost/polygon/polygon.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <boost/regex.hpp>

#include "gdal/gdal_priv.h"
#include "gdal/cpl_conv.h" // for CPLMalloc()
#include <gdal/ogr_spatialref.h>

#include <chrono>

namespace gtl = boost::polygon;
using namespace boost::polygon::operators;

typedef gtl::polygon_data<int> polygon_type;
typedef gtl::polygon_traits<polygon_type>::point_type point_type;
typedef gtl::polygon_set_data<int> polygon_set_type;
typedef std::map<std::set<int>, polygon_set_type> property_merge_result_type;

typedef gtl::property_merge<int, int> pm_type;

struct DemInfo {
  const char* filename;
  GDALDataset  *poDataset;
  GDALRasterBand  *poBand;
  int width;
  int height;
  double xOrigin;
  double yOrigin;
  double xRes;
  double yRes;
  int UTMZone; 
  OGRCoordinateTransformation *poL2U;
  OGRCoordinateTransformation *poU2L;
};

std::ostream& operator<<(std::ostream& stream, const std::set<int> &s) 
{
  bool first = true;
  BOOST_FOREACH(int i, s) {
    if (!first) { stream << ", "; }
    stream << i;
    first = false;
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const polygon_type &poly)
{
  boost::polygon::polygon_data<int>::iterator_type pditer;
  for(pditer = poly.begin(); pditer != poly.end(); ++pditer) {
    stream << (*pditer).x() << "," << (*pditer).y() << " ";
  }
  return stream;
}

std::ostream& operator<<(std::ostream& stream, const std::vector<polygon_type> &polys)
{
  std::vector<polygon_type>::const_iterator psiter;
  for(psiter=polys.begin(); psiter!=polys.end(); ++psiter) {
stream << (*psiter);
stream << std::endl;
}
}

void write_poly_kml(std::ostream& stream, 
		    const polygon_type &poly,  
		    DemInfo &di)
{
  boost::polygon::polygon_data<int>::iterator_type pditer;
  for(pditer = poly.begin(); pditer != poly.end(); ++pditer) {
    double x = (*pditer).x();
    double y = (*pditer).y();
    di.poU2L->Transform( 1, &x, &y );
    stream << x << "," << y << ",50 ";
  }
  stream << std::endl;
}

const char* aspects[] = {
"ne",
  "se",
  "sw",
  "nw"
  };

void write_aspect_kml(std::ostream& stream, 
			property_merge_result_type &result, 
			std::vector<std::pair<std::string, std::string> >&parcels, 
			DemInfo &di)
{
  std::ifstream file;
  file.open("aspectbegin.txt", std::ios::binary);
  std::string line ;
  //while( std::getline( file, line, '.' ) ) { stream << line << '\n'; }
  stream << file.rdbuf();
  file.close();

  property_merge_result_type::const_iterator rtiter;
  for(rtiter=result.begin(); rtiter!=result.end(); ++rtiter) {
    const std::set<int>& s = (*rtiter).first;
    bool hasneg = false;
    int parcelnum = -1;
    BOOST_FOREACH(int i, s) { 
      if (i<0) { 
		      hasneg = true; 
      } else {
	parcelnum = i;
      }
    }
    if (!hasneg) { continue; }
    if (parcelnum == -1) { continue; }
if (parcels[parcelnum].first != "WN1001001") { continue; }
		      //stream << "for set: " << (*rtiter).first << std::endl;
    std::vector<polygon_type> polys;
    (*rtiter).second.get(polys);
    BOOST_FOREACH(int i, s) {
      if(i>=0) { continue; }
      std::vector<polygon_type>::const_iterator psiter;
      for(psiter=polys.begin(); psiter!=polys.end(); ++psiter) {
      	stream << "<Placemark>\n";
      	stream << "  <name>" << parcels[parcelnum].first << "</name>\n";
      	stream << "  <styleUrl>#aspect_" << aspects[-i-1] << "</styleUrl>\n";
      	stream << "  <Polygon>\n";
      	stream << "    <tessellate>1</tessellate>\n";
      	stream << "    <altitudeMode>relativeToGround</altitudeMode>\n";
      	stream << "    <outerBoundaryIs>\n";
      	stream << "      <LinearRing>\n";
      	stream << "        <coordinates>\n";
      	write_poly_kml(stream, *psiter,  di);
      	stream << "        </coordinates>\n";
      	stream << "      </LinearRing>\n";
      	stream << "    </outerBoundaryIs>\n";
      	stream << "  </Polygon>\n";
      	stream << "</Placemark>\n";
      }
    }
  }
  
  file.open("aspectend.txt", std::ios::binary);
  //while( std::getline( file, line, '.' ) ) { stream << line << '\n'; }
  stream << file.rdbuf();
  file.close();
}


void bound_to_poly(std::string &bound, polygon_type &poly,  DemInfo &di)
{
  std::list<std::string> nums;
  split(nums, bound, boost::is_any_of(", "), boost::token_compress_on ); 

  std::vector<point_type> poly_points;
  while(!nums.empty()) {
    if (nums.front() == "") {
      nums.pop_front();
      continue;
    }
    double x = atof(nums.front().c_str());
    nums.pop_front(); 
    double y = atof(nums.front().c_str());
    nums.pop_front();
    // lose the extra 0
    nums.pop_front();
    di.poL2U->Transform( 1, &x, &y );
    poly_points.push_back(gtl::construct<point_type>(int(x), int(y)));
  }
    
  gtl::set_points(poly, poly_points.begin(), poly_points.end());
}

void print_dem_info(const char* filename, GDALDataset  *poDataset)
{
  printf("for file %s\n", filename);
  // USGSDEM/USGS Optional ASCII DEM (and CDED)
  printf( "Driver: %s/%s\n",
          poDataset->GetDriver()->GetDescription(), 
          poDataset->GetDriver()->GetMetadataItem( GDAL_DMD_LONGNAME ) );

  printf( "Size is %dx%dx%d\n", 
          poDataset->GetRasterXSize(), poDataset->GetRasterYSize(),
          poDataset->GetRasterCount() );

  // PROJCS["UTM Zone 10, Northern Hemisphere",
  //        GEOGCS["NAD27",DATUM["North_American_Datum_1927"
  if( poDataset->GetProjectionRef()  != NULL ) {
    printf( "Projection is `%s'\n", poDataset->GetProjectionRef() );
  }

}
	
bool get_dem_info(const char* filename, DemInfo &di)
{

  GDALDataset  *poDataset = di.poDataset = 
    (GDALDataset *) GDALOpen(filename, GA_ReadOnly );
  if( poDataset == NULL ) {
    std::cerr << "got an errorreading dataset " << filename << "\n";
    return false;
  } 

  OGRSpatialReference sref(poDataset->GetProjectionRef());
  int north;
  di.UTMZone = sref.GetUTMZone(&north);
  printf("zone %d\n", di.UTMZone);

  double        adfGeoTransform[6];
  if( poDataset->GetGeoTransform( adfGeoTransform ) == CE_None ) {
    printf( "Origin = (%.6f,%.6f)\n",
            adfGeoTransform[0], adfGeoTransform[3] );
    di.xOrigin = adfGeoTransform[0];
    di.yOrigin = adfGeoTransform[3];

    // given UTM, the pixel size is in meters.
    // y is negative because we're going north to south
    printf( "Pixel Size = (%.6f,%.6f)\n", adfGeoTransform[1], adfGeoTransform[5] );
    di.xRes = adfGeoTransform[1];
    di.yRes = adfGeoTransform[5];


  }

  // rastercount may be > 1 if you have R, G, and B components or 
  // somesort of multilayer thing      
  printf("raster count %d\n", poDataset->GetRasterCount());
  di.poBand = poDataset->GetRasterBand(1);
  di.width = di.poBand->GetXSize();
  di.height = di.poBand->GetYSize();
  //di.poBand->GetBlockSize( &(di.width), &(di.height) );
  printf("Block=%dx%d Type=%s, ColorInterp=%s\n",
         di.width, di.height,
         GDALGetDataTypeName(di.poBand->GetRasterDataType()),
         GDALGetColorInterpretationName(di.poBand->GetColorInterpretation())
         );

  printf("x %d y %d\n", di.poBand->GetXSize(), di.poBand->GetYSize());

  // Google Earth uses Simple Cylindrical projection with a WGS84 datum for its imagery base.
  // Lat Long
  OGRSpatialReference oSourceSRS;
  oSourceSRS.SetWellKnownGeogCS( "WGS84" );

  // UTM
  OGRSpatialReference oTargetSRS;
  oTargetSRS.SetWellKnownGeogCS("NAD27");
  oTargetSRS.SetUTM( di.UTMZone, TRUE );

  di.poL2U = OGRCreateCoordinateTransformation( &oSourceSRS,
						&oTargetSRS );
  di.poU2L = OGRCreateCoordinateTransformation( &oTargetSRS,
						&oSourceSRS );


}	    

int main(int argc, char* argv[])
{

  GDALAllRegister();

  const char* dbfile = "../Cowlitz/parcels.db";

  // Open a database file in readonly mode
  SQLite::Database    dbobj(dbfile);  // SQLITE_OPEN_READONLY
  std::cout << "SQLite database file '" << dbobj.getFilename().c_str() << "' opened successfully\n";

  // Test if the 'test' table exists
  bool bExists = dbobj.tableExists("Parcels");
  std::cout << "SQLite table 'test' exists=" << bExists << "\n";

  SQLite::Statement   query(dbobj, "SELECT PARCNO, ACCOUNTNO, Coordinates FROM Parcels");

  DemInfo di;
  get_dem_info("../Cowlitz/dems/f2216_aspect.tif", di);

  pm_type pm;
  std::vector<std::pair<std::string, std::string> > parcels;
  // Loop to execute the query step by step, to get rows of result
  while (query.executeStep()) {
#if 0
    std::cout << 
      query.getColumn(0).getText() << " " << 
      query.getColumn(1).getText() << std::endl << " " << 
      query.getColumn(2).getText() << std::endl << std::endl;
#endif

    parcels.push_back(std::pair<std::string, std::string>(query.getColumn(0).getText(), query.getColumn(1).getText()));

    polygon_type poly;
    std::string bound(query.getColumn(2).getText());
    bound_to_poly(bound, poly, di);

    //std::cout << "poly " << poly << std::endl;
    pm.insert(poly, parcels.size()-1);
  }



  int   nXSize = di.poBand->GetXSize();
  float *pafScanline = (float *) CPLMalloc(sizeof(float)*nXSize);
  const double nodata = di.poBand->GetNoDataValue();

  int num_added=0;
  for(int y=0; y<di.height/8; y++) {
    di.poBand->RasterIO( GF_Read, 0, y, nXSize, 1, 
			 pafScanline, nXSize, 1, GDT_Float32, 
			 0, 0 );

    std::vector<gtl::rectangle_data<int> > test_data;
    for(int x=0; x<nXSize;x++) {
      if (nodata == pafScanline[x]) {
	continue;
      } else {
	//printf("%f ", pafScanline[x]);
	pm.insert(gtl::rectangle_data<int>(int((di.xOrigin+x*di.xRes)),
					   int((di.yOrigin+y*di.yRes)),
					   int((di.xOrigin+(x+1)*di.xRes)),
					   int((di.yOrigin+(y+1)*di.yRes))),
		  -int(pafScanline[x]/90.0)-1);
	num_added++;
      }      
    } /* end of for(int x=0; i<nXSize;i++) */
  } // end for y
  
  std::cout << "got " << num_added << " rectangles" << std::endl;

  property_merge_result_type result;

  std::chrono::time_point<std::chrono::system_clock> start, end;
  start = std::chrono::system_clock::now();
  pm.merge(result);
  end = std::chrono::system_clock::now();
 
  std::chrono::duration<double> elapsed_seconds = end-start;

  std::cout << "done merging in " << elapsed_seconds.count() << "s\n";

  write_aspect_kml(std::cout, 
		   result, 
		   parcels, 
		   di);
  
#if 0

  property_merge_result_type::const_iterator rtiter;
  for(rtiter=result.begin(); rtiter!=result.end(); ++rtiter) {
    const std::set<int>& s = (*rtiter).first;
    if (s.size() <= 2) { continue; }
    bool hasneg = false;
    BOOST_FOREACH(int i, s) { if (i<0) { hasneg = true; }}
    if (!hasneg) { continue; }
      
    std::cout << "for set: " << (*rtiter).first << std::endl;
    std::vector<polygon_type> polys;
    (*rtiter).second.get(polys);
    BOOST_FOREACH(int i, s) {
      if(i<0) { continue; }
      std::cout << "p " << parcels[i].first << " " << parcels[i].second << std::endl;
    }
    write_poly_kml(std::cout, polys, di);
  }
#endif

#if 0
  exit(0);
  std::vector<gtl::rectangle_data<int> > test_data;
  test_data.push_back(gtl::rectangle_data<int>(11, 10, 31, 30));
  test_data.push_back(gtl::rectangle_data<int>(1, 0, 21, 20));
  test_data.push_back(gtl::rectangle_data<int>(6, 15, 16, 25));

  //pm_type pm;

  //insert our test geometry into the property merge algorithm
  for(unsigned int i = 0; i < test_data.size(); ++i) {
    pm.insert(test_data[i], i); //notice I use the index as the property value
  }

  //property_merge_result_type result;
  pm.merge(result);

  property_merge_result_type::const_iterator rtiter;
  for(rtiter=result.begin(); rtiter!=result.end(); ++rtiter) {
    std::cout << "for set: " << (*rtiter).first << std::endl;
    std::vector<polygon_type> polys;
    (*rtiter).second.get(polys);
    std::cout << polys;
  }
#endif
}
