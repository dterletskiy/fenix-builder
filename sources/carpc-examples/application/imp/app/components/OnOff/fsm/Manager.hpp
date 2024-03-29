#pragma once

#include "carpc/runtime/fsm/TStateManager.hpp"
#include "imp/app/components/OnOff/fsm/Types.hpp"



namespace application::components::onoff {

   class Manager : public carpc::fsm::TStateManager< Types >
   {
      public:
         using tBase = carpc::fsm::TStateManager< Types >;

      public:
         Manager( );
         ~Manager( ) override = default;

      private:
         Subscriber::tSptr mp_subscriber = nullptr;
   };

} // namespace carpc::events
