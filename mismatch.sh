#!/bin/bash

#######################################################################
#
# Copyright (C) 2020-2022 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

for i in {1..500}
do
  ./compare.sh $1 $2 $3 $4 $5 $6 $7 $8 | tee mismatch.out
  cnt=`grep -c Mismatch mismatch.out`
  if [[ $cnt == 1 ]]
  then
    exit  
  fi
done
