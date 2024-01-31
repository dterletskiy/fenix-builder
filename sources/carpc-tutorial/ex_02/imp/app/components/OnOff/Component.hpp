#pragma once

// Framework
#include "carpc/runtime/application/RootComponent.hpp"



namespace application::components::onoff {

   class Component
      : public carpc::application::RootComponent
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_boot( const std::string& ) override;
         void process_shutdown( carpc::callback::tBlocker ) override;
   };

} // namespace application::components::onoff
