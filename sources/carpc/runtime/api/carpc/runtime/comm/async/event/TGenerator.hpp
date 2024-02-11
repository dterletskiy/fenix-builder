#pragma once

#include "carpc/base/helpers/macros/types.hpp"
#include "carpc/runtime/comm/async/event/TEvent.hpp"
#include "carpc/runtime/comm/async/event/TSignature.hpp"
#include "carpc/runtime/comm/async/event/TConsumer.hpp"
#include "carpc/runtime/comm/async/event/TData.hpp"



namespace carpc::async {

   // Base Event Generator
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType >
   class TGeneratorBase
   {
      using _Generator = TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _IdType >;

      public:
         struct Config
         {
            using tEvent            = TEvent< _Generator >;
            using tSignature        = TSignature< _Generator >;
            using tConsumer         = TConsumer< _Generator >;
            using tData             = _DataType;
            using tService          = _ServiceType;
            using tProcessor        = void ( tConsumer::* )( const tEvent& );
            using tID               = _IdType;
         };
   };



   // Generic Event Generator
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType, typename _Activator = void >
   class TGenerator
      : public TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _IdType >
   {
   };

   namespace generic {

      template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType >
         using tGenerator =
            TGenerator< _ServiceType, _EventNamespace, _DataType, _IdType, void >;

   }



   // Event Generator with user signature type = integral value
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType >
   class TGenerator< _ServiceType, _EventNamespace, _DataType, _IdType, CARPC_ENABLE_IF_INTEGRAL_TYPE( _IdType ) >
      : public TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _IdType >
   {
   };

   namespace integral {

      template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType >
         using tGenerator =
            TGenerator< _ServiceType, _EventNamespace, _DataType, _IdType, CARPC_ENABLE_IF_INTEGRAL_TYPE( _IdType ) >;

   }



   // Event Generator with user signature type = enum
   template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType >
   class TGenerator< _ServiceType, _EventNamespace, _DataType, _IdType, CARPC_ENABLE_IF_ENUM_TYPE( _IdType ) >
      : public TGeneratorBase< _ServiceType, _EventNamespace, _DataType, _IdType >
   {
   };

   namespace enumeration {

      template< typename _ServiceType, typename _EventNamespace, typename _DataType, typename _IdType >
         using tGenerator =
            TGenerator< _ServiceType, _EventNamespace, _DataType, _IdType, CARPC_ENABLE_IF_ENUM_TYPE( _IdType ) >;

   }

} // namespace carpc::async
