#!/usr/bin/env sh
# generated from catkin/cmake/template/setup.sh.in

# Sets various environment variables and sources additional environment hooks.
# It tries it's best to undo changes from a previously sourced setup file before.
# Supported command line options:
# --extend: skips the undoing of changes from a previously sourced setup file

_SETUP_UTIL="/opt/ros/groovy/share/nxt/nxt_ros/build/devel/_setup_util.py"

if [ ! -f "$_SETUP_UTIL" ]; then
  echo "Missing Python script: $_SETUP_UTIL"
  return 22
fi

# detect if running on Darwin platform
_UNAME=`uname -s`
_IS_DARWIN=0
if [ "$_UNAME" = "Darwin" ]; then
  _IS_DARWIN=1
fi

# make sure to export all environment variables
export CMAKE_PREFIX_PATH
export CPATH
if [ $_IS_DARWIN -eq 0 ]; then
  export LD_LIBRARY_PATH
else
  export DYLD_LIBRARY_PATH
fi
export PATH
export PKG_CONFIG_PATH
export PYTHONPATH

# remember type of shell if not already set
if [ -z "$CATKIN_SHELL" ]; then
  CATKIN_SHELL=sh
fi

# invoke Python script to generate necessary exports of environment variables
_SETUP_TMP=`mktemp /tmp/setup.sh.XXXXXXXXXX`
if [ $? -ne 0 -o ! -f "$_SETUP_TMP" ]; then
  echo "Could not create temporary file: $_SETUP_TMP"
  return 1
fi
CATKIN_SHELL=$CATKIN_SHELL "$_SETUP_UTIL" $@ > $_SETUP_TMP
. $_SETUP_TMP
rm -f $_SETUP_TMP

# save value of IFS, including if it was unset
# the "+x" syntax helps differentiate unset from empty
_IFS=$IFS
if [ -z ${IFS+x} ]; then
  _IFS_WAS_UNSET=1
fi

# source all environment hooks
IFS=":"
for _envfile in $_CATKIN_ENVIRONMENT_HOOKS; do
  # restore value of IFS, including if it was unset
  IFS=$_IFS
  if [ $_IFS_WAS_UNSET ]; then
    unset IFS
  fi
  . "$_envfile"
done

# restore value of IFS, including if it was unset
IFS=$_IFS
if [ $_IFS_WAS_UNSET ]; then
  unset IFS
  unset _IFS_WAS_UNSET
fi
unset _IFS

unset _CATKIN_ENVIRONMENT_HOOKS
