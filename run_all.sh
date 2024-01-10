cd src/play
make 
cd -
for n in 0.75 0.7;
do
	for FILE in /home/mingxuan/layers/*; 
	do 
		./src/play/play $FILE 4 1 1 $n 2 >> out 
		python src/AIM-Simulator/traces/traceGenerate.py 	
		./src/AIM-Simulator/DRAMSim -t ./k6_aoe_02_short.trc -s ./src/AIM-Simulator/system.ini.example -d ./src/AIM-Simulator/ini/AIM.ini -c 10000000
	done
	mv out $n.espim4.txt
done 

