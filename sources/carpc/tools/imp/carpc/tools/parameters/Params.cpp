#include <fstream>
#include <filesystem>
#include <algorithm>

#include "carpc/base/helpers/functions/format.hpp"
#include "carpc/tools/parameters/Params.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "PARAMS"



using namespace carpc::tools::parameters;



Params::Params(
         int argc, char** argv, char** envp, const tMap& default_parameters
      )
{
   m_params.push_back( CmdLine::create( argc, argv ) );
   m_params.push_back( Env::create( envp ) );

   std::string file_name = std::filesystem::path( *argv ).filename( );
   file_name += std::string( ".cfg" );
   file_name = m_params[0]->value_or( "config", file_name ).first;
   m_params.push_back( Config::create( file_name ) );
}



bool Params::is_exist( const tParameter& parameter ) const
{
   for( auto item: m_params )
   {
      if( item && item->is_exist( parameter ) )
         return true;
   }

   return false;
}

std::pair< tValueOpt, bool > Params::value( const tParameter& parameter ) const
{
   for( auto item: m_params )
   {
      if( nullptr == item )
         continue;

      auto value = item->value( parameter );
      if( value.second )
         return value;
   }

   return std::make_pair( invalid_value, false );
}

std::pair< tValue, bool > Params::value_or( const tParameter& parameter, const tValue& default_value ) const
{
   for( auto item: m_params )
   {
      if( nullptr == item )
         continue;

      auto value = item->value_or( parameter, default_value );
      if( value.second )
         return value;
   }

   return std::make_pair( default_value, false );
}

void Params::print( ) const
{
   for( auto item: m_params )
      if( item )
         item->print( );
}
