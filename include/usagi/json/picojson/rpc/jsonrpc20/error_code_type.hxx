#pragma once

#include <stdint>
#include <string>
#include <type_traits>

namespace usagi::json::picojson::rpc::jsonrpc20
{
  
  enum class error_code_type
    : std::int16_t
  { parse_error       = -32700
  , invalid_request   = -32600
  , method_not_found  = -32601
  , invalid_params    = -32602
  , internal_error    = -32603
  , server_error_00   = -32000
  , server_error_01   = -32001
  , server_error_02   = -32002
  , server_error_03   = -32003
  , server_error_04   = -32004
  , server_error_05   = -32005
  , server_error_06   = -32006
  , server_error_07   = -32007
  , server_error_08   = -32008
  , server_error_09   = -32009
  , server_error_10   = -32010
  , server_error_11   = -32011
  , server_error_12   = -32012
  , server_error_13   = -32013
  , server_error_14   = -32014
  , server_error_15   = -32015
  , server_error_16   = -32016
  , server_error_17   = -32017
  , server_error_18   = -32018
  , server_error_19   = -32019
  , server_error_20   = -32020
  , server_error_21   = -32021
  , server_error_22   = -32022
  , server_error_23   = -32023
  , server_error_24   = -32024
  , server_error_25   = -32025
  , server_error_26   = -32026
  , server_error_27   = -32027
  , server_error_28   = -32028
  , server_error_29   = -32029
  , server_error_30   = -32030
  , server_error_31   = -32031
  , server_error_32   = -32032
  , server_error_33   = -32033
  , server_error_34   = -32034
  , server_error_35   = -32035
  , server_error_36   = -32036
  , server_error_37   = -32037
  , server_error_38   = -32038
  , server_error_39   = -32039
  , server_error_40   = -32040
  , server_error_41   = -32041
  , server_error_42   = -32042
  , server_error_43   = -32043
  , server_error_44   = -32044
  , server_error_45   = -32045
  , server_error_46   = -32046
  , server_error_47   = -32047
  , server_error_48   = -32048
  , server_error_49   = -32049
  , server_error_50   = -32050
  , server_error_51   = -32051
  , server_error_52   = -32052
  , server_error_53   = -32053
  , server_error_54   = -32054
  , server_error_55   = -32055
  , server_error_56   = -32056
  , server_error_57   = -32057
  , server_error_58   = -32058
  , server_error_59   = -32059
  , server_error_60   = -32060
  , server_error_61   = -32061
  , server_error_62   = -32062
  , server_error_63   = -32063
  , server_error_64   = -32064
  , server_error_65   = -32065
  , server_error_66   = -32066
  , server_error_67   = -32067
  , server_error_68   = -32068
  , server_error_69   = -32069
  , server_error_70   = -32070
  , server_error_71   = -32071
  , server_error_72   = -32072
  , server_error_73   = -32073
  , server_error_74   = -32074
  , server_error_75   = -32075
  , server_error_76   = -32076
  , server_error_77   = -32077
  , server_error_78   = -32078
  , server_error_79   = -32079
  , server_error_80   = -32080
  , server_error_81   = -32081
  , server_error_82   = -32082
  , server_error_83   = -32083
  , server_error_84   = -32084
  , server_error_85   = -32085
  , server_error_86   = -32086
  , server_error_87   = -32087
  , server_error_88   = -32088
  , server_error_89   = -32089
  , server_error_90   = -32090
  , server_error_91   = -32091
  , server_error_92   = -32092
  , server_error_93   = -32093
  , server_error_94   = -32094
  , server_error_95   = -32095
  , server_error_96   = -32096
  , server_error_97   = -32097
  , server_error_98   = -32098
  , server_error_99   = -32099
  };
  
  static inline auto to_number( const error_code_type e )
    -> number_type
  {
    return
      static_cast
      < double
      >
      ( static_cast
        < std::underlying_type_t
          < error_code_type
          >
        >
        ( e )
      );
  }
  
  static inline auto to_string( const error_code_type e )
    -> std::string
  {
    switch ( e )
    {
      case error_code_type::parse_error:
        return "Parse error";
      case error_code_type::invalid_request:
        return "Invalid request";
      case error_code_type::method_not_found:
        return "Method not found";
      case error_code_type::invalid_params:
        return "Invalid params";
      case error_code_type::internal_error:
        return "Internal error";
      default:
        if ( n >= error_code_type::server_error_00 and n <= error_code_type::server_error_99 )
          return "Server error ( " + std::to_string( static_cast< std::underlying_type_t< error_code_type > >( e )  ) + " )";
     }
    
    return "Unknown error code ( " + std::to_string( static_cast< std::underlying_type_t< error_code_type > >( e )  ) + " )";
  }
  
}