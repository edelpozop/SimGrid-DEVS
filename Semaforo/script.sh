#!/bin/bash

for i in {1..10}
do
   ./main platform_user.xml 86400 --cfg=network/model:Constant >> ./Resultados/s4u_1.csv
done
for i in {1..10}
do
   ./main platform_user.xml 172800 --cfg=network/model:Constant >> ./Resultados/s4u_2.csv
done
for i in {1..10}
do
   ./main platform_user.xml 259200 --cfg=network/model:Constant >> ./Resultados/s4u_3.csv
done
for i in {1..10}
do
   ./main platform_user.xml 345600 --cfg=network/model:Constant >> ./Resultados/s4u_4.csv
done
for i in {1..10}
do
   ./main platform_user.xml 432000 --cfg=network/model:Constant >> ./Resultados/s4u_5.csv
done
for i in {1..10}
do
   ./main platform_user.xml 518400 --cfg=network/model:Constant >> ./Resultados/s4u_6.csv
done
for i in {1..10}
do
   ./main platform_user.xml 604800 --cfg=network/model:Constant >> ./Resultados/s4u_7.csv
done
for i in {1..10}
do
   ./main platform_user.xml 691200 --cfg=network/model:Constant >> ./Resultados/s4u_8.csv
done
for i in {1..10}
do
   ./main platform_user.xml 777600 --cfg=network/model:Constant >> ./Resultados/s4u_9.csv
done
for i in {1..10}
do
   ./main platform_user.xml 864000 --cfg=network/model:Constant >> ./Resultados/s4u_10.csv
done
