#include <usagi/task_system/yieldable_task_system.hxx>
#include <chrono>
#include <iostream>

auto main() -> int
{
  using namespace std::chrono;
  
  usagi::task_system::yieldable_task_system_type yieldable_task_system;
  
  yieldable_task_system.push
  ( [ t0 = high_resolution_clock::now() ]
    ( auto& yield )
    {
      std::cout << 'a' << std::flush;
      yield();
      
      do
      {
        std::cout << 'b' << std::flush;
        yield();
      }
      while ( high_resolution_clock::now() - t0 < microseconds( 512 ) );
      
      std::cout << 'c' << std::flush;
      yield();
    }
  );
  
  yieldable_task_system.push
  ( []
    ( auto& yield )
    {
      for ( const auto value : { 'X', 'Y', 'Z' } )
      {
        std::cout << value << std::flush;
        yield();
      }
    }
  );
  
  yieldable_task_system.push
  ( []
    ( auto& yield )
    {
      std::cout << '1' << std::flush;
      yield();
      std::cout << '2' << std::flush;
      yield();
    }
  );
  
  yieldable_task_system();
}
