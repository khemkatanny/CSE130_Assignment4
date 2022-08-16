#!/bin/bash

#######################################################################
#
# Copyright (C) 2020-2022 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

if [ $# -eq "0" ]
then
  echo "USAGE: $0 [Assignment No.]"
  exit
fi
a=$1
rm -rf confirm 2>/dev/null
mkdir confirm
cd confirm
tar xvf /var/classes/CSE130/Assignment$a.tar.gz >/dev/null
tar xvf ~/CSE130-Assignment$a.tar.gz >/dev/null
make --no-print-directory grade
cd ..
rm -rf confirm 2>/dev/null