#include <set>
#include <string>
#include <boost/algorithm/string/join.hpp>
#include <boost/range/adaptor/transformed.hpp>
#include <iostream>

  boost::range_detail::transform_holder<std::basic_string<char> (*)(int)> int_to_str = 
    boost::adaptors::transformed( static_cast<std::string(*)(int)>(std::to_string) );

main()
{
  std::set<std::string> ss;
  ss.insert("hello");
  ss.insert("world");
  std::cout << boost::algorithm::join(ss, " ") << std::endl;


  std::set<int> s;
  s.insert(1);
  s.insert(2);
  std::cout << boost::algorithm::join(s|int_to_str,
				      " ") << std::endl;

}
