#pragma once

// Framework
#include "carpc/runtime/application/RootComponent.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"




namespace application::components::onoff {

   class Component
      : public carpc::application::RootComponent
      , public events::SimpleNoSigNoData::Consumer
      , public events::SimpleIdSigNoData::Consumer
      , public events::SimpleIdSig::Consumer
      , public events::Simple::Consumer
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;

      private:
         void process_event( const events::SimpleNoSigNoData::Event& ) override;
         void process_event( const events::SimpleIdSigNoData::Event& ) override;
         void process_event( const events::SimpleIdSig::Event& ) override;
         void process_event( const events::Simple::Event& ) override;

      private:
         void process_boot( const std::string& ) override;
         void process_shutdown( carpc::callback::tBlocker ) override;
   };

} // namespace application::components::onoff
