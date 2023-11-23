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

   events::SimpleD0S0::Event::set_notification( this, events::SimpleD0S0::UserSignature{ 45, 23 } );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::process_boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );
}

void Component::process_shutdown( carpc::callback::tBlocker blocker )
{
   MSG_DBG( "shutting down application" );
}

// void Component::process_event( const events::SimpleD0S0::Event& event )
// {
//    const auto& event_info = event.info( );
//    const auto& event_data = event.data( ) ? *( event.data( ) ) : events::SimpleD0S0::Data{ };

//    MSG_DBG( "signature: id = '%zu' / level = '%zu'", event_info.id, event_info.level );
//    MSG_DBG( "data: message = '%s'", event_data.message.c_str( ) );
// }
