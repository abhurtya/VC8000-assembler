#include "stdafx.h"
#include "Errors.h"

vector<string> Errors::m_ErrorMsgs;

/**/
/*
Errors::DisplayErrors()
NAME
	void Errors::DisplayErrors() - Prints out the error
SYNOPSIS
	void Errors::DisplayErrors()
DESCRIPTION
	This function prints out the latest error among all existing errors
RETURNS
	VOID
*/
/**/

void Errors::DisplayErrors() {

	if (latesterror) {
		cout << m_ErrorMsgs.back()  << endl;
	}
	else return;
	latesterror = false;
}
/*void Errors::DisplayErrors()*/


/**/
/*
Errors::InitErrorReporting()
NAME
	Errors::InitErrorReporting - initializes the error reporting vector 
SYNOPSIS
	void Errors::InitErrorReporting();
DESCRIPTION
	empties the vector storing error messages and sets latest error to false
Returns
	void

*/
/**/
void Errors::InitErrorReporting() {
	m_ErrorMsgs.clear();
	latesterror = false;
}
/*void Errors::InitErrorReporting()*/


/**/
/*
Errors::RecordError()
NAME
	Errors::RecordError - Records the error message.
SYNOPSIS
	Errors::RecordError(string a_emsg);
	a_emsg -> The string that contains the error message.
DESCRIPTION
	Stores the error message a_emsg to the end of the vector
	with a list of error messages.
	sets latest error to true
Returns
	void

*/
/**/
void  Errors::RecordError(string a_emsg) {
	m_ErrorMsgs.push_back(a_emsg);
	latesterror = true;
}
/*void  Errors::RecordError(string a_emsg) */


/**/
/*
Errors::NoErrors()
NAME
	NoErrors - tells if the errors were encountered.
SYNOPSIS
	bool  Errors::NoErrors()
DESCRIPTION
	This function checks if the error has been pushed at least once.
RETURNS

	It returns a boolean value true if the vector is empty, false otherwise.
*/
/**/
bool Errors::NoErrors() {
	return m_ErrorMsgs.empty();
}
/*bool Errors::NoErrors()*/