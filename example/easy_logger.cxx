#include <usagi/log/easy_logger.hxx>
auto main() -> int
{
  LOGI << "info  log: " << 12345;
  LOGD << "debug log: " << 1.234f;
  LOGW << "warn  log: ／(^o^)＼";
  LOGE << "error log: ＼(^o^)／";
}