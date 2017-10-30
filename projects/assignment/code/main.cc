//------------------------------------------------------------------------------
// main.cc
// (C) 2015-2017 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "assignmentapp.h"

int
main(int argc, const char** argv)
{
	Assignment::AssignmentApp & app = Assignment::AssignmentApp::GetInstance();
	if (app.Open())
	{
		app.Run();
		app.Close();
	}
	app.Exit();
	
}