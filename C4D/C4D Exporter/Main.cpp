#include "c4d.h"
#include "ExportCommand.hpp"

Bool PluginStart (void)
{
	return ExportCommand::registerPlugin ();
}

void PluginEnd (void)
{}

Bool PluginMessage (int id, void *data)
{
	switch (id)
	{

		case C4DPL_INIT_SYS:
			return resource.Init ();

		default:
			return false;
				
	}
}