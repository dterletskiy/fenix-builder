#pragma once

// Framework
#include "carpc/runtime/application/Component.hpp"



namespace application::components::driver {

   class Component
      : public carpc::application::Component
   {
      public:
         static carpc::application::IComponent::tSptr creator( );

      private:
         Component( const std::string& );
      public:
         ~Component( ) override;
   };

} // namespace application::components::driver
