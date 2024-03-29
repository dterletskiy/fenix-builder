#pragma once

#include "carpc/runtime/application/IComponent.hpp"



namespace carpc::application {

   class Component : public IComponent
   {
      public:
         Component( const std::string& );
         ~Component( ) override;

      private:
         const bool is_root( ) const override final { return false; }
         void process_boot( const std::string& info = "process_boot" ) override final { }
         void process_shutdown( carpc::callback::tBlocker ) override final { }
   };

} // namespace carpc::application
