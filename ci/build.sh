#!/usr/bin/env zsh

# Purpose: compiles gemc and installs it in gemc

# Container run:
# docker run -it --rm jeffersonlab//geant4:g4v11.2.2-almalinux93
# git clone http://github.com/gemc/src /root/src && cd /root/src
# git clone http://github.com/maureeungaro/src /root/src && cd /root/src
# ./ci/build.sh

# if we are in the docker container, we need to load the modules
if [[ -z "${AUTOBUILD}" ]]; then
    echo "\nNot in container"
else
    echo "\nIn container: ${DISTTAG}"
    source  /etc/profile.d/localSetup.sh
fi

function compileGEMC {
	meson setup build --native-file=release.ini -Duse_root=true --wipe
  if [ $? -ne 0 ]; then
    echo Meson setup failed
  	exit 1
  fi
	cd build
  module load geant4
  module load sim_system
  mkdir -p $SIM_HOME/gemc
	meson configure -Dprefix=$SIM_HOME/gemc
	if [ $? -ne 0 ]; then
    echo Meson configure failed
    exit 1
  fi
	meson install
	meson test
	cd ..
}

compileGEMC
echo
echo "- Content of $SIM_HOME/gemcC"
ls -lrt $SIM_HOME/gemc
echo "- Content of $SIM_HOME/gemc/bin"
ls -lrt $SIM_HOME/gemc/bin
echo
echo "gemc version:"
$SIM_HOME/gemc/bin/gemc -v
