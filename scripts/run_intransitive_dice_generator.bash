#!/bin/bash

printf "\033c"
pushd ../build_linux
rm -f intransitive_dice_generator.log
./intransitive_dice_generator 2>&1 | tee -a intransitive_dice_generator.log
popd
