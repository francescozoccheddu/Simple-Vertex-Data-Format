#include "ProviderTag.hpp"

#include "c4d.h"
#include "c4d_gui.h"
#include "c4d_tagdata.h"
#include "c4d_symbols.h"
#include "SVDF/Declaration.hpp"

const Int32 ProviderTag::id{ 900000110 };

Bool ProviderTag::Init (GeListNode * node)
{
	return true;
}

EXECUTIONRESULT ProviderTag::Execute (BaseTag * tag, BaseDocument * doc, BaseObject * op, BaseThread * bt, Int32 priority, EXECUTIONFLAGS flags)
{
	return EXECUTIONRESULT_OK;
}

Bool ProviderTag::GetDDescription (GeListNode * node, Description * description, DESCFLAGS_DESC & flags)
{
	if (!node || !description)
		return false;

	if (!description->LoadDescription (node->GetType ()))
		return false;

	flags |= DESCFLAGS_DESC_LOADED;

	return SUPER::GetDDescription (node, description, flags);
}

SVDF::DataDeclaration<float> ProviderTag::ProvideForExport () const
{
	throw this;
}

NodeData * ProviderTag::Alloc ()
{
	return NewObj (ProviderTag);
}

Bool ProviderTag::RegisterPlugin ()
{
	return RegisterTagPlugin (id, GeLoadString (IDS_TAG_TITLE), TAG_VISIBLE, ProviderTag::Alloc, "Tprovider", AutoBitmap("icon.tif"), 0);
}
