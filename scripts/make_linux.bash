#!/bin/bash

if [ -d ../build_linux ] ; then rm -rf ../build_linux ; fi
if [ ! -d ../build_linux ] ; then mkdir -p ../build_linux ; fi

pushd ../build_linux
cmake -G "Unix Makefiles" ..
make -j4

ls -al
if [ ! -f ./intransitive_dice_generator ] ; then
    echo -e "\n## ERROR building intransitive_dice_generator\n"
else
    echo -e "\nmake intransitive_dice_generator finished."
fi
popd 
