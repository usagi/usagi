#pragma once

#include <string>
#include <map>
#include <functional>

namespace usagi
{
  namespace imgui
  {
    class small_button_cloud
      : public std::multimap
        < std::string
        , std::function< auto () -> void >
        >
    {
      float _width = 0.0f;
    public:
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
            p.second();
        }
        
        ImGui::EndGroup();
      }
    };
  }
}