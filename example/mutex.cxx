#include <usagi/mutex.hxx>
#include <iostream>
#include <thread>
#include <memory>
#include <stdexcept>

struct values_type
{
  int x = 0, y = 0, z = 0;
};

auto main() -> int
{
  auto to_exit = std::make_shared< bool >( false );
  auto values = std::make_shared< values_type >();
  auto read_write_mutex = std::make_shared< usagi::mutex::read_write_mutex_type >();
  
  std::thread reader
  ( [=]
    {
      auto sum = 0;
      while ( sum < 256 )
      {
        {
          usagi::mutex::read_lock_type read_lock( *read_write_mutex );
          sum = values->x + values->y + values->z;
        }
        if ( sum % 3 )
          throw std::logic_error( "what's !?" );
        else
          std::cout << sum << std::endl;
      }
      *to_exit = true;
    }
  );
  
  do
  {
    {
      usagi::mutex::write_lock_type write_lock( *read_write_mutex );
      ++values->x;
      ++values->y;
      ++values->z;
    }
  }
  while ( not *to_exit );
  
  reader.join();
}
