// Application
#include "imp/app/components/OnOff/Component.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "OnOff"



using namespace application::components::onoff;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "OnOff" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::RootComponent( _name )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::process_boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );
   shutdown( );
}

void Component::process_shutdown( carpc::callback::tBlocker blocker )
{
   MSG_DBG( "shutting down application" );
}
