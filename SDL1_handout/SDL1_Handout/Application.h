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
		bool ret = true;
		for(int i = 0; i < NUM_MODULES; ++i)
		{
			if(modules[i]->Init() == false)
			{
				ret = false;
			}
		}
		return ret;
	}

	// UPDATE all modules
	update_status Update()
	{
		update_status ret = UPDATE_CONTINUE;

		for (int i = 0; i < NUM_MODULES; i++)
		{
			if (modules[i]->PreUpdate() == UPDATE_STOP)
			{
				ret = UPDATE_STOP;
			}
		}
		for(int i = 0; i < NUM_MODULES; i++)
		{
			if (modules[i]->Update() == UPDATE_STOP)
			{
				ret = UPDATE_STOP;
			}
		}
		for (int i = 0; i < NUM_MODULES; i++)
		{
			if (modules[i]->PostUpdate() == UPDATE_STOP)
			{
				ret = UPDATE_STOP;
			}
		}

		return ret;
	}

	// EXIT Update 
	bool CleanUp()
	{
		bool ret = true;

		for(int i = NUM_MODULES; i > 0 && ret == true; i--)
		{
			if (modules[i]->CleanUp() == false)
			{
				ret = false;
			}
		}
		return ret;
	}

};

#endif // __APPLICATION_H__