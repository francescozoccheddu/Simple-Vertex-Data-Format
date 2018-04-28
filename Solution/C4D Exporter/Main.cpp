#include "c4d.h"

Bool PluginStart (void)
{
	MessageDialog ("Ciaooo");
	return TRUE;
}

void PluginEnd (void)
{}

Bool PluginMessage (int id, void *data)
{
	return TRUE;
}