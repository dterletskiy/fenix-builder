#pragma once

#include <string>
#include <memory>
#include <optional>
#include "carpc/runtime/fsm/Types.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "TState"



namespace carpc::fsm {

   template< typename TYPES >
   class TStateManager;



   template< typename TYPES >
   class TState
   {
      private:
         friend class TStateManager< TYPES >;

      public:
         using tState = TState< TYPES >;
         using tSptr = std::shared_ptr< tState >;
         using tManager = TStateManager< TYPES >;
         static constexpr std::optional< typename TYPES::tID > NoTransition = std::nullopt;

      public:
         TState( const typename TYPES::tID&, const std::string& );
         TState( const tState& ) = delete;
         virtual ~TState( ) = default;

      private:
         const std::optional< typename TYPES::tID > in( );
         void out( );
         virtual const std::optional< typename TYPES::tID > enter( ) = 0;
         virtual void leave( ) = 0;
         virtual const std::optional< typename TYPES::tID > process( const typename TYPES::tData& ) = 0;

      public:
         const typename state::ID& id( ) const;
      private:
         typename state::ID m_id = state::ID::generate( );

      public:
         const std::string& name( ) const;
      private:
         std::string m_name;

      public:
         const typename TYPES::tID& uid( ) const;
      private:
         typename TYPES::tID m_uid;

      public:
         const tManager& manager( ) const;
      private:
         void manager( const tManager& );
         const tManager* mp_manager = nullptr;
   };



   template< typename TYPES >
   TState< TYPES >::TState( const typename TYPES::tID& uid, const std::string& name )
      : m_name( name )
      , m_uid( uid )
   {
   }

   template< typename TYPES >
   const typename state::ID& TState< TYPES >::id( ) const
   {
      return m_id;
   }

   template< typename TYPES >
   const std::string& TState< TYPES >::name( ) const
   {
      return m_name;
   }

   template< typename TYPES >
   const typename TYPES::tID& TState< TYPES >::uid( ) const
   {
      return m_uid;
   }

   template< typename TYPES >
   void TState< TYPES >::manager( const tManager& manager )
   {
      mp_manager = &manager;
   }

   template< typename TYPES >
   const typename TState< TYPES >::tManager& TState< TYPES >::manager( ) const
   {
      if( nullptr == mp_manager )
      {
         SYS_ERR( "%s: manager is not registered", m_name.c_str( ) );
      }

      return *mp_manager;
   }

   template< typename TYPES >
   const std::optional< typename TYPES::tID > TState< TYPES >::in( )
   {
      SYS_VRB( "%s: enter", m_name.c_str( ) );
      auto next_state_uid = enter( );
      return next_state_uid;
   }

   template< typename TYPES >
   void TState< TYPES >::out( )
   {
      SYS_VRB( "%s: leave", m_name.c_str( ) );
      leave( );
   }

} // namespace carpc::events



#undef CLASS_ABBR
