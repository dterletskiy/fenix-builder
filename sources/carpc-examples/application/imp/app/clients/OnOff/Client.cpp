#include "carpc/base/helpers/macros/strings.hpp"
// Application
#include "Client.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "OnOffClient"



using namespace application::clients::onoff;



Client::Client( const std::string& role_name, const std::string& name, tConnectedCallback connected_callback )
   : service::onoff::V2_0_0::Client( role_name )
   , m_name( name )
   , m_connected_callback( connected_callback )
{
   MSG_VRB( "%s: created", m_name.c_str( ) );
}

Client::~Client( )
{
   MSG_VRB( "%s: destroyed", m_name.c_str( ) );
   unsubscribe_current_state( );
}

void Client::connected( )
{
   MSG_DBG( );
   m_connected_callback( );
   subscribe_current_state( );
}

void Client::disconnected( )
{
   MSG_DBG( );
   unsubscribe_current_state( );
}

void Client::request_start( )
{
   MSG_DBG( "%s", m_name.c_str( ) );
   service::onoff::V2_0_0::Client::request_start( );
}

void Client::response_trigger_state( const bool& result )
{
   MSG_DBG( "%s: result: %s", m_name.c_str( ), BOOL_TO_STRING( result ) );
}

carpc::comm::sequence::ID Client::request_trigger_state( const std::string& state, const std::size_t& delay )
{
   MSG_DBG( "%s: state: %s / delay: %zu", m_name.c_str( ), state.c_str( ), delay );
   return service::onoff::V2_0_0::Client::request_trigger_state( state, delay );
}

void Client::request_trigger_state_failed( const carpc::service::eError& error )
{
   MSG_ERR( "%s: request error: %s", m_name.c_str( ), error.c_str( ) );
}

void Client::on_current_state( const std::string& state )
{
   MSG_DBG( "%s: current state changed to '%s'", m_name.c_str( ), state.c_str( ) );
}
