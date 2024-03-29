#pragma once

// Framework
#include "carpc/runtime/application/RootComponent.hpp"
// Application
#include "imp/symbiont/events/AppEvent.hpp"
#include "imp/symbiont/components/onoff/server/OnOff.hpp"
#include "imp/symbiont/components/onoff/client/OnOff.hpp"



namespace symbiont::components::onoff {

   class Component
      : public carpc::application::RootComponent
      , public events::AppEvent::Consumer
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_boot( const std::string& ) override;

      private:
         void process_event( const events::AppEvent::Event& ) override;

      private:
         interface::onoff::Server m_server_onoff;
         interface::onoff::Client m_client_onoff;
   };

} // namespace symbiont::components::onoff
