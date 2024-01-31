#pragma once

// Framework
#include "carpc/runtime/comm/async/event/Event.hpp"



namespace application::events {

   enum class eAppEventID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eAppEventID );

   struct AppEventData
   {
      std::string message = "";
   };

   struct AppEventSignature
   {
      bool operator<( const AppEventSignature& other ) const;

      std::string dbg_name( ) const;

      std::size_t id = 0;
      std::size_t level = 0;
   };



   DEFINE_EVENT_NOSIG_NODATA( SimpleNoSigNoData );
   DEFINE_EVENT_IDSIG_NODATA( SimpleIdSigNoData, eAppEventID );
   DEFINE_EVENT_IDSIG( SimpleIdSig, AppEventData, eAppEventID );
   DEFINE_EVENT( Simple, AppEventData, AppEventSignature );

} // namespace application::events
