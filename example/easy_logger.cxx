//↓これを定義すると easy_logger は LOGI などで何も出力しなくなります。
//#define DISABLE_USAGI_LOG_EASY_LOGGER
#include <usagi/log/easy_logger.hxx>

#include <thread>
#include <random>

auto main() -> int
{
  // 単純な使い方
  LOGI << "test1";
  // 複数の operator<< で繋いでも綺麗に出力されます。
  LOGD << "test2 multiple output operators in the one line " << 12345 << ' ' << 1.2345f;
  // マルチスレッドで競争的にログ出力しても分断されずに綺麗に出力されます。
  std::random_device r;
  auto a = std::thread( [&]{ for ( auto n = 0; n < 16; ++n ) LOGW << "／(^o^)＼ " << r() << ' ' << r()<< ' ' << r() << ' ' << ' ' << r(); } );
  std::thread( [&]{ for ( auto n = 0; n < 16; ++n ) LOGE << "＼(^o^)／ " << r() << ' ' << r()<< ' ' << r() << ' ' << ' ' << r(); } ).join();
  a.join();
}