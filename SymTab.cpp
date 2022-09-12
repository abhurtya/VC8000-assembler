//
//		Implementation of the symbol table class.  This is the format I want for commenting functions.
//
#include "stdafx.h"
#include "SymTab.h"

/**/
/*
* SymbolTable::AddSymbol()
NAME
    AddSymbol - adds a new symbol to the symbol table.
SYNOPSIS
    void AddSymbol( const string &a_symbol, int a_loc );
        a_symbol	-> The name of the symbol to be added to the symbol table.
        a_loc		-> the location to be associated with the symbol.

DESCRIPTION
    This function will place the symbol "a_symbol" and its location "a_loc"
    in the symbol table.
*/
/**/
void SymbolTable::AddSymbol(const string& a_symbol, int a_loc)
{
    // If the symbol is already in the symbol table, record it as multiply defined.
    map<string, int>::iterator st = m_symbolTable.find(a_symbol);
    if (st != m_symbolTable.end()) {

        st->second = multiplyDefinedSymbol;
        return;
    }
    // Record a the  location in the symbol table.
    m_symbolTable[a_symbol] = a_loc;
}
/*void SymbolTable::AddSymbol(const string& a_symbol, int a_loc)*/



/**/

/*
SymbolTable::DisplaySymbolTable()
NAME
    SymbolTable::DisplaySymbolTable - displays symbol table.
SYNOPSIS
    void SymbolTable::DisplaySymbolTable();
DESCRIPTION

    This function will output the symbol and location 
*/
/**/
void SymbolTable::DisplaySymbolTable() {

    cout << "Output from test program:" << endl << endl;
    cout << "Symbol Table:" << endl << endl;
    cout << left << setw(15) << "Symbol#" << left << setw(15) << "Symbol" << left << setw(15) << "Location" << endl;

    int sym_num = 0;

    // Display the symbols and their location
    map<string, int>::iterator itr;     //iterator for the symbol table map
    for ( itr = m_symbolTable.begin(); itr != m_symbolTable.end(); ++itr) {
        cout << left << setw(15) << sym_num << left << setw(15) << itr->first << left << setw(15) << itr->second << endl;
        sym_num++;
    }
    cout << endl;
    cout << "______________________________________________________________" << endl << endl;
}
/*void SymbolTable::DisplaySymbolTable()*/

/**/
/*
SymbolTable::LookupSymbol()
NAME
    SymbolTable::LookupSymbol - Checks if the symbol exists in the symbol table.
SYNOPSIS
    bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc);
        a_symbol  ->  The Symbol to look for
        a_loc     ->  If the symbol is found places the symbol's location in a_loc
DESCRIPTION
    Checks if the symbol a_symbol exists in the symbol table. If the symbol exists
    the value of a_loc is changed to the location of the symbol.
RETURNS
    Returns true if the symbol is found. Returns false otherwise.

*/
/**/
bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc) {

    for (auto itr = m_symbolTable.begin(); itr != m_symbolTable.end(); ++itr) {
        if (itr->first == a_symbol) {
            a_loc = m_symbolTable[a_symbol];
            return true;
        }
    }

    return false;
}
/*bool SymbolTable::LookupSymbol(const string& a_symbol, int& a_loc)*/
