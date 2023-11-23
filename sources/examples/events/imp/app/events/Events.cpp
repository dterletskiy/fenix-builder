// Framework
#include "carpc/base/helpers/functions/format.hpp"
// Application
#include "imp/app/events/Events.hpp"




namespace application::events {

   const char* c_str( const eID id )
   {
      switch( id )
      {
         case eID::BOOT:         return "BOOT";
         case eID::SHUTDOWN:     return "SHUTDOWN";
         case eID::PING:         return "PING";
         case eID::UNDEFINED:    return "UNDEFINED";
         default:                return "UNDEFINED";
      }
   }

} // namespace application::events



using namespace application::events;
