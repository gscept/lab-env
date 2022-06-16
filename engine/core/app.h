#pragma once
//------------------------------------------------------------------------------
/**
	Base class for applications, override this class
	and use it as an entry point for your lab.
	
	(C) 2015-2020 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include <GL/glew.h>
namespace Core
{
class App
{
public:
	/// constructor
	App();
	/// destructor
	~App();

	/// open application
	virtual bool Open();
	/// perform per-frame loop
	virtual void Run();
	/// stop application
	virtual void Close();
	/// kill application
	virtual void Exit();
	/// set the exit code
	void SetExitCode(int exitCode);
	/// get the current exit code
	int ExitCode();

private:
	bool isOpen;
	int exitCode;
};
} // namespace App