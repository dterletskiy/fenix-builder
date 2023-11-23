// Framework
#include "carpc/tools/Performance.hpp"
#include "carpc/base/common/TypeInfo.hpp"
#include "carpc/runtime/application/main.hpp"



const carpc::application::Thread::Configuration::tVector services = { };



#if 1
#include <bitset>
#include <fstream>
#include <utility>
#include "carpc/base/common/IPC.hpp"
#include "carpc/base/helpers/functions/print.hpp"
#include "api/stream/sensor.pb.h"



namespace test_stream {

   using tStreamIPC = carpc::ipc::tStreamEx;



   template< typename ... TYPES >
   class Container
   {
      public:
         Container( const TYPES& ... args )
            : m_data( args... )
            , m_data_test( )
         { }

         bool run( const char* const message = "" )
         {
            MSG_VRB( "----- BEGIN: '%s' -----", message );

            serialize( m_stream );
            deserialize( m_stream );

            bool result = test( );
            if( result )
            {
               MSG_DBG( "OK" );
               print( );
            }
            else
            {
               MSG_ERR( "NOK" );
               print( );
            }
            MSG_VRB( "------ END: '%s' ------", message );
            return result;
         }

         bool serialize( tStreamIPC& stream )
         {
            auto serialize_element = [ ]( const auto& element, auto& stream )
            {
               // std::cout << "s: " << element << std::endl;
               if( false == stream.push( element ) )
               {
                  MSG_ERR( "serialize error" );
                  return false;
               }
               return true;
            };

            std::apply(
               [&]( const auto&... elements )
               {
                  ( serialize_element( elements, stream ), ... );
               },
               m_data
            );

            return true;
         }

         bool deserialize( tStreamIPC& stream )
         {
            bool result = true;
            // https://stackoverflow.com/questions/65261797/varadic-template-to-tuple-is-reversed
            // m_data_test = std::make_tuple( deserialize_element< TYPES >( stream, result )... );
            std::apply(
               [ &stream, &result, this ]( auto&... m_data_test )
               {
                  ( ( m_data_test = deserialize_element< std::decay_t< decltype( m_data_test ) > >( stream, result ) ), ... );
               },
               m_data_test
            );

            return result;
         }

         template< typename T >
         T deserialize_element( tStreamIPC& stream, bool& result )
         {
            T element;
            result &= stream.pop( element );
            // std::cout << "d: " << element << std::endl;
            return element;
         }

         bool test( ) const
         {
            return m_data == m_data_test;
         }

         void print( ) const
         {
            std::cout << "Container:" << std::endl;
            std::cout << "   original:   ";
            carpc::print( m_data, true );
            std::cout << "   restored:   ";
            carpc::print( m_data_test, true );
            std::cout << std::endl;
         }

         void dump( ) const
         {
            m_stream.dump( );
         }

      private:
         std::tuple< TYPES... > m_data;
         std::tuple< TYPES... > m_data_test;
         tStreamIPC m_stream;
   };

   struct Data
   {
      Data( ) = default;

      Data( const std::size_t& id, const std::string& name )
         : m_id( id )
         , m_name( name )
      {
      }

      bool to_stream( tStreamIPC& stream ) const
      {
         return carpc::ipc::serialize( stream, m_id, m_name );
      }
      bool from_stream( tStreamIPC& stream )
      {
         return carpc::ipc::deserialize( stream, m_id, m_name );
      }

      void print( ) const
      {
         MSG_DBG( "id: 0x%X / name: %s", m_id, m_name.c_str( ) );
      }

      const std::size_t& id( ) const
      {
         return m_id;
      }

      const std::string& name( ) const
      {
         return m_name;
      }

      std::size_t m_id = 0xFFFFFFFF;
      std::string m_name = "NoName";
   };





   void run( )
   {
      if( false )
      {
         {
            Container< void* > container( malloc( 64 ) );
            container.run( "void*");
         }

         {
            Container< const void* > container( malloc( 64 ) );
            container.run( "const void*");
         }

         {
            Container< bool > container( true );
            container.run( "bool");
         }

         {
            Container< char > container( -127 );
            container.run( "char");
         }

         {
            Container< unsigned char > container( 255 );
            container.run( "unsigned char");
         }

         {
            Container< short > container( -123 );
            container.run( "short");
         }

         {
            Container< unsigned short > container( 123 );
            container.run( "unsigned short");
         }

         {
            Container< int > container( -12345 );
            container.run( "int");
         }

         {
            Container< unsigned int > container( 12345 );
            container.run( "unsigned int");
         }

         {
            Container< long > container( -1234567 );
            container.run( "long");
         }

         {
            Container< unsigned long > container( 1234567 );
            container.run( "unsigned long");
         }

         {
            Container< long long > container( -123456789 );
            container.run( "long long");
         }

         {
            Container< unsigned long long > container( 123456789 );
            container.run( "unsigned long long");
         }

         {
            Container< float > container( 123.456 );
            container.run( "float");
         }

         {
            Container< double > container( -654.321 );
            container.run( "double");
         }

         {
            Container< long double > container( -6540.0321 );
            container.run( "long double");
         }
      }

      if( false )
      {
         {
            Container< std::string > container( "Testing std::string" );
            container.run( "std::string" );
         }

         {
            Container< std::basic_string< wchar_t > > container( L"Testing std::wstring" );
            container.run( "std::wstring" );
         }

         {
            Container< std::vector< int > > container( { 11, 22, 33 } );
            container.run( "std::vector< int >" );
         }

         {
            Container< std::list< int > > container( { 11, 22, 33 } );
            container.run( "std::list< int >" );
         }

         {
            Container< std::set< int > > container( { 44, 55, 66 } );
            container.run( "std::set< int >" );
         }

         {
            Container< std::pair< int, float > > container( { 111, 11.11 } );
            container.run( "std::pair< int, float >" );
         }

         {
            Container< std::map< int, float > > container( { { 111, 1.1 }, { 222, 2.2 }, { 333, 3.3 } } );
            container.run( "std::map< int, float >" );
         }

         {
            Container< std::optional< double > > container( 987.654 );
            container.run( "std::optional< double >" );
         }

         {
            Container< std::optional< long > > container( std::nullopt );
            container.run( "std::optional< long >" );
         }
      }

      if( false )
      {
         Container<
               void*, void*, 
               bool,
               char, unsigned char,
               short, unsigned short,
               int, unsigned int,
               long, unsigned long,
               long long, unsigned long long,
               float,
               double,
               long double
            >
            container(
                  malloc( 64 ), nullptr,
                  true,
                  'T', 'D',
                  -123, 123,
                  -12345, 12345,
                  -1234567, 1234567,
                  -123456789, 123456789,
                  123.456,
                  -654.321,
                  -6540.0321
               );
         container.run( );
      }

      if( true )
      {
         Container<
               std::map<
                     std::size_t,
                     std::map<
                        std::size_t,
                        std::vector< std::optional< std::set< double > > >
                     >
               >
            >
            container_map(
                  {
                     {
                        100,
                        {
                           { 11111, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 1.1, 2.2, 3.3 } } },
                           { 22222, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 4.4, 5.5, 6.6 } } },
                           { 33333, std::vector< std::optional< std::set< double > > >{ std::nullopt } }
                        }
                     },
                     {
                        200,
                        {
                           { 44444, std::vector< std::optional< std::set< double > > >{ std::nullopt } },
                           { 55555, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 7.7, 8.8, 9.9 } } },
                           { 66666, std::vector< std::optional< std::set< double > > >
                              {
                                 std::set< double >{ 1.0, 2.0, 3.0 }, std::nullopt, std::nullopt
                              }
                           },
                        }
                     },
                  }
               );
         container_map.run( );
      }

      // if( false )
      // {
      //    Container<
      //          void*, void*, 
      //          bool,
      //          char, unsigned char,
      //          short, unsigned short,
      //          int, unsigned int,
      //          long, unsigned long,
      //          long long, unsigned long long,
      //          float,
      //          double,
      //          long double
      //       >
      //       container_simple(
      //             malloc( 64 ), nullptr,
      //             true,
      //             'T', 'D',
      //             -123, 123,
      //             -12345, 12345,
      //             -1234567, 1234567,
      //             -123456789, 123456789,
      //             123.456,
      //             -654.321,
      //             -6540.0321
      //          );
      //    container_simple.print( );
      //    container_simple.run( );

      //    Container<
      //          std::string,
      //          std::vector< int >,
      //          std::set< int >,
      //          std::optional< double >,
      //          std::optional< long >
      //       >
      //       container_containers(
      //             "Hello CARPC",
      //             { 11, 22, 33 },
      //             { 44, 55, 66 },
      //             987.654,
      //             std::nullopt
      //          );
      //    container_containers.print( );
      //    container_containers.run( );

      //    Container<
      //          std::map<
      //                std::size_t,
      //                std::map<
      //                   std::size_t,
      //                   std::vector< std::optional< std::set< double > > >
      //                >
      //          >
      //       >
      //       container_map(
      //             {
      //                {
      //                   100,
      //                   {
      //                      { 11111, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 1.1, 2.2, 3.3 } } },
      //                      { 22222, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 4.4, 5.5, 6.6 } } },
      //                      { 33333, std::vector< std::optional< std::set< double > > >{ std::nullopt } }
      //                   }
      //                },
      //                {
      //                   200,
      //                   {
      //                      { 44444, std::vector< std::optional< std::set< double > > >{ std::nullopt } },
      //                      { 55555, std::vector< std::optional< std::set< double > > >{ std::set< double >{ 7.7, 8.8, 9.9 } } },
      //                      { 66666, std::vector< std::optional< std::set< double > > >
      //                         {
      //                            std::set< double >{ 1.0, 2.0, 3.0 }, std::nullopt, std::nullopt
      //                         }
      //                      },
      //                   }
      //                },
      //             }
      //          );
      //    container_map.print( );
      //    container_map.run( );

      //    Container<
      //          std::shared_ptr< Data >,
      //          std::shared_ptr< Data >
      //       >
      //       container_pointers(
      //             std::make_shared< Data >( 0xABCDEF, "SHARED" ),
      //             nullptr
      //          );
      //    container_pointers.print( );
      //    container_pointers.run( );
      // }

   }

} // namespace test_stream
#endif



#if 0
#include <json/reader.h>

namespace test_jsoncpp {

} // namespace test_jsoncpp
#endif



#if 0
#include <iostream>
#include <fstream>
#include "nlohmann/json.hpp"

namespace test_nlohmann_json {

   struct Area
   {
      std::string    name;
      int32_t        id;
   };

   struct Areas
   {
      std::vector< Area >  areas;
      std::string          operation;
   };

   struct VissProperty
   {
      std::string    name;
   };

   struct AreaMapping
   {
      Areas          areas_combination;
      VissProperty   viss_property;
   };

   struct Property
   {
      std::string    name;
      int32_t        id;
   };

   struct Value
   {
      std::string                   type;
      std::vector< std::string >    initial;
      std::string                   min;
      std::string                   max;
   };

   struct MappingElement
   {
      std::string                description;
      std::string                area_type;
      Property                   property;
      std::string                access_mode;
      std::string                change_mode;
      Value                      value;
      std::vector< AreaMapping > area_mapping;
   };

   struct Root
   {
      void dump( )
      {
         MSG_DBG( "elements: %zu", mapping_elements.size( ) );
         for( auto mapping_element : mapping_elements )
         {
            MSG_DBG( "description: %s", mapping_element.description.c_str( ) );
            MSG_DBG( "area_type: %s", mapping_element.area_type.c_str( ) );
            MSG_DBG( "property:" );
            MSG_DBG( "   name: %s", mapping_element.property.name.c_str( ) );
            MSG_DBG( "   id: %d", mapping_element.property.id );
            MSG_DBG( "access_mode: %s", mapping_element.access_mode.c_str( ) );
            MSG_DBG( "change_mode: %s", mapping_element.change_mode.c_str( ) );
            MSG_DBG( "value:" );
            MSG_DBG( "   type: %s", mapping_element.value.type.c_str( ) );
            MSG_DBG( "   min: %s", mapping_element.value.min.c_str( ) );
            MSG_DBG( "   max: %s", mapping_element.value.max.c_str( ) );
            MSG_DBG( "   initial:" );
            for( const auto& initial : mapping_element.value.initial )
               MSG_DBG( "      %s", initial.c_str( ) );
            MSG_DBG( "area_mapping:" );
            for( const auto& mapping : mapping_element.area_mapping )
            {
               MSG_DBG( "   areas_combination:" );
               MSG_DBG( "     operation: %s", mapping.areas_combination.operation.c_str( ) );
               for( const auto& area : mapping.areas_combination.areas )
               {
                  MSG_DBG( "     area:" );
                  MSG_DBG( "        name: %s", area.name.c_str( ) );
                  MSG_DBG( "        name: %d", area.id );
               }
               MSG_DBG( "   viss_property:" );
               MSG_DBG( "      name: %s", mapping.viss_property.name.c_str( ) );
            }
         }
      }
      std::vector< MappingElement > mapping_elements;
   };



   #define FROM_JSON_ALL( JSON_OBJECT, JSON_ATTR_NAME, FIELD_NAME ) \
   { \
      MSG_DBG( "JSON_ATTR_NAME: %s", JSON_ATTR_NAME ); \
      MSG_DBG( "JSON_OBJECT: %s", JSON_OBJECT.dump( ).c_str( ) ); \
      auto foundElement = JSON_OBJECT.find( JSON_ATTR_NAME ); \
      if( JSON_OBJECT.end( ) != foundElement && false == foundElement->empty( ) ) \
      { \
         MSG_DBG( "foundElement: %s", foundElement->dump( ).c_str( ) ); \
         foundElement->get_to( FIELD_NAME ); \
      } \
   }

   #define FROM_JSON( JSON_OBJECT, JSON_ATTR_NAME, FIELD_NAME ) \
   { \
      MSG_DBG( "JSON_ATTR_NAME: %s", JSON_ATTR_NAME ); \
      auto foundElement = JSON_OBJECT.find( JSON_ATTR_NAME ); \
      if( JSON_OBJECT.end( ) != foundElement && false == foundElement->empty( ) ) \
      { \
         foundElement->get_to( FIELD_NAME ); \
      } \
   }



   // Area
   void to_json( nlohmann::json& j, const Area& val )
   {
      j = nlohmann::json
      {
         { "name", val.name },
         { "id", val.id }
      };
   }
   void from_json( const nlohmann::json& j, Area& val )
   {
      FROM_JSON( j, "name", val.name );
      FROM_JSON( j, "id", val.id );
   }

   // Areas
   void to_json( nlohmann::json& j, const Areas& val )
   {
      j = nlohmann::json
      {
         { "areas", val.areas },
         { "operation", val.operation }
      };
   }
   void from_json( const nlohmann::json& j, Areas& val )
   {
      FROM_JSON( j, "areas", val.areas );
      FROM_JSON( j, "operation", val.operation );
   }

   // VissProperty
   void to_json( nlohmann::json& j, const VissProperty& val )
   {
      j = nlohmann::json
      {
         { "name", val.name }
      };
   }
   void from_json( const nlohmann::json& j, VissProperty& val )
   {
      FROM_JSON( j, "name", val.name );
   }

   // AreaMapping
   void to_json( nlohmann::json& j, const AreaMapping& val )
   {
      j = nlohmann::json
      {
         { "areas_combination", val.areas_combination },
         { "viss_property", val.viss_property }
      };
   }
   void from_json( const nlohmann::json& j, AreaMapping& val )
   {
      FROM_JSON( j, "areas_combination", val.areas_combination );
      FROM_JSON( j, "viss_property", val.viss_property );
   }

   // Property
   void to_json( nlohmann::json& j, const Property& val )
   {
      j = nlohmann::json
      {
         { "name", val.name },
         { "id", val.id }
      };
   }
   void from_json( const nlohmann::json& j, Property& val )
   {
      FROM_JSON( j, "name", val.name );
      FROM_JSON( j, "id", val.id );
   }

   // Value
   void to_json( nlohmann::json& j, const Value& val )
   {
      j = nlohmann::json
      {
         { "type", val.type },
         { "initial", val.initial },
         { "min", val.min },
         { "max", val.max }
      };
   }
   void from_json( const nlohmann::json& j, Value& val )
   {
      FROM_JSON( j, "type", val.type );
      FROM_JSON( j, "initial", val.initial );
      FROM_JSON( j, "min", val.min );
      FROM_JSON( j, "max", val.max );
   }

   // MappingElement
   void to_json( nlohmann::json& j, const MappingElement& val )
   {
      j = nlohmann::json
      {
         { "description", val.description },
         { "area_type", val.area_type },
         { "property", val.property },
         { "access_mode", val.access_mode },
         { "change_mode", val.change_mode },
         { "value", val.value },
         { "area_mapping", val.area_mapping }
      };
   }
   void from_json( const nlohmann::json& j, MappingElement& val )
   {
      FROM_JSON( j, "description", val.description );
      FROM_JSON( j, "area_type", val.area_type );
      FROM_JSON( j, "property", val.property );
      FROM_JSON( j, "access_mode", val.access_mode );
      FROM_JSON( j, "change_mode", val.change_mode );
      FROM_JSON( j, "value", val.value );
      FROM_JSON( j, "area_mapping", val.area_mapping );
   }

   // Root
   void to_json( nlohmann::json& j, const Root& val )
   {
      j = nlohmann::json
      {
         { "mapping_elements", val.mapping_elements }
      };
   }
   void from_json( const nlohmann::json& j, Root& val )
   {
      FROM_JSON( j, "mapping_elements", val.mapping_elements );
   }



   bool parse_to_cpp( const std::string& json, Root& root, std::string& error )
   {
      bool result = false;

      try
      {
         nlohmann::json j_filtered = nlohmann::json::parse( json );
         // std::cout << std::setw(4) << j_filtered << std::endl;
         root = j_filtered.get< Root >( );
         result = true;
      }
      catch( nlohmann::json::exception& e )
      {
         result = false;
         error = e.what( );
         MSG_ERR( "error: %s", error.c_str( ) );
      }

      if( result )
         root.dump( );

      return result;
   }

   const char* const to_string( const nlohmann::json::parse_event_t& event )
   {
      switch( event )
      {
         case nlohmann::json::parse_event_t::object_start:  return "object_start";
         case nlohmann::json::parse_event_t::object_end:    return "object_end";
         case nlohmann::json::parse_event_t::array_start:   return "array_start";
         case nlohmann::json::parse_event_t::array_end:     return "array_end";
         case nlohmann::json::parse_event_t::key:           return "key";
         case nlohmann::json::parse_event_t::value:         return "value";
         default:                                           return "undefined";
      }
      return "undefined";
   }

   bool parse( const std::string& json, std::string& error )
   {
      bool result = false;

      try
      {
         nlohmann::json::parser_callback_t cb = [ ]( int depth, nlohmann::json::parse_event_t event, nlohmann::json& parsed )
         {
            MSG_DBG( "event: '%s' / parsed: '%s'", to_string( event ), parsed.dump( ).c_str( ) );
            return true;
         };

         nlohmann::json j_filtered = nlohmann::json::parse( json, cb, false );
         // std::cout << std::setw(4) << j_filtered << std::endl;

         result = true;
      }
      catch( nlohmann::json::exception& e )
      {
         result = false;
         error = e.what( );
         MSG_ERR( "error: %s", error.c_str( ) );
      }

      return result;
   }



   bool run( )
   {
      const char* const file = "/mnt/dev/EPAM/android/source/android-12.1.0_r8/device/epam/aosp-vhal/vehicle/hal/impl/production/config/viss/vehicle_viss_prop_mapping.json";
      std::ifstream ifstream( file );

      if( !ifstream.is_open( ) )
      {
         MSG_ERR( "Couldn't open: file '%s'", file );
         return false;
      }

      std::string json( ( std::istreambuf_iterator< char >( ifstream ) ), std::istreambuf_iterator< char >( ) );

      Root root;
      std::string error;

      // parse_to_cpp( json, root, error );
      // parse( json, error );
      parse( std::string{ "{\n   \"Signal.Drivetrain.FuelSystem.Level\" : 1000,\n}" }, error );

      return true;
   }

} // namespace test_nlohmann_json
#endif



#if 0
#include "carpc/tools/Performance.hpp"

// Given number 'a'
// Find all 'x'and 'y' numbers such that 'x + y + x*y = a'
// This could be changed to 'm*n=b' where:
// - m = x + 1
// - n = y + 1
// - b = a + 1
namespace exercise_01 {

   using TYPE = std::size_t;

   const TYPE value = 500008500036;

   struct Point
   {
      TYPE x{ 0 };
      TYPE y{ 0 };
   };

   std::vector< Point > processor( const TYPE& a )
   {
      MSG_DBG( "processing value: %zu", a );

      const TYPE b = a + 1;

      std::vector< Point > result;
      result.reserve( 100 );

      TYPE m = 1;
      TYPE limit = b / m;

      carpc::tools::Performance performance( "calculate" );
      performance.start( );

      while( ++m < limit )
      {
         if( ( b % m ) == 0 )
         {
            limit = b / m;
            result.push_back( { m - 1, limit - 1 } );
         }
      }

      performance.stop( );

      if( result.empty( ) )
         printf( "no answer\n" );

      for( auto& item: result )
         printf( "x = %zu / y = %zu\n", item.x, item.y );

      return result;
   }

} // namespace exercise_01
#endif




template< typename Test, template< typename... > class Ref >
struct is_specialization : std::false_type { };

template< template< typename... > class Ref, typename... Args >
struct is_specialization< Ref< Args... >, Ref >: std::true_type { } ;



bool test( int argc, char** argv, char** envp )
{
   MSG_MARKER( "TEST" );

   test_stream::run( );
   // test_nlohmann_json::run( );

   MSG_MARKER( "TEST" );

   return false;
}
