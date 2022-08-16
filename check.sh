#!/bin/bash

#######################################################################
#
# Copyright (C) 2020-2022 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

(( gtotal = 0))
(( gpass = 0))
(( check = ( RANDOM % 1024 )  + 1 ))
(( apass = 0 ))
(( mpass = 0 ))
(( ecpass = 0 ))

echo ""
function alg() {
  flag=$1
  name=$2
  echo -n "$name: "
  (( total = 0))
  > test.out
  p=2
  while (( p < 32 ))
  do
    for f in {1..5}
    do
      quantum=`shuf -i 1-4 -n 1`
      (( total += 1))
      (( cnt = p ));
      echo -n "$cnt "
      timeout 2s ./scheduler -q $quantum -t $cnt --$flag -c $check >> test.out
      exit_status=$?
      if [[ $exit_status -eq 124 ]]; then
        echo "FAIL - Timeout" >> test.out
      fi
    done
    (( p += 1 ))
  done

  if [[ "$flag" == "fcfs" ]]
  then
    cnt=`shuf -i 2048-4096 -n 1` 
    echo -n "$cnt "
    timeout --preserve-status 10s ./scheduler -t $cnt --$flag -c $check  > /dev/null
    exit_status=$?
    if (( exit_status != 0 )); then 
      exit
    fi
  fi
  echo ""

  pass=`grep "PASS $check" test.out | wc -l`

  if [ "$pass" -lt "0" ]; then
    pass=0
  fi

  (( ecpass += pass ))
  (( gtotal += total ))

  pct=0;
  if (( pass > 0 ))
  then
    pct=`echo "scale=2; $pass / $total * 100.0" | bc -l`
  fi

  printf "\n%48s:  %3d/%3d  %6.1f%%\n\n" "$name" $pass $total $pct
  (( apass = pass ));

  if [ "$apass" -gt "$mpass" ]; then
    mpass=$apass
  fi
}

(( max = 150 ))

(( apass = 0 ))
alg "fcfs" "First Come First Served"
(( fcpass = apass ))
(( gpass += fcpass ))
rrpct=0;
if (( apass > 0 ))
then
  fcpct=`echo "scale=2; $apass / $max * 30.0" | bc -l`
fi

(( apass = 0 ))
alg "rr" "Round Robin"
(( rrpass = apass ))
(( gpass += rrpass ))
rrpct=0;
if (( apass > 0 ))
then
  rrpct=`echo "scale=2; $apass / $max * 20.0" | bc -l`
fi

(( mpass = 0 ))
alg "np-priority" "Non Preemptive Priority"
alg "np-sjf" "Non Preemptive Shortest Job First"
alg "np-srtf" "Non Preemptive Shortest Remaining Time First"
if [ "$mpass" -gt "$max" ]; then
  mpass=$max
fi
(( nppass = mpass ))
(( gpass += nppass ))
nppct=0;
if (( mpass > 0 ))
then
  nppct=`echo "scale=2; $nppass / $max * 15.0" | bc -l`
fi

(( mpass = 0 ))
alg "p-priority" "Preemptive Priority"
alg "p-sjf" "Preemptive Shortest Job First"
alg "p-srtf" "Preemptive Shortest Remaining Time First"
if [ "$mpass" -gt "$max" ]; then
  mpass=$max
fi
(( ppass = mpass ))
(( gpass += ppass ))
ppct=0;
if (( mpass > 0 ))
then
  ppct=`echo "scale=2; $ppass / $max * 15.0" | bc -l`
fi

echo ""
echo "      ---------------------------------------------------"
echo ""
printf "%30s:  %3d/%3d  %6.1f%% of 30%%\n" "First Come First Served"    $fcpass $max $fcpct
printf "%30s:  %3d/%3d  %6.1f%% of 20%%\n" "Round Robin"    $rrpass $max $rrpct
printf "%30s:  %3d/%3d  %6.1f%% of 15%%\n" "1 x Non Preemptive" $nppass $max $nppct
printf "%30s:  %3d/%3d  %6.1f%% of 15%%\n" "1 x Preemptive"     $ppass  $max $ppct

(( max = max*8 ))
ec=0
if (( ecpass == max ))
then
  (( ec = 10 ))
  printf "%30s:           %6.1f%% of 10%%\n" "All Others" $ec
else
  printf "%30s:              %s\n" "All Others" "¯\_(ツ)_/¯"
fi

(( max = 140*3 ))
gpct=0;
if (( gpass > 0 ))
then
  gpct=`echo "scale=2; $fcpct + $rrpct + $nppct + $ppct + $ec" | bc -l`
fi

echo ""
printf "%30s:   - / -   %6.1f%% of 90%%\n" "Tests" $gpct

