
#include <set>
#include <boost/foreach.hpp>
#include <boost/algorithm/string/join.hpp>
#include <boost/polygon/polygon.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <iostream>




boost::range_detail::transform_holder<std::basic_string<char> (*)(int)> int_to_str = 
  boost::adaptors::transformed( static_cast<std::string(*)(int)>(std::to_string) );


namespace gtl = boost::polygon;
using namespace boost::polygon::operators;


typedef gtl::polygon_data<int> polygon_type;
typedef gtl::polygon_traits<polygon_type>::point_type point_type;
typedef gtl::polygon_set_data<int> polygon_set_type;
typedef std::map<std::set<int>, polygon_set_type> property_merge_result_type;

typedef gtl::property_merge<int, int> pm_type;

void test_pm() {
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
    std::set<int> s = (*rtiter).first;
    std::cout << "for set: " << boost::algorithm::join(s|int_to_str, " ") << std::endl;
    std::vector<polygon_type> polys;
    (*rtiter).second.get(polys);
    std::vector<polygon_type>::const_iterator psiter;
    for(psiter=polys.begin(); psiter!=polys.end(); ++psiter) {
      boost::polygon::polygon_data<int>::iterator_type ptiter;
      for(ptiter = (*psiter).begin(); ptiter != (*psiter).end(); ++ptiter) {
	std::cout << (*ptiter).x() << "," << (*ptiter).y() << " ";
      }
      std::cout << std::endl;
    }
  }

}

int main() {
  test_pm();
  return 0;
}
//Now you know how to use the manhattan and arbitrary angle property
//merge algorithms to perform map overlay on n layers of input geometry
 
