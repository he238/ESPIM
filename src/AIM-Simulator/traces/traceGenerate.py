import numpy as np
import sys

#read ini file 
class dram_system():
    NUM_BANKS = 0
    NUM_ROWS = 0
    NUM_COLS = 0
    JEDEC_DATA_BUS_BITS = 0
    clk_cycle = 0
    BL = 0
    row = 9
    def __init__(self, ini_path, system_path):
        self.fw = open("./k6_aoe_02_short.trc", "w")
        f = open(ini_path, "r")
        line = f.readline()
        line = line.split('=')
        if (line[0] == "NUM_BANKS"):
            self.NUM_BANKS = int (line[1])
        else:
            print ("Ini format error")
            f.close()
        line = f.readline()
        line = line.split('=')
        if (line[0] == "NUM_ROWS"):
            self.NUM_ROWS = int(line[1])
        else:
            print ("Ini format error")
            f.close()
        line = f.readline()
        line = line.split('=')
        if (line[0] == "NUM_COLS"):
            self.NUM_COLS = int(line[1])
        else:
            print ("Ini format error")
            f.close()

        while line:
            line = f.readline()
            line = line.split('=')
            if (line[0] == "BL"):
                self.BL = int(line[1].split(';')[0]) 
                break
        f.close()    
        self.NUM_COLS = self.NUM_COLS / self.BL    
        f = open(system_path, "r")
        line = f.readline()
        line = line.split('=')
        while line:
            line = f.readline()
            line = line.split('=')
            if (line[0] == "JEDEC_DATA_BUS_BITS"):
                f.close()
                self.JEDEC_DATA_BUS_BITS = int(line[1].split(';')[0]) * self.BL
                return
        print ("System format error")

        

    def addr_generate(self, bank, row, col):
        addr = 0
        addr = int (addr) | int(bank)
        addr = addr << int(np.log2(self.NUM_COLS))
        addr = addr | int(col)
        addr = addr << (int(np.log2(self.NUM_ROWS)))
        addr = addr | int(row)
        addr = addr << int(np.log2(self.JEDEC_DATA_BUS_BITS/8))
        return addr
        
    def open_banks (self, row):
            self.fw.write(hex(self.addr_generate(0, row, 0)))
            self.fw.write(" OPEN_ROW ")
            self.fw.write(str(self.clk_cycle))
            self.fw.write("\n")
            self.clk_cycle =  self.clk_cycle + 1
            
    def mult_row (self, row):
        self.open_banks (row)
        for i in range (int(self.NUM_COLS)):
            self.fw.write(hex(self.addr_generate(self.NUM_BANKS, row, i)))
            self.fw.write(" MULT ")
            self.fw.write(str(self.clk_cycle))
            self .fw.write("\n")
            self.clk_cycle = self.clk_cycle + 1
    
    def multM_N_row_major (self, row, M, N):
        N = N / self.NUM_BANKS
        M = M / self.NUM_COLS / 16 #NUM OF MACs
        for i in range (M):
            for j in range (N):
                self.mult_row (row + i + j*M)
                for x in range (self.NUM_COLS):
                    self.fw.write(hex(self.addr_generate(15, 0, 0)))
                    self.fw.write(" BUFFER_WRITE ")
                    self.fw.write(str(self.clk_cycle))
                    self.fw.write("\n")
                    self.clk_cycle =  self.clk_cycle + 1  
            self.fw.write(hex(self.addr_generate(15, 0, 0)))
            self.fw.write(" BUFFER_READ ")
            self.fw.write(str(self.clk_cycle))
            self.fw.write("\n")
            self.clk_cycle =  self.clk_cycle + 1   
            
    def multM_N_col_major (self, row, M1, N):
        N = N / self.NUM_BANKS
        M = M1 / self.NUM_COLS / 16 #NUM OF MACs
        print("M",M)
        for j in range (int (M)): 
            for x in range (int (self.NUM_COLS)):
                self.fw.write(hex(self.addr_generate(15, 0, 0)))
                self.fw.write(" BUFFER_WRITE ")
                self.fw.write(str(self.clk_cycle))
                self.fw.write("\n")
                self.clk_cycle =  self.clk_cycle + 1
            for i in range (int(N)):
                self.mult_row (row + i + j*M)
                self.fw.write(hex(self.addr_generate(15, 0, 0)))
                self.fw.write(" BUFFER_READ ")
                self.fw.write(str(self.clk_cycle))
                self.fw.write("\n")
                self.clk_cycle =  self.clk_cycle + 1  
        if M1 / 16 % (self.NUM_COLS) != 0:
            for x in range (M1 / 16 % (self.NUM_COLS)):
                self.fw.write(hex(self.addr_generate(0, 0, 0)))
                self.fw.write(" BUFFER_WRITE ")
                self.fw.write(str(self.clk_cycle))
                self.fw.write("\n")
                self.clk_cycle =  self.clk_cycle + 1

            for i in range (N):
                self.mult_row_partial(row + i, M1 / 16 % (self.NUM_COLS))
                self.fw.write(hex(self.addr_generate(0, 0, 0)))
                self.fw.write(" BUFFER_READ ")
                self.fw.write(str(self.clk_cycle))
                self.fw.write("\n")
                self.clk_cycle =  self.clk_cycle + 1


        for i in range (self.NUM_BANKS):
            self.fw.write(hex(self.addr_generate(15, 0, 0)))
            self.fw.write(" BUFFER_READ ")
            self.fw.write(str(self.clk_cycle))
            self.fw.write("\n")
            self.clk_cycle =  self.clk_cycle + 1  

    def read_all_out(self, M, N):
        for j in range (N):
            for i in range (M/16):
                self.fw.write(hex(self.addr_generate(i, j, 0)))
                self.fw.write(" P_FETCH ")
                self.fw.write(str(self.clk_cycle))
                self.fw.write("\n")
                self.clk_cycle =  self.clk_cycle + 1 


    def read_commands(self):
        commandFile = open("./Command.txt", "r")
        Lines = commandFile.readlines()
        gb_index = 0
        col = 0

        for line in Lines:
            if (col % self.NUM_COLS == 0):
                col = 0
                self.row = self.row + 1 
                self.open_banks(self.row)
            self.fw.write(hex(self.addr_generate(self.NUM_BANKS, self.row, col)))
            self.fw.write(" MULT ")
            self.fw.write(str(self.clk_cycle))
            self .fw.write("\n")
            self.clk_cycle = self.clk_cycle + 1

            if (line == "Compute\n"):
                #print(line) 
                pass
            elif (line == "LoadIndex\n"):
                pass
            elif (line == "NextCompute\n"):
                gb_index = gb_index  + 1
                if (gb_index % (4096/16) == 0):
                    for i in range(12):
                        self.fw.write(hex(self.addr_generate(0, self.row+1, 0)))
                        self.fw.write(" BUFFER_READ ")
                        self.fw.write(str(self.clk_cycle))
                        self.fw.write("\n")
                        self.clk_cycle =  self.clk_cycle + 1  
            col = col + 1
        self.fw.write(hex(self.addr_generate(self.NUM_BANKS, self.row, col)))
        self.fw.write(" MULT_END ")
        self.fw.write(str(self.clk_cycle))
        self .fw.write("\n")
        self.clk_cycle = self.clk_cycle + 1
        for i in range(12):
            self.fw.write(hex(self.addr_generate(0, self.row+1, 0)))
            self.fw.write(" BUFFER_READ ")
            self.fw.write(str(self.clk_cycle))
            self.fw.write("\n")
            self.clk_cycle =  self.clk_cycle + 1    
#print ("Size of Matrix", sys.argv[1], sys.argv[2])
ini_path = "/home/mingxuan/sparseNewton/src/AIM-Simulator/ini/AIM.ini"
system_path = "/home/mingxuan/sparseNewton/src/AIM-Simulator/system.ini.example"
system = dram_system(ini_path, system_path)
print ("bank, row, col", system.NUM_BANKS, system.NUM_ROWS, system.NUM_COLS)
print(system.JEDEC_DATA_BUS_BITS) 
system.read_commands()
# if (int(sys.argv[4]) == 0):
#     system.multM_N_col_major (10, int(sys.argv[1]), int(sys.argv[2]) * int(sys.argv[3]))
# elif (int(sys.argv[4]) == 1):
#     system.multM_N_row_major(10, int(sys.argv[1]), int(sys.argv[2]) * int(sys.argv[3]))
# else:      
#     system.read_all_out(int(sys.argv[1]), int(sys.argv[2]))
system.fw.close()
