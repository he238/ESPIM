rm sim_result.txt

echo -e "\n python traces/traceGenerate.py 4096 1024 1 0 \n" >> sim_result.txt
python traces/traceGenerate.py 4096 1024 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 10000000
echo -e "\npython traces/traceGenerate.py 4096 2048 1 0 \n" >> sim_result.txt
python traces/traceGenerate.py 4096 2048 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 10000000
echo -e "\npython traces/traceGenerate.py 1024 1024 1 0 \n" >> sim_result.txt
python traces/traceGenerate.py 1024 1024 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 10000000
echo -e "\npython traces/traceGenerate.py 1200 1024 1 0 \n" >> sim_result.txt
python traces/traceGenerate.py 1024 1200 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 10000000
echo -e "\npython traces/traceGenerate.py 1024 512 V1 0 \n" >> sim_result.txt
python traces/traceGenerate.py 1024 512 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 10000000
echo -e "\npython traces/traceGenerate.py  21632 2048 1 0 \n"  >> sim_result.txt
python traces/traceGenerate.py 21632 2048 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 100000000
echo -e "\npython traces/traceGenerate.py 2048 2048 1 0 \n"  >> sim_result.txt
python traces/traceGenerate.py 2048 2048 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 100000000
echo -e "\npython traces/traceGenerate.py 512 256 1 0 \n"  >> sim_result.txt
python traces/traceGenerate.py 512 256 1 0 ini/$1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/$1 -c 1000000
