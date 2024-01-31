#pragma once

// Framework
#include "carpc/runtime/application/RootComponent.hpp"
// Application
#include "imp/app/events/Events.hpp"




namespace application::components::onoff {

   class Component
      : public carpc::application::RootComponent
      // , public events::SimpleD0S0::Consumer
      , public events::Z
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_event( const events::SimpleD0S0::Event& ) override {};

      private:
         void process_boot( const std::string& ) override;
         void process_shutdown( carpc::callback::tBlocker ) override;
   };

} // namespace application::components::onoff
