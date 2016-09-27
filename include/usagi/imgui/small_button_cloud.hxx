#pragma once

#include <string>
#include <map>
#include <functional>

namespace usagi
{
  namespace imgui
  {
    using small_button_cloud_functor_type
      = std::function< auto () -> void >;
    
    using small_button_cloud_base_type
      = std::multimap
        < std::string
        , std::tuple
          < small_button_cloud_functor_type
          , std::string
          >
        >
      ;
    
    class small_button_cloud
      : public small_button_cloud_base_type
    {
      float _width = 0.0f;
    public:
      using functor_type = std::function< auto () -> void >;
      
      auto emplace( std::string&& key, small_button_cloud_functor_type&& f )
      { return small_button_cloud_base_type::emplace( key, std::make_tuple( f, std::string() ) ); }
      
      auto emplace( std::string&& key, std::string&& description )
      { return small_button_cloud_base_type::emplace( key, std::make_tuple( []{}, description ) ); }
      
      auto emplace( std::string&& key, small_button_cloud_functor_type&& f, std::string&& description )
      { return small_button_cloud_base_type::emplace( key, std::make_tuple( f, description ) ); }
      
      auto insert( const std::string& key, const small_button_cloud_functor_type& f )
      { return small_button_cloud_base_type::emplace( key, std::make_tuple( f, std::string() ) ); }
      
      auto insert( const std::string& key, const std::string& description )
      { return small_button_cloud_base_type::emplace( key, std::make_tuple( []{}, description ) ); }
      
      auto insert( const std::string& key, const small_button_cloud_functor_type& f, const std::string& description )
      { return small_button_cloud_base_type::emplace( key, std::make_tuple( f, description ) ); }
      
      auto width() { return _width; }
      auto width( const decltype( _width ) in ) { return _width = in; }
      auto get_width_pointer() { return &_width; }
      auto operator()()
      {
        ImGui::BeginGroup();
        
        const auto s = ImGui::GetStyle();
        const auto iis2 = s.ItemInnerSpacing.x * 2;
        const auto is   = s.ItemSpacing.x;
        
        auto current_line_width = 0.0f;
        
        for ( const auto& p : *this )
        {
          const auto entity_width = ImGui::CalcTextSize( p.first.c_str() ).x;
          const auto tmp_line_width = current_line_width + is + iis2 + entity_width;
          
          if ( current_line_width > 0.0f and tmp_line_width < _width )
          {
            ImGui::SameLine();
            current_line_width = tmp_line_width;
          }
          else
            current_line_width = iis2 + entity_width;
          
          if ( ImGui::SmallButton( p.first.c_str() ) )
            std::get< small_button_cloud_functor_type >( p.second )();
          
          const auto& description = std::get< std::string >( p.second );
          
          if ( not description.empty() and ImGui::IsItemHovered() )
            ImGui::SetTooltip( description.c_str() );
        }
        
        ImGui::EndGroup();
      }
    };
  }
}