#include "carpc/base/helpers/macros/strings.hpp"
// Application
#include "imp/symbiont/events/AppEvent.hpp"
#include "OnOff.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "OnOff_Client"



using namespace symbiont::interface::onoff;



Client::Client( )
   : service::onoff::V2_0_0::Client( "hybrid" )
{
   MSG_VRB( "created" );
}

Client::~Client( )
{
   MSG_VRB( "destroyed" );
   unsubscribe_current_state( );
}

void Client::connected( )
{
   MSG_DBG( "connected" );
   // subscribe_current_state( );
   test( );
}

void Client::disconnected( )
{
   MSG_DBG( "disconnected" );
   unsubscribe_current_state( );
}

void Client::request_start( )
{
   MSG_DBG( );
   service::onoff::V2_0_0::Client::request_start( );
}


void Client::response_trigger_state( const bool& result )
{
   // MSG_DBG( "result: %s", BOOL_TO_STRING( result ) );
   test( );
}

carpc::comm::sequence::ID Client::request_trigger_state( const std::string& state, const std::size_t& delay )
{
   // MSG_DBG( "state: %s / delay: %zu", state.c_str( ), delay );
   return service::onoff::V2_0_0::Client::request_trigger_state( state, delay );
}

void Client::request_trigger_state_failed( const carpc::service::eError& error )
{
   MSG_ERR( "request error: %s", error.c_str( ) );
}

void Client::on_current_state( const std::string& state )
{
   MSG_DBG( "current state changed to '%s'", state.c_str( ) );
}

void Client::test( )
{
   static std::size_t count = 0;

   if( 0 == count )
   {
      MSG_DBG( "TEST STARTED CARPC" );
      m_performance.start( );
   }

   if( 1000000 > ++count )
   {
      request_trigger_state( "REQUEST_TEST", 0 );
   }
   else
   {
      m_performance.stop( );
      MSG_DBG( "TEST FINISHED CARPC: count = %zu", count );
      events::AppEvent::Event::create_send( { events::eAppEventID::BOOT }, { "booting" } );
   }
}
