#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "Globals.h"
#include "Module.h"
#include "Dummy.h"
#include "DummyESC.h"

#define NUM_MODULES 2

class Application
{
public:

	Module* modules[NUM_MODULES];

public:

	Application()
	{
		modules[0] = new ModuleDummy();
		modules[1] = new ModuleDummyESC();
	}

	~Application()
	{
		delete[]modules;
	}

	// INIT all modules
	bool Init() 
	{
		for(int i = 0; i < NUM_MODULES; ++i)
		{
			if(modules[i]->Init() == false)
			{
				return false;
			}
		}
		return true;
	}

	// UPDATE all modules
	update_status Update()
	{
		for (int i = 0; i < NUM_MODULES; i++)
		{
			if (modules[i]->PreUpdate() == UPDATE_STOP)
			{
				return UPDATE_STOP;
			}
		}
		for(int i = 0; i < NUM_MODULES; i++)
		{
			if (modules[i]->Update() == UPDATE_STOP)
			{
				return UPDATE_STOP;
			}
		}
		for (int i = 0; i < NUM_MODULES; i++)
		{
			if (modules[i]->PostUpdate() == UPDATE_STOP)
			{
				return UPDATE_STOP;
			}
		}
		return UPDATE_CONTINUE;
	}

	// EXIT Update 
	bool CleanUp()
	{
		for(int i = NUM_MODULES; i > 0; i--)
		{
			if (modules[i]->CleanUp() == false)
			{
				return false;
			}
		}
		return true;
	}

};

#endif // __APPLICATION_H__