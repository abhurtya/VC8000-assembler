//
//		Emulator class - supports the emulation of VC8000 programs
//
#ifndef _EMULATOR_H      // UNIX way of preventing multiple inclusions.
#define _EMULATOR_H

class emulator {

public:

    const static int MEMSZ = 1'000'000;	// The size of the memory of the VC8000.
    emulator() {

        m_memory.resize(MEMSZ, 0);
        memset(m_reg, 0, 10 * sizeof(int));
    }
    // Records instructions and data into simulated memory.
    bool insertMemory(int a_location, long long a_contents);

    // Runs the program recorded in memory.
    bool runProgram();

private:

    vector<long long> m_memory;  	// Memory for the VC8000
    long long m_reg[10];			        // The accumulator for the Quack8000
    
};

#endif

