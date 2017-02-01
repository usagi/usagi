#pragma once

#include "request.hxx"
#include "response.hxx"

#include <boost/signals2/signal.hpp>

#include <string>
#include <functional>
#include <unordered_map>

namespace usagi::json::picojson::rpc::jsonrpc20
{
  class server_type
  {
    public:
      using method_signature_type = auto ( const value_type& params ) -> value_type;
      using method_functor_type = std::function< method_signature_type >;
      using method_signal_type = boost::signals2::signal< method_signature_type >;
      using method_slot_type = method_signal_type::slot_type;
    private:
      std::unordered_map< std::string, method_signal_type > mapper;
    public:
      auto connect( const std::string& method, const method_functor_type& functor )
      { return mapper[ method ].connect( functor ); }
      
      template < typename T >
      auto connect( const std::string& method, const method_functor_type& functor, const T& track_target )
      { return mapper[ method ].connect( method_slot_type( functor ).track_foreign( track_target ) ); }
      
      /// @brief request を受けて処理し result または error の value_type を返す
      auto operator()( const value_type& request )
      {
        try
        {
          //const auto& [ method, params ] = extract_request( request );
          auto x = get_method( request );
          const auto& method_params = extract_request( request );
          const auto& method = std::get< 0 >( method_params );
          const auto& params = std::get< 1 >( method_params );
          auto optional_result = mapper.at( method )( params );
          if ( not optional_result )
            throw exception_type( error_code_type::method_not_found, "sigal is empty." );
          return make_result_from_request( request, *optional_result );
        }
        catch ( const exception_type& e )
        { return make_error_from_request( request, e ); }
        catch ( const std::out_of_range& e )
        { return make_error_from_request( request, exception_type( error_code_type::method_not_found ) ); }
        catch ( const std::exception& e )
        { return make_error_from_request( request, exception_type( error_code_type::internal_error, e.what() ) ); }
        catch ( ... )
        { return make_error_from_request( request, exception_type( error_code_type::internal_error ) ); }
      }
      
      auto operator()( const std::string& request_string )
      {
        try
        {
          value_type request;
          const auto& e = ::picojson::parse( request, request_string );
          if ( not e.empty() )
            return make_error_from_request( request, exception_type( error_code_type::parse_error ) );
          return operator()( request );
        }
        catch ( const exception_type& e )
        { return make_error_from_request( request, e ); }
        catch ( const std::out_of_range& e )
        { return make_error_from_request( request, exception_type( error_code_type::method_not_found ) ); }
        catch ( const std::exception& e )
        { return make_error_from_request( request, exception_type( error_code_type::internal_error, e.what() ) ); }
        catch ( ... )
        { return make_error_from_request( request, exception_type( error_code_type::internal_error ) ); }
      }
  };
}