#include <memory>

namespace usagi
{
  namespace memory
  {
    template < typename T >
    class wake_ptr
      : public std::weak_ptr < T >
    {
    public:
      wake_ptr( const std::shared_ptr< T >& s )
        : std::weak_ptr< T >( s )
      { }
      
      auto operator->()
      {
        return std::weak_ptr<T>::lock();
      }
      
      auto operator*()
      {
        return *std::weak_ptr<T>::lock();
      }
    };
  }
}
