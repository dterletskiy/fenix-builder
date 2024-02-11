#pragma once

#include "carpc/runtime/comm/async/event/IEvent.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "TEvent"



namespace carpc::async {

   template< typename _Generator >
   class TEvent : public IEvent
   {
      // using and types
      public:
         using tEvent         = typename _Generator::Config::tEvent;
         using tEventPtr      = typename std::shared_ptr< tEvent >;
         using tConsumer      = typename _Generator::Config::tConsumer;
         using tService       = typename _Generator::Config::tService;
         using tData          = typename _Generator::Config::tData;
         using tDataPtr       = typename std::shared_ptr< tData >;
         using tSignature     = typename _Generator::Config::tSignature;
         using tID            = typename _Generator::Config::tID;

      // constructors
      protected:
         TEvent( const tID& id, const tPriority& priority )
            : IEvent( )
            , m_priority( priority )
         {
            mp_signature = tSignature::create( id );
         }
         TEvent( const tID& id, const tData& data, const tPriority& priority )
            : IEvent( )
            , m_priority( priority )
         {
            mp_signature = tSignature::create( id );
            mp_data = std::make_shared< tData >( data );
         }
      public:
         ~TEvent( ) override = default;

      // static functions
      public:
         static const bool set_notification( tConsumer* p_consumer, const tID& id = { } )
         {
            return IEvent::set_notification( p_consumer, tSignature::create( id ) );
         }
         static const bool clear_notification( tConsumer* p_consumer, const tID& id = { } )
         {
            return IEvent::clear_notification( p_consumer, tSignature::create( id ) );
         }
         static const bool clear_all_notifications( tConsumer* p_consumer )
         {
            return IEvent::clear_all_notifications( p_consumer, tSignature::create( ) );
         }

      public:
         // This "create" static function is used only for creating concrete event type with all empty data
         // what should be filled during serialization.
         static IEvent::tSptr create_empty( )
         {
            return std::shared_ptr< tEvent >( new tEvent( tID( ), tPriority( ) ) );
         }

         // Create empty
         static std::shared_ptr< tEvent > create( )
         {
            return std::shared_ptr< tEvent >( new tEvent( tID( ), carpc::priority( ePriority::DEFAULT ) ) );
         }
         // Create with id
         static std::shared_ptr< tEvent > create( const tID& id )
         {
            return std::shared_ptr< tEvent >( new tEvent( id, carpc::priority( ePriority::DEFAULT ) ) );
         }
         // Create with data
         static std::shared_ptr< tEvent > create( const tData& data )
         {
            return std::shared_ptr< tEvent >( new tEvent( tID( ), data, carpc::priority( ePriority::DEFAULT ) ) );
         }
         // Create with priority
         static std::shared_ptr< tEvent > create( const tPriority& priority )
         {
            return std::shared_ptr< tEvent >( new tEvent( tID( ), priority ) );
         }
         // Create with id and priority
         static std::shared_ptr< tEvent > create( const tID& id, const tPriority& priority )
         {
            return std::shared_ptr< tEvent >( new tEvent( id, priority ) );
         }
         // Create with id and data
         static std::shared_ptr< tEvent > create( const tID& id, const tData& data )
         {
            return std::shared_ptr< tEvent >( new tEvent( id, data, carpc::priority( ePriority::DEFAULT ) ) );
         }
         // Create with data and priority
         static std::shared_ptr< tEvent > create( const tData& data, const tPriority& priority )
         {
            return std::shared_ptr< tEvent >( new tEvent( tSignature( ), data, priority ) );
         }
         // Create with id, data and priority
         static std::shared_ptr< tEvent > create( const tID& id, const tData& data, const tPriority& priority )
         {
            return std::shared_ptr< tEvent >( new tEvent( id, data, priority ) );
         }

         static const bool create_send(
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( )->send( to_context );
         }
         static const bool create_send(
               const tID& id,
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( id )->send( to_context );
         }
         static const bool create_send(
               const tData& data,
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( data )->send( to_context );
         }
         static const bool create_send(
               const tPriority& priority,
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( priority )->send( to_context );
         }
         static const bool create_send(
               const tID& id,
               const tPriority& priority,
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( id, priority )->send( to_context );
         }
         static const bool create_send(
               const tID& id,
               const tData& data,
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( id, data )->send( to_context );
         }
         static const bool create_send(
               const tData& data,
               const tPriority& priority,
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( data, priority )->send( to_context );
         }
         static const bool create_send(
               const tID& id,
               const tData& data,
               const tPriority& priority,
               const application::Context& to_context = application::Context::internal_broadcast
            )
         {
            return create( id, data, priority )->send( to_context );
         }

      // virual function
      public:
         void process_event( IAsync::IConsumer* p_consumer ) const override
         {
            static_cast< tConsumer* >( p_consumer )->process_event( *this );
         }

      // serialization / deserialization
      public:
         const bool to_stream_t( ipc::tStream& stream ) const override final
         {
            if constexpr( CARPC_IS_IPC_TYPE( tService ) )
            {
               return ipc::serialize( stream, mp_signature, m_context, m_priority, mp_data );
            }

            return false;
         }
         const bool from_stream_t( ipc::tStream& stream ) override final
         {
            if constexpr( CARPC_IS_IPC_TYPE( tService ) )
            {
               return ipc::deserialize( stream, mp_signature, m_context, m_priority, mp_data );
            }

            return false;
         }

      public:
         const bool is_ipc( ) const override { return CARPC_IS_IPC_TYPE( tService ); }

      // signature
      public:
         const tID& id( ) const
         {
            return mp_signature->id( );
         }
         tEventPtr id( const tID& _id )
         {
            mp_signature = tSignature::create( _id );
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
         const IAsync::ISignature::tSptr signature( ) const override
         {
            return mp_signature;
         }
      private:
         typename tSignature::tSptr mp_signature = nullptr;

      // data
      public:
         const tDataPtr data( ) const
         {
            return mp_data;
         }
         tEventPtr data( const tData& data )
         {
            mp_data = std::make_shared< tData >( data );
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
         tEventPtr data( const tDataPtr data )
         {
            mp_data = data;
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
      private:
         tDataPtr mp_data = nullptr;

      // context
      public:
         const application::Context& context( ) const override
         {
            return m_context;
         }
      private:
         application::Context m_context = application::Context::current( );

      // priority
      public:
         const tPriority priority( ) const override
         {
            return m_priority;
         }
         tEventPtr priority( const tPriority& value )
         {
            m_priority = value;
            return std::shared_ptr< tEvent >( shared_from_this( ), this );
         }
      protected:
         tPriority m_priority = carpc::priority( ePriority::DEFAULT );
   };

} // namespace carpc::async



#undef CLASS_ABBR
