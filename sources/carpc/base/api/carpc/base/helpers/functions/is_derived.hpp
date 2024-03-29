#pragma once

#include "carpc/base/helpers/functions/is_base.hpp"



namespace carpc {

   /****************************************************************************************************
    * 
    * This checks is class B is the base for class D
    * Is D inherited from B
    * Usage:
    *    
    *    class A { };
    *    class B : public A { };
    *    class C : public B { };
    *    class D { };
    *    int main( )
    *    {
    *       printf( "A is %s base for B", carpc::is_base_of< A, B >::value ? "" : "not" );
    *       printf( "A is %s base for C", carpc::is_base_of< A, C >::value ? "" : "not" );
    *       printf( "A is %s base for D", carpc::is_base_of< A, D >::value ? "" : "not" );
    *    
    *       return 0;
    *    }
    * 
    ***************************************************************************************************/

   template< typename B, typename D >
   struct is_base_of
   {
      static YES test( B* );
      static NO test( ... );

      static bool const value = sizeof( YES ) == sizeof( test( (D*)0 ) );
   };

   template< typename D >
   struct is_base_of< D, D >
   {
      static bool const value = false;
   };

} // namespace carpc
