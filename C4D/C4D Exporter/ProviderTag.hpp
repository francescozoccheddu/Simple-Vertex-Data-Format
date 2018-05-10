#ifndef PROVIDER_TAG_H__
#define PROVIDER_TAG_H__

#include "c4d.h"
#include "c4d_tagdata.h"
#include "SVDF/Declaration.hpp"

class ProviderTag : public TagData
{
	INSTANCEOF (ProviderTag, TagData);

public:

	static const Int32 id;

	Bool Init (GeListNode* node) override;

	EXECUTIONRESULT Execute (BaseTag* tag, BaseDocument* doc, BaseObject* op, BaseThread* bt, Int32 priority, EXECUTIONFLAGS flags) override;

	Bool GetDDescription (GeListNode* node, Description* description, DESCFLAGS_DESC& flags) override;

	SVDF::DataDeclaration<float> ProvideForExport () const;

	static NodeData * Alloc ();

	static Bool RegisterPlugin ();

};

#endif