#!/usr/bin/env zsh

# Purpose: compiles gemc and installs it in gemc

# Container run:
# docker_run_image jeffersonlab/geant4:g4v11.2.2-almalinux93
# docker_run_image jeffersonlab/geant4:g4v11.2.2-ubuntu24
# docker_run_image jeffersonlab/geant4:g4v11.2.2-fedora36
#
# local build:
# git clone http://github.com/gemc/src /root/src && cd /root/src
# git clone http://github.com/maureeungaro/src /root/src && cd /root/src
# ./ci/build.sh

# if we are in the docker container, we need to load the modules
if [[ -z "${AUTOBUILD}" ]]; then
    echo "\nNot in container"
else
    echo "\n > Running In a Docker Container"
    source  /etc/profile.d/localSetup.sh
fi

function lib_or_lib64 {
  if [ -d $GEMC/lib ]; then
    echo "- Content of $GEMC/lib"
    ls -lrt $GEMC/lib
    export LD_LIBRARY_PATH=$GEMC/lib:$LD_LIBRARY_PATH
  elif [ -d $GEMC/lib64 ]; then
    echo "- Content of $GEMC/lib64"
    ls -lrt $GEMC/lib64
    export LD_LIBRARY_PATH=$GEMC/lib64:$LD_LIBRARY_PATH
  fi
}

function compileGEMC {
  echo " > Current dir: $(pwd)"
  ls -l
  git fetch --prune --unshallow --tags
  git describe --tags --always --abbrev=0
  echo " > After git describe"
	meson setup build --native-file=release.ini -Duse_root=true --wipe
	cd build
  module load geant4
  module load sim_system
  export GEMC=$SIM_HOME/gemc
  mkdir -p $GEMC
  echo " > Running meson configure -Dprefix=$GEMC"
	meson configure -Dprefix=$GEMC
  echo " > Running meson compile and install"
	meson compile -v
	meson install
  lib_or_lib64
  echo " > Running meson test"
	meson test -v
	cd ..
}

compileGEMC
echo
echo "- Content of $GEMC"
ls -lrt $GEMC
echo "- Content of $GEMC/bin"
ls -lrt $GEMC/bin
lib_or_lib64
echo

echo
echo "gemc version:"
$GEMC/bin/gemc -v
if [ $? -ne 0 ]; then
  echo Running gemc  failed
  exit 1
fi
