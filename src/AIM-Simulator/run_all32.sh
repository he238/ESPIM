printf "GNMT AIM Result" >> sim_result.txt
printf "\nLayer: 4096 1024\n" >> sim_result.txt
python traces/traceGenerate32.py 4096 1024 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 4096 2048\n" >> sim_result.txt
python traces/traceGenerate32.py 4096 2048 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000

printf "\nBert AIM Result" >> sim_result.txt
printf "\nLayer: 1024 1024\n" >> sim_result.txt
python traces/traceGenerate32.py 1024 1024 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 1024 4096\n" >> sim_result.txt
python traces/traceGenerate32.py 1024 4096 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 4096 1024\n" >> sim_result.txt
python traces/traceGenerate32.py 4096 1024 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000

printf "\nFacebook FC AIM Result" >> sim_result.txt
printf "\nLayer: 512 256\n" >> sim_result.txt
python traces/traceGenerate32.py 21632 2048 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000


printf "\nAlexnet FC AIM Result" >> sim_result.txt
printf "\nLayer: 21632 2048\n" >> sim_result.txt
python traces/traceGenerate32.py 21632 2048 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 2048 2048\n" >> sim_result.txt
python traces/traceGenerate32.py 2048 2048 0
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000



printf "\nGNMT GPU Result" >> sim_result.txt
printf "\nLayer: 4096 1024\n" >> sim_result.txt
python traces/traceGenerate32.py 4096 1024 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 4096 2048\n" >> sim_result.txt
python traces/traceGenerate32.py 4096 2048 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nBert GPU Result" >> sim_result.txt
printf "\nLayer: 1024 1024\n" >> sim_result.txt
python traces/traceGenerate32.py 1024 1024 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 1024 4096\n" >> sim_result.txt
python traces/traceGenerate32.py 1024 4096 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 4096 1024\n" >> sim_result.txt
python traces/traceGenerate32.py 4096 1024 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nAlexnet FC GPU Result" >> sim_result.txt
printf "\nLayer: 21632 2048\n" >> sim_result.txt
python traces/traceGenerate32.py 21632 2048 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nLayer: 2048 2048\n" >> sim_result.txt
python traces/traceGenerate32.py 2048 2048 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
printf "\nFacebook FC GPU Result" >> sim_result.txt
printf "\nLayer: 512 256\n" >> sim_result.txt
python traces/traceGenerate32.py 21632 2048 1
./DRAMSim -t traces/k6_aoe_02_short.trc -s system.ini.example -d ini/AIM.ini -c 10000000
