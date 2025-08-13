#!/bin/bash

printf "\033c"
pushd ../build_linux
./intransitive_dice_generator
popd
