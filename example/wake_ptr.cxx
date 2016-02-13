#include <usagi/memory/wake_ptr.hxx>

#include <vector>
#include <iostream>

auto main() -> int
{
  using t = std::vector<int>;
  auto b = std::make_shared< t >();
  
  using namespace usagi::memory;
  wake_ptr< t > a = b;
  a->emplace_back( 123 );
  a->emplace_back( 456 );
  std::cout << a->front() << (*a)[1];
}
