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
#include <Fui/ClipPlanePanel.h>
#include <Global/Global.h>
#include <wx/valnum.h>
#include <png_resource.h>
#include <img/icons.h>

using namespace FUI;

BEGIN_EVENT_TABLE(ClipPlanePanel, wxPanel)
EVT_TOOL(ID_LinkChannelsBtn, ClipPlanePanel::OnLinkChannelsBtn)
EVT_TOOL(ID_HoldPlanesBtn, ClipPlanePanel::OnHoldPlanesBtn)
EVT_TOOL(ID_PlaneModesBtn, ClipPlanePanel::OnPlaneModesBtn)
EVT_BUTTON(ID_SetZeroBtn, ClipPlanePanel::OnSetZeroBtn)
EVT_BUTTON(ID_RotResetBtn, ClipPlanePanel::OnRotResetBtn)
EVT_BUTTON(ID_ClipResetBtn, ClipPlanePanel::OnClipResetBtn)

EVT_COMMAND_SCROLL(ID_X1ClipSldr, ClipPlanePanel::OnX1ClipChange)
EVT_COMMAND_SCROLL(ID_X2ClipSldr, ClipPlanePanel::OnX2ClipChange)
EVT_COMMAND_SCROLL(ID_Y1ClipSldr, ClipPlanePanel::OnY1ClipChange)
EVT_COMMAND_SCROLL(ID_Y2ClipSldr, ClipPlanePanel::OnY2ClipChange)
EVT_COMMAND_SCROLL(ID_Z1ClipSldr, ClipPlanePanel::OnZ1ClipChange)
EVT_COMMAND_SCROLL(ID_Z2ClipSldr, ClipPlanePanel::OnZ2ClipChange)

EVT_TEXT(ID_X1ClipText, ClipPlanePanel::OnX1ClipEdit)
EVT_TEXT(ID_X2ClipText, ClipPlanePanel::OnX2ClipEdit)
EVT_TEXT(ID_Y1ClipText, ClipPlanePanel::OnY1ClipEdit)
EVT_TEXT(ID_Y2ClipText, ClipPlanePanel::OnY2ClipEdit)
EVT_TEXT(ID_Z1ClipText, ClipPlanePanel::OnZ1ClipEdit)
EVT_TEXT(ID_Z2ClipText, ClipPlanePanel::OnZ2ClipEdit)

EVT_IDLE(ClipPlanePanel::OnIdle)

EVT_TOOL(ID_LinkXChk, ClipPlanePanel::OnLinkXCheck)
EVT_TOOL(ID_LinkYChk, ClipPlanePanel::OnLinkYCheck)
EVT_TOOL(ID_LinkZChk, ClipPlanePanel::OnLinkZCheck)

EVT_COMMAND_SCROLL(ID_XRotSldr, ClipPlanePanel::OnXRotChange)
EVT_COMMAND_SCROLL(ID_YRotSldr, ClipPlanePanel::OnYRotChange)
EVT_COMMAND_SCROLL(ID_ZRotSldr, ClipPlanePanel::OnZRotChange)
EVT_TEXT(ID_XRotText, ClipPlanePanel::OnXRotEdit)
EVT_TEXT(ID_YRotText, ClipPlanePanel::OnYRotEdit)
EVT_TEXT(ID_ZRotText, ClipPlanePanel::OnZRotEdit)

//spin buttons
EVT_SPIN_UP(ID_XRotSpin, ClipPlanePanel::OnXRotSpinUp)
EVT_SPIN_DOWN(ID_XRotSpin, ClipPlanePanel::OnXRotSpinDown)
EVT_SPIN_UP(ID_YRotSpin, ClipPlanePanel::OnYRotSpinUp)
EVT_SPIN_DOWN(ID_YRotSpin, ClipPlanePanel::OnYRotSpinDown)
EVT_SPIN_UP(ID_ZRotSpin, ClipPlanePanel::OnZRotSpinUp)
EVT_SPIN_DOWN(ID_ZRotSpin, ClipPlanePanel::OnZRotSpinDown)

//clip buttons
EVT_BUTTON(ID_YZClipBtn, ClipPlanePanel::OnYZClipBtn)
EVT_BUTTON(ID_XZClipBtn, ClipPlanePanel::OnXZClipBtn)
EVT_BUTTON(ID_XYClipBtn, ClipPlanePanel::OnXYClipBtn)
END_EVENT_TABLE()

ClipPlanePanel::ClipPlanePanel(wxWindow* frame,
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxPanel(parent, id, pos, size, style, name),
	m_frame(frame),
	m_sel_type(0),
	m_draw_clip(false),
	m_hold_planes(false),
	m_plane_mode(kNormal),
	m_link_x(false),
	m_link_y(false),
	m_link_z(false)
{
	// temporarily block events during constructor:
	wxEventBlocker blocker(this);

	m_agent =
		FL::Global::instance().getAgentFactory().
		getOrAddClipPlaneAgent("ClipPlanePanel", *this);

	//validator: floating point 1
	wxFloatingPointValidator<double> vald_fp1(1);
	vald_fp1.SetRange(-180.0, 180.0);
	//validator: integer
	wxIntegerValidator<unsigned int> vald_int;

	wxStaticText *st = 0;

	//sync channels 1
	wxBoxSizer* sizer_1 = new wxBoxSizer(wxHORIZONTAL);
	m_toolbar = new wxToolBar(this, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTB_NODIVIDER);
	wxBitmap bitmap;
	bitmap = wxGetBitmapFromMemory(sync_chan);
#ifdef _DARWIN
	m_toolbar->SetToolBitmapSize(bitmap.GetSize());
#endif
	m_toolbar->AddCheckTool(ID_LinkChannelsBtn, "Sync All Channels",
		bitmap, wxNullBitmap,
		"Link all data channels to this cropping",
		"Link all data channels to this cropping");
	m_toolbar->ToggleTool(ID_LinkChannelsBtn, false);
	bitmap = wxGetBitmapFromMemory(hold_clip);
	m_toolbar->AddCheckTool(ID_HoldPlanesBtn, "Hold Plane Display",
		bitmap, wxNullBitmap,
		"Clipping planes are always shown",
		"Clipping planes are always shown");
	m_toolbar->ToggleTool(ID_HoldPlanesBtn, false);
	bitmap = wxGetBitmapFromMemory(clip_normal);
	m_toolbar->AddTool(ID_PlaneModesBtn, "Display Modes",
		bitmap, "Toggle clipping plane display modes");
	m_toolbar->Realize();
	sizer_1->Add(5, 5, 0);
	sizer_1->Add(m_toolbar, 0, wxALIGN_CENTER, 0);

	wxStaticText* st_cb = 0;

	//sliders for clipping planes
	//x
	wxBoxSizer* sizer_cx = new wxBoxSizer(wxVERTICAL);
	m_xpanel = new wxPanel(this);
	st = new wxStaticText(this, 0, "X");
	m_x1_clip_sldr = new wxSlider(m_xpanel, ID_X1ClipSldr, 0, 0, 512,
		wxPoint(0, 0), wxDefaultSize, wxSL_VERTICAL);
	m_xBar = new wxStaticText(m_xpanel, 0, "",
		wxPoint(20, 10), wxSize(3, m_x1_clip_sldr->GetSize().GetHeight() - 20));
	m_xBar->SetBackgroundColour(wxColor(255, 128, 128));
	m_x2_clip_sldr = new wxSlider(m_xpanel, ID_X2ClipSldr, 512, 0, 512,
		wxPoint(23, 0), wxDefaultSize, wxSL_VERTICAL);
	m_x1_clip_text = new wxTextCtrl(this, ID_X1ClipText, "0",
		wxDefaultPosition, wxSize(34, 20), 0, vald_int);
	st_cb = new wxStaticText(this, 0, "",
		wxDefaultPosition, wxSize(5, 5));
	st_cb->SetBackgroundColour(wxColor(255, 128, 128));
	m_x2_clip_text = new wxTextCtrl(this, ID_X2ClipText, "512",
		wxDefaultPosition, wxSize(34, 20), 0, vald_int);
	//add the items
	sizer_cx->Add(5, 5, 0);
	sizer_cx->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_cx->Add(m_x1_clip_text, 0, wxALIGN_CENTER, 0);
	sizer_cx->Add(5, 5, 0);
	sizer_cx->Add(st_cb, 0, wxEXPAND);
	sizer_cx->Add(m_xpanel, 1, wxALIGN_CENTER, 0);
	st_cb = new wxStaticText(this, 0, "",
		wxDefaultPosition, wxSize(5, 5));
	st_cb->SetBackgroundColour(wxColor(255, 128, 255));
	sizer_cx->Add(st_cb, 0, wxEXPAND);
	sizer_cx->Add(5, 5, 0);
	sizer_cx->Add(m_x2_clip_text, 0, wxALIGN_CENTER, 0);

	//y
	wxBoxSizer* sizer_cy = new wxBoxSizer(wxVERTICAL);
	wxPanel * ypanel = new wxPanel(this);
	st = new wxStaticText(this, 0, "Y");
	m_y1_clip_sldr = new wxSlider(ypanel, ID_Y1ClipSldr, 0, 0, 512,
		wxPoint(0, 0), wxDefaultSize, wxSL_VERTICAL);
	m_yBar = new wxStaticText(ypanel, 0, "",
		wxPoint(20, 10), wxSize(3, m_x1_clip_sldr->GetSize().GetHeight() - 20));
	m_yBar->SetBackgroundColour(wxColor(128, 255, 128));
	m_y2_clip_sldr = new wxSlider(ypanel, ID_Y2ClipSldr, 512, 0, 512,
		wxPoint(23, 0), wxDefaultSize, wxSL_VERTICAL);
	m_y1_clip_text = new wxTextCtrl(this, ID_Y1ClipText, "0",
		wxDefaultPosition, wxSize(34, 20), 0, vald_int);
	st_cb = new wxStaticText(this, 0, "",
		wxDefaultPosition, wxSize(5, 5));
	st_cb->SetBackgroundColour(wxColor(128, 255, 128));
	m_y2_clip_text = new wxTextCtrl(this, ID_Y2ClipText, "512",
		wxDefaultPosition, wxSize(34, 20), 0, vald_int);
	//add the items
	sizer_cy->Add(5, 5, 0);
	sizer_cy->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_cy->Add(m_y1_clip_text, 0, wxALIGN_CENTER, 0);
	sizer_cy->Add(5, 5, 0);
	sizer_cy->Add(st_cb, 0, wxEXPAND);
	sizer_cy->Add(ypanel, 1, wxALIGN_CENTER, 0);
	st_cb = new wxStaticText(this, 0, "",
		wxDefaultPosition, wxSize(5, 5));
	st_cb->SetBackgroundColour(wxColor(255, 255, 128));
	sizer_cy->Add(st_cb, 0, wxEXPAND);
	sizer_cy->Add(5, 5, 0);
	sizer_cy->Add(m_y2_clip_text, 0, wxALIGN_CENTER, 0);

	//z
	wxBoxSizer* sizer_cz = new wxBoxSizer(wxVERTICAL);
	wxPanel * zpanel = new wxPanel(this);
	st = new wxStaticText(this, 0, "Z");
	m_z1_clip_sldr = new wxSlider(zpanel, ID_Z1ClipSldr, 0, 0, 512,
		wxPoint(0, 0), wxDefaultSize, wxSL_VERTICAL);
	m_zBar = new wxStaticText(zpanel, 0, "",
		wxPoint(20, 10), wxSize(3, m_x1_clip_sldr->GetSize().GetHeight() - 20));
	m_zBar->SetBackgroundColour(wxColor(128, 128, 255));
	m_z2_clip_sldr = new wxSlider(zpanel, ID_Z2ClipSldr, 512, 0, 512,
		wxPoint(23, 0), wxDefaultSize, wxSL_VERTICAL);
	m_z1_clip_text = new wxTextCtrl(this, ID_Z1ClipText, "0",
		wxDefaultPosition, wxSize(34, 20), 0, vald_int);
	st_cb = new wxStaticText(this, 0, "",
		wxDefaultPosition, wxSize(5, 5));
	st_cb->SetBackgroundColour(wxColor(128, 128, 255));
	m_z2_clip_text = new wxTextCtrl(this, ID_Z2ClipText, "512",
		wxDefaultPosition, wxSize(34, 20), 0, vald_int);
	//add the items
	sizer_cz->Add(5, 5, 0);
	sizer_cz->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_cz->Add(m_z1_clip_text, 0, wxALIGN_CENTER, 0);
	sizer_cz->Add(5, 5, 0);
	sizer_cz->Add(st_cb, 0, wxEXPAND);
	sizer_cz->Add(zpanel, 1, wxALIGN_CENTER, 0);
	st_cb = new wxStaticText(this, 0, "",
		wxDefaultPosition, wxSize(5, 5));
	st_cb->SetBackgroundColour(wxColor(128, 255, 255));
	sizer_cz->Add(st_cb, 0, wxEXPAND);
	sizer_cz->Add(5, 5, 0);
	sizer_cz->Add(m_z2_clip_text, 0, wxALIGN_CENTER, 0);

	//link sliders
	m_x1_clip_sldr->Connect(ID_X1ClipSldr, wxEVT_RIGHT_DOWN,
		wxCommandEventHandler(ClipPlanePanel::OnSliderRClick),
		NULL, this);
	m_x2_clip_sldr->Connect(ID_X2ClipSldr, wxEVT_RIGHT_DOWN,
		wxCommandEventHandler(ClipPlanePanel::OnSliderRClick),
		NULL, this);
	m_y1_clip_sldr->Connect(ID_Y1ClipSldr, wxEVT_RIGHT_DOWN,
		wxCommandEventHandler(ClipPlanePanel::OnSliderRClick),
		NULL, this);
	m_y2_clip_sldr->Connect(ID_Y2ClipSldr, wxEVT_RIGHT_DOWN,
		wxCommandEventHandler(ClipPlanePanel::OnSliderRClick),
		NULL, this);
	m_z1_clip_sldr->Connect(ID_Z1ClipSldr, wxEVT_RIGHT_DOWN,
		wxCommandEventHandler(ClipPlanePanel::OnSliderRClick),
		NULL, this);
	m_z2_clip_sldr->Connect(ID_Z2ClipSldr, wxEVT_RIGHT_DOWN,
		wxCommandEventHandler(ClipPlanePanel::OnSliderRClick),
		NULL, this);

	//keys
	m_x1_clip_sldr->Connect(ID_X1ClipSldr, wxEVT_KEY_DOWN,
		wxKeyEventHandler(ClipPlanePanel::OnSliderKeyDown),
		NULL, this);
	m_x2_clip_sldr->Connect(ID_X2ClipSldr, wxEVT_KEY_DOWN,
		wxKeyEventHandler(ClipPlanePanel::OnSliderKeyDown),
		NULL, this);
	m_y1_clip_sldr->Connect(ID_Y1ClipSldr, wxEVT_KEY_DOWN,
		wxKeyEventHandler(ClipPlanePanel::OnSliderKeyDown),
		NULL, this);
	m_y2_clip_sldr->Connect(ID_Y2ClipSldr, wxEVT_KEY_DOWN,
		wxKeyEventHandler(ClipPlanePanel::OnSliderKeyDown),
		NULL, this);
	m_z1_clip_sldr->Connect(ID_Z1ClipSldr, wxEVT_KEY_DOWN,
		wxKeyEventHandler(ClipPlanePanel::OnSliderKeyDown),
		NULL, this);
	m_z2_clip_sldr->Connect(ID_Z2ClipSldr, wxEVT_KEY_DOWN,
		wxKeyEventHandler(ClipPlanePanel::OnSliderKeyDown),
		NULL, this);

	//2
	wxBoxSizer *sizer_2 = new wxBoxSizer(wxHORIZONTAL);
	sizer_2->Add(sizer_cx, 1, wxEXPAND);
	sizer_2->Add(sizer_cy, 1, wxEXPAND);
	sizer_2->Add(sizer_cz, 1, wxEXPAND);

	//3 -- link checkbox images
	wxBoxSizer *sizer_3 = new wxBoxSizer(wxHORIZONTAL);
	m_check_tb = new wxToolBar(this, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTB_NODIVIDER);
	bitmap = wxGetBitmapFromMemory(unlink);
#ifdef _DARWIN
	m_check_tb->SetToolBitmapSize(bitmap.GetSize());
#endif
	m_check_tb->AddCheckTool(ID_LinkXChk, "Lock X Planes",
		bitmap, wxNullBitmap, "Lock X Planes");
	m_check_tb->AddSeparator();
	m_check_tb->AddSeparator();
	bitmap = wxGetBitmapFromMemory(unlink);
	m_check_tb->AddCheckTool(ID_LinkYChk, "Lock Y Planes",
		bitmap, wxNullBitmap, "Lock Y Planes");
	m_check_tb->AddSeparator();
	m_check_tb->AddSeparator();
	bitmap = wxGetBitmapFromMemory(unlink);
	m_check_tb->AddCheckTool(ID_LinkZChk, "Lock Z Planes",
		bitmap, wxNullBitmap, "Lock Z Planes");
	m_check_tb->Realize();
	sizer_3->Add(5, 5, 0);
	sizer_3->Add(m_check_tb, 1, wxALIGN_TOP | wxSHRINK);

	//clip buttons 4
	wxBoxSizer* sizer_4 = new wxBoxSizer(wxHORIZONTAL);
	m_yz_clip_btn = new wxButton(this, ID_YZClipBtn, "YZ",
		wxDefaultPosition, wxSize(34, 22));
	m_xz_clip_btn = new wxButton(this, ID_XZClipBtn, "XZ",
		wxDefaultPosition, wxSize(34, 22));
	m_xy_clip_btn = new wxButton(this, ID_XYClipBtn, "XY",
		wxDefaultPosition, wxSize(34, 22));
	sizer_4->Add(m_yz_clip_btn, 1, wxEXPAND);
	sizer_4->AddSpacer(5);
	sizer_4->Add(m_xz_clip_btn, 1, wxEXPAND);
	sizer_4->AddSpacer(5);
	sizer_4->Add(m_xy_clip_btn, 1, wxEXPAND);

	//clip distance 5
	wxBoxSizer* sizer_5 = new wxBoxSizer(wxHORIZONTAL);
	m_yz_dist_text = new wxTextCtrl(this, ID_YZDistText, "1",
		wxDefaultPosition, wxSize(34, 22), 0, vald_int);
	m_xz_dist_text = new wxTextCtrl(this, ID_XZDistText, "1",
		wxDefaultPosition, wxSize(34, 22), 0, vald_int);
	m_xy_dist_text = new wxTextCtrl(this, ID_XYDistText, "1",
		wxDefaultPosition, wxSize(34, 22), 0, vald_int);
	sizer_5->Add(m_yz_dist_text, 1, wxEXPAND);
	sizer_5->AddSpacer(5);
	sizer_5->Add(m_xz_dist_text, 1, wxEXPAND);
	sizer_5->AddSpacer(5);
	sizer_5->Add(m_xy_dist_text, 1, wxEXPAND);

	//reset clipping 6
	wxBoxSizer* sizer_6 = new wxBoxSizer(wxHORIZONTAL);
#ifndef _DARWIN
	m_clip_reset_btn = new wxButton(this, ID_ClipResetBtn, "Reset Clips",
		wxDefaultPosition, wxSize(120, 22));
#else
	m_clip_reset_btn = new wxButton(this, ID_ClipResetBtn, "Reset Clips",
		wxDefaultPosition, wxSize(125, 30));
#endif
	m_clip_reset_btn->SetBitmap(wxGetBitmapFromMemory(reset));
	sizer_6->Add(5, 5, 0);
	sizer_6->Add(m_clip_reset_btn, 0, wxALIGN_CENTER);

	//rotations
	//set sero rotation for clipping planes 7
	wxBoxSizer* sizer_7 = new wxBoxSizer(wxHORIZONTAL);
#ifndef _DARWIN
	m_set_zero_btn = new wxButton(this, ID_SetZeroBtn, "Align to View",
		wxDefaultPosition, wxSize(120, 22));
#else
	m_set_zero_btn = new wxButton(this, ID_SetZeroBtn, "Align to View",
		wxDefaultPosition, wxSize(125, 30));
#endif
	m_set_zero_btn->SetBitmap(wxGetBitmapFromMemory(align));
	sizer_7->Add(5, 5, 0);
	sizer_7->Add(m_set_zero_btn, 0, wxALIGN_CENTER);

	//reset rotations 8
	wxBoxSizer* sizer_8 = new wxBoxSizer(wxHORIZONTAL);
#ifndef _DARWIN
	m_rot_reset_btn = new wxButton(this, ID_RotResetBtn, "Reset to 0",
		wxDefaultPosition, wxSize(120, 22));
#else
	m_rot_reset_btn = new wxButton(this, ID_RotResetBtn, "Reset to 0",
		wxDefaultPosition, wxSize(125, 30));
#endif
	m_rot_reset_btn->SetBitmap(wxGetBitmapFromMemory(reset));
	sizer_8->Add(5, 5, 0);
	sizer_8->Add(m_rot_reset_btn, 0, wxALIGN_CENTER);

	//sliders for rotating clipping planes 
	//x
	wxBoxSizer* sizer_rx = new wxBoxSizer(wxVERTICAL);
	st = new wxStaticText(this, 0, "X");
	m_x_rot_sldr = new wxSlider(this, ID_XRotSldr, 0, -180, 180,
		wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);
	m_x_rot_text = new wxTextCtrl(this, ID_XRotText, "0.0",
		wxDefaultPosition, wxSize(34, 20), 0, vald_fp1);
	m_x_rot_spin = new wxSpinButton(this, ID_XRotSpin,
		wxDefaultPosition, wxSize(30, 20), wxSP_VERTICAL);
	sizer_rx->Add(5, 5, 0);
	sizer_rx->Add(m_x_rot_text, 0, wxALIGN_CENTER, 0);
	sizer_rx->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_rx->Add(5, 5, 0);
	sizer_rx->Add(m_x_rot_spin, 0, wxALIGN_CENTER, 0);
	sizer_rx->Add(m_x_rot_sldr, 1, wxALIGN_CENTER, 0);
	//y
	wxBoxSizer* sizer_ry = new wxBoxSizer(wxVERTICAL);
	st = new wxStaticText(this, 0, "Y");
	m_y_rot_sldr = new wxSlider(this, ID_YRotSldr, 0, -180, 180,
		wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);
	m_y_rot_text = new wxTextCtrl(this, ID_YRotText, "0.0",
		wxDefaultPosition, wxSize(34, 20), 0, vald_fp1);
	m_y_rot_spin = new wxSpinButton(this, ID_YRotSpin,
		wxDefaultPosition, wxSize(30, 20), wxSP_VERTICAL);
	sizer_ry->Add(5, 5, 0);
	sizer_ry->Add(m_y_rot_text, 0, wxALIGN_CENTER, 0);
	sizer_ry->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_ry->Add(5, 5, 0);
	sizer_ry->Add(m_y_rot_spin, 0, wxALIGN_CENTER, 0);
	sizer_ry->Add(m_y_rot_sldr, 1, wxALIGN_CENTER, 0);
	//z
	wxBoxSizer* sizer_rz = new wxBoxSizer(wxVERTICAL);
	st = new wxStaticText(this, 0, "Z");
	m_z_rot_sldr = new wxSlider(this, ID_ZRotSldr, 0, -180, 180,
		wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);
	m_z_rot_text = new wxTextCtrl(this, ID_ZRotText, "0.0",
		wxDefaultPosition, wxSize(34, 20), 0, vald_fp1);
	m_z_rot_spin = new wxSpinButton(this, ID_ZRotSpin,
		wxDefaultPosition, wxSize(30, 20), wxSP_VERTICAL);
	sizer_rz->Add(5, 5, 0);
	sizer_rz->Add(m_z_rot_text, 0, wxALIGN_CENTER, 0);
	sizer_rz->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_rz->Add(5, 5, 0);
	sizer_rz->Add(m_z_rot_spin, 0, wxALIGN_CENTER, 0);
	sizer_rz->Add(m_z_rot_sldr, 1, wxALIGN_CENTER, 0);


	//sizer 9
	wxBoxSizer *sizer_9 = new wxBoxSizer(wxHORIZONTAL);
	sizer_9->Add(sizer_rx, 1, wxEXPAND);
	sizer_9->Add(sizer_ry, 1, wxEXPAND);
	sizer_9->Add(sizer_rz, 1, wxEXPAND);

	//v
	wxBoxSizer *sizer_v = new wxBoxSizer(wxVERTICAL);
	sizer_v->Add(sizer_1, 0, wxALIGN_CENTER);
	sizer_v->Add(5, 5, 0);
	sizer_v->Add(sizer_2, 1, wxEXPAND);
	sizer_v->Add(5, 5, 0);
	sizer_v->Add(sizer_3, 0, wxEXPAND);
	sizer_v->AddSpacer(5);
	sizer_v->Add(new wxStaticText(this, 0,
		"Set Clip Slab Width"), 0, wxALIGN_CENTER);
	sizer_v->Add(5, 5, 0);
	sizer_v->Add(sizer_4, 0, wxALIGN_CENTER);
	sizer_v->Add(5, 5, 0);
	sizer_v->Add(sizer_5, 0, wxALIGN_CENTER);
	sizer_v->Add(5, 5, 0);
	sizer_v->Add(sizer_6, 0, wxALIGN_CENTER);
	sizer_v->Add(5, 5, 0);

	st = new wxStaticText(this, 0, "", wxDefaultPosition, wxSize(5, 5));
	st->SetBackgroundColour(wxColor(100, 100, 100));
	sizer_v->Add(10, 10, 0);
	sizer_v->Add(st, 0, wxEXPAND);

	st = new wxStaticText(this, 0, "Rotations:");
	sizer_v->Add(10, 10, 0);
	sizer_v->Add(st, 0, wxALIGN_CENTER);
	sizer_v->Add(10, 10, 0);
	sizer_v->Add(sizer_7, 0, wxALIGN_CENTER);
	sizer_v->Add(sizer_8, 0, wxALIGN_CENTER);
	sizer_v->Add(10, 10, 0);
	sizer_v->Add(sizer_9, 1, wxEXPAND);

	SetSizer(sizer_v);
	Layout();

	DisableAll();
}

ClipPlanePanel::~ClipPlanePanel()
{
}

void ClipPlanePanel::AssociateNode(FL::Node* node)
{
	m_agent->setObject(node);
}

void ClipPlanePanel::SetChannLink(bool chann)
{
	m_toolbar->ToggleTool(ID_LinkChannelsBtn, chann);
}

void ClipPlanePanel::SetHoldPlanes(bool hold)
{
	m_hold_planes = hold;
	m_toolbar->ToggleTool(ID_HoldPlanesBtn, hold);
	if (hold)
	{
		//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
		//if (!vrender_frame)
		//	return;
		//vector <VRenderView*>* vrv_list = vrender_frame->GetViewList();
		//for (int i = 0; i<(int)vrv_list->size(); i++)
		//{
		//	if ((*vrv_list)[i])
		//	{
		//		(*vrv_list)[i]->m_glview->m_draw_clip = true;
		//		(*vrv_list)[i]->m_glview->m_clip_mask = -1;
		//	}
		//}
	}
}

void ClipPlanePanel::SetPlaneMode(PLANE_MODES mode)
{
	m_plane_mode = mode;
	switch (m_plane_mode)
	{
	case kNormal:
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_normal));
		break;
	case kFrame:
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_frame));
		break;
	case kLowTrans:
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_low));
		break;
	case kLowTransBack:
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_low_back));
		break;
	case kNormalBack:
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_normal_back));
		break;
	case kNone:
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_none));
		break;
	}
}

void ClipPlanePanel::OnLinkChannelsBtn(wxCommandEvent &event)
{
	if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	{
		//if (!m_mgr)
		//	return;

		//wxString str;
		////x1
		//str = m_x1_clip_text->GetValue();
		//long x1_val = 0;
		//str.ToLong(&x1_val);
		////x2
		//str = m_x2_clip_text->GetValue();
		//long x2_val = 0;
		//str.ToLong(&x2_val);
		////y1
		//str = m_y1_clip_text->GetValue();
		//long y1_val = 0;
		//str.ToLong(&y1_val);
		////y2
		//str = m_y2_clip_text->GetValue();
		//long y2_val = 0;
		//str.ToLong(&y2_val);
		////z1
		//str = m_z1_clip_text->GetValue();
		//long z1_val = 0;
		//str.ToLong(&z1_val);
		////z2
		//str = m_z2_clip_text->GetValue();
		//long z2_val = 0;
		//str.ToLong(&z2_val);

		//size_t num = FL::Global::instance().getVolumeFactory().getNum();
		//for (size_t i = 0; i<num; i++)
		//{
		//	FL::VolumeData* vd =
		//		FL::Global::instance().getVolumeFactory().get(i);
		//	if (!vd || vd == m_vd)
		//		continue;

		//	vector<Plane*> *planes = 0;
		//	if (vd->GetRenderer())
		//		planes = vd->GetRenderer()->get_planes();
		//	if (!planes)
		//		continue;
		//	if (planes->size() != 6)
		//		continue;

		//	long resx, resy, resz;
		//	//vd->GetResolution(resx, resy, resz);
		//	vd->getValue("res x", resx);
		//	vd->getValue("res y", resy);
		//	vd->getValue("res z", resz);

		//	(*planes)[0]->ChangePlane(Point(double(x1_val) / double(resx), 0.0, 0.0), Vector(1.0, 0.0, 0.0));
		//	(*planes)[1]->ChangePlane(Point(double(x2_val) / double(resx), 0.0, 0.0), Vector(-1.0, 0.0, 0.0));
		//	(*planes)[2]->ChangePlane(Point(0.0, double(y1_val) / double(resy), 0.0), Vector(0.0, 1.0, 0.0));
		//	(*planes)[3]->ChangePlane(Point(0.0, double(y2_val) / double(resy), 0.0), Vector(0.0, -1.0, 0.0));
		//	(*planes)[4]->ChangePlane(Point(0.0, 0.0, double(z1_val) / double(resz)), Vector(0.0, 0.0, 1.0));
		//	(*planes)[5]->ChangePlane(Point(0.0, 0.0, double(z2_val) / double(resz)), Vector(0.0, 0.0, -1.0));
		//}

		//RefreshVRenderViews();
	}
}

void ClipPlanePanel::OnHoldPlanesBtn(wxCommandEvent &event)
{
	m_hold_planes = m_toolbar->GetToolState(ID_HoldPlanesBtn);
}

void ClipPlanePanel::OnPlaneModesBtn(wxCommandEvent &event)
{
	switch (m_plane_mode)
	{
	case kNormal:
		m_plane_mode = kFrame;
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_frame));
		break;
	case kFrame:
		m_plane_mode = kLowTrans;
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_low));
		break;
	case kLowTrans:
		m_plane_mode = kLowTransBack;
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_low_back));
		break;
	case kLowTransBack:
		m_plane_mode = kNormalBack;
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_normal_back));
		break;
	case kNormalBack:
		m_plane_mode = kNone;
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_none));
		break;
	case kNone:
		m_plane_mode = kNormal;
		m_toolbar->SetToolNormalBitmap(ID_PlaneModesBtn,
			wxGetBitmapFromMemory(clip_normal));
		break;
	}

	//RefreshVRenderViews();
}

void ClipPlanePanel::OnClipResetBtn(wxCommandEvent &event)
{
	//if (!m_vd)
	//	return;
	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//vector<Plane*> *planes = 0;
	//if (m_vd->GetRenderer())
	//	planes = m_vd->GetRenderer()->get_planes();
	//if (!planes)
	//	return;
	//if (planes->size() != 6)
	//	return;

	//Plane* plane = (*planes)[0];
	//plane->ChangePlane(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	//plane = (*planes)[1];
	//plane->ChangePlane(Point(1.0, 0.0, 0.0), Vector(-1.0, 0.0, 0.0));
	//plane = (*planes)[2];
	//plane->ChangePlane(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));
	//plane = (*planes)[3];
	//plane->ChangePlane(Point(0.0, 1.0, 0.0), Vector(0.0, -1.0, 0.0));
	//plane = (*planes)[4];
	//plane->ChangePlane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	//plane = (*planes)[5];
	//plane->ChangePlane(Point(0.0, 0.0, 1.0), Vector(0.0, 0.0, -1.0));

	////links
	//m_link_x = false;
	//m_link_y = false;
	//m_link_z = false;
	//m_check_tb->ToggleTool(ID_LinkXChk, false);
	//m_check_tb->ToggleTool(ID_LinkYChk, false);
	//m_check_tb->ToggleTool(ID_LinkZChk, false);
	//m_check_tb->SetToolNormalBitmap(ID_LinkXChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_check_tb->SetToolNormalBitmap(ID_LinkYChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_check_tb->SetToolNormalBitmap(ID_LinkZChk,
	//	wxGetBitmapFromMemory(unlink));

	////controls
	////sliders
	//m_x1_clip_sldr->SetValue(0);
	//m_x2_clip_sldr->SetValue(resx);
	//m_y1_clip_sldr->SetValue(0);
	//m_y2_clip_sldr->SetValue(resy);
	//m_z1_clip_sldr->SetValue(0);
	//m_z2_clip_sldr->SetValue(resz);
	////texts
	//m_x1_clip_text->SetValue("0");
	//m_x2_clip_text->SetValue(wxString::Format("%d", resx));
	//m_y1_clip_text->SetValue("0");
	//m_y2_clip_text->SetValue(wxString::Format("%d", resy));
	//m_z1_clip_text->SetValue("0");
	//m_z2_clip_text->SetValue(wxString::Format("%d", resz));

	////link
	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		size_t num = FL::Global::instance().getVolumeFactory().getNum();
	//		for (size_t i = 0; i<num; i++)
	//		{
	//			FL::VolumeData* vd =
	//				FL::Global::instance().getVolumeFactory().get(i);
	//			if (!vd || vd == m_vd)
	//				continue;

	//			planes = 0;
	//			if (m_vd->GetRenderer())
	//				planes = vd->GetRenderer()->get_planes();
	//			if (!planes)
	//				continue;
	//			if (planes->size() != 6)
	//				continue;

	//			(*planes)[0]->ChangePlane(Point(0.0, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	//			(*planes)[1]->ChangePlane(Point(1.0, 0.0, 0.0), Vector(-1.0, 0.0, 0.0));
	//			(*planes)[2]->ChangePlane(Point(0.0, 0.0, 0.0), Vector(0.0, 1.0, 0.0));
	//			(*planes)[3]->ChangePlane(Point(0.0, 1.0, 0.0), Vector(0.0, -1.0, 0.0));
	//			(*planes)[4]->ChangePlane(Point(0.0, 0.0, 0.0), Vector(0.0, 0.0, 1.0));
	//			(*planes)[5]->ChangePlane(Point(0.0, 0.0, 1.0), Vector(0.0, 0.0, -1.0));
	//		}
	//	}
	//}

	////views
	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			vrv->m_glview->m_clip_mask = -1;

	//			double rotx, roty, rotz;
	//			vrv->GetRotations(rotx, roty, rotz);
	//			vrv->SetRotations(rotx, roty, rotz);

	//			vrv->RefreshGL();
	//		}
	//	}
	//}
}

void ClipPlanePanel::OnX1ClipChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	m_x1_clip_text->SetValue(wxString::Format("%d", ival));
}

void ClipPlanePanel::OnX1ClipEdit(wxCommandEvent &event)
{
	long resx;
	m_agent->getValue("res x", resx);

	wxString str = m_x1_clip_text->GetValue();
	long ival = 0;
	double dval = 0.0;
	str.ToLong(&ival);
	dval = (double)ival / (double)resx;
	m_x1_clip_sldr->SetValue(ival);
	int barsize = (m_x1_clip_sldr->GetSize().GetHeight() - 20);
	m_xBar->SetPosition(wxPoint(20, 10 + dval*barsize));
	m_xBar->SetSize(wxSize(3, barsize*((double)
		(m_x2_clip_sldr->GetValue() - ival) / (double)m_x1_clip_sldr->GetMax())));
	FLTYPE::PlaneSet planes;
	m_agent->getValue("clip planes", planes);
	if (planes.GetSize() != 6)
		return;
	planes[0].ChangePlane(FLTYPE::Point(dval, 0.0, 0.0), FLTYPE::Vector(1.0, 0.0, 0.0));
	m_agent->setValue("clip planes", planes);

	//if (m_link_x)
	//{
	//	if (ival + m_x_sldr_dist > resx)
	//	{
	//		ival = resx - m_x_sldr_dist;
	//		ival2 = resx;
	//	}
	//	else
	//		ival2 = ival + m_x_sldr_dist;
	//}
	//else if (ival > ival2)
	//	ival = ival2;

	//val = (double)ival / (double)resx;
	////str = wxString::Format("%d", ival);
	////m_x1_clip_text->ChangeValue(str);
	//m_x1_clip_sldr->SetValue(ival);
	//int barsize = (m_x1_clip_sldr->GetSize().GetHeight() - 20);
	//m_xBar->SetPosition(wxPoint(20, 10 + val*barsize));
	//m_xBar->SetSize(wxSize(3, barsize*((double)
	//	(m_x2_clip_sldr->GetValue() - ival) / (double)m_x1_clip_sldr->GetMax())));
	//Plane* plane = (*planes)[0];
	//plane->ChangePlane(Point(val, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	//if (m_link_x)
	//{
	//	val2 = (double)ival2 / (double)resx;
	//	str = wxString::Format("%d", ival2);
	//	m_x2_clip_text->ChangeValue(str);
	//	m_x2_clip_sldr->SetValue(ival2);
	//	plane = (*planes)[1];
	//	plane->ChangePlane(Point(val2, 0.0, 0.0), Vector(-1.0, 0.0, 0.0));
	//}

	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		size_t num = FL::Global::instance().getVolumeFactory().getNum();
	//		for (size_t i = 0; i<num; i++)
	//		{
	//			FL::VolumeData* vd =
	//				FL::Global::instance().getVolumeFactory().get(i);
	//			if (!vd || vd == m_vd)
	//				continue;

	//			planes = 0;
	//			if (m_vd->GetRenderer())
	//				planes = vd->GetRenderer()->get_planes();
	//			if (!planes)
	//				continue;
	//			if (planes->size() != 6)
	//				continue;

	//			(*planes)[0]->ChangePlane(Point(val, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	//			if (m_link_x)
	//				(*planes)[1]->ChangePlane(Point(val2, 0.0, 0.0), Vector(-1.0, 0.0, 0.0));
	//		}
	//	}
	//}

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			if (m_link_x)
	//				vrv->m_glview->m_clip_mask = 3;
	//			else
	//				vrv->m_glview->m_clip_mask = 1;

	//			double rotx, roty, rotz;
	//			vrv->GetRotations(rotx, roty, rotz);
	//			vrv->SetRotations(rotx, roty, rotz);
	//		}
	//	}
	//}
	//RefreshVRenderViews(true);
}

void ClipPlanePanel::OnX2ClipChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	int ival2 = m_x1_clip_sldr->GetValue();

	if (!m_link_x && ival<ival2)
	{
		ival = ival2;
		m_x2_clip_sldr->SetValue(ival);
	}
	m_x2_clip_text->SetValue(wxString::Format("%d", ival));
}

void ClipPlanePanel::OnX2ClipEdit(wxCommandEvent &event)
{
	long resx;
	m_agent->getValue("res x", resx);

	wxString str = m_x2_clip_text->GetValue();
	long ival = 0;
	double dval = 0.0;
	str.ToLong(&ival);
	dval = (double)ival / (double)resx;
	m_x2_clip_sldr->SetValue(ival);
	int barsize = (m_x1_clip_sldr->GetSize().GetHeight() - 20);
	m_xBar->SetPosition(wxPoint(20, 10 + ((double)m_x1_clip_sldr->GetValue() /
		(double)m_x1_clip_sldr->GetMax())*barsize));
	m_xBar->SetSize(wxSize(3, barsize*((double)
		(ival - m_x1_clip_sldr->GetValue()) / (double)m_x1_clip_sldr->GetMax())));
	FLTYPE::PlaneSet planes;
	m_agent->getValue("clip planes", planes);
	if (planes.GetSize() != 6)
		return;
	planes[1].ChangePlane(FLTYPE::Point(dval, 0.0, 0.0), FLTYPE::Vector(-1.0, 0.0, 0.0));
	m_agent->setValue("clip planes", planes);
	//if (!m_vd)
	//	return;
	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//vector<Plane*> *planes = 0;
	//if (m_vd->GetRenderer())
	//	planes = m_vd->GetRenderer()->get_planes();
	//if (!planes)
	//	return;
	//if (planes->size() != 6)
	//	return;

	//wxString str = m_x2_clip_text->GetValue();
	//long ival = 0;
	//str.ToLong(&ival);
	//int ival2 = m_x1_clip_sldr->GetValue();
	//double val, val2;

	//if (m_link_x)
	//{
	//	if (ival - m_x_sldr_dist < 0)
	//	{
	//		ival = m_x_sldr_dist;
	//		ival2 = 0;
	//	}
	//	else
	//		ival2 = ival - m_x_sldr_dist;
	//}
	//else if (ival < ival2)
	//	return;

	//val = (double)ival / (double)resx;
	////str = wxString::Format("%d", ival);
	////m_x2_clip_text->ChangeValue(str);
	//m_x2_clip_sldr->SetValue(ival);
	//int barsize = (m_x1_clip_sldr->GetSize().GetHeight() - 20);
	//m_xBar->SetPosition(wxPoint(20, 10 + ((double)m_x1_clip_sldr->GetValue() /
	//	(double)m_x1_clip_sldr->GetMax())*barsize));
	//m_xBar->SetSize(wxSize(3, barsize*((double)
	//	(ival - m_x1_clip_sldr->GetValue()) / (double)m_x1_clip_sldr->GetMax())));
	//Plane* plane = (*planes)[1];
	//plane->ChangePlane(Point(val, 0.0, 0.0), Vector(-1.0, 0.0, 0.0));
	//if (m_link_x)
	//{
	//	val2 = (double)ival2 / (double)resx;
	//	str = wxString::Format("%d", ival2);
	//	m_x1_clip_text->ChangeValue(str);
	//	m_x1_clip_sldr->SetValue(ival2);
	//	plane = (*planes)[0];
	//	plane->ChangePlane(Point(val2, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	//}

	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		size_t num = FL::Global::instance().getVolumeFactory().getNum();
	//		for (size_t i = 0; i<num; i++)
	//		{
	//			FL::VolumeData* vd =
	//				FL::Global::instance().getVolumeFactory().get(i);
	//			if (!vd || vd == m_vd)
	//				continue;

	//			planes = 0;
	//			if (m_vd->GetRenderer())
	//				planes = vd->GetRenderer()->get_planes();
	//			if (!planes)
	//				continue;
	//			if (planes->size() != 6)
	//				continue;
	//			(*planes)[1]->ChangePlane(Point(val, 0.0, 0.0), Vector(-1.0, 0.0, 0.0));
	//			if (m_link_x)
	//				(*planes)[0]->ChangePlane(Point(val2, 0.0, 0.0), Vector(1.0, 0.0, 0.0));
	//		}
	//	}
	//}

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			if (m_link_x)
	//				vrv->m_glview->m_clip_mask = 3;
	//			else
	//				vrv->m_glview->m_clip_mask = 2;

	//			double rotx, roty, rotz;
	//			vrv->GetRotations(rotx, roty, rotz);
	//			vrv->SetRotations(rotx, roty, rotz);
	//		}
	//	}
	//}

	//RefreshVRenderViews(true);
}

void ClipPlanePanel::OnY1ClipChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	m_y1_clip_text->SetValue(wxString::Format("%d", ival));
}

void ClipPlanePanel::OnY1ClipEdit(wxCommandEvent &event)
{
	long resy;
	m_agent->getValue("res y", resy);

	wxString str = m_y1_clip_text->GetValue();
	long ival = 0;
	double dval = 0.0;
	str.ToLong(&ival);
	dval = (double)ival / (double)resy;
	m_y1_clip_sldr->SetValue(ival);
	int barsize = (m_y1_clip_sldr->GetSize().GetHeight() - 20);
	m_yBar->SetPosition(wxPoint(20, 10 + dval*barsize));
	m_yBar->SetSize(wxSize(3, barsize*((double)
		(m_y2_clip_sldr->GetValue() - ival) / (double)m_y1_clip_sldr->GetMax())));
	FLTYPE::PlaneSet planes;
	m_agent->getValue("clip planes", planes);
	if (planes.GetSize() != 6)
		return;
	planes[2].ChangePlane(FLTYPE::Point(0.0, dval, 0.0), FLTYPE::Vector(0.0, 1.0, 0.0));
	m_agent->setValue("clip planes", planes);
	//if (!m_vd)
	//	return;
	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//vector<Plane*> *planes = 0;
	//if (m_vd->GetRenderer())
	//	planes = m_vd->GetRenderer()->get_planes();
	//if (!planes)
	//	return;
	//if (planes->size() != 6)
	//	return;

	//wxString str = m_y1_clip_text->GetValue();
	//long ival = 0;
	//str.ToLong(&ival);
	//int ival2 = m_y2_clip_sldr->GetValue();
	//double val, val2;

	//if (m_link_y)
	//{
	//	if (ival + m_y_sldr_dist >resy)
	//	{
	//		ival = resy - m_y_sldr_dist;
	//		ival2 = resy;
	//	}
	//	else
	//		ival2 = ival + m_y_sldr_dist;
	//}
	//else if (ival > ival2)
	//	ival = ival2;

	//val = (double)ival / (double)resy;
	////str = wxString::Format("%d", ival);
	////m_y1_clip_text->ChangeValue(str);
	//m_y1_clip_sldr->SetValue(ival);
	//int barsize = (m_y1_clip_sldr->GetSize().GetHeight() - 20);
	//m_yBar->SetPosition(wxPoint(20, 10 + val*barsize));
	//m_yBar->SetSize(wxSize(3, barsize*((double)
	//	(m_y2_clip_sldr->GetValue() - ival) / (double)m_y1_clip_sldr->GetMax())));
	//Plane* plane = (*planes)[2];
	//plane->ChangePlane(Point(0.0, val, 0.0), Vector(0.0, 1.0, 0.0));
	//if (m_link_y)
	//{
	//	val2 = (double)ival2 / (double)resy;
	//	str = wxString::Format("%d", ival2);
	//	m_y2_clip_text->ChangeValue(str);
	//	m_y2_clip_sldr->SetValue(ival2);
	//	plane = (*planes)[3];
	//	plane->ChangePlane(Point(0.0, val2, 0.0), Vector(0.0, -1.0, 0.0));
	//}

	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		size_t num = FL::Global::instance().getVolumeFactory().getNum();
	//		for (size_t i = 0; i<num; i++)
	//		{
	//			FL::VolumeData* vd =
	//				FL::Global::instance().getVolumeFactory().get(i);
	//			if (!vd || vd == m_vd)
	//				continue;

	//			planes = 0;
	//			if (m_vd->GetRenderer())
	//				planes = vd->GetRenderer()->get_planes();
	//			if (!planes)
	//				continue;
	//			if (planes->size() != 6)
	//				continue;

	//			(*planes)[2]->ChangePlane(Point(0.0, val, 0.0), Vector(0.0, 1.0, 0.0));
	//			if (m_link_y)
	//				(*planes)[3]->ChangePlane(Point(0.0, val2, 0.0), Vector(0.0, -1.0, 0.0));
	//		}
	//	}
	//}

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			if (m_link_y)
	//				vrv->m_glview->m_clip_mask = 12;
	//			else
	//				vrv->m_glview->m_clip_mask = 4;

	//			double rotx, roty, rotz;
	//			vrv->GetRotations(rotx, roty, rotz);
	//			vrv->SetRotations(rotx, roty, rotz);
	//		}
	//	}
	//}

	//RefreshVRenderViews(true);
}

void ClipPlanePanel::OnY2ClipChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	int ival2 = m_y1_clip_sldr->GetValue();

	if (!m_link_y && ival<ival2)
	{
		ival = ival2;
		m_y2_clip_sldr->SetValue(ival);
	}
	m_y2_clip_text->SetValue(wxString::Format("%d", ival));
}

void ClipPlanePanel::OnY2ClipEdit(wxCommandEvent &event)
{
	long resy;
	m_agent->getValue("res y", resy);

	wxString str = m_y2_clip_text->GetValue();
	long ival = 0;
	double dval = 0.0;
	str.ToLong(&ival);
	dval = (double)ival / (double)resy;
	m_y2_clip_sldr->SetValue(ival);
	int barsize = (m_y1_clip_sldr->GetSize().GetHeight() - 20);
	m_yBar->SetPosition(wxPoint(20, 10 + ((double)m_y1_clip_sldr->GetValue() /
		(double)m_y1_clip_sldr->GetMax())*barsize));
	m_yBar->SetSize(wxSize(3, barsize*((double)
		(ival - m_y1_clip_sldr->GetValue()) / (double)m_y1_clip_sldr->GetMax())));
	FLTYPE::PlaneSet planes;
	m_agent->getValue("clip planes", planes);
	if (planes.GetSize() != 6)
		return;
	planes[3].ChangePlane(FLTYPE::Point(0.0, dval, 0.0), FLTYPE::Vector(0.0, -1.0, 0.0));
	m_agent->setValue("clip planes", planes);
	//if (!m_vd)
	//	return;
	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);
	//FLTYPE::PlaneSet planeset;
	//vector<Plane*> *planes = 0;
	//if (m_vd->GetRenderer())
	//	planes = m_vd->GetRenderer()->get_planes();
	//if (!planes)
	//	return;
	//if (planes->size() != 6)
	//	return;

	//wxString str = m_y2_clip_text->GetValue();
	//long ival = 0;
	//str.ToLong(&ival);
	//int ival2 = m_y1_clip_sldr->GetValue();
	//double val, val2;

	//if (m_link_y)
	//{
	//	if (ival - m_y_sldr_dist < 0)
	//	{
	//		ival = m_y_sldr_dist;
	//		ival2 = 0;
	//	}
	//	else
	//		ival2 = ival - m_y_sldr_dist;
	//}
	//else if (ival < ival2)
	//	return;

	//val = (double)ival / (double)resy;
	////str = wxString::Format("%d", ival);
	////m_y2_clip_text->ChangeValue(str);
	//m_y2_clip_sldr->SetValue(ival);
	//int barsize = (m_y1_clip_sldr->GetSize().GetHeight() - 20);
	//m_yBar->SetPosition(wxPoint(20, 10 + ((double)m_y1_clip_sldr->GetValue() /
	//	(double)m_y1_clip_sldr->GetMax())*barsize));
	//m_yBar->SetSize(wxSize(3, barsize*((double)
	//	(ival - m_y1_clip_sldr->GetValue()) / (double)m_y1_clip_sldr->GetMax())));
	//Plane* plane = (*planes)[3];
	//plane->ChangePlane(Point(0.0, val, 0.0), Vector(0.0, -1.0, 0.0));
	//if (m_link_y)
	//{
	//	val2 = (double)ival2 / (double)resy;
	//	str = wxString::Format("%d", ival2);
	//	m_y1_clip_text->ChangeValue(str);
	//	m_y1_clip_sldr->SetValue(ival2);
	//	plane = (*planes)[2];
	//	plane->ChangePlane(Point(0.0, val2, 0.0), Vector(0.0, 1.0, 0.0));
	//}

	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		size_t num = FL::Global::instance().getVolumeFactory().getNum();
	//		for (size_t i = 0; i<num; i++)
	//		{
	//			FL::VolumeData* vd =
	//				FL::Global::instance().getVolumeFactory().get(i);
	//			if (!vd || vd == m_vd)
	//				continue;

	//			planes = 0;
	//			if (m_vd->GetRenderer())
	//				planes = vd->GetRenderer()->get_planes();
	//			if (!planes)
	//				continue;
	//			if (planes->size() != 6)
	//				continue;

	//			(*planes)[3]->ChangePlane(Point(0.0, val, 0.0), Vector(0.0, -1.0, 0.0));
	//			if (m_link_y)
	//				(*planes)[2]->ChangePlane(Point(0.0, val2, 0.0), Vector(0.0, 1.0, 0.0));
	//		}
	//	}
	//}

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			if (m_link_y)
	//				vrv->m_glview->m_clip_mask = 12;
	//			else
	//				vrv->m_glview->m_clip_mask = 8;

	//			double rotx, roty, rotz;
	//			vrv->GetRotations(rotx, roty, rotz);
	//			vrv->SetRotations(rotx, roty, rotz);
	//		}
	//	}
	//}

	//RefreshVRenderViews(true);
}

void ClipPlanePanel::OnZ1ClipChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	m_z1_clip_text->SetValue(wxString::Format("%d", ival));
}

void ClipPlanePanel::OnZ1ClipEdit(wxCommandEvent &event)
{
	long resz;
	m_agent->getValue("res z", resz);

	wxString str = m_z1_clip_text->GetValue();
	long ival = 0;
	double dval = 0.0;
	str.ToLong(&ival);
	dval = (double)ival / (double)resz;
	m_z1_clip_sldr->SetValue(ival);
	int barsize = (m_z1_clip_sldr->GetSize().GetHeight() - 20);
	m_zBar->SetPosition(wxPoint(20, 10 + dval*barsize));
	m_zBar->SetSize(wxSize(3, barsize*((double)
		(m_z2_clip_sldr->GetValue() - ival) / (double)m_z1_clip_sldr->GetMax())));
	FLTYPE::PlaneSet planes;
	m_agent->getValue("clip planes", planes);
	if (planes.GetSize() != 6)
		return;
	planes[4].ChangePlane(FLTYPE::Point(0.0, 0.0, dval), FLTYPE::Vector(0.0, 0.0, 1.0));
	m_agent->setValue("clip planes", planes);
	//if (!m_vd)
	//	return;
	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);
	//vector<Plane*> *planes = 0;
	//if (m_vd->GetRenderer())
	//	planes = m_vd->GetRenderer()->get_planes();
	//if (!planes)
	//	return;
	//if (planes->size() != 6)
	//	return;

	//wxString str = m_z1_clip_text->GetValue();
	//long ival = 0;
	//str.ToLong(&ival);
	//int ival2 = m_z2_clip_sldr->GetValue();
	//double val, val2;

	//if (m_link_z)
	//{
	//	if (ival + m_z_sldr_dist > resz)
	//	{
	//		ival = resz - m_z_sldr_dist;
	//		ival2 = resz;
	//	}
	//	else
	//		ival2 = ival + m_z_sldr_dist;
	//}
	//else if (ival > ival2)
	//	ival = ival2;

	//val = (double)ival / (double)resz;
	////str = wxString::Format("%d", ival);
	////m_z1_clip_text->ChangeValue(str);
	//m_z1_clip_sldr->SetValue(ival);
	//int barsize = (m_z1_clip_sldr->GetSize().GetHeight() - 20);
	//m_zBar->SetPosition(wxPoint(20, 10 + val*barsize));
	//m_zBar->SetSize(wxSize(3, barsize*((double)
	//	(m_z2_clip_sldr->GetValue() - ival) / (double)m_z1_clip_sldr->GetMax())));
	//Plane* plane = (*planes)[4];
	//plane->ChangePlane(Point(0.0, 0.0, val), Vector(0.0, 0.0, 1.0));
	//if (m_link_z)
	//{
	//	val2 = (double)ival2 / (double)resz;
	//	str = wxString::Format("%d", ival2);
	//	m_z2_clip_text->ChangeValue(str);
	//	m_z2_clip_sldr->SetValue(ival2);
	//	plane = (*planes)[5];
	//	plane->ChangePlane(Point(0.0, 0.0, val2), Vector(0.0, 0.0, -1.0));
	//}

	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		size_t num = FL::Global::instance().getVolumeFactory().getNum();
	//		for (size_t i = 0; i<num; i++)
	//		{
	//			FL::VolumeData* vd =
	//				FL::Global::instance().getVolumeFactory().get(i);
	//			if (!vd || vd == m_vd)
	//				continue;

	//			planes = 0;
	//			if (m_vd->GetRenderer())
	//				planes = vd->GetRenderer()->get_planes();
	//			if (!planes)
	//				continue;
	//			if (planes->size() != 6)
	//				continue;

	//			(*planes)[4]->ChangePlane(Point(0.0, 0.0, val), Vector(0.0, 0.0, 1.0));
	//			if (m_link_z)
	//				(*planes)[5]->ChangePlane(Point(0.0, 0.0, val2), Vector(0.0, 0.0, -1.0));
	//		}
	//	}
	//}

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			if (m_link_z)
	//				vrv->m_glview->m_clip_mask = 48;
	//			else
	//				vrv->m_glview->m_clip_mask = 16;

	//			double rotx, roty, rotz;
	//			vrv->GetRotations(rotx, roty, rotz);
	//			vrv->SetRotations(rotx, roty, rotz);
	//		}
	//	}
	//}

	//RefreshVRenderViews(true);
}

void ClipPlanePanel::OnZ2ClipChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	int ival2 = m_z1_clip_sldr->GetValue();

	if (!m_link_z && ival<ival2)
	{
		ival = ival2;
		m_z2_clip_sldr->SetValue(ival);
	}
	m_z2_clip_text->SetValue(wxString::Format("%d", ival));
}

void ClipPlanePanel::OnZ2ClipEdit(wxCommandEvent &event)
{
	long resz;
	m_agent->getValue("res z", resz);

	wxString str = m_z2_clip_text->GetValue();
	long ival = 0;
	double dval = 0.0;
	str.ToLong(&ival);
	dval = (double)ival / (double)resz;
	m_z2_clip_sldr->SetValue(ival);
	int barsize = (m_z1_clip_sldr->GetSize().GetHeight() - 20);
	m_zBar->SetPosition(wxPoint(20, 10 + ((double)m_z1_clip_sldr->GetValue() /
		(double)m_z1_clip_sldr->GetMax())*barsize));
	m_zBar->SetSize(wxSize(3, barsize*((double)
		(ival - m_z1_clip_sldr->GetValue()) / (double)m_z1_clip_sldr->GetMax())));
	FLTYPE::PlaneSet planes;
	m_agent->getValue("clip planes", planes);
	if (planes.GetSize() != 6)
		return;
	planes[5].ChangePlane(FLTYPE::Point(0.0, 0.0, dval), FLTYPE::Vector(0.0, 0.0, -1.0));
	m_agent->setValue("clip planes", planes);
	//if (!m_vd)
	//	return;
	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);
	//vector<Plane*> *planes = 0;
	//if (m_vd->GetRenderer())
	//	planes = m_vd->GetRenderer()->get_planes();
	//if (!planes)
	//	return;
	//if (planes->size() != 6)
	//	return;

	//wxString str = m_z2_clip_text->GetValue();
	//long ival = 0;
	//str.ToLong(&ival);
	//int ival2 = m_z1_clip_sldr->GetValue();
	//double val, val2;

	//if (m_link_z)
	//{
	//	if (ival - m_z_sldr_dist < 0)
	//	{
	//		ival = m_z_sldr_dist;
	//		ival2 = 0;
	//	}
	//	else
	//		ival2 = ival - m_z_sldr_dist;
	//}
	//else if (ival < ival2)
	//	return;

	//val = (double)ival / (double)resz;
	////str = wxString::Format("%d", ival);
	////m_z2_clip_text->ChangeValue(str);
	//m_z2_clip_sldr->SetValue(ival);
	//int barsize = (m_z1_clip_sldr->GetSize().GetHeight() - 20);
	//m_zBar->SetPosition(wxPoint(20, 10 + ((double)m_z1_clip_sldr->GetValue() /
	//	(double)m_z1_clip_sldr->GetMax())*barsize));
	//m_zBar->SetSize(wxSize(3, barsize*((double)
	//	(ival - m_z1_clip_sldr->GetValue()) / (double)m_z1_clip_sldr->GetMax())));
	//Plane* plane = (*planes)[5];
	//plane->ChangePlane(Point(0.0, 0.0, val), Vector(0.0, 0.0, -1.0));
	//if (m_link_z)
	//{
	//	val2 = (double)ival2 / (double)resz;
	//	str = wxString::Format("%d", ival2);
	//	m_z1_clip_text->ChangeValue(str);
	//	m_z1_clip_sldr->SetValue(ival2);
	//	plane = (*planes)[4];
	//	plane->ChangePlane(Point(0.0, 0.0, val2), Vector(0.0, 0.0, 1.0));
	//}

	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		size_t num = FL::Global::instance().getVolumeFactory().getNum();
	//		for (size_t i = 0; i<num; i++)
	//		{
	//			FL::VolumeData* vd =
	//				FL::Global::instance().getVolumeFactory().get(i);
	//			if (!vd || vd == m_vd)
	//				continue;

	//			planes = 0;
	//			if (vd->GetRenderer())
	//				planes = vd->GetRenderer()->get_planes();
	//			if (!planes)
	//				continue;
	//			if (planes->size() != 6)
	//				continue;

	//			(*planes)[5]->ChangePlane(Point(0.0, 0.0, val), Vector(0.0, 0.0, -1.0));
	//			if (m_link_z)
	//				(*planes)[4]->ChangePlane(Point(0.0, 0.0, val2), Vector(0.0, 0.0, 1.0));
	//		}
	//	}
	//}

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			if (m_link_z)
	//				vrv->m_glview->m_clip_mask = 48;
	//			else
	//				vrv->m_glview->m_clip_mask = 32;

	//			double rotx, roty, rotz;
	//			vrv->GetRotations(rotx, roty, rotz);
	//			vrv->SetRotations(rotx, roty, rotz);
	//		}
	//	}
	//}

	//RefreshVRenderViews(true);
}

void ClipPlanePanel::OnIdle(wxIdleEvent &event)
{
	if (!IsShown())
		return;
	int sz = m_xpanel->GetSize().GetHeight();
	if (m_x1_clip_sldr->GetSize().GetHeight() != sz) {
		m_x1_clip_sldr->SetSize(20, sz);
		m_x2_clip_sldr->SetSize(20, sz);
		m_y1_clip_sldr->SetSize(20, sz);
		m_y2_clip_sldr->SetSize(20, sz);
		m_z1_clip_sldr->SetSize(20, sz);
		m_z2_clip_sldr->SetSize(20, sz);
		int barsize = sz - 20;
		//x
		int mx = m_x1_clip_sldr->GetMax();
		int v1 = m_x1_clip_sldr->GetValue();
		int v2 = m_x2_clip_sldr->GetValue();
		double clipSz = ((double)(v2 - v1)) / ((double)mx);
		double pct = ((double)v1) / ((double)mx);
		m_xBar->SetPosition(wxPoint(20, 10 + pct*barsize));
		m_xBar->SetSize(wxSize(3, barsize*clipSz));
		//y
		mx = m_y1_clip_sldr->GetMax();
		v1 = m_y1_clip_sldr->GetValue();
		v2 = m_y2_clip_sldr->GetValue();
		clipSz = ((double)(v2 - v1)) / ((double)mx);
		pct = ((double)v1) / ((double)mx);
		m_yBar->SetPosition(wxPoint(20, 10 + pct*barsize));
		m_yBar->SetSize(wxSize(3, barsize*clipSz));
		//z
		mx = m_z1_clip_sldr->GetMax();
		v1 = m_z1_clip_sldr->GetValue();
		v2 = m_z2_clip_sldr->GetValue();
		clipSz = ((double)(v2 - v1)) / ((double)mx);
		pct = ((double)v1) / ((double)mx);
		m_zBar->SetPosition(wxPoint(20, 10 + pct*barsize));
		m_zBar->SetSize(wxSize(3, barsize*clipSz));
	}

	if (m_hold_planes)
	{
		m_draw_clip = true;
		return;
	}

	//int i;
	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (!vrender_frame)
	//	return;

	//for (i = 0; i<vrender_frame->GetViewNum(); i++)
	//{
	//	VRenderView *vrv = vrender_frame->GetView(i);
	//	if (vrv)
	//	{
	//		if (vrv->m_glview->m_capture)
	//			return;
	//	}
	//}

	//wxPoint pos = wxGetMousePosition();
	//wxRect reg = GetScreenRect();
	//wxWindow *window = wxWindow::FindFocus();
	//if (window && reg.Contains(pos))
	//{
	//	if (!m_draw_clip)
	//	{
	//		vector <VRenderView*>* vrv_list = vrender_frame->GetViewList();
	//		for (i = 0; i<(int)vrv_list->size(); i++)
	//		{
	//			if ((*vrv_list)[i])
	//			{
	//				(*vrv_list)[i]->m_glview->m_draw_clip = true;
	//				(*vrv_list)[i]->m_glview->m_clip_mask = -1;
	//			}
	//		}
	//		RefreshVRenderViews();
	//		m_draw_clip = true;
	//	}
	//}
	//else
	//{
	//	if (m_draw_clip)
	//	{
	//		vector <VRenderView*>* vrv_list = vrender_frame->GetViewList();
	//		for (i = 0; i<(int)vrv_list->size(); i++)
	//		{
	//			if ((*vrv_list)[i])
	//				(*vrv_list)[i]->m_glview->m_draw_clip = false;
	//		}
	//		RefreshVRenderViews();
	//		m_draw_clip = false;
	//	}
	//}

}

void ClipPlanePanel::OnLinkXCheck(wxCommandEvent &event)
{
	m_link_x = m_check_tb->GetToolState(ID_LinkXChk);
	if (m_link_x)
	{
		m_x_sldr_dist = m_x2_clip_sldr->GetValue() -
			m_x1_clip_sldr->GetValue();
		m_check_tb->SetToolNormalBitmap(ID_LinkXChk,
			wxGetBitmapFromMemory(link));
	}
	else {
		m_check_tb->SetToolNormalBitmap(ID_LinkXChk,
			wxGetBitmapFromMemory(unlink));
	}
}

void ClipPlanePanel::OnLinkYCheck(wxCommandEvent &event)
{
	m_link_y = m_check_tb->GetToolState(ID_LinkYChk);
	if (m_link_y)
	{
		m_y_sldr_dist = m_y2_clip_sldr->GetValue() -
			m_y1_clip_sldr->GetValue();
		m_check_tb->SetToolNormalBitmap(ID_LinkYChk,
			wxGetBitmapFromMemory(link));
	}
	else {
		m_check_tb->SetToolNormalBitmap(ID_LinkYChk,
			wxGetBitmapFromMemory(unlink));
	}
}

void ClipPlanePanel::OnLinkZCheck(wxCommandEvent &event)
{
	m_link_z = m_check_tb->GetToolState(ID_LinkZChk);
	if (m_link_z)
	{
		m_z_sldr_dist = m_z2_clip_sldr->GetValue() -
			m_z1_clip_sldr->GetValue();
		m_check_tb->SetToolNormalBitmap(ID_LinkZChk,
			wxGetBitmapFromMemory(link));
	}
	else {
		m_check_tb->SetToolNormalBitmap(ID_LinkZChk,
			wxGetBitmapFromMemory(unlink));
	}
}

void ClipPlanePanel::OnSetZeroBtn(wxCommandEvent &event)
{
	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			vrv->SetClipMode(2);
	//			vrv->RefreshGL();
	//			double rotx, roty, rotz;
	//			vrv->GetClippingPlaneRotations(rotx, roty, rotz);
	//			m_x_rot_sldr->SetValue(int(rotx));
	//			m_y_rot_sldr->SetValue(int(roty));
	//			m_z_rot_sldr->SetValue(int(rotz));
	//			m_x_rot_text->ChangeValue(wxString::Format("%.1f", rotx));
	//			m_y_rot_text->ChangeValue(wxString::Format("%.1f", roty));
	//			m_z_rot_text->ChangeValue(wxString::Format("%.1f", rotz));
	//		}
	//	}
	//}
}

void ClipPlanePanel::OnRotResetBtn(wxCommandEvent &event)
{
	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			//reset rotations
	//			vrv->SetClippingPlaneRotations(0.0, 0.0, 0.0);
	//			vrv->RefreshGL();
	//		}
	//	}
	//}
	//wxString str = "0.0";
	//m_x_rot_sldr->SetValue(0);
	//m_x_rot_text->ChangeValue(str);
	//m_y_rot_sldr->SetValue(0);
	//m_y_rot_text->ChangeValue(str);
	//m_z_rot_sldr->SetValue(0);
	//m_z_rot_text->ChangeValue(str);
}

void ClipPlanePanel::OnXRotChange(wxScrollEvent &event)
{
	int val = event.GetPosition();
	wxString str = wxString::Format("%.1f", double(val));
	m_x_rot_text->SetValue(str);
}

void ClipPlanePanel::OnXRotEdit(wxCommandEvent &event)
{
	//wxString str = m_x_rot_text->GetValue();
	//double val = 0.0;
	//str.ToDouble(&val);
	//m_x_rot_sldr->SetValue(int(val));

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			double rotx, roty, rotz;
	//			vrv->GetClippingPlaneRotations(rotx, roty, rotz);
	//			vrv->SetClippingPlaneRotations(val, roty, rotz);
	//			vrv->RefreshGL();
	//		}
	//	}
	//}
}

void ClipPlanePanel::OnYRotChange(wxScrollEvent &event)
{
	int val = event.GetPosition();
	wxString str = wxString::Format("%.1f", double(val));
	m_y_rot_text->SetValue(str);
}

void ClipPlanePanel::OnYRotEdit(wxCommandEvent &event)
{
	//wxString str = m_y_rot_text->GetValue();
	//double val = 0.0;
	//str.ToDouble(&val);
	//m_y_rot_sldr->SetValue(int(val));

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			double rotx, roty, rotz;
	//			vrv->GetClippingPlaneRotations(rotx, roty, rotz);
	//			vrv->SetClippingPlaneRotations(rotx, val, rotz);
	//			vrv->RefreshGL();
	//		}
	//	}
	//}
}

void ClipPlanePanel::OnZRotChange(wxScrollEvent &event)
{
	int val = event.GetPosition();
	wxString str = wxString::Format("%.1f", double(val));
	m_z_rot_text->SetValue(str);
}

void ClipPlanePanel::OnZRotEdit(wxCommandEvent &event)
{
	//wxString str = m_z_rot_text->GetValue();
	//double val = 0.0;
	//str.ToDouble(&val);
	//m_z_rot_sldr->SetValue(int(val));

	//VRenderFrame* vrender_frame = (VRenderFrame*)m_frame;
	//if (vrender_frame)
	//{
	//	for (int i = 0; i<(int)vrender_frame->GetViewList()->size(); i++)
	//	{
	//		VRenderView *vrv = (*vrender_frame->GetViewList())[i];
	//		if (vrv)
	//		{
	//			double rotx, roty, rotz;
	//			vrv->GetClippingPlaneRotations(rotx, roty, rotz);
	//			vrv->SetClippingPlaneRotations(rotx, roty, val);
	//			vrv->RefreshGL();
	//		}
	//	}
	//}
}

void ClipPlanePanel::OnXRotSpinUp(wxSpinEvent& event)
{
	wxString str_val = m_x_rot_text->GetValue();
	double val;
	str_val.ToDouble(&val);
	val += 1.0;
	if (val > 180.0) val -= 360.0;
	if (val <-180.0) val += 360.0;
	wxString str = wxString::Format("%.1f", val);
	m_x_rot_text->SetValue(str);
}

void ClipPlanePanel::OnXRotSpinDown(wxSpinEvent& event)
{
	wxString str_val = m_x_rot_text->GetValue();
	double val;
	str_val.ToDouble(&val);
	val -= 1.0;
	if (val > 180.0) val -= 360.0;
	if (val <-180.0) val += 360.0;
	wxString str = wxString::Format("%.1f", val);
	m_x_rot_text->SetValue(str);
}

void ClipPlanePanel::OnYRotSpinUp(wxSpinEvent& event)
{
	wxString str_val = m_y_rot_text->GetValue();
	double val;
	str_val.ToDouble(&val);
	val += 1.0;
	if (val > 180.0) val -= 360.0;
	if (val <-180.0) val += 360.0;
	wxString str = wxString::Format("%.1f", val);
	m_y_rot_text->SetValue(str);
}

void ClipPlanePanel::OnYRotSpinDown(wxSpinEvent& event)
{
	wxString str_val = m_y_rot_text->GetValue();
	double val;
	str_val.ToDouble(&val);
	val -= 1.0;
	if (val > 180.0) val -= 360.0;
	if (val <-180.0) val += 360.0;
	wxString str = wxString::Format("%.1f", val);
	m_y_rot_text->SetValue(str);
}

void ClipPlanePanel::OnZRotSpinUp(wxSpinEvent& event)
{
	wxString str_val = m_z_rot_text->GetValue();
	double val;
	str_val.ToDouble(&val);
	val += 1.0;
	if (val > 180.0) val -= 360.0;
	if (val <-180.0) val += 360.0;
	wxString str = wxString::Format("%.1f", val);
	m_z_rot_text->SetValue(str);
}

void ClipPlanePanel::OnZRotSpinDown(wxSpinEvent& event)
{
	wxString str_val = m_z_rot_text->GetValue();
	double val;
	str_val.ToDouble(&val);
	val -= 1.0;
	if (val > 180.0) val -= 360.0;
	if (val <-180.0) val += 360.0;
	wxString str = wxString::Format("%.1f", val);
	m_z_rot_text->SetValue(str);
}

void ClipPlanePanel::OnSliderRClick(wxCommandEvent& event)
{
	//if (m_sel_type != 2 || !m_vd)
	//	return;

	//int id = event.GetId();

	//wxString str;
	//double val;

	////good rate
	//double sample_rate;
	//m_vd->getValue("sample rate", sample_rate);
	//if (sample_rate < 2.0)
	//{
	//	sample_rate = 2.0;
	//	m_vd->setValue("sample rate", sample_rate);
	//}
	//if (m_toolbar->GetToolState(ID_LinkChannelsBtn))
	//{
	//	if (m_mgr)
	//	{
	//		//int i;
	//		//for (i=0; i<m_mgr->GetVolumeNum(); i++)
	//		//{
	//		//	VolumeData* vd = m_mgr->GetVolumeData(i);
	//		//	if (!vd || vd == m_vd)
	//		//		continue;
	//		//	if (vd->GetSampleRate()<2.0)
	//		//		vd->SetSampleRate(2.0);
	//		//}
	//	}
	//}

	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//if (id == ID_X1ClipSldr)
	//{
	//	str = m_x1_clip_text->GetValue();
	//	str.ToDouble(&val);
	//	if (val < resx)
	//	{
	//		SetXLink(false);
	//		m_x2_clip_text->SetValue(
	//			wxString::Format("%d", (int)(val + 1.0)));
	//		SetXLink(true);
	//	}
	//	m_x1_clip_sldr->SetFocus();
	//}
	//else if (id == ID_X2ClipSldr)
	//{
	//	str = m_x2_clip_text->GetValue();
	//	str.ToDouble(&val);
	//	if (val > 0)
	//	{
	//		SetXLink(false);
	//		m_x1_clip_text->SetValue(
	//			wxString::Format("%d", (int)(val - 1.0)));
	//		SetXLink(true);
	//	}
	//	m_x2_clip_sldr->SetFocus();
	//}
	//else if (id == ID_Y1ClipSldr)
	//{
	//	str = m_y1_clip_text->GetValue();
	//	str.ToDouble(&val);
	//	if (val < resy)
	//	{
	//		SetYLink(false);
	//		m_y2_clip_text->SetValue(
	//			wxString::Format("%d", (int)(val + 1.0)));
	//		SetYLink(true);
	//	}
	//	m_y1_clip_sldr->SetFocus();
	//}
	//else if (id == ID_Y2ClipSldr)
	//{
	//	str = m_y2_clip_text->GetValue();
	//	str.ToDouble(&val);
	//	if (val > 0)
	//	{
	//		SetYLink(false);
	//		m_y1_clip_text->SetValue(
	//			wxString::Format("%d", (int)(val - 1.0)));
	//		SetYLink(true);
	//	}
	//	m_y2_clip_sldr->SetFocus();
	//}
	//else if (id == ID_Z1ClipSldr)
	//{
	//	str = m_z1_clip_text->GetValue();
	//	str.ToDouble(&val);
	//	if (val < resz)
	//	{
	//		SetZLink(false);
	//		m_z2_clip_text->SetValue(
	//			wxString::Format("%d", (int)(val + 1.0)));
	//		SetZLink(true);
	//	}
	//	m_z1_clip_sldr->SetFocus();
	//}
	//else if (id == ID_Z2ClipSldr)
	//{
	//	str = m_z2_clip_text->GetValue();
	//	str.ToDouble(&val);
	//	if (val > 0)
	//	{
	//		SetZLink(false);
	//		m_z1_clip_text->SetValue(
	//			wxString::Format("%d", (int)(val - 1.0)));
	//		SetZLink(true);
	//	}
	//	m_z2_clip_sldr->SetFocus();
	//}

	////reset others
	//vector<Plane*> *planes = 0;
	//if (m_vd->GetRenderer())
	//	planes = m_vd->GetRenderer()->get_planes();
	//if (!planes)
	//	return;
	//if (planes->size() != 6)
	//	return;

	//if (id == ID_X1ClipSldr ||
	//	id == ID_X2ClipSldr)
	//{
	//	m_link_y = false;
	//	m_check_tb->ToggleTool(ID_LinkYChk, false);
	//	m_check_tb->SetToolNormalBitmap(ID_LinkYChk,
	//		wxGetBitmapFromMemory(unlink));
	//	m_link_z = false;
	//	m_check_tb->ToggleTool(ID_LinkZChk, false);
	//	m_check_tb->SetToolNormalBitmap(ID_LinkZChk,
	//		wxGetBitmapFromMemory(unlink));
	//	m_y1_clip_text->SetValue("0");
	//	m_y2_clip_text->SetValue(
	//		wxString::Format("%d", resy));
	//	m_z1_clip_text->SetValue("0");
	//	m_z2_clip_text->SetValue(
	//		wxString::Format("%d", resz));
	//}
	//else if (id == ID_Y1ClipSldr ||
	//	id == ID_Y2ClipSldr)
	//{
	//	m_link_x = false;
	//	m_check_tb->ToggleTool(ID_LinkXChk, false);
	//	m_check_tb->SetToolNormalBitmap(ID_LinkXChk,
	//		wxGetBitmapFromMemory(unlink));
	//	m_link_z = false;
	//	m_check_tb->ToggleTool(ID_LinkZChk, false);
	//	m_check_tb->SetToolNormalBitmap(ID_LinkZChk,
	//		wxGetBitmapFromMemory(unlink));
	//	m_x1_clip_text->SetValue("0");
	//	m_x2_clip_text->SetValue(
	//		wxString::Format("%d", resx));
	//	m_z1_clip_text->SetValue("0");
	//	m_z2_clip_text->SetValue(
	//		wxString::Format("%d", resz));
	//}
	//else if (id == ID_Z1ClipSldr ||
	//	id == ID_Z2ClipSldr)
	//{
	//	m_link_x = false;
	//	m_check_tb->ToggleTool(ID_LinkXChk, false);
	//	m_check_tb->SetToolNormalBitmap(ID_LinkXChk,
	//		wxGetBitmapFromMemory(unlink));
	//	m_link_y = false;
	//	m_check_tb->ToggleTool(ID_LinkYChk, false);
	//	m_check_tb->SetToolNormalBitmap(ID_LinkYChk,
	//		wxGetBitmapFromMemory(unlink));
	//	m_x1_clip_text->SetValue("0");
	//	m_x2_clip_text->SetValue(
	//		wxString::Format("%d", resx));
	//	m_y1_clip_text->SetValue("0");
	//	m_y2_clip_text->SetValue(
	//		wxString::Format("%d", resy));
	//}
}

void ClipPlanePanel::OnYZClipBtn(wxCommandEvent& event)
{
	//if (m_sel_type != 2 || !m_vd)
	//	return;

	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//wxString str = m_yz_dist_text->GetValue();
	//long dist;
	//str.ToLong(&dist);

	////reset yz
	//m_link_y = false;
	//m_check_tb->ToggleTool(ID_LinkYChk, false);
	//m_check_tb->SetToolNormalBitmap(ID_LinkYChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_link_z = false;
	//m_check_tb->ToggleTool(ID_LinkZChk, false);
	//m_check_tb->SetToolNormalBitmap(ID_LinkZChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_y1_clip_text->SetValue("0");
	//m_y2_clip_text->SetValue(
	//	wxString::Format("%d", resy));
	//m_z1_clip_text->SetValue("0");
	//m_z2_clip_text->SetValue(
	//	wxString::Format("%d", resz));

	////set x
	//SetXLink(false);
	//if (dist < resx)
	//{
	//	int x1 = resx / 2 - dist / 2;
	//	int x2 = x1 + dist;
	//	m_x1_clip_text->SetValue(
	//		wxString::Format("%d", x1));
	//	m_x2_clip_text->SetValue(
	//		wxString::Format("%d", x2));
	//	SetXLink(true);
	//	//int distx, disty, distz;
	//	//m_vd->GetClipDistance(distx, disty, distz);
	//	//m_vd->SetClipDistance(dist, disty, distz);
	//	m_vd->setValue("clip dist x", dist);
	//}
	//else
	//{
	//	m_x1_clip_text->SetValue("0");
	//	m_x2_clip_text->SetValue(
	//		wxString::Format("%d", resx));
	//}

	//m_x1_clip_sldr->SetFocus();
}

void ClipPlanePanel::OnXZClipBtn(wxCommandEvent& event)
{
	//if (m_sel_type != 2 || !m_vd)
	//	return;

	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//wxString str = m_xz_dist_text->GetValue();
	//long dist;
	//str.ToLong(&dist);

	////reset xz
	//m_link_x = false;
	//m_check_tb->ToggleTool(ID_LinkXChk, false);
	//m_check_tb->SetToolNormalBitmap(ID_LinkXChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_link_z = false;
	//m_check_tb->ToggleTool(ID_LinkZChk, false);
	//m_check_tb->SetToolNormalBitmap(ID_LinkZChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_x1_clip_text->SetValue("0");
	//m_x2_clip_text->SetValue(
	//	wxString::Format("%d", resx));
	//m_z1_clip_text->SetValue("0");
	//m_z2_clip_text->SetValue(
	//	wxString::Format("%d", resz));

	////set y
	//SetYLink(false);
	//if (dist < resy)
	//{
	//	int y1 = resy / 2 - dist / 2;
	//	int y2 = y1 + dist;
	//	m_y1_clip_text->SetValue(
	//		wxString::Format("%d", y1));
	//	m_y2_clip_text->SetValue(
	//		wxString::Format("%d", y2));
	//	SetYLink(true);
	//	//int distx, disty, distz;
	//	//m_vd->GetClipDistance(distx, disty, distz);
	//	//m_vd->SetClipDistance(distx, dist, distz);
	//	m_vd->setValue("clip dist y", dist);
	//}
	//else
	//{
	//	m_y1_clip_text->SetValue("0");
	//	m_y2_clip_text->SetValue(
	//		wxString::Format("%d", resy));
	//}

	//m_y1_clip_sldr->SetFocus();
}

void ClipPlanePanel::OnXYClipBtn(wxCommandEvent& event)
{
	//if (m_sel_type != 2 || !m_vd)
	//	return;

	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//wxString str = m_xy_dist_text->GetValue();
	//long dist;
	//str.ToLong(&dist);

	////reset xy
	//m_link_x = false;
	//m_check_tb->ToggleTool(ID_LinkXChk, false);
	//m_check_tb->SetToolNormalBitmap(ID_LinkXChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_link_y = false;
	//m_check_tb->ToggleTool(ID_LinkYChk, false);
	//m_check_tb->SetToolNormalBitmap(ID_LinkYChk,
	//	wxGetBitmapFromMemory(unlink));
	//m_x1_clip_text->SetValue("0");
	//m_x2_clip_text->SetValue(
	//	wxString::Format("%d", resx));
	//m_y1_clip_text->SetValue("0");
	//m_y2_clip_text->SetValue(
	//	wxString::Format("%d", resy));

	////set z
	//SetZLink(false);
	//if (dist < resz)
	//{
	//	int z1 = resz / 2 - dist / 2;
	//	int z2 = z1 + dist;
	//	m_z1_clip_text->SetValue(
	//		wxString::Format("%d", z1));
	//	m_z2_clip_text->SetValue(
	//		wxString::Format("%d", z2));
	//	SetZLink(true);
	//	//int distx, disty, distz;
	//	//m_vd->GetClipDistance(distx, disty, distz);
	//	//m_vd->SetClipDistance(distx, disty, dist);
	//	m_vd->setValue("clip dist z", dist);
	//}
	//else
	//{
	//	m_z1_clip_text->SetValue("0");
	//	m_z2_clip_text->SetValue(
	//		wxString::Format("%d", resz));
	//}

	//m_z1_clip_sldr->SetFocus();
}

//move linked clipping planes
//dir: 0-lower; 1-higher
void ClipPlanePanel::MoveLinkedClippingPlanes(int dir)
{
	//if (m_sel_type != 2 || !m_vd)
	//	return;

	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//wxString str;
	//long dist;

	//if (dir == 0)
	//{
	//	//moving lower
	//	if (m_link_x)
	//	{
	//		str = m_yz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_x1_clip_text->GetValue();
	//		long x1;
	//		str.ToLong(&x1);
	//		x1 = x1 - dist + 1;
	//		x1 = x1<0 ? 0 : x1;
	//		m_x1_clip_text->SetValue(
	//			wxString::Format("%d", x1));
	//	}
	//	if (m_link_y)
	//	{
	//		str = m_xz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_y1_clip_text->GetValue();
	//		long y1;
	//		str.ToLong(&y1);
	//		y1 = y1 - dist + 1;
	//		y1 = y1<0 ? 0 : y1;
	//		m_y1_clip_text->SetValue(
	//			wxString::Format("%d", y1));
	//	}
	//	if (m_link_z)
	//	{
	//		str = m_xy_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_z1_clip_text->GetValue();
	//		long z1;
	//		str.ToLong(&z1);
	//		z1 = z1 - dist + 1;
	//		z1 = z1<0 ? 0 : z1;
	//		m_z1_clip_text->SetValue(
	//			wxString::Format("%d", z1));
	//	}
	//}
	//else
	//{
	//	//moving higher
	//	if (m_link_x)
	//	{
	//		str = m_yz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_x2_clip_text->GetValue();
	//		long x2;
	//		str.ToLong(&x2);
	//		x2 = x2 + dist - 1;
	//		x2 = x2>resx ? resx : x2;
	//		m_x2_clip_text->SetValue(
	//			wxString::Format("%d", x2));
	//	}
	//	if (m_link_y)
	//	{
	//		str = m_xz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_y2_clip_text->GetValue();
	//		long y2;
	//		str.ToLong(&y2);
	//		y2 = y2 + dist - 1;
	//		y2 = y2>resy ? resy : y2;
	//		m_y2_clip_text->SetValue(
	//			wxString::Format("%d", y2));
	//	}
	//	if (m_link_z)
	//	{
	//		str = m_xy_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_z2_clip_text->GetValue();
	//		long z2;
	//		str.ToLong(&z2);
	//		z2 = z2 + dist - 1;
	//		z2 = z2>resz ? resz : z2;
	//		m_z2_clip_text->SetValue(
	//			wxString::Format("%d", z2));
	//	}
	//}
}

void ClipPlanePanel::OnSliderKeyDown(wxKeyEvent& event)
{
	//if (m_sel_type != 2 || !m_vd)
	//	return;

	//long resx, resy, resz;
	////m_vd->GetResolution(resx, resy, resz);
	//m_vd->getValue("res x", resx);
	//m_vd->getValue("res y", resy);
	//m_vd->getValue("res z", resz);

	//int id = event.GetId();
	//int key = event.GetKeyCode();

	//wxString str;
	//long dist;

	//if (key == wxKeyCode(','))
	//{
	//	if (id == ID_X1ClipSldr ||
	//		id == ID_X2ClipSldr)
	//	{
	//		if (!m_link_x)
	//		{
	//			event.Skip();
	//			return;
	//		}

	//		str = m_yz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_x1_clip_text->GetValue();
	//		long x1;
	//		str.ToLong(&x1);
	//		x1 = x1 - dist + 1;
	//		x1 = x1<0 ? 0 : x1;
	//		m_x1_clip_text->SetValue(
	//			wxString::Format("%d", x1));
	//	}
	//	else if (id == ID_Y1ClipSldr ||
	//		id == ID_Y2ClipSldr)
	//	{
	//		if (!m_link_y)
	//		{
	//			event.Skip();
	//			return;
	//		}

	//		str = m_xz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_y1_clip_text->GetValue();
	//		long y1;
	//		str.ToLong(&y1);
	//		y1 = y1 - dist + 1;
	//		y1 = y1<0 ? 0 : y1;
	//		m_y1_clip_text->SetValue(
	//			wxString::Format("%d", y1));
	//	}
	//	else if (id == ID_Z1ClipSldr ||
	//		id == ID_Z2ClipSldr)
	//	{
	//		if (!m_link_z)
	//		{
	//			event.Skip();
	//			return;
	//		}

	//		str = m_xy_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_z1_clip_text->GetValue();
	//		long z1;
	//		str.ToLong(&z1);
	//		z1 = z1 - dist + 1;
	//		z1 = z1<0 ? 0 : z1;
	//		m_z1_clip_text->SetValue(
	//			wxString::Format("%d", z1));
	//	}
	//}
	//else if (key == wxKeyCode('.'))
	//{
	//	if (id == ID_X1ClipSldr ||
	//		id == ID_X2ClipSldr)
	//	{
	//		if (!m_link_x)
	//		{
	//			event.Skip();
	//			return;
	//		}

	//		str = m_yz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_x2_clip_text->GetValue();
	//		long x2;
	//		str.ToLong(&x2);
	//		x2 = x2 + dist - 1;
	//		x2 = x2>resx ? resx : x2;
	//		m_x2_clip_text->SetValue(
	//			wxString::Format("%d", x2));
	//	}
	//	else if (id == ID_Y1ClipSldr ||
	//		id == ID_Y2ClipSldr)
	//	{
	//		if (!m_link_y)
	//		{
	//			event.Skip();
	//			return;
	//		}

	//		str = m_xz_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_y2_clip_text->GetValue();
	//		long y2;
	//		str.ToLong(&y2);
	//		y2 = y2 + dist - 1;
	//		y2 = y2>resy ? resy : y2;
	//		m_y2_clip_text->SetValue(
	//			wxString::Format("%d", y2));
	//	}
	//	else if (id == ID_Z1ClipSldr ||
	//		id == ID_Z2ClipSldr)
	//	{
	//		if (!m_link_z)
	//		{
	//			event.Skip();
	//			return;
	//		}

	//		str = m_xy_dist_text->GetValue();
	//		str.ToLong(&dist);
	//		dist = dist<2 ? dist + 1 : dist;
	//		str = m_z2_clip_text->GetValue();
	//		long z2;
	//		str.ToLong(&z2);
	//		z2 = z2 + dist - 1;
	//		z2 = z2>resz ? resz : z2;
	//		m_z2_clip_text->SetValue(
	//			wxString::Format("%d", z2));
	//	}
	//}

	event.Skip();
}

void ClipPlanePanel::EnableAll()
{
	m_toolbar->Enable();
	m_set_zero_btn->Enable();
	m_rot_reset_btn->Enable();
	m_x_rot_sldr->Enable();
	m_y_rot_sldr->Enable();
	m_z_rot_sldr->Enable();
	m_x_rot_text->Enable();
	m_y_rot_text->Enable();
	m_z_rot_text->Enable();
	m_x_rot_spin->Enable();
	m_y_rot_spin->Enable();
	m_z_rot_spin->Enable();
	m_x1_clip_sldr->Enable();
	m_x1_clip_text->Enable();
	m_x2_clip_sldr->Enable();
	m_x2_clip_text->Enable();
	m_y1_clip_sldr->Enable();
	m_y1_clip_text->Enable();
	m_y2_clip_sldr->Enable();
	m_y2_clip_text->Enable();
	m_z1_clip_sldr->Enable();
	m_z1_clip_text->Enable();
	m_z2_clip_sldr->Enable();
	m_z2_clip_text->Enable();
	m_check_tb->Enable();
	m_clip_reset_btn->Enable();
	m_yz_clip_btn->Enable();
	m_xz_clip_btn->Enable();
	m_xy_clip_btn->Enable();
	m_yz_dist_text->Enable();
	m_xz_dist_text->Enable();
	m_xy_dist_text->Enable();
}

void ClipPlanePanel::DisableAll()
{
	m_toolbar->Disable();
	m_set_zero_btn->Disable();
	m_rot_reset_btn->Disable();
	m_x_rot_sldr->Disable();
	m_y_rot_sldr->Disable();
	m_z_rot_sldr->Disable();
	m_x_rot_text->Disable();
	m_y_rot_text->Disable();
	m_z_rot_text->Disable();
	m_x_rot_spin->Disable();
	m_y_rot_spin->Disable();
	m_z_rot_spin->Disable();
	m_x1_clip_sldr->Disable();
	m_x1_clip_text->Disable();
	m_x2_clip_sldr->Disable();
	m_x2_clip_text->Disable();
	m_y1_clip_sldr->Disable();
	m_y1_clip_text->Disable();
	m_y2_clip_sldr->Disable();
	m_y2_clip_text->Disable();
	m_z1_clip_sldr->Disable();
	m_z1_clip_text->Disable();
	m_z2_clip_sldr->Disable();
	m_z2_clip_text->Disable();
	m_check_tb->Disable();
	m_clip_reset_btn->Disable();
	m_yz_clip_btn->Disable();
	m_xz_clip_btn->Disable();
	m_xy_clip_btn->Disable();
	m_yz_dist_text->Disable();
	m_xz_dist_text->Disable();
	m_xy_dist_text->Disable();
}