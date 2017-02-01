#include <usagi/json/picojson/rpc/jsonrpc20.hxx>

#include <memory>

auto main() -> int
{
  using namespace usagi::json::picojson;
  using namespace usagi::json::picojson::rpc::jsonrpc20;
  
  server_type s;
  
  // 野生のλ
  auto fa = []( const value_type& request ){ return value_type( request.get< double >() * 2.0 ); };
  
  // s に存在しないメソッド aaa を uuidv4 の id 付きで呼ぶ
  std::cout << "trial-1: " << s( make_request_with_uuidv4( "aaa", value_type( 1.23 ) ) ) << '\n';
  
  // s に fa をメソッド名 aaa として接続しコネクター ca を取得
  auto ca = s.connect( "aaa", fa );
  
  // s に存在するメソッド aaa を呼ぶ（期待される型の引数を伴う）
  std::cout << "trial-2: " << s( make_request_with_uuidv4( "aaa", value_type( 1.23 ) ) ) << '\n';
  // s に存在するメソッド aaa を呼ぶ（期待されない型の引数を伴う）
  std::cout << "trial-3: " << s( make_request_with_uuidv4( "aaa", value_type( "1.23" ) ) ) << '\n';
  
  // s に接続した fa をコネクター ca から切断
  ca.disconnect();
  
  // 有効なスロットが存在しないメソッド aaa を呼ぶ
  std::cout << "trial-4: " << s( make_request_with_uuidv4( "aaa", value_type( 1.23 ) ) ) << '\n';
  
  // shared_ptr の functor
  auto fb = std::make_shared< server_type::method_functor_type >( []( const auto& ) { return value_type(); } );
  
  // s に存在しないメソッド bbb を id なし（ = null ）で呼ぶ
  std::cout << "trial-5: " << s( make_request( "bbb", value_type( 1.23 ) ) ) << '\n';
  
  // s に fb の中身の functor を fb をトラッキングさせつつ接続しコネクター cb を取得
  auto cb = s.connect( "bbb", *fb, fb );
  
  // s に存在するメソッド bbb を呼ぶ
  std::cout << "trial-6: " << s( make_request( "bbb" ) ) << '\n';
  
  // s のメソッド bbb に接続した fb の中身の functor との接続をトラッキングする fb を無効化する（同時に接続が自動的に切断される）
  fb.reset();
  
  // （自動的に切断され）有効なスロットが存在しないメソッド bbb を呼ぶ
  std::cout << "trial-7: " << s( make_request( "bbb" ) ) << '\n';
}