#pragma once

#include "carpc/runtime/fsm/TState.hpp"
#include "imp/app/components/OnOff/fsm/Types.hpp"



namespace application::components::onoff {

   class Manager;



   class Unloaded : public carpc::fsm::TState< Types >
   {
      public:
         Unloaded( Manager& );
         ~Unloaded( ) override = default;

      private:
         const std::optional< Types::tID > enter( ) override;
         void leave( ) override;

      private:
         const std::optional< Types::tID > process( const Types::tData& ) override;

      private:
         Manager& m_manager;
   };

} // namespace application::components::onoff
