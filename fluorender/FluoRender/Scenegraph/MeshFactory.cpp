/*
For more information, please see: http://software.sci.utah.edu

The MIT License

Copyright (c) 2018 Scientific Computing and Imaging Institute,
University of Utah.


Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE.
*/

#include <Scenegraph/MeshFactory.h>

using namespace FL;

MeshFactory::MeshFactory()
{
	m_name = "mesh factory";
	default_object_name_ = "default mesh";
}

MeshFactory::~MeshFactory()
{

}

void MeshFactory::createDefault()
{
	if (!getDefault())
	{
		MeshData* md = new MeshData();
		md->setName(default_object_name_);
		objects_.push_back(md);

		//add default values here
		md->addValue("data path", std::wstring());//path to obj file
		md->addValue("bounds", FLTYPE::BBox());//bounding box
		md->addValue("center", FLTYPE::Point());

		md->addValue("display", bool(true));
		md->addValue("draw bounds", bool(false));

		//lighting
		md->addValue("light enable", bool(true));
		md->addValue("depth atten", bool(false));
		md->addValue("mat amb", FLTYPE::Color());
		md->addValue("mat diff", FLTYPE::Color());
		md->addValue("mat spec", FLTYPE::Color());
		md->addValue("mat shine", double(0));
		md->addValue("alpha", double(1));

		//shadow
		md->addValue("shadow enable", bool(false));
		md->addValue("shadoe int", double(1));

		//size limiter
		md->addValue("limit enable", bool(false));
		md->addValue("limit", long(0));

		//transforms
		md->addValue("trans x", double(0));
		md->addValue("trans y", double(0));
		md->addValue("trans z", double(0));
		md->addValue("rot x", double(0));
		md->addValue("rot y", double(0));
		md->addValue("rot z", double(0));
		md->addValue("scale x", double(1));
		md->addValue("scale y", double(1));
		md->addValue("scale z", double(1));

		//legend
		md->addValue("legend", bool(true));

		//viewport
		md->addValue("viewport", FLTYPE::GLint4());
	}
}

MeshData* MeshFactory::build(const std::string &exp)
{
	unsigned int default_id = 0;
	return clone(default_id);
}

MeshData* MeshFactory::clone(MeshData* md)
{
	incCounter();

	Object* new_md = md->clone(CopyOp::DEEP_COPY_ALL);
	new_md->setId(global_id_);
	std::string name = "mesh" + std::to_string(local_id_);
	new_md->setName(name);

	objects_.push_back(new_md);

	return dynamic_cast<MeshData*>(new_md);
}

MeshData* MeshFactory::clone(const unsigned int id)
{
	Object* object = find(id);
	if (object)
	{
		MeshData* md = dynamic_cast<MeshData*>(object);
		if (md)
			return clone(md);
	}
	return 0;
}
