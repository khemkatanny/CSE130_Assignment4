#!/bin/bash

#######################################################################
#
# Copyright (C) 2020-2022 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

timeout 0.5s ./scheduler -v $1 $2 $3 $4 $5 $6 $7 $8 $9 > compare.out
exit_status=$?
cat compare.out
if [[ $exit_status -eq 124 ]]; then
  echo "FAIL - Timeout"
  exit
fi
awk '/Scheduled Usage/,/^$/' compare.out | sed '1d' > scheduled.out
awk '/Expected Usage/,/^$/' compare.out | sed '1d' > expected.out
diff expected.out scheduled.out
error=$?
if [ $error -eq 0 ]
then
  echo "Schedules Match";
else
  echo "ERROR Schedules Mismatched";
fi
rm compare.out;
rm scheduled.out;
rm expected.out;
