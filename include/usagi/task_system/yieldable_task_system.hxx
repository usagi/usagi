#include <boost/coroutine/all.hpp>
#include <boost/signals2/signal.hpp>
#include <memory>

namespace usagi
{
  namespace task_system
  {
    
    class yieldable_task_system_type
    {
      boost::signals2::signal< auto () -> void > signal;
      
    public:
      template < typename COROUTINE_TYPE >
      auto push( COROUTINE_TYPE&& coroutine ) -> void
      {
        push( boost::coroutines::coroutine< void >::pull_type( std::move( coroutine ) ) );
      }
      
      auto push( boost::coroutines::coroutine< void >::pull_type&& coroutine ) -> void
      {
        auto shared_coroutine  = std::make_shared< boost::coroutines::coroutine< void >::pull_type >( std::move( coroutine ) );
        auto shared_connection = std::make_shared< boost::signals2::connection >();
        *shared_connection = signal.connect
        ( [ = ]
          {
            if ( not (*shared_coroutine)() )
              shared_connection->disconnect();
          }
        );
      }
      
      virtual auto consume_one() -> bool
      {
        signal();
        return not signal.empty();
      }
      
      auto consume_all() -> void
      {
        while ( consume_one() );
      }
      
      auto operator()() -> void
      {
        consume_all();
      }
    };
    
  }
}

