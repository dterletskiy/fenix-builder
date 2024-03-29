#include "carpc/runtime/application/Component.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Component"


using namespace carpc::application;



Component::Component( const std::string& name )
   : IComponent( name )
{
   SYS_DBG( "Created: %s", m_name.c_str( ) );
}

Component::~Component( )
{
   SYS_DBG( "Destroyed: %s", m_name.c_str( ) );
}
