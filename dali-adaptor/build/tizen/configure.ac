#
# Copyright (c) 2015 Samsung Electronics Co., Ltd.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

m4_define([dali_version],[0.1.0])
AC_INIT([dali], [dali_version])
AM_INIT_AUTOMAKE([-Wall foreign])

AC_CONFIG_MACRO_DIR([m4])

AC_PROG_CXX
AC_PROG_LIBTOOL
AC_PROG_MKDIR_P

m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([yes])])

LT_INIT

DALI_ADAPTOR_VERSION=dali_version
AC_SUBST(DALI_ADAPTOR_VERSION)

FREETYPE_REQUIRED=9.16.3
# 17.1.11 = Freetype version 2.5.2
FREETYPE_BITMAP_SUPPORT_VERSION=17.1.11

PKG_CHECK_MODULES(DALICORE, dali-core)
PKG_CHECK_MODULES(ELEMENTARY, elementary)
PKG_CHECK_MODULES(ECORE, ecore)
PKG_CHECK_MODULES(ECORE_IPC, ecore-ipc)
PKG_CHECK_MODULES(EXIF, libexif)
PKG_CHECK_MODULES(FREETYPE, [freetype2 >= $FREETYPE_REQUIRED])
PKG_CHECK_MODULES(FREETYPE_BITMAP_SUPPORT, [freetype2 >= $FREETYPE_BITMAP_SUPPORT_VERSION], [ freetype_bitmap_support=yes  ], [ freetype_bitmap_support=no ] )
PKG_CHECK_MODULES(FONTCONFIG, fontconfig)
PKG_CHECK_MODULES(PNG, libpng)
PKG_CHECK_MODULES(LIBEXIF, libexif)
PKG_CHECK_MODULES(LIBDRM, libdrm)
PKG_CHECK_MODULES(LIBCURL, libcurl)
PKG_CHECK_MODULES(HARFBUZZ, harfbuzz)
PKG_CHECK_MODULES(FRIBIDI, fribidi)
PKG_CHECK_MODULES(TTRACE,  ttrace, AC_DEFINE(ENABLE_TTRACE, 1, [ttrace available]),
                  [ AC_MSG_NOTICE([Tizen Trace not avaiable]) ]
                  )
PKG_CHECK_MODULES(ECORE_IMF, [ecore-imf >= 1.13], [ecore_imf_1_13=yes], [ecore_imf_1_13=no])


# Check for EldBus.h in ECore
PKG_CHECK_MODULES(ELDBUS, eldbus, [ eldbus_available=yes ],  [ eldbus_available=no ] )

DALI_ELDBUS_AVAILABLE=
if test "x$eldbus_available" = "xyes"; then
  DALI_ELDBUS_AVAILABLE=true
  DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DDALI_ELDBUS_AVAILABLE "
fi
AC_SUBST(DALI_ELDBUS_AVAILABLE)


AC_ARG_ENABLE([feedback],
              [AC_HELP_STRING([ --enable-feedback],
                              [Enable feedback plugin])],
              [enable_feedback=yes],
              [enable_feedback=no])

AM_CONDITIONAL([USE_FEEDBACK], [test x$enable_feedback = xyes])

DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DPLATFORM_TIZEN"

AC_ARG_ENABLE(exportall,
              [AC_HELP_STRING([--enable-exportall],
                              [enables the exporting of all the symbols in the library])],
              [enable_exportall=yes],
              [enable_exportall=no])

AC_ARG_ENABLE([debug],
              [AC_HELP_STRING([--enable-debug],
                              [Turns on debugging])],
              [enable_debug=$enableval],
              [enable_debug=no])

AC_ARG_ENABLE(shaderbincache,
              [AC_HELP_STRING([--enable-shaderbincache],
                              [enables shader binary cache])],
              [enable_shaderbincache=$enableval],
              [enable_shaderbincache=DISABLE])

AC_ARG_ENABLE(networklogging,
              [AC_HELP_STRING([--enable-networklogging],
                              [enables network for debug tool])],
              [enable_networklogging=$enableval],
              [enable_networklogging=no])


if test "x$enable_debug" = "xyes"; then
  DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DDEBUG_ENABLED"
fi

if test "x$enable_debug" = "xno" -a "x$enable_exportall" = "xno"; then
  DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -fvisibility=hidden -DHIDE_DALI_INTERNALS"
fi

if test "x$enable_shaderbincache" = "xENABLE"; then
  DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DSHADERBIN_CACHE_ENABLED"
fi

if test "x$enable_networklogging" = "xyes"; then
  DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DNETWORK_LOGGING_ENABLED"
fi

# If Ecore IMF version is greater than 1.13, then some structures are different
if test "x$ecore_imf_1_13" = "xyes"; then
  DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DECORE_IMF_1_13"
fi

AC_ARG_ENABLE([gles],
              [AC_HELP_STRING([--enable-gles],
                              [Specify the OpenGL ES version for backwards compatibility])],
              [enable_gles=$enableval],
              [enable_gles=30])

DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DDALI_GLES_VERSION=${enable_gles}"

# node.js by default statically links against libuv, so it doesn't need to install
# a libuv headers/ shared library. So we can't use pkg-config to access any headers.
# As a work around we pass the node deps path so we can access the libuv headers inside nodes
# directory
AC_ARG_WITH([node-js],
              [AC_HELP_STRING([--with-node-js],
                              [Node.JS path that contains Lib UV headers. Setting this configures DALi to work with LibUV mainloop used in Node.JS.
                              For example /usr/tmp/downloads/node-v0.12.4/deps/uv/include/ ])],
              [with_node_js=$withval],
              [with_node_js=no])

# Node.JS already has a libuv main loop running,so we have to integrate with it
AM_CONDITIONAL(LIB_UV_EVENT_LOOP, test x$with_node_js != xno)


build_for_node_js=no
if test "x$with_node_js" != "xno"; then
  AC_MSG_NOTICE("build for node_js == yes");
  [build_for_node_js=yes]
  DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DNODE_JS_SUPPORT  -I${with_node_js}"
else
 #not using node.js build
  AC_MSG_NOTICE("build for node_js == no");
fi


AC_ARG_WITH([over-tizen_2_2],
            [AC_HELP_STRING([--with-over-tizen_2_2],
                            [Use tizen API over ver. 2.2])],
            [with_over_tizen_2_2=yes],
            [with_over_tizen_2_2=no])

# Tizen Profile options
AC_ARG_ENABLE([profile],
              [AC_HELP_STRING([--enable-profile=COMMON,MOBILE,WEARABLE,TV,UBUNTU],
                            [Select the variant of tizen])],
              [enable_profile=$enableval],
              [enable_profile=COMMON])

# Ensure valid profile selected
if test "x$enable_profile" != "xCOMMON" -a "x$enable_profile" != "xMOBILE" -a "x$enable_profile" != "xWEARABLE" -a "x$enable_profile" != "xTV" -a "x$enable_profile" != "xUBUNTU"; then
  AC_MSG_ERROR([$enable_profile is an invalid profile])
fi

AC_ARG_ENABLE(wayland,
              [  --enable-wayland       Build on Wayland],
              enable_wayland=yes,
              enable_wayland=no)

DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DDALI_PROFILE_${enable_profile}"
DALI_PROFILE_CFLAGS=" -DDALI_PROFILE_${enable_profile}"
AM_CONDITIONAL([COMMON_PROFILE], [test x$enable_profile = xCOMMON])
AM_CONDITIONAL([MOBILE_PROFILE], [test x$enable_profile = xMOBILE])
AM_CONDITIONAL([WEARABLE_PROFILE], [test x$enable_profile = xWEARABLE])
AM_CONDITIONAL([TV_PROFILE], [test x$enable_profile = xTV])
AM_CONDITIONAL([UBUNTU_PROFILE], [test x$enable_profile = xUBUNTU])
AM_CONDITIONAL([WAYLAND], [test x$enable_wayland = xyes])

AM_CONDITIONAL([ENABLE_NETWORK_LOGGING], [test x$enable_network_logging = xyes])

# Platforms should either enable features or remove them, they
# should not disable features. This allows the developer to override
# features through the command line.

PKG_CHECK_MODULES(OPENGLES20, glesv2 egl)

if test "x$enable_profile" != "xUBUNTU"; then
PKG_CHECK_MODULES(CAPI_APPFW_APPLICATION, capi-appfw-application)
PKG_CHECK_MODULES(DLOG, dlog)
PKG_CHECK_MODULES(SENSOR, sensor)
PKG_CHECK_MODULES(TTS, tts)
PKG_CHECK_MODULES(VCONF, vconf)
PKG_CHECK_MODULES(CAPI_SYSTEM_SYSTEM_SETTINGS, capi-system-system-settings)

if test "x$with_over_tizen_2_2" = "xyes"; then
PKG_CHECK_MODULES(CAPI_SYSTEM_INFO, capi-system-info)
fi

fi # ubuntu profile test

if test "x$enable_wayland" = "xyes"; then
PKG_CHECK_MODULES(WAYLAND, [ecore-wayland egl wayland-egl wayland-client >= 1.2.0 xkbcommon],
                  [DALI_HAS_ECOREWL=yes],
                  [DALI_HAS_ECOREWL=no])
else
PKG_CHECK_MODULES(ECORE_X, [ecore-x],
                  [DALI_HAS_ECOREX=yes],
                  [DALI_HAS_ECOREX=no])
PKG_CHECK_MODULES(X11, [x11],
                  [DALI_HAS_X11=yes],
                  [DALI_HAS_X11=no])
fi

if test "x$enable_feedback" = "xyes"; then

if test "x$enable_profile" = "xCOMMON" || test "x$enable_profile" = "xTV"; then
PKG_CHECK_MODULES(HAPTIC, haptic)
fi

if test "x$enable_profile" = "xMOBILE"; then
PKG_CHECK_MODULES(DEVICED, deviced)
fi

PKG_CHECK_MODULES(FEEDBACK, feedback)
PKG_CHECK_MODULES(MMFSOUND, mm-sound)
fi

if test x$DALI_DATA_RW_DIR != x; then
  dataReadWriteDir=$DALI_DATA_RW_DIR
else
  dataReadWriteDir=${prefix}/share/dali/
fi

if test x$DALI_DATA_RO_DIR != x; then
  dataReadOnlyDir=$DALI_DATA_RO_DIR
else
  dataReadOnlyDir=${prefix}/share/dali/
fi

if test x$FONT_CONFIGURATION_FILE != x; then
  fontConfigurationFile=$FONT_CONFIGURATION_FILE
fi

AC_SUBST(dataReadWriteDir)
AC_SUBST(dataReadOnlyDir)
AC_SUBST(DALI_ADAPTOR_CFLAGS)
AC_SUBST(DALI_PROFILE_CFLAGS)
AC_SUBST(fontConfigurationFile)


# Specify the include directory for development headers
#devincludepath=${includedir}/dali/internal
devincludepath=${includedir}
AC_SUBST(devincludepath)

AC_CONFIG_FILES([
 Makefile
 adaptor/Makefile
 dali.pc
 adaptor/dali-adaptor-integration.pc
])

AM_CONDITIONAL([USE_PLUGIN], [test x$enable_feedback = xyes])

if test "x$enable_feedback" = "xyes"; then
AC_CONFIG_FILES([
 plugins/Makefile
])
fi

if test "x$freetype_bitmap_support" = "xyes"; then
DALI_ADAPTOR_CFLAGS="$DALI_ADAPTOR_CFLAGS -DFREETYPE_BITMAP_SUPPORT"
fi

AC_OUTPUT


echo "
Configuration
-------------
  Prefix:                           $prefix
  Debug Build:                      $enable_debug
  Compile flags                     $DALI_ADAPTOR_CFLAGS
  Freetype bitmap support (Emoji):  $freetype_bitmap_support
  Profile:                          $enable_profile
  Data Dir (Read/Write):            $dataReadWriteDir
  Data Dir (Read Only):             $dataReadOnlyDir
  OVERTIZEN2.2:                     $with_over_tizen_2_2
  EldBus:                           $eldbus_available
  Shader Binary Cache:              $enable_shaderbincache
  Build for Node.JS (LibUV)         $build_for_node_js
  Ecore Version At Least 1.13.0     $ecore_imf_1_13
  Network logging enabled:          $enable_network_logging
  Font config file:                 $fontConfigurationFile
"
# optional output of node.js source path if we're building for node.js
if test "x$build_for_node_js" != "xno"; then
echo "  Node.JS LibUV header path         $with_node_js"
fi
