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

   events::SimpleNoSigNoData::Event::set_notification( this );
   events::SimpleIdSigNoData::Event::set_notification( this, events::SimpleIdSigNoData::UserSignature{ events::eAppEventID::PING } );
   events::SimpleIdSig::Event::set_notification( this, events::SimpleIdSig::UserSignature{ events::eAppEventID::SHUTDOWN } );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}

void Component::process_boot( const std::string& command )
{
   MSG_DBG( "%s", command.c_str( ) );
   events::SimpleNoSigNoData::Event::create_send( );
}

void Component::process_shutdown( carpc::callback::tBlocker blocker )
{
   MSG_DBG( "shutting down application" );
}

void Component::process_event( const events::SimpleNoSigNoData::Event& event )
{
   MSG_DBG( "SimpleNoSigNoData" );

   events::SimpleIdSigNoData::Event::create_send( { events::eAppEventID::PING } );
}

void Component::process_event( const events::SimpleIdSigNoData::Event& event )
{
   const auto& event_id = event.info( ).id( );

   MSG_DBG( "SimpleIdSigNoData signature: id = '%s'", events::c_str( event_id ) );

   events::SimpleIdSig::Event::create_send( { events::eAppEventID::SHUTDOWN }, { "shutdown message" } );
}

void Component::process_event( const events::SimpleIdSig::Event& event )
{
   const auto& event_id = event.info( ).id( );
   const auto& event_data = event.data( ) ? *( event.data( ) ) : events::SimpleIdSig::Data{ };

   MSG_DBG( "SimpleIdSig signature: id = '%s'", events::c_str( event_id ) );
   MSG_DBG( "SimpleIdSig data: message = '%s'", event_data.message.c_str( ) );

   switch( event_id )
   {
      case events::eAppEventID::SHUTDOWN:
      {
         shutdown( );
         break;
      }
      default: break;
   }
}

void Component::process_event( const events::Simple::Event& event )
{
   const auto& event_info = event.info( );
   const auto& event_data = event.data( ) ? *( event.data( ) ) : events::Simple::Data{ };

   MSG_DBG( "Simple: id = '%zu' / level = '%zu'", event_info.id, event_info.level );
   MSG_DBG( "SimpleIdSig data: message = '%s'", event_data.message.c_str( ) );
}
