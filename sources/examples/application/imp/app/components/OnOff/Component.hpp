#pragma once

// Framework
#include "carpc/runtime/application/RootComponent.hpp"
#include "carpc/runtime/comm/timer/Timer.hpp"
#include "carpc/runtime/fsm/TStateManager.hpp"
// Application
#include "imp/app/components/OnOff/fsm/Manager.hpp"
#include "imp/app/components/OnOff/server/Server.hpp"



namespace application::components::onoff {

   class Component
      : public carpc::application::RootComponent
      , public carpc::timer::ITimerConsumer
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

      private:
         Server m_server_onoff;

      // Timer
      private:
         void process_timer( const carpc::comm::timer::ID ) override;
         carpc::timer::Timer m_timer;

      public:
         void on_timer( const carpc::comm::timer::ID );

      private:
         Manager m_fsm;
   };

} // namespace application::components::onoff
