//
//      Implementation of the Assembler class.
//
#include "stdafx.h"
#include "Assembler.h"
#include "Errors.h"
#include <iostream>
#include <iomanip>

/**/
/*
Assembler::Assembler()
NAME
    Assembler::Assembler - the constructor of the assembler class.
SYNOPSIS
    Assembler::Assembler(int argc, char *argv[]);
        argc	->	 Total number of arguments in the command line
        argv	->	 User input array of strings
DESCRIPTION
    It is the constructor for the assembler class.
RETURN
    No return value
/**/
Assembler::Assembler(int argc, char* argv[])
    : m_facc(argc, argv)
{
    // Nothing else to do here at this point.
}
/*Assembler::Assembler(int argc, char *argv[]);*/


// Destructor currently does nothing.  You might need to add something as you develope this project.  If not, we can delete it.
Assembler::~Assembler()
{
}

/**/
/*
Assembler::PassI()
NAME
    Assembler::PassI() - Populates the symbol.
SYNOPSIS
    void Assembler::PassI();
DESCRIPTION
    This function establishes the location of the labels. It stores the location for all the labels required for
    the assembler
RETURNS
    VOID
*/
/**/

void Assembler::PassI()
{
    int loc = 0;        // Tracks the location of the instructions to be generated.

    // Successively process each line of source code.
    for (; ; ) {

        // Read the next line from the source file.
        string line;
        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            // We will let this error be reported by Pass II.
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // If this is an end statement, there is nothing left to do in pass I.
        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End) return;

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if (st == Instruction::InstructionType::ST_Comment)
        {
            continue;
        }

        // If the instruction has a label, record it and its location in the
        // symbol table.
        if (m_inst.isLabel()) {

            m_symtab.AddSymbol(m_inst.GetLabel(), loc);
        }

        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);
    }
}
/*Assembler::PassI();*/

/**/
/*
Assembler::PassII()
NAME
    Assembler::PassII - Does the translation for the assembler code 
SYNOPSIS
    Assembler::PassII();
DESCRIPTION
   This function translates the code from the text file provided by the user to machine language by 
	computing the content of the machine code.
RETURNS
    void

*/
/**/

void Assembler::PassII() {

    cout << "Press ENTER to continue...";
    cin.ignore();

    cout << "Translation of Program:" << endl << endl;
    cout << left << setw(15) << "Location" << left << setw(15) << "Contents" << left << setw(15) << "Original Statement" << endl;
    int loc = 0;

    m_facc.rewind();
    string line;

    string contents = "";
    long long num_contents = 0;

    Errors::InitErrorReporting();

    for (; ; ) {

        // Read the next line from the source file.
        string line;

        if (!m_facc.GetNextLine(line)) {

            // If there are no more lines, we are missing an end statement.
            Errors::RecordError("ERROR: No end statement");
            Errors::DisplayErrors();
            return;
        }
        // Parse the line and get the instruction type.
        Instruction::InstructionType st = m_inst.ParseInstruction(line);

        // Pass II will determine if the end is the last statement and report an error if it isn't.
        if (st == Instruction::InstructionType::ST_End) {
            cout << "\t\t\t\t" << line << endl;

            while (m_facc.GetNextLine(line)) { 
                if (!line.empty()) {
                    Errors::RecordError("ERROR::End is not the last statement!");
                    Errors::DisplayErrors();
                }
            }
            return;
        }

        // Labels can only be on machine language and assembler language
        // instructions.  So, skip comments.
        if (st == Instruction::InstructionType::ST_Comment){
            cout <<"\t\t\t" << line << endl;
            continue;
        }

        ///there can be label on a machine or assembler instruction. So, ensure that the labels are valid
        if (m_inst.isLabel()) {
            if (!m_inst.isValidLabel()) {
                Errors::RecordError("ERROR: " + m_inst.GetLabel() + " is invalid label!");
            }
        }

        // Handle the case where there is an error.
        if (st == Instruction::InstructionType::ST_MachineLanguage) {

            contents = m_inst.GetMachineContents(m_symtab);
        }
        else if (st == Instruction::InstructionType::ST_AssemblerInstr) {
            contents = m_inst.GetAssemblyContents(m_symtab);
        }

        //pass the contents to the emulator
        if (!contents.empty()) {
            num_contents = stoll(contents);
            m_emul.insertMemory(loc, num_contents);
        }

        cout << left << setw(15) <<  loc<<  left << setw(15) << contents<<  left << setw(15) << line << endl;
        // Compute the location of the next instruction.
        loc = m_inst.LocationNextInstruction(loc);

        if (loc < 0 || loc >999'999)
        {
            Errors::RecordError("ERROR: Insufficient Memory location !");
            Errors::DisplayErrors();
            return;
        }

        Errors::DisplayErrors();  
    }

    RunProgramInEmulator();

    cout << endl;
}
/*  void Assembler::PassII()*/


/**/
/*
Assembler::RunProgramInEmulator()
NAME
    Assembler::RunProgramInEmulator() - Checks if there are any errors and runs the emulator is none are found.
SYNOPSIS
    void Assembler::RunProgramInEmulator()
DESCRIPTION
    This function checks for errors and runs the emulator if there no errors are found.
RETURNS
    VOID
*/
/**/

void Assembler::RunProgramInEmulator() {

    cout << "Press Enter to continue...\n";
    cin.ignore();

    

    if (!Errors::NoErrors()){
        cout << "Emulator couldn't start due to Errors! " << endl;

    }else {

        cout << "Results from Emulating Program" << endl << endl;
        m_emul.runProgram();
    }

    cout << endl;
    cout << "END OF EMULATION! " << endl;
}
/* void Assembler::RunProgramInEmulator() */