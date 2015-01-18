
#include <iostream>
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

namespace gtl = boost::polygon;
using namespace boost::polygon::operators;

typedef gtl::polygon_data<int> polygon_type;
typedef gtl::polygon_traits<polygon_type>::point_type point_type;
typedef gtl::polygon_set_data<int> polygon_set_type;
typedef std::map<std::set<int>, polygon_set_type> property_merge_result_type;

typedef gtl::property_merge<int, int> pm_type;

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

void bound_to_poly(std::string &bound, polygon_type &poly)
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
    poly_points.push_back(gtl::construct<point_type>(int(x*10000), int(y*10000)));
  }
    
  gtl::set_points(poly, poly_points.begin(), poly_points.end());
}


int main(int argc, char* argv[])
{
  const char* dbfile = "../Cowlitz/parcels.db";

  // Open a database file in readonly mode
  SQLite::Database    dbobj(dbfile);  // SQLITE_OPEN_READONLY
  std::cout << "SQLite database file '" << dbobj.getFilename().c_str() << "' opened successfully\n";

  // Test if the 'test' table exists
  bool bExists = dbobj.tableExists("Parcels");
  std::cout << "SQLite table 'test' exists=" << bExists << "\n";

  SQLite::Statement   query(dbobj, "SELECT PARCNO, ACCOUNTNO, Coordinates FROM Parcels limit 2");

  const boost::regex e("(\\-?\\d+(\\.\\d+)?),(\\-?\\d+(\\.\\d+)?)\\s*");




  // Loop to execute the query step by step, to get rows of result
  while (query.executeStep()) {
    std::cout << 
      query.getColumn(0).getText() << " " << 
      query.getColumn(1).getText() << std::endl << " " << 
      query.getColumn(2).getText() << std::endl << std::endl;

    polygon_type poly;
    std::string bound(query.getColumn(2).getText());
    bound_to_poly(bound, poly);

    std::cout << "poly " << poly << std::endl;

  }

  exit(0);
  std::vector<gtl::rectangle_data<int> > test_data;
  test_data.push_back(gtl::rectangle_data<int>(11, 10, 31, 30));
  test_data.push_back(gtl::rectangle_data<int>(1, 0, 21, 20));
  test_data.push_back(gtl::rectangle_data<int>(6, 15, 16, 25));

  pm_type pm;

  //insert our test geometry into the property merge algorithm
  for(unsigned int i = 0; i < test_data.size(); ++i) {
    pm.insert(test_data[i], i); //notice I use the index as the property value
  }

  property_merge_result_type result;
  pm.merge(result);

  property_merge_result_type::const_iterator rtiter;
  for(rtiter=result.begin(); rtiter!=result.end(); ++rtiter) {
    std::cout << "for set: " << (*rtiter).first << std::endl;
    std::vector<polygon_type> polys;
    (*rtiter).second.get(polys);
    std::cout << polys;
  }

}
