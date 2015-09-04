#include <usagi/container/circular_tile.hxx>

#include <iostream>

auto main() -> int
{
  usagi::container::circular_tile< float > tiles( 7 );
  tiles.from_center(  0,  0 ) = 1;
  tiles.from_center( -1,  0 ) = 2;
  tiles.from_center(  0, -2 ) = 3;
  tiles.from_center(  4,  4 ) = 4;
  tiles.shift_to_left({1,2,3,4,5,6,7});
  tiles.shift_to_bottom({1,2,3,4,5,6,7});
  
  std::cout << tiles << "\n";
}
