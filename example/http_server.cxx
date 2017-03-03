/// @note Boost-1.63.0 で Asio が内部的に使う Coroutine について warning となるため
#define BOOST_COROUTINES_NO_DEPRECATION_WARNING

#include <usagi/http/server_type.hxx>

#include <thread>
#include <chrono>

auto main() -> int
{
  using namespace std;
  using namespace usagi;
  
  {
    atomic< bool > is_running;
    
    auto s = http::server_type::make_unique( 55555, "127.0.0.1" );
    
    s->only_localhost = true;
    
    s->add_handler
    ( // path
      "/"
    , [&]
      ( const auto& method, const auto& path, const auto& headers, const auto& body )
        mutable
        -> http::handler_return_type
      {
        is_running = false;
        return 
          { // headers
            { { "content-type"s, "text/html"s }
            }
            // body
          , "<h1>usagi::http::server_type example<h1>\n<p>hello?</p>"s
          }
          ;
      }
    );
    
    cout << "start server ( port = " << s->port << " )\n" << flush;
    
    is_running = true;
    
    thread( [&]{ (*s)(); } ).detach();
    
    while( is_running )
      this_thread::sleep_for( std::chrono::milliseconds( 100 ) );
  }
  
  cout << "server was stopped\n" << flush;
}