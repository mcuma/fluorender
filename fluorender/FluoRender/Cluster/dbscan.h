/*
For more information, please see: http://software.sci.utah.edu

The MIT License

Copyright (c) 2014 Scientific Computing and Imaging Institute,
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
#ifndef FL_Dbscan_h
#define FL_Dbscan_h

#include "ClusterMethod.h"

namespace FL
{
	class ClusterDbscan : public ClusterMethod
	{
	public:
		ClusterDbscan();
		~ClusterDbscan();

		void SetSize(unsigned int size)
		{ m_size = size; }
		void SetEps(float eps)
		{ m_eps = eps; }
		bool Execute();
		float GetProb();

	private:
		unsigned int m_size;
		float m_eps;
		float m_intw;

	private:
		void Dbscan();
		void ResetData();
		void ExpandCluster(pClusterPoint& p, Cluster& neighbors, Cluster& cluster);
		Cluster GetNeighbors(pClusterPoint &p, float eps, float intw);
		void RemoveNoise();
		bool ClusterNoise(pClusterPoint& p, Cluster& neighbors);
	};

}
#endif//FL_Dbscan_h