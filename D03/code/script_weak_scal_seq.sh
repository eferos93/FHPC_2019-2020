n=(1000000000 2000000000 4000000000 8000000000 10000000000)

for i in 1 2 4 ; do
   for j in 0 1 2 3 4 ; do
      /usr/bin/time ./pi.x $((${n[j]}* $i))
   done
done


