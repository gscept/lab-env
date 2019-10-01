//------------------------------------------------------------------------------
// app.cc
// (C) 2015-2018 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "config.h"
#include "app.h"

namespace Core
{
	App::App() : isOpen(false) { }
	
	App::~App() { }

	bool App::Open()
	{
		assert(!this->isOpen);
		this->isOpen = true;
		return true;
	}
	
	void App::Run() { }
	
	void App::Close()
	{
		assert(this->isOpen);
		this->isOpen = false;
	}
	
	void App::Exit() { }

}