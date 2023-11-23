#pragma once

// Framework
#include "carpc/runtime/application/RootComponent.hpp"
#include "carpc/runtime/comm/timer/Timer.hpp"
// Application
#include "imp/hmi/components/onoff/client/Client.hpp"



namespace hmi::event {

   enum class eID : size_t { boot, shutdown, ping, undefined };
   const char* c_str( const eID );

   struct AppData
   {
      bool to_stream( carpc::ipc::tStream& _stream ) const { return true; }
      bool from_stream( carpc::ipc::tStream& _stream ) { return true; }
   };

   DEFINE_IPC_EVENT( App, AppData, carpc::async::id::TSignature< eID > );

}



namespace hmi::components::onoff {

   class Component
      : public carpc::application::RootComponent
      , public carpc::timer::ITimerConsumer
      , public hmi::event::App::Consumer
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
         void process_timer( const carpc::comm::timer::ID ) override;
         carpc::timer::Timer m_timer;

      public:
         void on_timer( const carpc::comm::timer::ID );

      private:
         void process_event( const hmi::event::App::Event& ) override;

      private:
         Client m_client_onoff;
   };

} // namespace hmi::components::onoff
