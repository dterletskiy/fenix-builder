// Framework
#include "carpc/runtime/comm/async/runnable/Runnable.hpp"
// Application
#include "imp/hmi/components/onoff/Component.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



namespace hmi::event {

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::boot:         return "hmi::event::eID::boot";
         case eID::shutdown:     return "hmi::event::eID::shutdown";
         case eID::ping:         return "hmi::event::eID::ping";
         case eID::undefined:
         default:                return "hmi::event::eID::undefined";
      }
   }

}



using namespace hmi::components::onoff;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "OnOff" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::RootComponent( _name )
   , m_timer( this )
   , m_client_onoff( )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
   hmi::event::App::Event::set_notification( this, { hmi::event::eID::boot } );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::process_boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );

   REGISTER_EVENT( hmi::event::App );
   // hmi::event::App::Event::create_send( { hmi::event::eID::boot },
   //    carpc::application::Context( carpc::application::process::ID::generate( ), carpc::application::thread::ID::generate( ) )
   // );
   // hmi::event::App::Event::create_send( { hmi::event::eID::boot } );
}

void Component::process_timer( const carpc::comm::timer::ID id )
{
   MSG_DBG( "Timer '%s' expired", id.dbg_name( ).c_str( ) );
   if( id == m_timer.id( ) )
   {
      MSG_WRN( "Shutting down system" );
      shutdown( );
   }
}

void Component::on_timer( const carpc::comm::timer::ID id )
{
   MSG_DBG( "Timer expired: %s", id.dbg_name( ).c_str( ) );

   MSG_WRN( "Preparing shutdown system" );
   m_timer.start( 5000000000, 1 );
}

void Component::process_event( const hmi::event::App::Event& event )
{
   MSG_DBG( "%s", event.signature( )->dbg_name( ).c_str( ) );

   const carpc::comm::timer::ID id = carpc::timer::start( 100000, 1, [ this ]( const carpc::comm::timer::ID id ){ on_timer( id ); } );
   MSG_DBG( "started timer: %s", id.dbg_name( ).c_str( ) );
}
