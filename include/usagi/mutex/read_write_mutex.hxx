#pragma once

#if __cplusplus >= 201402
  #include <shared_mutex>
#else
  #include <mutex>
#endif

namespace usagi
{
  namespace mutex
  {
#if __cplusplus > 201402
    // C++17
    using read_write_mutex_type = std::shared_mutex;
    using read_lock_type = std::shared_lock< read_write_mutex_type >;
    using write_lock_type = std::lock_guard< read_write_mutex_type >;
    using upgradable_read_lock_type = std::upgrade_lock< read_write_mutex_type >;
    using upgraded_write_lock_type = std::upgrade_to_unique_lock_type< upgrade_lock_type >;
#elif __cplusplus == 201402
    // C++14
    using read_write_mutex_type = std::shared_timed_mutex;
    using read_lock_type = std::shared_lock< read_write_mutex_type >;
    using write_lock_type = std::lock_guard< read_write_mutex_type >;
    using upgradable_read_lock_type = std::upgrade_lock< read_write_mutex_type >;
    using upgraded_write_lock_type = std::upgrade_to_unique_lock_type< upgrade_lock_type >;
#elif __cplusplus >= 201103
    // C++11
    using read_write_mutex_type = std::mutex;
    using read_lock_type = std::lock_guard< read_write_mutex_type >;
    using write_lock_type = std::lock_guard< read_write_mutex_type >;
    using upgradable_read_lock_type = std::lock_guard< read_write_mutex_type >;
    struct upgraded_write_lock_type { explicit upgraded_lock_type( const upgradable_lock_type& ){ } };
#else
    #error not supported
#endif
  }
}
