#include <string.h>
#include <errno.h>
#include <cstring>

#include "carpc/oswrappers/linux/kernel.hpp"
#include "carpc/oswrappers/linux/socket.hpp"
#include "carpc/base/helpers/functions/format.hpp"

#include "carpc/trace/Trace.hpp"
#define CLASS_ABBR "Socket"



namespace carpc::os::os_linux::socket {

   int thread_local error = 0;



   const char* const socket_domain_to_string( const int domain )
   {
      switch( domain )
      {
         case AF_UNIX:        return "AF_UNIX"; // "AF_LOCAL" - Synonym for "AF_UNIX"
         case AF_INET:        return "AF_INET";
         case AF_AX25:        return "AF_AX25";
         case AF_IPX:         return "AF_IPX";
         case AF_APPLETALK:   return "AF_APPLETALK";
         case AF_X25:         return "AF_X25";
         case AF_INET6:       return "AF_INET6";
         case AF_DECnet:      return "AF_DECnet";
         case AF_KEY:         return "AF_KEY";
         case AF_NETLINK:     return "AF_NETLINK";
         case AF_PACKET:      return "AF_PACKET";
         case AF_RDS:         return "AF_RDS";
         case AF_PPPOX:       return "AF_PPPOX";
         case AF_LLC:         return "AF_LLC";
         case AF_CAN:         return "AF_CAN";
         case AF_TIPC:        return "AF_TIPC";
         case AF_BLUETOOTH:   return "AF_BLUETOOTH";
         case AF_ALG:         return "AF_ALG";
         case AF_VSOCK:       return "AF_VSOCK";
         case AF_KCM:         return "AF_KCM";
      #if OS_TARGET == OS_LINUX
         case AF_IB:          return "AF_IB";
         case AF_MPLS:        return "AF_MPLS";
         case AF_XDP:         return "AF_XDP";
      #endif
         default:             return "AF_UNDEFINED";
      }
      return "AF_UNDEFINED";
   }

   int socket_domain_from_string( const char* const domain )
   {
      if     ( 0 == std::strcmp( domain, "AF_UNIX" )        ) return AF_UNIX;
      else if( 0 == std::strcmp( domain, "AF_LOCAL" )       ) return AF_LOCAL;
      else if( 0 == std::strcmp( domain, "AF_INET" )        ) return AF_INET;
      else if( 0 == std::strcmp( domain, "AF_AX25" )        ) return AF_AX25;
      else if( 0 == std::strcmp( domain, "AF_IPX" )         ) return AF_IPX;
      else if( 0 == std::strcmp( domain, "AF_APPLETALK" )   ) return AF_APPLETALK;
      else if( 0 == std::strcmp( domain, "AF_X25" )         ) return AF_X25;
      else if( 0 == std::strcmp( domain, "AF_INET6" )       ) return AF_INET6;
      else if( 0 == std::strcmp( domain, "AF_DECnet" )      ) return AF_DECnet;
      else if( 0 == std::strcmp( domain, "AF_KEY" )         ) return AF_KEY;
      else if( 0 == std::strcmp( domain, "AF_NETLINK" )     ) return AF_NETLINK;
      else if( 0 == std::strcmp( domain, "AF_PACKET" )      ) return AF_PACKET;
      else if( 0 == std::strcmp( domain, "AF_RDS" )         ) return AF_RDS;
      else if( 0 == std::strcmp( domain, "AF_PPPOX" )       ) return AF_PPPOX;
      else if( 0 == std::strcmp( domain, "AF_LLC" )         ) return AF_LLC;
      else if( 0 == std::strcmp( domain, "AF_CAN" )         ) return AF_CAN;
      else if( 0 == std::strcmp( domain, "AF_TIPC" )        ) return AF_TIPC;
      else if( 0 == std::strcmp( domain, "AF_BLUETOOTH" )   ) return AF_BLUETOOTH;
      else if( 0 == std::strcmp( domain, "AF_ALG" )         ) return AF_ALG;
      else if( 0 == std::strcmp( domain, "AF_VSOCK" )       ) return AF_VSOCK;
      else if( 0 == std::strcmp( domain, "AF_KCM" )         ) return AF_KCM;
   #if OS_TARGET == OS_LINUX
      else if( 0 == std::strcmp( domain, "AF_IB" )          ) return AF_IB;
      else if( 0 == std::strcmp( domain, "AF_MPLS" )        ) return AF_MPLS;
      else if( 0 == std::strcmp( domain, "AF_XDP" )         ) return AF_XDP;
   #endif
      else                                                    return -1;

      return -1;
   }

   const char* const socket_type_to_string( const int type )
   {
      switch( type )
      {
         case SOCK_STREAM:    return "SOCK_STREAM";
         case SOCK_DGRAM:     return "SOCK_DGRAM";
         case SOCK_SEQPACKET: return "SOCK_SEQPACKET";
         case SOCK_RAW:       return "SOCK_RAW";
         case SOCK_RDM:       return "SOCK_RDM";
         case SOCK_PACKET:    return "SOCK_PACKET";
         default:             return "SOCK_UNDEFINED";
      }
      return "SOCK_UNDEFINED";
   }

   int socket_type_from_string( const char* const type )
   {
      if     ( 0 == std::strcmp( type, "SOCK_STREAM" )    ) return SOCK_STREAM;
      else if( 0 == std::strcmp( type, "SOCK_DGRAM" )     ) return SOCK_DGRAM;
      else if( 0 == std::strcmp( type, "SOCK_SEQPACKET" ) ) return SOCK_SEQPACKET;
      else if( 0 == std::strcmp( type, "SOCK_RAW" )       ) return SOCK_RAW;
      else if( 0 == std::strcmp( type, "SOCK_RDM" )       ) return SOCK_RDM;
      else if( 0 == std::strcmp( type, "SOCK_PACKET" )    ) return SOCK_PACKET;
      else                                                  return -1;

      return -1;
   }



   const std::string configuration::dbg_name( ) const
   {
      return carpc::format_string( domain, ".", type, ".", protocole, "-", address, ":", port );
   }

   void configuration::print( const std::string& _message ) const
   {
      SYS_INF(
         "%s => domain: %d / type: %d / protocole: %d / address: %s / port: %d",
         _message.c_str( ), domain, type, protocole, address.c_str( ), port
      );
   }

   bool configuration::operator==( const configuration& other ) const
   {
      return ( other.domain == domain ) && ( other.type == type ) && ( other.protocole == protocole )
               && ( other.address == address ) && ( other.port == port );
   }

   bool configuration::operator!=( const configuration& other ) const
   {
      return !( *this == other );
   }

   bool configuration::eq( const configuration& other ) const
   {
      return ( other.address == address ) && ( other.port == port );
   }



   socket_addr::socket_addr( const int _domain, const char* const _address, const int _port )
   {
      switch( _domain )
      {
         case AF_UNIX:
         {
            // SYS_INF( "AF_UNIX" );

            struct sockaddr_un* addr_un = (sockaddr_un*)malloc( sizeof( sockaddr_un ) );
            memset( addr_un, 0, sizeof( sockaddr_un ) );

            addr_un->sun_family = _domain;
            strncpy( addr_un->sun_path, _address, sizeof( addr_un->sun_path ) - 1 );

            m_addr = reinterpret_cast< sockaddr* >( addr_un );
            m_len = sizeof( addr_un->sun_family ) + strlen( addr_un->sun_path );
            break;
         }
         case AF_INET:
         {
            // SYS_INF( "AF_INET" );

            struct sockaddr_in* addr_in = (sockaddr_in*)malloc( sizeof( sockaddr_in ) );
            memset( addr_in, 0, sizeof( sockaddr_in ) );

            addr_in->sin_family = _domain;
            addr_in->sin_addr.s_addr = inet_addr( _address );
            addr_in->sin_port = htons( _port );

            m_addr = reinterpret_cast< sockaddr* >( addr_in );
            m_len = sizeof( sockaddr_in );
            break;
         }
         case AF_INET6:
         {
            // SYS_INF( "AF_INET6" );

            break;
         }
         case AF_VSOCK:
         {
            // SYS_INF( "AF_VSOCK" );

            struct sockaddr_vm* addr_vm = (sockaddr_vm*)malloc( sizeof( sockaddr_vm ) );
            memset( addr_vm, 0, sizeof( sockaddr_vm ) );

            addr_vm->svm_family = _domain;
            addr_vm->svm_cid = atoi( _address );
            addr_vm->svm_port = _port;

            m_addr = reinterpret_cast< sockaddr* >( addr_vm );
            m_len = sizeof( addr_vm );
            break;
         }
         default:
         {
            break;
         }
      }

      // print( m_addr );
   }

   socket_addr::~socket_addr( )
   {
      free( m_addr );
   }

   const sockaddr* const socket_addr::addr( ) const
   {
      return m_addr;
   }

   const socklen_t socket_addr::len( ) const
   {
      return m_len;
   }



   fds::fds( )
   {
      mp_read = new fd_set;
      mp_write = new fd_set;
      mp_except = new fd_set;
   }

   fds::~fds( )
   {
      delete mp_read;
      delete mp_write;
      delete mp_except;
   }

   void fds::reset( )
   {
      FD_ZERO( mp_read );
      FD_ZERO( mp_write );
      FD_ZERO( mp_except );
   }

   void fds::set( const tSocket _socket, const eType type )
   {
      fd_set* p_fd_set = convert( type );
      FD_SET( _socket, p_fd_set );
   }

   void fds::set( const std::set< tSocket > socket_set, const std::set< eType > type_set )
   {
      std::set< fd_set* > p_fd_set = convert( type_set );
      for( const auto& _socket : socket_set )
         set( _socket, p_fd_set );
   }

   void fds::clear( const tSocket _socket, const eType type )
   {
      fd_set* p_fd_set = convert( type );
      FD_CLR( _socket, p_fd_set );
   }

   void fds::clear( const std::set< tSocket > socket_set, const std::set< eType > type_set )
   {
      std::set< fd_set* > p_fd_set = convert( type_set );
      for( const auto& _socket : socket_set )
         clear( _socket, p_fd_set );
   }

   bool fds::is_set( const tSocket _socket, const eType type )
   {
      fd_set* p_fd_set = convert( type );

      return p_fd_set ? FD_ISSET( _socket, p_fd_set ) : false;
   }

   void fds::set( tSocket _socket, std::set< fd_set* > p_fd_set )
   {
      for( const auto& item : p_fd_set )
         FD_SET( _socket, item );
   }

   void fds::clear( tSocket _socket, std::set< fd_set* > p_fd_set )
   {
      for( auto& item : p_fd_set )
         FD_CLR( _socket, item );
   }

   fd_set* fds::convert( const eType type ) const
   {
      switch( type )
      {
         case eType::READ:       return mp_read;
         case eType::WRITE:      return mp_write;
         case eType::EXCEPT:     return mp_except;
         default:                return nullptr;
      }
      return nullptr;
   }

   std::set< fd_set* > fds::convert( const std::set< eType > type_set ) const
   {
      std::set< fd_set* > fds;
      for( auto& type : type_set )
      {
         switch( type )
         {
            case eType::READ:       fds.insert( mp_read );     break;
            case eType::WRITE:      fds.insert( mp_write );    break;
            case eType::EXCEPT:     fds.insert( mp_except );   break;
            default:                                           break;
         }
      }
      return fds;
   }
   
   fd_set* const fds::read( ) const
   {
      return mp_read;
   }
   
   fd_set* const fds::write( ) const
   {
      return mp_write;
   }
   
   fd_set* const fds::except( ) const
   {
      return mp_except;
   }

} // namespace carpc::os::os_linux::socket



namespace carpc::os::os_linux::socket {

   void print( const sockaddr* sa )
   {
      char* c_address = nullptr;
      std::string domain( "AF_UNDEFINED" );

      switch( sa->sa_family )
      {
         case AF_UNIX:
         {
            domain = "AF_UNIX";
            const size_t maxlen = 256;
            c_address = (char*)malloc( maxlen );
            struct sockaddr_un* addr_un = (struct sockaddr_un*)sa;
            strncpy( c_address, addr_un->sun_path, maxlen );
            break;
         }

         case AF_INET:
         {
            domain = "AF_INET";
            const size_t maxlen = INET_ADDRSTRLEN;
            c_address = (char*)malloc( maxlen );
            struct sockaddr_in* addr_in = (struct sockaddr_in*)sa;
            inet_ntop( AF_INET, &(addr_in->sin_addr), c_address, maxlen );
            break;
         }

         case AF_INET6:
         {
            domain = "AF_INET6";
            const size_t maxlen = INET6_ADDRSTRLEN;
            c_address = (char*)malloc( maxlen );
            struct sockaddr_in6* addr_in6 = (struct sockaddr_in6*)sa;
            inet_ntop( AF_INET6, &(addr_in6->sin6_addr), c_address, maxlen );
            break;
         }

         case AF_VSOCK:
         {
            domain = "AF_VSOCK";
            const size_t maxlen = 8;
            c_address = (char*)malloc( maxlen );
            struct sockaddr_vm* addr_vm = (struct sockaddr_vm*)sa;
            // https://stackoverflow.com/a/8257728
            // itoa( addr_vm->svm_cid, c_address, 10 );
            sprintf( c_address, "%d", addr_vm->svm_cid );
            break;
         }

         default:
         {
            const size_t maxlen = 256;
            c_address = (char*)malloc( maxlen );
            strncpy( c_address, "AF_UNDEFINED", maxlen );
            break;
         }
      }

      SYS_INF( "sockaddr: %p / domain: %s / address: %s", sa, domain.c_str( ), c_address );
      free( c_address );
   }

   void info( const tSocket _socket, const char* _message )
   {
      struct sockaddr_in address;
      socklen_t addrlen = sizeof( address );

      // getpeername( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      getsockname( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      SYS_INF( "%s, ip:port/socket - %s:%d/%d ", _message, inet_ntoa(address.sin_addr), ntohs(address.sin_port), _socket );

      // printf( "%hd / %hu / %u / ", address.sin_family, address.sin_port, address.sin_addr.s_addr );
      // for( size_t i = 0; i < 8; ++i )
      //    printf( "%#x ", address.sin_zero[i] );
      // printf( "\n" );
   }

   void info( const tSocket _socket, configuration& _config )
   {
      struct sockaddr_in address;
      socklen_t addrlen = sizeof( address );

      // getpeername( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      getsockname( _socket , (struct sockaddr*)&address , (socklen_t*)&addrlen );
      _config.address = inet_ntoa(address.sin_addr);
      _config.port = ntohs(address.sin_port);

   }

   tSocket create_server( const configuration& _config )
   {
      tSocket _socket = carpc::os::os_linux::socket::socket( _config );
      if( InvalidSocket == _socket )
         return InvalidSocket;
      if( false == carpc::os::os_linux::socket::bind( _socket, _config ) )
         return InvalidSocket;
      carpc::os::os_linux::set_nonblock( _socket );
      if( false == carpc::os::os_linux::socket::listen( _socket ) )
         return InvalidSocket;
      return _socket;
   }

   tSocket create_clint( const configuration& _config )
   {
      tSocket _socket = carpc::os::os_linux::socket::socket( _config );
      if( InvalidSocket == _socket )
         return InvalidSocket;
      if( false == carpc::os::os_linux::socket::connect( _socket, _config ) )
         return InvalidSocket;
      carpc::os::os_linux::set_nonblock( _socket );
      return _socket;
   }

   tSocket socket( const int _domain, const int _type, const int _protocole )
   {
      tSocket _socket = ::socket( _domain, _type, _protocole );
      error = errno;
      if( -1 == _socket )
      {
         SYS_ERR( "socket(%d, %d, %d) error: %d", _domain, _type, _protocole, error );
         return _socket;
      }

      SYS_VRB( "socket(%d)", _socket );
      return _socket;
   }

   tSocket socket( const configuration _config )
   {
      return socket( _config.domain, _config.type, _config.protocole );
   }

   bool setsockopt( tSocket _socket, int level, int option_name, const void *option_value, socklen_t option_len )
   {
      int result = ::setsockopt( _socket, level, option_name, option_value, option_len );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "setsockopt(%d) error: %d", _socket, error );
         return false;
      }
      SYS_VRB( "setsockopt(%d)", _socket );
      return true;
   }

   bool bind( const tSocket _socket, const sockaddr* _address, const socklen_t _address_len )
   {
      if( nullptr == _address )
      {
         SYS_ERR( "bind(%d): address is nullptr", _socket );
         return false;
      }

      int result = ::bind( _socket, _address, _address_len );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "bind(%d) error: %d", _socket, error );
         return false;
      }
      SYS_VRB( "bind(%d)", _socket );
      return true;
   }

   bool bind( const tSocket _socket, const int _domain, const char* const _address, const int _port )
   {
      socket_addr sa( _domain, _address, _port );
      return carpc::os::os_linux::socket::bind( _socket, sa.addr( ), sa.len( ) );
   }

   bool bind( const tSocket _socket, const configuration _config )
   {
      return carpc::os::os_linux::socket::bind( _socket, _config.domain, _config.address.c_str( ), _config.port );
   }

   bool connect( const tSocket _socket, const sockaddr* _address, const socklen_t _address_len )
   {
      if( nullptr == _address )
      {
         SYS_ERR( "connect(%d): address is nullptr", _socket );
         return false;
      }

      int result = ::connect( _socket, _address, _address_len );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "connect(%d): error %d", _socket, error );
         return false;
      }
      SYS_VRB( "connect(%d)", _socket );
      return true;
   }

   bool connect( const tSocket _socket, const int _domain, const char* const _address, const int _port )
   {
      socket_addr sa( _domain, _address, _port );
      return carpc::os::os_linux::socket::connect( _socket, sa.addr( ), sa.len( ) );
   }

   bool connect( const tSocket _socket, const configuration _config )
   {
      return carpc::os::os_linux::socket::connect( _socket, _config.domain, _config.address.c_str( ), _config.port );
   }

   bool listen( const tSocket _socket, const int _backlog )
   {
      int result = ::listen( _socket, _backlog );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "listen(%d) error: %d", _socket, error );
         return false;
      }
      SYS_VRB( "listen(%d)", _socket );
      return true;
   }

   ssize_t send( const tSocket _socket, const void* _buffer, const size_t _size, const int _flags )
   {
      if( nullptr == _buffer )
      {
         SYS_ERR( "send(%d): buffer is nullptr", _socket );
         return 0;
      }

      ssize_t size = ::send( _socket, _buffer, _size, _flags );
      error = errno;
      if( size != static_cast< ssize_t >( _size ) )
      {
         SYS_ERR( "send(%d): error %d / bytes %zd", _socket, error, size );
         return size;
      }
      SYS_VRB( "send(%d): %zd bytes", _socket, size );
      return size;
   }

   ssize_t sendto( const tSocket _socket, const void* _buffer, const size_t _size, const int _flags,
                        const sockaddr* const _address, const socklen_t _address_len )
   {
      if( nullptr == _buffer )
      {
         SYS_ERR( "sendto(%d): buffer is nullptr", _socket );
         return 0;
      }

      ssize_t size = ::sendto( _socket, _buffer, _size, _flags, _address, _address_len );
      error = errno;
      if( size != static_cast< ssize_t >( _size ) )
      {
         SYS_ERR( "sendto(%d): error %d / bytes %zd", _socket, error, size );
         return size;
      }
      SYS_VRB( "sendto(%d): %zd bytes", _socket, size );
      return size;
   }

   ssize_t sendmsg( const tSocket _socket, const msghdr* const _message, const int _flags )
   {
      ssize_t size = ::sendmsg( _socket, _message, _flags );
      error = errno;
      SYS_VRB( "sendmsg(%d): %zd bytes", _socket, size );
      return size;
   }

   ssize_t recv( const tSocket _socket, void* const _buffer, const size_t _size, const int _flags )
   {
      if( nullptr == _buffer )
      {
         SYS_ERR( "recv(%d): buffer is nullptr", _socket );
         return 0;
      }

      ssize_t size = ::recv( _socket, _buffer, _size, _flags );
      error = errno;
      if( 0 >= size )
      {
         SYS_ERR( "recv(%d): error %d", _socket, error );
         return size;
      }
      SYS_VRB( "recv(%d): %zd bytes", _socket, size );
      return size;
   }

   ssize_t recvfrom( const tSocket _socket, void* const _buffer, const size_t _size, const int _flags,
                        sockaddr* const _address, socklen_t* const _address_len )
   {
      if( nullptr == _buffer )
      {
         SYS_ERR( "recvfrom(%d): buffer is nullptr", _socket );
         return 0;
      }

      ssize_t size = ::recvfrom( _socket, _buffer, _size, _flags, _address, _address_len );
      error = errno;
      if( 0 >= size )
      {
         SYS_ERR( "recvfrom(%d): error %d", _socket, error );
         return size;
      }
      SYS_VRB( "recvfrom(%d): %zd bytes", _socket, size );
      return size;
   }

   ssize_t recvmsg( const tSocket _socket, msghdr* const _message, const int _flags )
   {
      ssize_t size = ::recvmsg( _socket, _message, _flags );
      error = errno;
      SYS_VRB( "recvmsg(%d): %zd bytes", _socket, size );
      return size;
   }

   tSocket accept( const tSocket _socket, sockaddr* const _address, socklen_t* const _address_len )
   {
      tSocket slave_socket = ::accept( _socket, _address, _address_len );
      error = errno;
      if( -1 == slave_socket )
      {
         SYS_ERR( "accept(%d): error %d", _socket, error );
         return slave_socket;
      }
      SYS_VRB( "accept(%d): %d slave socket", _socket, slave_socket );
      return slave_socket;
   }

   bool select( const tSocket _max_socket, fd_set* const _fd_set_read, fd_set* const _fd_set_write, fd_set* const _fd_set_except, timeval* const _timeout )
   {
      int result = ::select( _max_socket + 1, _fd_set_read, _fd_set_write, _fd_set_except, _timeout );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "select(%d + 1) error: %d", _max_socket, error );
         return false;
      }
      SYS_VRB( "select(%d + 1)", _max_socket );
      return true;
   }

   bool select( const tSocket _max_socket, fds& _fd, timeval* const _timeout )
   {
      return carpc::os::os_linux::socket::select( _max_socket, _fd.read( ), _fd.write( ), _fd.except( ), _timeout );
   }

   bool shutdown( tSocket _socket, int _how )
   {
      int result = ::shutdown( _socket, _how );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "shutdown(%d) error: %d", _socket, error );
         return false;
      }
      SYS_VRB( "shutdown(%d)", _socket );
      return true;
   }

   bool close( tSocket _socket )
   {
      int result = ::close( _socket );
      error = errno;
      if( -1 == result )
      {
         SYS_ERR( "close(%d) error: %d", _socket, error );
         return false;
      }
      SYS_VRB( "close(%d)", _socket );
      return true;
   }

} // namespace carpc::os::os_linux::socket



namespace carpc::os::os_linux {

   bool get_mac( const std::string& connection_name, std::string& connection_mac )
   {
      socket::tSocket fd = socket::socket( AF_UNIX, SOCK_DGRAM, IPPROTO_IP );
      if( -1 == fd )
         return false;

      struct ifreq ifreq_data;
      strcpy( ifreq_data.ifr_name, connection_name.c_str( ) );
      int result = ioctl( fd, SIOCGIFHWADDR, &ifreq_data );
      int errno_ioctl = errno;
      SYS_INF( "ioctl result: %d", result );
      if( 0 != result )
      {
         SYS_ERR( "error: %d", errno_ioctl );
         return false;
      }

      connection_mac.clear( );
      for( size_t i = 0; i < 6; ++i )
      {
         char buffer[3];
         sprintf( buffer, "%02x", ( uint8_t )ifreq_data.ifr_addr.sa_data[i] );
         connection_mac += buffer;
      }
      SYS_INF( "Connection: %s / MAC: %s", connection_name.c_str( ), connection_mac.c_str( ) );

      return true;
   }

} // namespace carpc::os::os_linux

