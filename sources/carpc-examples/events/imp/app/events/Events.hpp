#pragma once

// Framework
#include "carpc/runtime/comm/async/event/Event.hpp"



namespace application::events {

   enum class eID { BOOT, SHUTDOWN, PING, UNDEFINED };
   const char* c_str( const eID );

   template< std::size_t tID >
   struct TData_
   {
      std::string message = "";
   };

   template< std::size_t tID >
   struct TSignature_
   {
      bool operator<( const TSignature_& other ) const
      {
         if( other.id != id )
            return id < other.id;

         return level < other.level;
      }

      std::string dbg_name( ) const
      {
         return carpc::format_string( id, ".", level );
      }

      std::size_t id = 0;
      std::size_t level = 0;
   };

   template< std::size_t D, std::size_t S >
   struct TWrapper_;

   #define DEFINE_EVENT_SIMPLE( tD, tS ) \
      DEFINE_EVENT( SimpleD##tD##S##tS, TData_< tD >, TSignature_< tS > ); \
      template< > \
      struct TWrapper_< tD, tS > \
      { \
         using tSimple = SimpleD##tD##S##tS; \
      };



   DEFINE_EVENT_SIMPLE( 0, 0 );
   DEFINE_EVENT_SIMPLE( 1, 1 );
   DEFINE_EVENT_SIMPLE( 2, 2 );
   DEFINE_EVENT_SIMPLE( 3, 3 );
   DEFINE_EVENT_SIMPLE( 4, 4 );
   DEFINE_EVENT_SIMPLE( 5, 5 );
   DEFINE_EVENT_SIMPLE( 6, 6 );
   DEFINE_EVENT_SIMPLE( 7, 7 );
   DEFINE_EVENT_SIMPLE( 8, 8 );
   DEFINE_EVENT_SIMPLE( 9, 9 );

   template< typename ... TYPES >
   class TConsumer_;

   template< typename T, typename ... TYPES >
   class TConsumer_ < T, TYPES... > : public T::Consumer, public TConsumer_< TYPES... >
   {
      void process_event( const typename T::Event& event ) override = 0;
   };

   template< typename T >
   class TConsumer_< T > : public T::Consumer
   {
      void process_event( const typename T::Event& event ) override = 0;
   };

   using X = TConsumer_< SimpleD0S0 >;
   // using X = TConsumer_< SimpleD0S0, SimpleD1S1 >;



   struct DS
   {
      std::size_t D;
      std::size_t S;
   };

   template< DS ... VALUES >
   class TConsumerDS_;

   template< DS V, DS ... VALUES >
   class TConsumerDS_< V, VALUES... > : public TWrapper_< V.D, V.S >::tSimple::Consumer, public TConsumerDS_< VALUES... >
   {
      void process_event( const typename TWrapper_< V.D, V.S >::tSimple::Event& event ) override = 0;
   };

   template< DS V >
   class TConsumerDS_< V > : public TWrapper_< V.D, V.S >::tSimple::Consumer
   {
      void process_event( const typename TWrapper_< V.D, V.S >::tSimple::Event& event ) override = 0;
   };

   using Y = TConsumerDS_< DS{ 0, 0 } >;
   // using Y = TConsumerDS_< DS{ 0, 0 }, DS{ 1, 1 } >;



   template< std::size_t ... VALUES >
   class TConsumerN_;

   template< std::size_t V, std::size_t ... VALUES >
   class TConsumerN_< V, VALUES... > : public TWrapper_< V, V >::tSimple::Consumer, public TConsumerN_< VALUES... >
   {
      void process_event( const typename TWrapper_< V, V >::tSimple::Event& event ) override = 0;
   };

   template< std::size_t V >
   class TConsumerN_< V > : public TWrapper_< V, V >::tSimple::Consumer
   {
      void process_event( const typename TWrapper_< V, V >::tSimple::Event& event ) override = 0;
   };

   using Z = TConsumerN_< 0 >;
   // using Z = TConsumerN_< 0, 1 >;



} // namespace application::events
