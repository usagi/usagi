#include <usagi/bullet.hxx>

#include <iostream>

template < typename T > auto print( const T& in ) { std::cout << in << std::endl; }

auto print( const btVector3& in ){ std::cout << in[0] << '\t' << in[1] << '\t' << in[2] << std::endl; }

auto print( const btVector3& in1, const btVector3& in2 ){ std::cout << in1[0] << '\t' << in1[1] << '\t' << in1[2] << '\t' << in2[0] << '\t' << in2[1] << '\t' << in2[2] << std::endl; }

auto main() -> int
{
  print('a');
  
  auto floor = usagi::bullet::rigid_object_generator_type::static_plane( { 0, 1, 0 }, 0 )
    ->set_restitution( 0.5 )
    ->generate()
    ;
  
  auto o1 = usagi::bullet::rigid_object_generator_type::sphere( )
    ->set_position( { 0, 10, 0 } )
    ->set_mass( 1 )
    ->set_restitution( 1 )
    ->generate()
    ;
  
  auto w = usagi::bullet::discrete_world_type::make_shared()
    ->initialize()
    ->set_gravity()
    ;
  
  print('b');
  
  // 登録テスト
  print( w->add( floor ) );
  print( w->add( o1 ) );
  
  print('c');
  // 多重登録対策のテスト
  print( w->add( o1 ) );
  print( w->add( o1 ) );
  
  print('d');
  // 除去テスト
  print( w->remove( o1 ) );
  
  print('e');
  // 多重除去対策のテスト
  print( w->remove( o1 ) );
  print( w->remove( o1 ) );
  
  print('f');
  // 再登録テスト
  print( w->add( o1 ) );
  
  // 初期位置出力
  print( o1->get_position() );
  
  print('g');
  
  // 10ステップ進行（ 100ms * 100 = 10s）
  for ( auto n = 0; n < 10; ++n )
  {
    // 100ms まで 1 << 16 サブステップ以内で 1ms/sub-step で進行する
    w->step( std::chrono::milliseconds( 100 ), 1 << 16, std::chrono::milliseconds( 1 ) );
    // ステップ後の位置出力
    print( o1->get_position() );
  }
  
  print('h');
  
  auto o2 = usagi::bullet::rigid_object_generator_type::sphere( )
    ->set_position( { 0.5, 10, 0.5 } )
    ->set_mass( 3 )
    ->set_restitution( 0.1 )
    ->generate()
    ;
  
  print('i');
  
  // 第2物体をワールドへ追加
  print( w->add( o2 ) );
  
  for ( auto n = 0; n < 1000; ++n )
  {
    w->step( std::chrono::milliseconds( 100 ), 1 << 16, std::chrono::milliseconds( 1 ) );
    print( o1->get_position(), o2->get_position() );
  }
  
  print('z');
}
