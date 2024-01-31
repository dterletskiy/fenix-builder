// Application
#include "imp/app/components/Driver/Component.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Driver"



using namespace application::components::driver;



carpc::application::IComponent::tSptr Component::creator( )
{
   return std::shared_ptr< Component >( new Component( "Driver" ) );
}

Component::Component( const std::string& _name )
   : carpc::application::Component( _name )
{
   MSG_DBG( "Created: %s", name( ).c_str( ) );
}

Component::~Component( )
{
   MSG_DBG( "Destroyed: %s", name( ).c_str( ) );
}
