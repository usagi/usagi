#include <usagi/http/get.hxx>

auto main() -> int
{
  std::cout << usagi::http::get( "https://www.google.co.jp/" );
}