//#define DISABLE_USAGI_LOG_EASY_LOGGER
#include <usagi/log/easy_logger.hxx>

#include <thread>
#include <random>

auto main() -> int
{
  // simplest usage
  LOGI << "test1";
  // 
  LOGD << "test2 multiple output operators in the one line " << 12345 << ' ' << 1.2345f;
  // multi threaded
  std::random_device r;
  auto a = std::thread( [&]{ for ( auto n = 0; n < 16; ++n ) LOGW << "／(^o^)＼ " << r() << ' ' << r()<< ' ' << r() << ' ' << ' ' << r(); } );
  std::thread( [&]{ for ( auto n = 0; n < 16; ++n ) LOGE << "＼(^o^)／ " << r() << ' ' << r()<< ' ' << r() << ' ' << ' ' << r(); } ).join();
  a.join();
}