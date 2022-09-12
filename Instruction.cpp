#include "stdafx.h"
#include "Errors.h"

#include "Instruction.h"

/**/
/*
 Instruction::InstructionType Instruction::ParseInstruction()
 NAME
    ParseInstruction - Find out the instruction type

 SYNOPSIS
    Instruction::InstructionType Instruction::ParseInstruction(string a_line);
    a_line --> The original statement that needs to be parsed

 DESCRIPTION
    This function checks if the opCode is in machine language, assembly language, a comment
        or an end instruction.
 RETURN
    The type of instruction (ST_MachineLanguage, ST_AssemblerInstr, ST_Comment, ST_End, or ST_Error)
*/
/**/
Instruction::InstructionType Instruction::ParseInstruction(string a_line)
{
    // Record the original statement.  This will be needed in the second pass.
    m_instruction = a_line;

    // Delete any comment from the line.
    DeleteComment(a_line);

    // Record label, opcode, and operands. 
    // returns if there is extra operands or not
    bool isFormatError = RecordFields(a_line);

    //throw error if extra operand
    if (isFormatError) {
        Errors::RecordError("ERROR: Extra operand found! ");
    }

    // Check if this is a comment.
    if (m_Label.empty() && m_OpCode.empty())
    {
        return InstructionType::ST_Comment;
    }

    // Return the instruction type.
    return m_type;
}
/*Instruction::InstructionType Instruction::ParseInstruction(string a_line);*/




/**/
/*
Instruction::RecordFields()
NAME
    Instruction::RecordFields - Records the kind of instruction by extracting fields from instruction.
SYNOPSIS
    bool Instruction::RecordFields( const string &a_line );
    a_line ->  instruction line to be parsed.
DESCRIPTION
    The function calls another function that parses the provided line into a label,
    an operation code, and the operands of instruction.
    The Instruction Type and the numeric Op Code are then determined. 
    If there are more operands specified, it also logs an error.

RETURN
     Returns true if parsing the line into fields(label, opcode, operand1, operand2)
     was successful and false otherwise
*/
/**/
bool Instruction::RecordFields(const string& a_line)
{
    // Get the fields that make up the instruction.
    bool isFormatError = !ParseLineIntoFields(a_line, m_Label, m_OpCode, m_Operand1, m_Operand2);

    // if code was a comment, there is nothing to do.
    if (m_OpCode.empty() && m_Label.empty()) return isFormatError;

    // Record whether the operands are numeric and their value if they are.
    m_IsNumericOperand1 = isStrNumber(m_Operand1);
    if (m_IsNumericOperand1) m_Operand1NumericValue = stoi(m_Operand1);

    m_IsNumericOperand2 = isStrNumber(m_Operand2);
    if (m_IsNumericOperand2) m_Operand2NumericValue = stoi(m_Operand2);

    // For the sake of comparing, convert the op code to upper case.
    for (char& c : m_OpCode)
    {
        c = toupper(c);
    }
    
    // - Determine and record the instruction type from the op code.
    // - Recording the numeric Op code for machine lanuage equivalents.

    if (m_Label.empty() && m_OpCode.empty()){
        m_type = InstructionType::ST_Comment;
    }
    else if (m_OpCode == "DC" || m_OpCode == "DS" || m_OpCode == "ORG") {
        m_type = InstructionType::ST_AssemblerInstr;
    }
    else if (m_OpCode == "END") {
        m_type = InstructionType::ST_End;
    }
    else {

        //check if the instruction is found in the array of machine opcodes
        //the opcode will be the index of array + 1
        bool found = false;

        for (int i = 0; i < m_MachineOpCode.size(); i++) {
            if (m_MachineOpCode[i] == m_OpCode) {
                found = true;
                m_NumOpCode = i + 1;
                m_type = InstructionType::ST_MachineLanguage;
            }
        }

        //if we have arrived at this point means the instruction is neither of the assembly, machine instrution,
        //nor comment or end statement. So, it's an error 
        if (found == false) {
            m_type = InstructionType::ST_Error;
            Errors::RecordError("ERROR: " + m_OpCode + " is invalid Opcode!");
        }
    }

    return isFormatError;
}
/*bool Instruction::RecordFields( const string &a_line );*/


/**/
/*
Instruction::ParseLineIntoFields()
NAME
    Instruction::ParseLineIntoFields - Divides the instruction line into parts or fields.
SYNOPSIS
    bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2);
    a_line -> a string that consists of a line in the file to be parsed.
    a_label -> a string that holds the parsed label
    a_OpCode -> a string that holds the parsed OpCode
    a_Operand1 -> a string that holds the parsed operand1
    a_Operand2 -> a string that holds the parsed operand2
DESCRIPTION
    From the given line, the function extracts the label, 
    operation code, and operands of instruction.

RETURN
    Returns true if the line had no extra operands,otherwise returns false
*/
/**/

bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2)
{
    // Get rid of any commas from the line.
    replace(a_line.begin(), a_line.end(), ',', ' ');

    // Get the elements of the line.  That is the label, op code, operand1, and operand2.
    string endStr;
    a_label = a_OpCode = a_Operand1 = a_Operand2 = "";
    istringstream ins(a_line);
    if (a_line[0] == ' ' || a_line[0] == '\t')
    {
        a_label = "";
        ins >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    else
    {
        ins >> a_label >> a_OpCode >> a_Operand1 >> a_Operand2 >> endStr;
    }
    // If there is extra data, return false.
    return endStr.empty() ? true : false;
}
/*bool Instruction::ParseLineIntoFields(string a_line, string& a_label, string& a_OpCode,
    string& a_Operand1, string& a_Operand2);*/



/**/
/*
Instruction::isStrNumber()
NAME
     Instruction::isStrNumber - Checks if a string is a number.
SYNOPSIS
    bool Instruction::isStrNumber(const string& a_str);
    a_str -> a string that is to be checked

DESCRIPTION
    The function determines whether the string is a number or not

Returns
    Returns true if a_str is numberic, otherwise returns false.
*/
/**/

bool Instruction::isStrNumber(const string& a_str)
{
    if (a_str.empty()) return false;

    // If there is a - or a plus, make sure there are more characters.
    int ichar = 0;
    if (a_str[0] == '-' || a_str[0] == '+')
    {
        ichar++;
        if (a_str.length() < 2) return false;
    }
    // Make sure that the remaining characters are all digits
    for (; ichar < a_str.length(); ichar++)
    {
        if (!isdigit(a_str[ichar])) return false;
    }
    return true;
}
/*    bool Instruction::isStrNumber(const string& a_str);*/


/**/
/*
Instruction::LocationNextInstruction()
NAME
    LocationNextInstruction - Returns the location of next instruction in the program code.
SYNOPSIS
    int LocationNextInstruction(int a_loc);
    a_loc -> the location of the current instruction.
DESCRIPTION
    This function will calculates the location "a_loc" for next instruction in the program.
    It identifies opcode and determines location accordingly.
RETURN
    This function returns integer location for next instruction.
*/
/**/

int Instruction::LocationNextInstruction(int a_loc) {
 
    // For origin and storage declaration add current location and defined storage.
    if (m_OpCode == "DS" || m_OpCode == "ORG") {

        if (m_IsNumericOperand1)
        return a_loc + m_Operand1NumericValue;
    }
    // For other instructions add one to the location. 
    return a_loc + 1;
}
/*int Instruction::LocationNextInstruction(int a_loc) */


/**/
/*
Instruction::GetMachineContents()
NAME
     Instruction::GetMachineContents - Generates the machine translation for an machine instruction.
SYNOPSIS
    string Instruction::GetAssemblyContents(SymbolTable&  m_symtab);
    m_symtab ->  symbol table object that holds labels and their location

DESCRIPTION
    This function converts one line of source code at a time into machine code and returns it as string.
    Machine code is formed by combining the values of Register, Opcode, and Operand.
    It also handles a variety of errorsin machine instructions.

Returns
    string that is the machine translation of the instuction
*/
/**/

string Instruction::GetMachineContents(SymbolTable& m_symtab) {
    string machine_code = "";
    string opcode = "";
    string register1 = "9";
    string register2 = "9";
    string address = "";

    opcode = to_string(m_NumOpCode);
    opcode = opcode.insert(0, 2 - opcode.length(), '0');

    int temp_loc = 0;

    //all operands must have operand1 except halt
    if (m_Operand1.empty() && m_OpCode != "HALT") {
        Errors::RecordError("ERROR: Missing operand for: " + m_OpCode + "!");
        return opcode + PrintWhatsString(7);
    }

    //halt instruction does not have operands
    if ( m_OpCode == "HALT") {
        if (!m_Operand1.empty())    Errors::RecordError("ERROR: Extra operand found for: " + m_OpCode + "!"); ;
        return opcode + register1 + "000000";
    }

    //opcodes dealing with two registers
    if (m_NumOpCode >= 7 && m_NumOpCode <= 10) {

        //check if the operands are not numeric
        if (!m_IsNumericOperand1 && !m_IsNumericOperand2) {
            Errors::RecordError("ERROR: Both Operands should be numeric !");
            return opcode + "?" + "?" + "00000";
        }

        if (!m_IsNumericOperand1) {
            Errors::RecordError("ERROR: Operand1 should be numeric !");
            return opcode + "?" + m_Operand2 + "00000";
        }

        if (!m_IsNumericOperand2) {
            Errors::RecordError("ERROR: Operand2 should be numeric !");
            return opcode + m_Operand1 + "?" + "00000";
        }

        // check if the register value != 0-9. Replace with '?' if not
        register1 = (m_Operand1NumericValue < 0 || m_Operand1NumericValue > 9) ? "?" : m_Operand1;
        register2 = (m_Operand1NumericValue < 0 || m_Operand1NumericValue > 9) ? "?" : m_Operand2;
        if (register1 == "?" || register2 == "?")   Errors::RecordError("ERROR: Register value must be integer [0,9] !");

        return opcode + register1 + register2 + "00000";
    }

    //for opcodes 1 to 10 operand  1 must numeric
    if (m_NumOpCode >= 1 && m_NumOpCode <= 10) {
        if (!m_IsNumericOperand1) {
            Errors::RecordError("ERROR: Operand 1 must be numeric for Opcode: " + m_OpCode + "!");
        }
    }


    //Now that we have dealt with registers requiring 2 numerical operands, we need to deal with the other ones
    //for numerical operands 1, second operand should be symbolic
    //if the operand2 is missing then the operand1 should be symbolic
    if (m_IsNumericOperand1) {
         
        //if location is not found-> error
        if (!m_symtab.LookupSymbol(m_Operand2, temp_loc)) {
            Errors::RecordError("ERROR: Use of undefined symbol: " + m_Operand2 + "!");
            return opcode + m_Operand1 + PrintWhatsString(6);
        }

        //  label was previously defined if the value becomes -999(multiplyDefinedSymbol).
        if (temp_loc == m_symtab.multiplyDefinedSymbol) {
            Errors::RecordError("ERROR: Use of multiply defined Symbol: " + m_Label + "!");
            return opcode + m_Operand1 + PrintWhatsString(6);
        }

        // check if the register value != 0-9. Replace with '?' if not
        register1 = (m_Operand1NumericValue < 0 || m_Operand1NumericValue > 9) ? "?" : m_Operand1;
        if (register1 == "?")   Errors::RecordError("ERROR: Register value must be integer [0,9] !");

        address = to_string(temp_loc);
        address = address.insert(0, 6 - address.length(), '0');
        machine_code = opcode + register1 + address;
    }
    else {

        if (!m_Operand2.empty())
            Errors::RecordError("ERROR: Operand 2 must be empty for symbolic Operand 1!");

        if (!m_symtab.LookupSymbol(m_Operand1, temp_loc)) {
            Errors::RecordError("ERROR: Use of undefined Symbol: " + m_Operand1 + "!");
            return opcode + "9" + PrintWhatsString(6);
        }

        if (temp_loc == m_symtab.multiplyDefinedSymbol) {
            Errors::RecordError("ERROR: Use of multiply defined Symbol: " + m_Operand1 + "!");
            return opcode + "9" + PrintWhatsString(6);
        }

        address = to_string(temp_loc);
        address = address.insert(0, 6 - address.length(), '0');
        machine_code = to_string(m_NumOpCode) + "9" + address;
    }
 
    return machine_code;
};
/*string Instruction::GetMachineContents(SymbolTable& m_symtab)*/


/**/
/*
Instruction::GetAssemblyContents()
NAME
     Instruction::GetAssemblyContents - Generates the machine translation for an assembly instruction.
SYNOPSIS
    string Instruction::GetAssemblyContents(SymbolTable&  m_symtab);
    m_symtab ->  symbol table object that holds labels and their location

DESCRIPTION
    This function converts one line of source code at a time into machine code and returns it as string.
    Machine code is formed by combining the numerical values of Register, Opcode, and Operand. 
    It also handles a variety of error types in assembly instructions.

Returns
    string that is the machine translation of the instuction
*/
/**/

string Instruction::GetAssemblyContents(SymbolTable&  m_symtab) {
    string machine_code = "";

    //only operand1 must be present and be numeric
    if (m_Operand1.empty()) {
        Errors::RecordError("ERROR: operand1 missing for " + m_OpCode +"!");
    }
    else if (!m_IsNumericOperand1) {
        Errors::RecordError("ERROR: operand1 non-numeric  " + m_OpCode + "!");
    }

    if (!m_Operand2.empty()) {
        Errors::RecordError("ERROR: Extra operand2 present  " + m_OpCode + "!");
    }

    //org should not have label
    if (m_OpCode == "ORG") {
        if (!m_Label.empty()) {
            Errors::RecordError("ERROR: Label present for ORG!"); 
        }
    }
    //if opcode is not org, label must be present for DC || DS
    else if (m_Label.empty()) {
        Errors::RecordError("ERROR: Label absent for " + m_OpCode + "!");
    }

    //Only DC in Instruction Type will have machine translations, 
    //for others assembly instructions content will be empty

    if (m_OpCode == "DC") {

        machine_code = m_Operand1;

        if (m_Operand1NumericValue > 10'000) {
            Errors::RecordError("ERROR: Constant too large for DC (max size=10,000)!");
            machine_code = PrintWhatsString(5);
        }

        machine_code = machine_code.insert(0, 9 - machine_code.length(), '0');
    }

    if (m_OpCode == "DS") {

        if (m_Operand1NumericValue > 10'000) {
            Errors::RecordError("ERROR: DS operand too large (max size=10,000)!");
        }
    }
    
    return machine_code;
};
/*
string Instruction::GetAssemblyContents(SymbolTable&  m_symtab)*/
