ROOT_DIR=/mnt/host/tda/fenix/
SOURCE_DIR=${ROOT_DIR}/fenix-builder/
PRODUCT_DIR=${ROOT_DIR}/_product_/
BUILD_DIR=${PRODUCT_DIR}/build/
GEN_DIR=${PRODUCT_DIR}/gen/
INSTALL_DIR=${PRODUCT_DIR}/deploy/
DOC_DIR=${PRODUCT_DIR}/doc/

export CC=/usr/bin/cc
export CXX=/usr/bin/c++

BUILD_VARIABLES=""
BUILD_VARIABLES+=" -DROOT_GEN_DIR:STRING=${GEN_DIR}"
BUILD_VARIABLES+=" -DTARGET_OS:STRING=linux"
BUILD_VARIABLES+=" -DDLT_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DSYS_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DMSG_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DDEBUG_STREAM:STRING=no"
BUILD_VARIABLES+=" -DCOLORED_TRACE:STRING=yes"
BUILD_VARIABLES+=" -DMEMORY_HOOK:STRING=no"
BUILD_VARIABLES+=" -DINSTRUMENTAL:STRING=no"
BUILD_VARIABLES+=" -DUSE_DEBUG:STRING=no"
BUILD_VARIABLES+=" -DUSE_GPB:STRING=yes"
BUILD_VARIABLES+=" -DUSE_RTTI:STRING=yes"




function clean( )
{
   cmake --build ${BUILD_DIR} --target clean
   rm -rf \
      ${BUILD_DIR}/CMakeFiles \
      ${BUILD_DIR}/CMakeCache.txt \
      ${BUILD_DIR}/Makefile \
      ${BUILD_DIR}/install_manifest.txt \
      ${BUILD_DIR}/cmake_install.cmake
   rm -fr ${PRODUCT_DIR}
}

function config( )
{
   cmake \
      -S ${SOURCE_DIR} \
      -B ${BUILD_DIR} \
      --install-prefix ${INSTALL_DIR} \
      --graphviz=${DOC_DIR}/graph/project \
      ${BUILD_VARIABLES}
}

function build( )
{
   cmake --build ${BUILD_DIR} -j8
}

function install( )
{
   cmake --build ${BUILD_DIR} --target install
}

function run( )
{
   export LD_LIBRARY_PATH="${INSTALL_DIR}/lib/"
   ${INSTALL_DIR}/bin/application config=${INSTALL_DIR}/etc/application.cfg
}

function validate_parameters( )
{
   if [ -z ${CMD_ACTION+x} ]; then
      echo "'--action' is not set"
      exit 1
   fi
}

function parse_arguments( )
{
   echo "Parsing arguments..."

   for option in "$@"; do
      echo "Processing option '${option}'"
      case ${option} in
         --action=*)
            if [ -z ${CMD_ACTION+x} ]; then
               CMD_ACTION="${option#*=}"
               shift # past argument=value
               echo "CMD_ACTION: ${CMD_ACTION}"
            else
               echo "'--action' is already set to '${CMD_ACTION}'"
               exit 1
            fi
         ;;
         --debug)
            CMD_DEBUG_FLAG=
            echo "CMD_DEBUG_FLAG: defined"
         ;;
         *)
            echo "undefined option: '${option}'"
            shift # past argument=value
         ;;
      esac
   done

   validate_parameters
}

function main( )
{
   parse_arguments $@

   case ${CMD_ACTION} in
      clean)
         clean
      ;;
      config)
         config
      ;;
      build)
         build
      ;;
      install)
         install
      ;;
      world)
         clean
         config
         build
         install
      ;;
      run)
         run
      ;;
      *)
         echo "undefined action: '${CMD_ACTION}'"
      ;;
   esac
}




cd ${ROOT_DIR}
main $@








# LD_LIBRARY_PATH="/mnt/host/tda/carpc/out/deploy/lib/" ./stream

# export LD_LIBRARY_PATH="/mnt/host/tda/carpc/out/deploy/lib/"
# /mnt/host/tda/carpc/out/deploy/bin/servicebrocker config=/mnt/host/tda/carpc/out/deploy/etc/servicebrocker.cfg

# export LD_LIBRARY_PATH="/mnt/host/tda/carpc/out/deploy/lib/;/mnt/host/tda/carpc-examples/out/deploy/lib/"
# /mnt/host/tda/carpc-examples/out/deploy/bin/core config=/mnt/host/tda/carpc-examples/out/deploy/etc/core.cfg
# /mnt/host/tda/carpc-examples/out/deploy/bin/controller config=/mnt/host/tda/carpc-examples/out/deploy/etc/controller.cfg
# /mnt/host/tda/carpc-examples/out/deploy/bin/hmi config=/mnt/host/tda/carpc-examples/out/deploy/etc/hmi.cfg
# /mnt/host/tda/carpc-examples/out/deploy/bin/application config=/mnt/host/tda/carpc-examples/out/deploy/etc/application.cfg
