// Framework
#include "carpc/base/helpers/functions/format.hpp"
// Application
#include "imp/app/events/AppEvent.hpp"




namespace application::events {

   const char* c_str( const eAppEventID id )
   {
      switch( id )
      {
         case eAppEventID::BOOT:       return "BOOT";
         case eAppEventID::SHUTDOWN:   return "SHUTDOWN";
         case eAppEventID::PING:       return "PING";
         case eAppEventID::UNDEFINED:  return "UNDEFINED";
         default:                      return "UNDEFINED";
      }
   }

} // namespace application::events



using namespace application::events;




bool AppEventSignature::operator<( const AppEventSignature& other ) const
{
   if( other.id != id )
      return id < other.id;

   return level < other.level;
}

std::string AppEventSignature::dbg_name( ) const
{
   return carpc::format_string( id, ".", level );
}