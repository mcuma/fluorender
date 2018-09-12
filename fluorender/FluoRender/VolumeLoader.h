//  
//  For more information, please see: http://software.sci.utah.edu
//  
//  The MIT License
//  
//  Copyright (c) 2018 Scientific Computing and Imaging Institute,
//  University of Utah.
//  
//  
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//  
//  The above copyright notice and this permission notice shall be included
//  in all copies or substantial portions of the Software.
//  
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//  

#ifndef _VOLUMELOADER_H_
#define _VOLUMELOADER_H_

#include <FLIVR/TextureBrick.h>
#include <Scenegraph/VolumeData.h>
#include <wx/thread.h>
#include <vector>
#include <unordered_map>

class VolumeLoader;

struct VolumeLoaderData
{
	FLIVR::FileLocInfo *finfo;
	FLIVR::TextureBrick *brick;
	FL::VolumeData *vd;
	unsigned long long datasize;
	int mode;
};

struct VolumeDecompressorData
{
	char *in_data;
	size_t in_size;
	FLIVR::TextureBrick *b;
	FLIVR::FileLocInfo *finfo;
	FL::VolumeData *vd;
	unsigned long long datasize;
	int mode;
};

class VolumeDecompressorThread : public wxThread
{
public:
	VolumeDecompressorThread(VolumeLoader *vl);
	~VolumeDecompressorThread();
protected:
	virtual ExitCode Entry();
	VolumeLoader* m_vl;
};

class VolumeLoaderThread : public wxThread
{
public:
	VolumeLoaderThread(VolumeLoader *vl);
	~VolumeLoaderThread();
protected:
	virtual ExitCode Entry();
	VolumeLoader* m_vl;
};

class VolumeLoader
{
public:
	VolumeLoader();
	~VolumeLoader();
	void Queue(VolumeLoaderData brick);
	void ClearQueues();
	void Set(std::vector<VolumeLoaderData> vld);
	void Abort();
	void StopAll();
	bool Run();
	void SetMaxThreadNum(int num) { m_max_decomp_th = num; }
	void SetMemoryLimitByte(long long limit) { m_memory_limit = limit; }
	void CleanupLoadedBrick();
	void RemoveAllLoadedBrick();
	void RemoveBrickVD(FL::VolumeData *vd);
	void GetPalams(long long &used_mem, int &running_decomp_th, int &queue_num, int &decomp_queue_num);

	static bool sort_data_dsc(const VolumeLoaderData b1, const VolumeLoaderData b2)
	{
		return b2.brick->get_d() > b1.brick->get_d();
	}
	static bool sort_data_asc(const VolumeLoaderData b1, const VolumeLoaderData b2)
	{
		return b2.brick->get_d() < b1.brick->get_d();
	}

protected:
	VolumeLoaderThread *m_thread;
	wxCriticalSection m_pThreadCS;
	std::vector<VolumeLoaderData> m_queues;
	std::vector<VolumeLoaderData> m_queued;
	std::vector<VolumeDecompressorData> m_decomp_queues;
	std::vector<VolumeDecompressorThread *> m_decomp_threads;
	std::unordered_map<FLIVR::TextureBrick*, VolumeLoaderData> m_loaded;
	int m_running_decomp_th;
	int m_max_decomp_th;
	bool m_valid;

	long long m_memory_limit;
	long long m_used_memory;

	inline void AddLoadedBrick(VolumeLoaderData lbd)
	{
		m_loaded[lbd.brick] = lbd;
		m_used_memory += lbd.datasize;
	}

	friend class VolumeLoaderThread;
	friend class VolumeDecompressorThread;
};

#endif//_VOLUMELOADER_H_