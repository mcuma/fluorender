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
#include <Fui/VolumePropPanel.h>
#include <Global/Global.h>
#include <wx/valnum.h>
#include <png_resource.h>
#include <img/icons.h>

using namespace FUI;

BEGIN_EVENT_TABLE(VolumePropPanel, wxPanel)
//1
EVT_COMMAND_SCROLL(ID_GammaSldr, VolumePropPanel::OnGammaChange)
EVT_TEXT(ID_GammaText, VolumePropPanel::OnGammaText)
EVT_COMMAND_SCROLL(ID_BoundarySldr, VolumePropPanel::OnBoundaryChange)
EVT_TEXT(ID_BoundaryText, VolumePropPanel::OnBoundaryText)
//2
EVT_COMMAND_SCROLL(ID_SaturationSldr, VolumePropPanel::OnSaturationChange)
EVT_TEXT(ID_SaturationText, VolumePropPanel::OnSaturationText)
EVT_COMMAND_SCROLL(ID_LeftThreshSldr, VolumePropPanel::OnLeftThreshChange)
EVT_TEXT(ID_LeftThreshText, VolumePropPanel::OnLeftThreshText)
EVT_COMMAND_SCROLL(ID_RightThreshSldr, VolumePropPanel::OnRightThreshChange)
EVT_TEXT(ID_RightThreshText, VolumePropPanel::OnRightThreshText)
//3
EVT_COMMAND_SCROLL(ID_LuminanceSldr, VolumePropPanel::OnLuminanceChange)
EVT_TEXT(ID_LuminanceText, VolumePropPanel::OnLuminanceText)
EVT_TOOL(ID_ShadowChk, VolumePropPanel::OnShadowEnable)
EVT_COMMAND_SCROLL(ID_ShadowSldr, VolumePropPanel::OnShadowChange)
EVT_TEXT(ID_ShadowText, VolumePropPanel::OnShadowText)
EVT_COMMAND_SCROLL(ID_HiShadingSldr, VolumePropPanel::OnHiShadingChange)
EVT_TEXT(ID_HiShadingText, VolumePropPanel::OnHiShadingText)
//4
EVT_TOOL(ID_AlphaChk, VolumePropPanel::OnAlphaCheck)
EVT_COMMAND_SCROLL(ID_AlphaSldr, VolumePropPanel::OnAlphaChange)
EVT_TEXT(ID_Alpha_Text, VolumePropPanel::OnAlphaText)
EVT_COMMAND_SCROLL(ID_SampleSldr, VolumePropPanel::OnSampleChange)
EVT_TEXT(ID_SampleText, VolumePropPanel::OnSampleText)
//5
EVT_COMMAND_SCROLL(ID_LowShadingSldr, VolumePropPanel::OnLowShadingChange)
EVT_TEXT(ID_LowShadingText, VolumePropPanel::OnLowShadingText)
EVT_TOOL(ID_ShadingEnableChk, VolumePropPanel::OnShadingEnable)
//colormap
EVT_TOOL(ID_ColormapEnableChk, VolumePropPanel::OnEnableColormap)
EVT_COMMAND_SCROLL(ID_ColormapHighValueSldr, VolumePropPanel::OnColormapHighValueChange)
EVT_TEXT(ID_ColormapHighValueText, VolumePropPanel::OnColormapHighValueText)
EVT_COMMAND_SCROLL(ID_ColormapLowValueSldr, VolumePropPanel::OnColormapLowValueChange)
EVT_TEXT(ID_ColormapLowValueText, VolumePropPanel::OnColormapLowValueText)
EVT_COMBOBOX(ID_ColormapCombo, VolumePropPanel::OnColormapCombo)
EVT_COMBOBOX(ID_ColormapCombo2, VolumePropPanel::OnColormapCombo2)
//6
//color 1
EVT_TEXT(ID_ColorText, VolumePropPanel::OnColorTextChange)
EVT_COLOURPICKER_CHANGED(ID_ColorBtn, VolumePropPanel::OnColorBtn)
EVT_TEXT(ID_Color2Text, VolumePropPanel::OnColor2TextChange)
EVT_COLOURPICKER_CHANGED(ID_Color2Btn, VolumePropPanel::OnColor2Btn)
//spacings
EVT_TEXT(ID_SpaceXText, VolumePropPanel::OnSpaceText)
EVT_TEXT(ID_SpaceYText, VolumePropPanel::OnSpaceText)
EVT_TEXT(ID_SpaceZText, VolumePropPanel::OnSpaceText)
//legend
EVT_TOOL(ID_LegendChk, VolumePropPanel::OnLegendCheck)
//EVT_TOOL
EVT_TOOL(ID_InterpolateChk, VolumePropPanel::OnInterpolateCheck)
//sync within group
EVT_TOOL(ID_SyncGroupChk, VolumePropPanel::OnSyncGroupCheck)
//save default
EVT_TOOL(ID_SaveDefault, VolumePropPanel::OnSaveDefault)
EVT_TOOL(ID_ResetDefault, VolumePropPanel::OnResetDefault)
//inversion
EVT_TOOL(ID_InvChk, VolumePropPanel::OnInvCheck)
//MIP
EVT_TOOL(ID_MipChk, VolumePropPanel::OnMIPCheck)
//noise reduction
EVT_TOOL(ID_NRChk, VolumePropPanel::OnNRCheck)
//depth mode
EVT_TOOL(ID_DepthChk, VolumePropPanel::OnDepthCheck)
END_EVENT_TABLE()

VolumePropPanel::VolumePropPanel(wxWindow* frame,
	wxWindow* parent,
	wxWindowID id,
	const wxPoint& pos,
	const wxSize& size,
	long style,
	const wxString& name) :
	wxPanel(parent, id, pos, size, style, name),
	m_frame(frame),
	m_max_val(255.0),
	m_space_x_text(0),
	m_space_y_text(0),
	m_space_z_text(0)
{
	// temporarily block events during constructor:
	wxEventBlocker blocker(this);

	m_agent = 
		FL::Global::instance().getAgentFactory().
		getOrAddVolumePropAgent("VolumePropAgent", *this);

	wxBoxSizer* sizer_all = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_left = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_middle = new wxBoxSizer(wxVERTICAL);
	wxBoxSizer* sizer_right = new wxBoxSizer(wxVERTICAL);

	wxBoxSizer* sizer_l1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_l2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_l3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_l4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_l5 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* sizer_m1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_m2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_m3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_m4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_m5 = new wxBoxSizer(wxHORIZONTAL);

	wxBoxSizer* sizer_r1 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_r2 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_r3 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_r4 = new wxBoxSizer(wxHORIZONTAL);
	wxBoxSizer* sizer_r5 = new wxBoxSizer(wxHORIZONTAL);

	wxStaticText* st = 0;

	//validator: floating point 1
	wxFloatingPointValidator<double> vald_fp1(1);
	//validator: floating point 2
	wxFloatingPointValidator<double> vald_fp2(2);
	//validator: floating point 3
	wxFloatingPointValidator<double> vald_fp3(3);
	//validator: floating point 4
	wxFloatingPointValidator<double> vald_fp4(4);
	//validator: integer
	wxIntegerValidator<unsigned int> vald_int;

	//left///////////////////////////////////////////////////
	//gamma
	m_gamma_st = new wxStaticText(this, ID_GammaSync, " : Gamma",
		wxDefaultPosition, wxSize(100, -1));
	m_gamma_st->Connect(ID_GammaSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnGammaSync), NULL, this);
	m_gamma_st->Connect(ID_GammaSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnGammaSync), NULL, this);
	m_gamma_sldr = new wxSlider(this, ID_GammaSldr, 100, 10, 400,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL | wxSL_INVERSE);
	m_gamma_text = new wxTextCtrl(this, ID_GammaText, "1.00",
		wxDefaultPosition, wxSize(40, 20), 0, vald_fp2);
	sizer_l1->Add(m_gamma_sldr, 1, wxEXPAND);
	sizer_l1->Add(m_gamma_text, 0, wxALIGN_CENTER);
	sizer_l1->Add(m_gamma_st, 0, wxALIGN_CENTER);
	//saturation point
	m_saturation_st = new wxStaticText(this, ID_SaturationSync, " : Saturation",
		wxDefaultPosition, wxSize(100, -1));
	m_saturation_st->Connect(ID_SaturationSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnSaturationSync), NULL, this);
	m_saturation_st->Connect(ID_SaturationSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnSaturationSync), NULL, this);
	m_saturation_sldr = new wxSlider(this, ID_SaturationSldr, 255, 0, 255,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_saturation_text = new wxTextCtrl(this, ID_SaturationText, "50",
		wxDefaultPosition, wxSize(40, 20)/*, 0, vald_int*/);
	sizer_l2->Add(m_saturation_sldr, 1, wxEXPAND);
	sizer_l2->Add(m_saturation_text, 0, wxALIGN_CENTER);
	sizer_l2->Add(m_saturation_st, 0, wxALIGN_CENTER);
	//luminance
	m_luminance_st = new wxStaticText(this, ID_LuminanceSync, " : Luminance",
		wxDefaultPosition, wxSize(100, -1));
	m_luminance_st->Connect(ID_LuminanceSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnLuminanceSync), NULL, this);
	m_luminance_st->Connect(ID_LuminanceSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnLuminanceSync), NULL, this);
	m_luminance_sldr = new wxSlider(this, ID_LuminanceSldr, 128, 0, 255,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_luminance_text = new wxTextCtrl(this, ID_LuminanceText, "128",
		wxDefaultPosition, wxSize(40, 20)/*, 0, vald_int*/);
	sizer_l3->Add(m_luminance_sldr, 1, wxEXPAND, 0);
	sizer_l3->Add(m_luminance_text, 0, wxALIGN_CENTER, 0);
	sizer_l3->Add(m_luminance_st, 0, wxALIGN_CENTER, 0);
	//alpha
	m_alpha_tool = new wxToolBar(this, ID_AlphaSync,
		wxDefaultPosition, wxDefaultSize, wxTB_NODIVIDER);
	wxBitmap bitmap = wxGetBitmapFromMemory(alpha);
#ifdef _DARWIN
	m_alpha_tool->SetToolBitmapSize(bitmap.GetSize());
#endif
	m_alpha_tool->AddCheckTool(ID_AlphaChk, "Alpha",
		bitmap, wxNullBitmap,
		"Enables Alpha Editing.",
		"Enables Alpha Editing.");
	m_alpha_tool->ToggleTool(ID_AlphaChk, true);
	m_alpha_tool->Realize();
	m_alpha_tool->Connect(ID_AlphaSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnAlphaSync), NULL, this);
	m_alpha_tool->Connect(ID_AlphaSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnAlphaSync), NULL, this);
	m_alpha_sldr = new wxSlider(this, ID_AlphaSldr, 127, 0, 255,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_alpha_text = new wxTextCtrl(this, ID_Alpha_Text, "127",
		wxDefaultPosition, wxSize(40, 20), 0, vald_int);
	sizer_l4->Add(m_alpha_sldr, 1, wxEXPAND);
	sizer_l4->Add(m_alpha_text, 0, wxALIGN_CENTER);
	sizer_l4->Add(new wxStaticText(this, 0, " : ",
		wxDefaultPosition, wxSize(13, -1)), 0, wxALIGN_CENTER);
	sizer_l4->Add(m_alpha_tool, 0, wxALIGN_CENTER);
	sizer_l4->Add(30, 10, 0);
	//shading
	m_low_shading_sldr = new wxSlider(this, ID_LowShadingSldr, 0, 0, 200,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_low_shading_text = new wxTextCtrl(this, ID_LowShadingText, "0.00",
		wxDefaultPosition, wxSize(40, 20), 0, vald_fp2);
	m_shade_tool = new wxToolBar(this, ID_ShadingSync,
		wxDefaultPosition, wxDefaultSize, wxTB_NODIVIDER);
	bitmap = wxGetBitmapFromMemory(shade);
#ifdef _DARWIN
	m_shade_tool->SetToolBitmapSize(bitmap.GetSize());
#endif
	m_shade_tool->AddCheckTool(ID_ShadingEnableChk, "Shading",
		bitmap, wxNullBitmap,
		"Enables Shading Editing.",
		"Enables Shading Editing.");
	m_shade_tool->ToggleTool(ID_ShadingEnableChk, true);
	m_shade_tool->Realize();
	m_shade_tool->Connect(ID_ShadingSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnShadingSync), NULL, this);
	m_shade_tool->Connect(ID_ShadingSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnShadingSync), NULL, this);
	sizer_l5->Add(m_low_shading_sldr, 1, wxEXPAND);
	sizer_l5->Add(m_low_shading_text, 0, wxALIGN_CENTER);
	sizer_l5->Add(new wxStaticText(this, 0, " : ",
		wxDefaultPosition, wxSize(13, -1)), 0, wxALIGN_CENTER);
	sizer_l5->Add(m_shade_tool, 0, wxALIGN_CENTER);
	sizer_l5->Add(30, 10, 0);
	//middle///////////////////////////////////////////////////
	//extract boundary
	m_boundary_st = new wxStaticText(this, ID_BoundarySync, "Extract Boundary : ",
		wxDefaultPosition, wxSize(127, -1), wxALIGN_RIGHT);
	m_boundary_st->Connect(ID_BoundarySync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnBoundarySync), NULL, this);
	m_boundary_st->Connect(ID_BoundarySync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnBoundarySync), NULL, this);
	m_boundary_sldr = new wxSlider(this, ID_BoundarySldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_boundary_text = new wxTextCtrl(this, ID_BoundaryText, "0.0000",
		wxDefaultPosition, wxSize(50, 20), 0, vald_fp4);
	sizer_m1->Add(m_boundary_st, 0, wxALIGN_CENTER);
	sizer_m1->Add(m_boundary_text, 0, wxALIGN_CENTER);
	sizer_m1->Add(m_boundary_sldr, 1, wxEXPAND);
	//thresholds
	m_threh_st = new wxStaticText(this, ID_ThreshSync, "Threshold : ",
		wxDefaultPosition, wxSize(127, -1), wxALIGN_RIGHT);
	m_threh_st->Connect(ID_ThreshSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnThreshSync), NULL, this);
	m_threh_st->Connect(ID_ThreshSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnThreshSync), NULL, this);
	m_left_thresh_sldr = new wxSlider(this, ID_LeftThreshSldr, 5, 0, 255,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_left_thresh_text = new wxTextCtrl(this, ID_LeftThreshText, "5",
		wxDefaultPosition, wxSize(50, 20), 0, vald_int);
	m_right_thresh_sldr = new wxSlider(this, ID_RightThreshSldr, 230, 0, 255,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_right_thresh_text = new wxTextCtrl(this, ID_RightThreshText, "230",
		wxDefaultPosition, wxSize(50, 20), 0, vald_int);
	sizer_m2->Add(m_threh_st, 0, wxALIGN_CENTER);
	sizer_m2->Add(m_left_thresh_text, 0, wxALIGN_CENTER);
	sizer_m2->Add(m_left_thresh_sldr, 1, wxEXPAND);
	sizer_m2->Add(m_right_thresh_text, 0, wxALIGN_CENTER);
	sizer_m2->Add(m_right_thresh_sldr, 1, wxEXPAND);
	//light/shadow
	//shadow
	m_hi_shading_st = new wxStaticText(this, ID_HiShadingSync, "Light /",
		wxDefaultPosition, wxSize(50, -1), wxALIGN_RIGHT);
	m_hi_shading_st->Connect(ID_HiShadingSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnHiShadingSync), NULL, this);
	m_hi_shading_st->Connect(ID_HiShadingSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnHiShadingSync), NULL, this);
	m_shadow_tool = new wxToolBar(this, ID_ShadowSync,
		wxDefaultPosition, wxDefaultSize, wxTB_NODIVIDER);
	bitmap = wxGetBitmapFromMemory(shadow);
#ifdef _DARWIN
	m_shadow_tool->SetToolBitmapSize(bitmap.GetSize());
#endif
	m_shadow_tool->AddCheckTool(ID_ShadowChk, "Shadow",
		bitmap, wxNullBitmap,
		"Enables Shadow Editing.",
		"Enables Shadow Editing.");
	m_shadow_tool->ToggleTool(ID_ShadowChk, false);
	m_shadow_tool->Realize();
	m_shadow_tool->Connect(ID_ShadowSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnShadowSync), NULL, this);
	m_shadow_tool->Connect(ID_ShadowSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnShadowSync), NULL, this);
	st = new wxStaticText(this, 0, " : ",
		wxDefaultPosition, wxSize(20, -1), wxALIGN_RIGHT);
	//highlight
	m_hi_shading_sldr = new wxSlider(this, ID_HiShadingSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_hi_shading_text = new wxTextCtrl(this, ID_HiShadingText, "0.00",
		wxDefaultPosition, wxSize(50, 20), 0, vald_fp2);
	m_shadow_sldr = new wxSlider(this, ID_ShadowSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_shadow_text = new wxTextCtrl(this, ID_ShadowText, "0.00",
		wxDefaultPosition, wxSize(50, 20), 0, vald_fp2);
	sizer_m3->Add(m_hi_shading_st, 0, wxALIGN_CENTER);
	sizer_m3->Add(m_shadow_tool, 0, wxALIGN_CENTER);
	sizer_m3->Add(st, 0, wxALIGN_CENTER);
	sizer_m3->Add(m_hi_shading_text, 0, wxALIGN_CENTER);
	sizer_m3->Add(m_hi_shading_sldr, 1, wxEXPAND);
	sizer_m3->Add(m_shadow_text, 0, wxALIGN_CENTER);
	sizer_m3->Add(m_shadow_sldr, 1, wxEXPAND);
	//sample rate
	m_sample_st = new wxStaticText(this, ID_SampleSync, "Sample Rate : ",
		wxDefaultPosition, wxSize(127, -1), wxALIGN_RIGHT);
	m_sample_st->Connect(ID_SampleSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnSampleSync), NULL, this);
	m_sample_st->Connect(ID_SampleSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnSampleSync), NULL, this);
	m_sample_sldr = new wxSlider(this, ID_SampleSldr, 10, 1, 50,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sample_text = new wxTextCtrl(this, ID_SampleText, "1.0",
		wxDefaultPosition, wxSize(50, 20), 0, vald_fp2);
	sizer_m4->Add(m_sample_st, 0, wxALIGN_CENTER);
	sizer_m4->Add(m_sample_text, 0, wxALIGN_CENTER);
	sizer_m4->Add(m_sample_sldr, 1, wxEXPAND);
	//colormap
	m_colormap_tool = new wxToolBar(this, ID_ColormapSync,
		wxDefaultPosition, wxDefaultSize, wxTB_NODIVIDER);
	bitmap = wxGetBitmapFromMemory(palette);
#ifdef _DARWIN
	m_colormap_tool->SetToolBitmapSize(bitmap.GetSize());
#endif
	m_colormap_tool->AddCheckTool(ID_ColormapEnableChk, "Color Map",
		bitmap, wxNullBitmap,
		"Enables Color Map Editing.",
		"Enables Color Map Editing.");
	m_colormap_tool->ToggleTool(ID_ColormapEnableChk, false);
	m_colormap_tool->Realize();
	m_colormap_tool->Connect(ID_ColormapSync, wxEVT_LEFT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnColormapSync), NULL, this);
	m_colormap_tool->Connect(ID_ColormapSync, wxEVT_RIGHT_DCLICK,
		wxMouseEventHandler(VolumePropPanel::OnColormapSync), NULL, this);
	sizer_m5->Add(50, 50, 0);
	sizer_m5->Add(m_colormap_tool, 0, wxALIGN_CENTER);
	st = new wxStaticText(this, 0, " : ",
		wxDefaultPosition, wxSize(20, -1), wxALIGN_RIGHT);
	sizer_m5->Add(st, 0, wxALIGN_CENTER);
	m_colormap_low_value_text = new wxTextCtrl(this,
		ID_ColormapLowValueText, "0",
		wxDefaultPosition, wxSize(50, 20), 0, vald_int);
	sizer_m5->Add(m_colormap_low_value_text, 0, wxALIGN_CENTER);
	m_colormap_low_value_sldr = new wxSlider(this,
		ID_ColormapLowValueSldr, 0, 0, 255,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	sizer_m5->Add(m_colormap_low_value_sldr, 1, wxEXPAND);
	m_colormap_high_value_text = new wxTextCtrl(this,
		ID_ColormapHighValueText, "255",
		wxDefaultPosition + wxPoint(10, 0), wxSize(50, 20), 0, vald_int);
	sizer_m5->Add(m_colormap_high_value_text, 0, wxALIGN_CENTER);
	m_colormap_high_value_sldr = new wxSlider(this,
		ID_ColormapHighValueSldr, 255, 0, 255,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	sizer_m5->Add(m_colormap_high_value_sldr, 1, wxEXPAND);

	//right ///////////////////////////////////////////////////
	m_options_toolbar = new wxToolBar(this, wxID_ANY,
		wxDefaultPosition, wxDefaultSize, wxTB_NODIVIDER);
	//MIP
	bitmap = wxGetBitmapFromMemory(mip);
#ifdef _DARWIN
	m_options_toolbar->SetToolBitmapSize(bitmap.GetSize());
#endif
	m_options_toolbar->AddCheckTool(ID_MipChk, "MIP",
		bitmap, wxNullBitmap,
		"Enable Maximum Intensity Projection.",
		"Enable Maximum Intensity Projection.");
	m_options_toolbar->ToggleTool(ID_MipChk, false);
	//inversion
	bitmap = wxGetBitmapFromMemory(invert_off);
	m_options_toolbar->AddCheckTool(ID_InvChk, "Inversion",
		bitmap, wxNullBitmap,
		"Inverts data values when checked.",
		"Inverts data values when checked.");
	m_options_toolbar->ToggleTool(ID_InvChk, false);
	//interpolation
	bitmap = wxGetBitmapFromMemory(interpolate);
	m_options_toolbar->AddCheckTool(ID_InterpolateChk, "Interpolate",
		bitmap, wxNullBitmap,
		"Interpolates between data when checked.",
		"Interpolates between data when checked.");
	m_options_toolbar->ToggleTool(ID_InterpolateChk, true);
	//noise reduction
	bitmap = wxGetBitmapFromMemory(smooth_off);
	m_options_toolbar->AddCheckTool(ID_NRChk, "Smoothing",
		bitmap, wxNullBitmap,
		"Enable Data Smoothing.",
		"Enable Data Smoothing.");
	m_options_toolbar->ToggleTool(ID_NRChk, false);
	//sync group
	bitmap = wxGetBitmapFromMemory(sync_chan);
	m_options_toolbar->AddCheckTool(ID_SyncGroupChk, "Group Sync",
		bitmap, wxNullBitmap,
		"Sync this channel with others in its group.",
		"Sync this channel with others in its group.");
	m_options_toolbar->ToggleTool(ID_SyncGroupChk, false);
	//depth mode
	bitmap = wxGetBitmapFromMemory(depth_off);
	m_options_toolbar->AddCheckTool(ID_DepthChk, "Depth Mode",
		bitmap, wxNullBitmap,
		"Enable Depth Mode.",
		"Enable Depth Mode.");
	m_options_toolbar->ToggleTool(ID_DepthChk, false);
	//legend
	bitmap = wxGetBitmapFromMemory(legend);
	m_options_toolbar->AddCheckTool(ID_LegendChk, "Legend",
		bitmap, wxNullBitmap,
		"Enable Legend for this channel.",
		"Enable Legend for this channel.");
	m_options_toolbar->ToggleTool(ID_LegendChk, true);
	//buttons
	bitmap = wxGetBitmapFromMemory(reset);
	m_options_toolbar->AddTool(ID_ResetDefault, "Reset",
		bitmap, "Reset Properties");
	bitmap = wxGetBitmapFromMemory(save_settings);
	m_options_toolbar->AddTool(ID_SaveDefault, "Save",
		bitmap, "Set as default settings");
	sizer_r1->AddStretchSpacer();
	sizer_r1->Add(m_options_toolbar, 0, wxALIGN_CENTER);
	sizer_r1->AddStretchSpacer();
	m_options_toolbar->Realize();
	//spacings
	//x
	st = new wxStaticText(this, 0, "Spacing");
	m_space_x_text = new wxTextCtrl(this, ID_SpaceXText, "1.000",
		wxDefaultPosition, wxSize(45, -1), 0, vald_fp3);
	sizer_r2->Add(st, 0, wxALIGN_CENTER);
	sizer_r2->AddStretchSpacer();
	st = new wxStaticText(this, 0, "X:");
	sizer_r2->Add(st, 0, wxALIGN_CENTER);
	sizer_r2->Add(m_space_x_text, 0, wxALIGN_CENTER);
	//y
	st = new wxStaticText(this, 0, "Y:");
	m_space_y_text = new wxTextCtrl(this, ID_SpaceYText, "1.000",
		wxDefaultPosition, wxSize(45, -1), 0, vald_fp3);
	sizer_r2->Add(3, 5, 0);
	sizer_r2->Add(st, 0, wxALIGN_CENTER);
	sizer_r2->Add(m_space_y_text, 0, wxALIGN_CENTER);
	//z
	st = new wxStaticText(this, 0, "Z:");
	m_space_z_text = new wxTextCtrl(this, ID_SpaceZText, "1.000",
		wxDefaultPosition, wxSize(45, -1), 0, vald_fp3);
	sizer_r2->Add(3, 5, 0);
	sizer_r2->Add(st, 0, wxALIGN_CENTER);
	sizer_r2->Add(m_space_z_text, 0, wxALIGN_CENTER);
	//color 1
	st = new wxStaticText(this, 0, "Pri Color:",
		wxDefaultPosition, wxSize(70, -1), wxALIGN_CENTER);
	m_color_text = new wxTextCtrl(this, ID_ColorText, "255 , 255 , 255",
		wxDefaultPosition, wxSize(85, 20));
	m_color_text->Connect(ID_ColorText, wxEVT_LEFT_DCLICK,
		wxCommandEventHandler(VolumePropPanel::OnColorTextFocus),
		NULL, this);
	m_color_btn = new wxColourPickerCtrl(this, ID_ColorBtn, *wxRED,
		wxDefaultPosition, wxSize(50, 25));
	sizer_r3->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_r3->Add(5, 5, 0);
	sizer_r3->Add(m_color_text, 0, wxALIGN_CENTER, 0);
	sizer_r3->Add(m_color_btn, 1, wxALIGN_CENTER, 0);
	//color 2
	st = new wxStaticText(this, 0, "Sec Color:",
		wxDefaultPosition, wxSize(70, -1), wxALIGN_CENTER);
	m_color2_text = new wxTextCtrl(this, ID_Color2Text, "255 , 255 , 255",
		wxDefaultPosition, wxSize(85, 20));
	m_color2_text->Connect(ID_Color2Text, wxEVT_LEFT_DCLICK,
		wxCommandEventHandler(VolumePropPanel::OnColor2TextFocus),
		NULL, this);
	m_color2_btn = new wxColourPickerCtrl(this, ID_Color2Btn, *wxRED,
		wxDefaultPosition, wxSize(50, 25));
	sizer_r4->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_r4->Add(5, 5, 0);
	sizer_r4->Add(m_color2_text, 0, wxALIGN_CENTER, 0);
	sizer_r4->Add(m_color2_btn, 1, wxALIGN_CENTER, 0);
	// colormap chooser
	st = new wxStaticText(this, 0, "Color Map:",
		wxDefaultPosition, wxSize(70, -1), wxALIGN_CENTER);
	m_colormap_combo = new wxComboBox(this, ID_ColormapCombo, "",
		wxDefaultPosition, wxSize(85, 25), 0, NULL, wxCB_READONLY);
	std::vector<std::string>colormap_list;
	colormap_list.push_back("Rainbow");
	colormap_list.push_back("Reverse Rainbow");
	colormap_list.push_back("Hot");
	colormap_list.push_back("Cool");
	colormap_list.push_back("Diverging");
	colormap_list.push_back("Monochrome");
	colormap_list.push_back("Reverse Mono");
	for (size_t i = 0; i<colormap_list.size(); ++i)
		m_colormap_combo->Append(colormap_list[i]);
	m_colormap_combo2 = new wxComboBox(this, ID_ColormapCombo2, "",
		wxDefaultPosition, wxSize(85, 25), 0, NULL, wxCB_READONLY);
	std::vector<std::string>colormap_list2;
	colormap_list2.push_back("Intensity");
	colormap_list2.push_back("Z Value");
	colormap_list2.push_back("Y Value");
	colormap_list2.push_back("X Value");
	colormap_list2.push_back("Gradient");
	colormap_list2.push_back("Differential");
	for (size_t i = 0; i<colormap_list2.size(); ++i)
		m_colormap_combo2->Append(colormap_list2[i]);
	sizer_r5->Add(st, 0, wxALIGN_CENTER, 0);
	sizer_r5->Add(5, 5, 0);
	sizer_r5->Add(m_colormap_combo, 0, wxALIGN_CENTER, 0);
	sizer_r5->Add(m_colormap_combo2, 0, wxALIGN_CENTER, 0);

	//ADD COLUMNS//////////////////////////////////////
	//left
	sizer_left->Add(sizer_l1, 0, wxEXPAND);
	sizer_left->Add(sizer_l2, 0, wxEXPAND);
	sizer_left->Add(sizer_l3, 0, wxEXPAND);
	sizer_left->Add(sizer_l4, 0, wxEXPAND);
	sizer_left->Add(sizer_l5, 0, wxEXPAND);
	//middle
	sizer_middle->Add(sizer_m1, 0, wxEXPAND);
	sizer_middle->Add(sizer_m2, 0, wxEXPAND);
	sizer_middle->Add(sizer_m3, 0, wxEXPAND);
	sizer_middle->Add(sizer_m4, 0, wxEXPAND);
	sizer_middle->Add(sizer_m5, 0, wxEXPAND);
	//right
	sizer_right->Add(sizer_r1, 0, wxEXPAND);
	sizer_right->Add(sizer_r2, 0, wxEXPAND);
	sizer_right->Add(sizer_r3, 0, wxEXPAND);
	sizer_right->Add(sizer_r4, 0, wxEXPAND);
	sizer_right->Add(sizer_r5, 0, wxEXPAND);
	//ADD ALL TOGETHER
	sizer_all->Add(sizer_left, 1, wxEXPAND);
	sizer_all->Add(sizer_middle, 1, wxEXPAND);
	sizer_all->Add(sizer_right, 0, wxSHRINK);
	SetSizer(sizer_all);
	Layout();
}

VolumePropPanel::~VolumePropPanel()
{
}

void VolumePropPanel::AssociateVolumeData(FL::VolumeData* vd)
{
	m_agent->setObject(vd);
}

//1
void VolumePropPanel::OnGammaSync(wxMouseEvent& event)
{
	wxString str = m_gamma_text->GetValue();
	double dVal;
	str.ToDouble(&dVal);
	//if (m_group)
	//	m_group->Set3DGamma(dVal);
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnGammaChange(wxScrollEvent & event)
{
	double val = (double)event.GetPosition() / 100.0;
	wxString str = wxString::Format("%.2f", val);
	m_gamma_text->SetValue(str);
}

void VolumePropPanel::OnGammaText(wxCommandEvent& event)
{
	wxString str = m_gamma_text->GetValue();
	double val = 0.0;
	str.ToDouble(&val);
	int ival = int(val*100.0 + 0.5);
	m_gamma_sldr->SetValue(ival);

	//set gamma value
	//if (m_sync_group && m_group)
	//	m_group->Set3DGamma(val);
	//else if (m_vd)
	//	m_vd->Set3DGamma(val);
	m_agent->setValue("gamma 3d", val);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnBoundarySync(wxMouseEvent& event)
{
	wxString str = m_boundary_text->GetValue();
	double dVal;
	str.ToDouble(&dVal);
	//if (m_group)
	//	m_group->SetBoundary(dVal);
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnBoundaryChange(wxScrollEvent & event)
{
	double val = (double)event.GetPosition() / 2000.0;
	wxString str = wxString::Format("%.4f", val);
	m_boundary_text->SetValue(str);
}

void VolumePropPanel::OnBoundaryText(wxCommandEvent& event)
{
	wxString str = m_boundary_text->GetValue();
	double val = 0.0;
	str.ToDouble(&val);
	int ival = int(val*2000.0 + 0.5);
	m_boundary_sldr->SetValue(ival);

	//set boundary value
	//if (m_sync_group && m_group)
	//	m_group->SetBoundary(val);
	//else if (m_vd)
	//	m_vd->SetBoundary(val);
	m_agent->setValue("extract boundary", val);

	//RefreshVRenderViews(false, true);
}

//2
void VolumePropPanel::OnSaturationSync(wxMouseEvent& event)
{
	wxString str = m_saturation_text->GetValue();
	long iVal;
	str.ToLong(&iVal);
	double dVal = double(iVal) / m_max_val;
	//if (m_group)
	//	m_group->SetOffset(dVal);
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnSaturationChange(wxScrollEvent & event)
{
	int ival = event.GetPosition();
	wxString str = wxString::Format("%d", ival);
	m_saturation_text->SetValue(str);
}

void VolumePropPanel::OnSaturationText(wxCommandEvent& event)
{
	wxString str = m_saturation_text->GetValue();
	long ival = 0;
	str.ToLong(&ival);
	if (double(ival) > m_max_val)
	{
		UpdateMaxVal(ival);
		str = wxString::Format("%d", ival);
		m_saturation_text->ChangeValue(str);
	}
	m_saturation_sldr->SetValue(ival);
	double val = double(ival) / m_max_val;

	//set contrast value
	//if (m_sync_group && m_group)
	//	m_group->SetOffset(val);
	//else if (m_vd)
	//	m_vd->SetOffset(val);
	m_agent->setValue("saturation", val);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnThreshSync(wxMouseEvent& event)
{
	wxString str = m_left_thresh_text->GetValue();
	long iVal;
	str.ToLong(&iVal);
	double dVal = double(iVal) / m_max_val;
	//if (m_group)
	//	m_group->SetLeftThresh(dVal);
	str = m_right_thresh_text->GetValue();
	str.ToLong(&iVal);
	dVal = double(iVal) / m_max_val;
	//if (m_group)
	//	m_group->SetRightThresh(dVal);
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnLeftThreshChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	wxString str = wxString::Format("%d", ival);
	m_left_thresh_text->SetValue(str);
}

void VolumePropPanel::OnLeftThreshText(wxCommandEvent &event)
{
	wxString str = m_left_thresh_text->GetValue();
	long ival = 0;
	str.ToLong(&ival);
	if (double(ival) > m_max_val)
	{
		UpdateMaxVal(ival);
		str = wxString::Format("%d", ival);
		m_left_thresh_text->ChangeValue(str);
	}
	double val = double(ival) / m_max_val;
	double right_val = (double)m_right_thresh_sldr->GetValue() / m_max_val;

	if (val > right_val)
	{
		val = right_val;
		ival = int(val*m_max_val + 0.5);
		wxString str2 = wxString::Format("%d", ival);
		m_left_thresh_text->ChangeValue(str2);
	}
	m_left_thresh_sldr->SetValue(ival);

	//set left threshold value
	//if (m_sync_group && m_group)
	//	m_group->SetLeftThresh(val);
	//else if (m_vd)
	//	m_vd->SetLeftThresh(val);
	m_agent->setValue("low threshold", val);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnRightThreshChange(wxScrollEvent & event)
{
	int ival = event.GetPosition();
	int ival2 = m_left_thresh_sldr->GetValue();

	if (ival < ival2)
	{
		ival = ival2;
		m_right_thresh_sldr->SetValue(ival);
	}
	wxString str = wxString::Format("%d", ival);
	m_right_thresh_text->SetValue(str);
}

void VolumePropPanel::OnRightThreshText(wxCommandEvent &event)
{
	wxString str = m_right_thresh_text->GetValue();
	long ival = 0;
	str.ToLong(&ival);
	if (double(ival) > m_max_val)
	{
		UpdateMaxVal(ival);
		str = wxString::Format("%d", ival);
		m_right_thresh_text->ChangeValue(str);
	}
	double val = double(ival) / m_max_val;
	double left_val = (double)m_left_thresh_sldr->GetValue() / m_max_val;

	if (val >= left_val)
	{
		m_right_thresh_sldr->SetValue(ival);

		//set right threshold value
		//if (m_sync_group && m_group)
		//	m_group->SetRightThresh(val);
		//else if (m_vd)
		//	m_vd->SetRightThresh(val);
		m_agent->setValue("high threshold", val);

		//RefreshVRenderViews(false, true);
	}
}

//3
void VolumePropPanel::OnLuminanceSync(wxMouseEvent& event)
{
	wxString str = m_luminance_text->GetValue();
	long iVal;
	str.ToLong(&iVal);
	double dVal = double(iVal) / m_max_val;
	//if (m_group)
	//	m_group->SetLuminance(dVal);
	//RefreshVRenderViews(true, true);
}

void VolumePropPanel::OnLuminanceChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	wxString str = wxString::Format("%d", ival);
	m_luminance_text->SetValue(str);
}

void VolumePropPanel::OnLuminanceText(wxCommandEvent &event)
{
	wxString str = m_luminance_text->GetValue();
	long ival = 0;
	str.ToLong(&ival);
	if (double(ival) > m_max_val)
	{
		UpdateMaxVal(ival);
		str = wxString::Format("%d", ival);
		m_luminance_text->ChangeValue(str);
	}
	double val = double(ival) / m_max_val;
	m_luminance_sldr->SetValue(ival);

	//if (m_sync_group && m_group)
	//	m_group->SetLuminance(val);
	//else if (m_vd)
	//	m_vd->SetLuminance(val);
	m_agent->setValue("luminance", val);

	//if (m_vd)
	//{
	//	FLTYPE::Color color;
	//	m_vd->getValue("color", color);
	//	wxColor wxc((unsigned char)(color.r() * 255),
	//		(unsigned char)(color.g() * 255),
	//		(unsigned char)(color.b() * 255));
	//	m_color_text->ChangeValue(wxString::Format("%d , %d , %d",
	//		wxc.Red(), wxc.Green(), wxc.Blue()));
	//	m_color_btn->SetBackgroundColour(wxc);
	//	m_lumi_change = true;
	//}

	//RefreshVRenderViews(true, true);
}

//hi shading
void VolumePropPanel::OnHiShadingSync(wxMouseEvent& event)
{
	wxString str = m_hi_shading_text->GetValue();
	double dVal;
	str.ToDouble(&dVal);
	//if (m_group)
	//	m_group->SetHiShading(dVal);
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnHiShadingChange(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 10.0;
	wxString str = wxString::Format("%.2f", val);
	m_hi_shading_text->SetValue(str);
}

void VolumePropPanel::OnHiShadingText(wxCommandEvent &event)
{
	wxString str = m_hi_shading_text->GetValue();
	double val = 0.0;
	str.ToDouble(&val);
	m_hi_shading_sldr->SetValue(int(val*10.0 + 0.5));

	//set high shading value
	//if (m_sync_group && m_group)
	//	m_group->SetHiShading(val);
	//else if (m_vd)
	//	m_vd->SetHiShading(val);
	m_agent->setValue("high shading", val);

	//RefreshVRenderViews(false, true);
}

//shadow
void VolumePropPanel::OnShadowSync(wxMouseEvent& event)
{
	bool bVal = m_shadow_tool->GetToolState(ID_ShadowChk);
	wxString str = m_shadow_text->GetValue();
	double dVal;
	str.ToDouble(&dVal);
	//if (m_group)
	//{
	//	m_group->SetShadow(bVal);
	//	m_group->SetShadowParams(dVal);
	//}
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnShadowEnable(wxCommandEvent &event)
{
	bool shadow = m_shadow_tool->GetToolState(ID_ShadowChk);
	//if (m_sync_group && m_group)
	//	m_group->SetShadow(shadow);
	//else if (m_group && m_group->GetBlendMode()==2)
	//	m_vd->SetShadow(shadow);
	//else if (m_vd)
	//	m_vd->SetShadow(shadow);
	m_agent->setValue("shadow enable", shadow);

	if (shadow)
		EnableShadow();
	else
		DisableShadow();

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnShadowChange(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 100.0;
	wxString str = wxString::Format("%.2f", val);
	m_shadow_text->SetValue(str);
}

void VolumePropPanel::OnShadowText(wxCommandEvent &event)
{
	wxString str = m_shadow_text->GetValue();
	double val = 0.0;
	str.ToDouble(&val);
	m_shadow_sldr->SetValue(int(val*100.0 + 0.5));

	//set shadow darkness
	//if (m_sync_group && m_group)
	//	m_group->SetShadowParams(val);
	//else if (m_group && m_group->GetBlendMode()==2)
	//	m_group->SetShadowParams(val);
	//else if (m_vd)
	//	m_vd->SetShadowParams(val);
	m_agent->setValue("shadow int", val);

	//RefreshVRenderViews(false, true);
}

//4
void VolumePropPanel::OnAlphaSync(wxMouseEvent& event)
{
	wxString str = m_alpha_text->GetValue();
	long iVal;
	str.ToLong(&iVal);
	double dVal = double(iVal) / m_max_val;
	//if (m_group)
	//	m_group->SetAlpha(dVal);
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnAlphaCheck(wxCommandEvent &event)
{
	bool alpha = m_alpha_tool->GetToolState(ID_AlphaChk);
	//if (m_sync_group && m_group)
	//	m_group->SetEnableAlpha(alpha);
	//else if (m_vd)
	//	m_vd->SetEnableAlpha(alpha);
	m_agent->setValue("alpha enable", alpha);

	if (alpha)
		EnableAlpha();
	else
		DisableAlpha();

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnAlphaChange(wxScrollEvent &event)
{
	int ival = event.GetPosition();
	wxString str = wxString::Format("%d", ival);
	m_alpha_text->SetValue(str);
}

void VolumePropPanel::OnAlphaText(wxCommandEvent& event)
{
	wxString str = m_alpha_text->GetValue();
	long ival = 0;
	str.ToLong(&ival);
	if (double(ival) > m_max_val)
	{
		UpdateMaxVal(ival);
		str = wxString::Format("%d", ival);
		m_alpha_text->ChangeValue(str);
	}
	double val = double(ival) / m_max_val;
	m_alpha_sldr->SetValue(ival);

	//set alpha value
	//if (m_sync_group && m_group)
	//	m_group->SetAlpha(val);
	//else if (m_vd)
	//	m_vd->SetAlpha(val);
	m_agent->setValue("alpha", val);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnSampleSync(wxMouseEvent& event)
{
	wxString str = m_sample_text->GetValue();
	double srate = 0.0;
	str.ToDouble(&srate);
	//if (m_group)
	//	m_group->SetSampleRate(srate);
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnSampleChange(wxScrollEvent & event)
{
	double val = event.GetPosition() / 10.0;
	wxString str = wxString::Format("%.1f", val);
	m_sample_text->SetValue(str);
}

void VolumePropPanel::OnSampleText(wxCommandEvent& event)
{
	wxString str = m_sample_text->GetValue();
	double srate = 0.0;
	str.ToDouble(&srate);
	double val = srate*10.0;
	m_sample_sldr->SetValue(int(val));

	//set sample rate value
	m_agent->setValue("sample rate", srate);
	//if (m_vrv && m_vrv->GetVolMethod() == VOL_METHOD_MULTI)
	//{
	//	//for (int i=0; i<m_vrv->GetAllVolumeNum(); i++)
	//	//{
	//	//	VolumeData* vd = m_vrv->GetAllVolumeData(i);
	//	//	if (vd)
	//	//		vd->SetSampleRate(srate);
	//	//}
	//}
	//else
	//{
	//	//if (m_sync_group && m_group)
	//	//	m_group->SetSampleRate(srate);
	//	//else if (m_group && m_group->GetBlendMode()==2)
	//	//	m_group->SetSampleRate(srate);
	//	//else if (m_vd)
	//	//	m_vd->SetSampleRate(srate);
	//	m_vd->setValue("sample rate", srate);
	//}

	//RefreshVRenderViews(false, true);
}

//5
void VolumePropPanel::OnShadingSync(wxMouseEvent& event)
{
	bool bVal = m_shade_tool->GetToolState(ID_ShadingEnableChk);
	wxString str = m_low_shading_text->GetValue();
	double dVal;
	str.ToDouble(&dVal);
	//if (m_group)
	//{
	//	m_group->SetShading(bVal);
	//	m_group->SetLowShading(dVal);
	//}
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnLowShadingChange(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 100.0;
	wxString str = wxString::Format("%.2f", val);
	m_low_shading_text->SetValue(str);
}

void VolumePropPanel::OnLowShadingText(wxCommandEvent &event)
{
	wxString str = m_low_shading_text->GetValue();
	double val = 0.0;
	str.ToDouble(&val);
	m_low_shading_sldr->SetValue(int(val*100.0 + 0.5));

	//set low shading value
	//if (m_sync_group && m_group)
	//	m_group->SetLowShading(val);
	//else if (m_vd)
	//	m_vd->SetLowShading(val);
	m_agent->setValue("low shading", val);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnShadingEnable(wxCommandEvent &event)
{
	bool shading = m_shade_tool->GetToolState(ID_ShadingEnableChk);
	//if (m_sync_group && m_group)
	//	m_group->SetShading(shading);
	//else if (m_vd)
	//	m_vd->SetShading(shading);
	m_agent->setValue("shading enable", shading);

	if (shading)
		EnableShading();
	else
		DisableShading();

	//RefreshVRenderViews(false, true);
}

//colormap controls
void VolumePropPanel::OnColormapSync(wxMouseEvent& event)
{
	bool bVal = m_colormap_tool->GetToolState(ID_ColormapEnableChk);
	long iVal;
	double dVal1, dVal2;
	wxString str = m_colormap_low_value_text->GetValue();
	str.ToLong(&iVal);
	dVal1 = double(iVal) / m_max_val;
	str = m_colormap_high_value_text->GetValue();
	str.ToLong(&iVal);
	dVal2 = double(iVal) / m_max_val;
	//if (m_group)
	//{
	//	m_group->SetColormapMode(bVal ? 1 : 0);
	//	m_group->SetColormapDisp(bVal);
	//	m_group->SetColormapValues(dVal1, dVal2);
	//}
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnEnableColormap(wxCommandEvent &event)
{
	bool colormap =
		m_colormap_tool->GetToolState(ID_ColormapEnableChk);

	//if (m_sync_group && m_group)
	//{
	//	m_group->SetColormapMode(colormap?1:0);
	//	m_group->SetColormapDisp(colormap);
	//}
	//else if (m_vd)
	//{
	//	m_vd->SetColormapMode(colormap?1:0);
	//	m_vd->SetColormapDisp(colormap);
	//}
	m_agent->setValue("colormap enable", colormap);
	m_agent->setValue("colormap mode", long(colormap ? 1 : 0));

	//VRenderFrame* vr_frame = (VRenderFrame*)m_frame;
	//if (vr_frame)
	//{
	//	AdjustView *adjust_view = vr_frame->GetAdjustView();
	//	if (adjust_view)
	//		adjust_view->UpdateSync();
	//}

	if (colormap)
		EnableColormap();
	else
		DisableColormap();

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnColormapHighValueChange(wxScrollEvent &event)
{
	int iVal = m_colormap_high_value_sldr->GetValue();
	int iVal2 = m_colormap_low_value_sldr->GetValue();

	if (iVal < iVal2)
	{
		iVal = iVal2;
		m_colormap_high_value_sldr->SetValue(iVal);
	}
	wxString str = wxString::Format("%d", iVal);
	m_colormap_high_value_text->SetValue(str);
}

void VolumePropPanel::OnColormapHighValueText(wxCommandEvent &event)
{
	wxString str = m_colormap_high_value_text->GetValue();
	long iVal = 0;
	str.ToLong(&iVal);
	if (double(iVal) > m_max_val)
	{
		UpdateMaxVal(iVal);
		str = wxString::Format("%d", iVal);
		m_colormap_high_value_text->ChangeValue(str);
	}
	long iVal2 = m_colormap_low_value_sldr->GetValue();

	if (iVal >= iVal2)
	{
		m_colormap_high_value_sldr->SetValue(iVal);

		double val = double(iVal) / m_max_val;

		//if (m_sync_group && m_group)
		//	m_group->SetColormapValues(-1, val);
		//else if (m_vd)
		//{
		//	double low, high;
		//	m_vd->GetColormapValues(low, high);
		//	m_vd->SetColormapValues(low, val);
		//}
		m_agent->setValue("colormap high", val);

		//RefreshVRenderViews(false, true);
	}
}

void VolumePropPanel::OnColormapLowValueChange(wxScrollEvent &event)
{
	int iVal = m_colormap_low_value_sldr->GetValue();
	wxString str = wxString::Format("%d", iVal);
	m_colormap_low_value_text->SetValue(str);
}

void VolumePropPanel::OnColormapLowValueText(wxCommandEvent &event)
{
	wxString str = m_colormap_low_value_text->GetValue();
	long iVal = 0;
	str.ToLong(&iVal);
	if (double(iVal) > m_max_val)
	{
		UpdateMaxVal(iVal);
		str = wxString::Format("%d", iVal);
		m_colormap_low_value_text->ChangeValue(str);
	}
	long iVal2 = m_colormap_high_value_sldr->GetValue();

	if (iVal > iVal2)
	{
		iVal = iVal2;
		str = wxString::Format("%d", iVal);
		m_colormap_low_value_text->ChangeValue(str);
	}
	m_colormap_low_value_sldr->SetValue(iVal);

	double val = double(iVal) / m_max_val;

	//if (m_sync_group && m_group)
	//	m_group->SetColormapValues(val, -1);
	//else if (m_vd)
	//{
	//	double low, high;
	//	m_vd->GetColormapValues(low, high);
	//	m_vd->SetColormapValues(val, high);
	//}
	m_agent->setValue("colormap low", val);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnColormapCombo(wxCommandEvent &event)
{
	long colormap = m_colormap_combo->GetCurrentSelection();

	//if (m_sync_group && m_group)
	//	m_group->SetColormap(colormap);
	//else if (m_vd)
	//	m_vd->SetColormap(colormap);
	m_agent->setValue("colormap type", colormap);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnColormapCombo2(wxCommandEvent &event)
{
	long colormap_proj = m_colormap_combo2->GetCurrentSelection();

	//if (m_sync_group && m_group)
	//	m_group->SetColormapProj(colormap_proj);
	//else if (m_vd)
	//	m_vd->SetColormapProj(colormap_proj);
	m_agent->setValue("colormap proj", colormap_proj);

	//RefreshVRenderViews(false, true);
}

//6
void VolumePropPanel::OnColorChange(wxColor c)
{
	FLTYPE::Color color(c.Red() / 255.0, c.Green() / 255.0, c.Blue() / 255.0);
	m_agent->setValue("color", color);
	//if (m_vd)
	//{
	//	//if (m_lumi_change)
	//	//{
	//	//	m_vd->SetColor(color, true);
	//	//	m_lumi_change = false;
	//	//}
	//	//else
	//	//	m_vd->SetColor(color);
	//	m_vd->setValue("color", color);

	//	//double lum = m_vd->GetLuminance();
	//	//int ilum = int(lum*m_max_val+0.5);
	//	//m_luminance_sldr->SetValue(ilum);
	//	//wxString str = wxString::Format("%d", ilum);
	//	//m_luminance_text->ChangeValue(str);

	//	bool sec_color_set;
	//	m_vd->getValue("sec color set", sec_color_set);
	//	if (!sec_color_set)
	//	{
	//		m_vd->getValue("sec color", color);
	//		wxColor wxc((unsigned char)(color.r() * 255),
	//			(unsigned char)(color.g() * 255),
	//			(unsigned char)(color.b() * 255));
	//		m_color2_text->ChangeValue(wxString::Format("%d , %d , %d",
	//			wxc.Red(), wxc.Green(), wxc.Blue()));
	//		m_color2_btn->SetColour(wxc);
	//	}

	//	VRenderFrame* vr_frame = (VRenderFrame*)m_frame;

	//	if (vr_frame)
	//	{
	//		AdjustView *adjust_view = vr_frame->GetAdjustView();
	//		if (adjust_view)
	//			adjust_view->UpdateSync();
	//	}

	//	RefreshVRenderViews(true, true);
	//}
}

void VolumePropPanel::OnColor2Change(wxColor c)
{
	FLTYPE::Color color(c.Red() / 255.0, c.Green() / 255.0, c.Blue() / 255.0);
	m_agent->setValue("sec color", color);
	//if (m_vd)
	//{
	//	m_vd->setValue("sec color", color);
	//	RefreshVRenderViews(true, true);
	//}
}

int VolumePropPanel::GetColorString(wxString& str, wxColor& wxc)
{
	int filled = 3;
	if (str == "a" || str == "A")
		wxc = wxColor(0, 127, 255);
	else if (str == "b" || str == "B")
		wxc = wxColor(0, 0, 255);
	else if (str == "c" || str == "C")
		wxc = wxColor(0, 255, 255);
	else if (str == "d" || str == "D")
		wxc = wxColor(193, 154, 107);
	else if (str == "e" || str == "E")
		wxc = wxColor(80, 200, 120);
	else if (str == "f" || str == "F")
		wxc = wxColor(226, 88, 34);
	else if (str == "g" || str == "G")
		wxc = wxColor(0, 255, 0);
	else if (str == "h" || str == "H")
		wxc = wxColor(70, 255, 0);
	else if (str == "i" || str == "I")
		wxc = wxColor(75, 0, 130);
	else if (str == "j" || str == "J")
		wxc = wxColor(0, 168, 107);
	else if (str == "k" || str == "K")
		wxc = wxColor(0, 0, 0);
	else if (str == "l" || str == "L")
		wxc = wxColor(181, 126, 220);
	else if (str == "m" || str == "M")
		wxc = wxColor(255, 0, 255);
	else if (str == "n" || str == "N")
		wxc = wxColor(0, 0, 128);
	else if (str == "o" || str == "O")
		wxc = wxColor(0, 119, 190);
	else if (str == "p" || str == "P")
		wxc = wxColor(254, 40, 162);
	else if (str == "q" || str == "Q")
		wxc = wxColor(232, 204, 215);
	else if (str == "r" || str == "R")
		wxc = wxColor(255, 0, 0);
	else if (str == "s" || str == "S")
		wxc = wxColor(236, 213, 64);
	else if (str == "t" || str == "T")
		wxc = wxColor(255, 99, 71);
	else if (str == "u" || str == "U")
		wxc = wxColor(211, 0, 63);
	else if (str == "v" || str == "V")
		wxc = wxColor(143, 0, 255);
	else if (str == "w" || str == "W")
		wxc = wxColor(255, 255, 255);
	else if (str == "x" || str == "X")
		wxc = wxColor(115, 134, 120);
	else if (str == "y" || str == "Y")
		wxc = wxColor(255, 255, 0);
	else if (str == "z" || str == "Z")
		wxc = wxColor(57, 167, 142);
	else
	{
		int index = 0;//1-red; 2-green; 3-blue;
		int state = 0;//0-idle; 1-reading digit; 3-finished
		wxString sColor;
		long r = 255;
		long g = 255;
		long b = 255;
		for (unsigned int i = 0; i<str.length(); i++)
		{
			wxChar c = str[i];
			if (isdigit(c) || c == '.')
			{
				if (state == 0 || state == 3)
				{
					sColor += c;
					index++;
					state = 1;
				}
				else if (state == 1)
				{
					sColor += c;
				}

				if (i == str.length() - 1)  //last one
				{
					switch (index)
					{
					case 1:
						sColor.ToLong(&r);
						filled = 1;
						break;
					case 2:
						sColor.ToLong(&g);
						filled = 2;
						break;
					case 3:
						sColor.ToLong(&b);
						filled = 3;
						break;
					}
				}
			}
			else
			{
				if (state == 1)
				{
					switch (index)
					{
					case 1:
						sColor.ToLong(&r);
						filled = 1;
						break;
					case 2:
						sColor.ToLong(&g);
						filled = 2;
						break;
					case 3:
						sColor.ToLong(&b);
						filled = 3;
						break;
					}
					state = 3;
					sColor = "";
				}
			}
		}
		wxc = wxColor(FLTYPE::Clamp(r, 0, 255),
			FLTYPE::Clamp(g, 0, 255),
			FLTYPE::Clamp(b, 0, 255));
	}
	return filled;
}

void VolumePropPanel::OnColorTextChange(wxCommandEvent& event)
{
	wxString str = m_color_text->GetValue();
	wxColor wxc;
	if (GetColorString(str, wxc) == 3)
	{
		wxString new_str = wxString::Format("%d , %d , %d",
			wxc.Red(), wxc.Green(), wxc.Blue());
		if (str != new_str)
			m_color_text->ChangeValue(new_str);
		m_color_btn->SetColour(wxc);

		OnColorChange(wxc);
	}
}

void VolumePropPanel::OnColor2TextChange(wxCommandEvent& event)
{
	wxString str = m_color2_text->GetValue();
	wxColor wxc;
	if (GetColorString(str, wxc) == 3)
	{
		wxString new_str = wxString::Format("%d , %d , %d",
			wxc.Red(), wxc.Green(), wxc.Blue());
		if (str != new_str)
			m_color2_text->ChangeValue(new_str);
		m_color2_btn->SetColour(wxc);

		OnColor2Change(wxc);
	}
}

void VolumePropPanel::OnColorBtn(wxColourPickerEvent& event)
{
	wxColor wxc = event.GetColour();

	m_color_text->ChangeValue(wxString::Format("%d , %d , %d",
		wxc.Red(), wxc.Green(), wxc.Blue()));

	OnColorChange(wxc);
}

void VolumePropPanel::OnColor2Btn(wxColourPickerEvent& event)
{
	wxColor wxc = event.GetColour();

	m_color2_text->ChangeValue(wxString::Format("%d , %d , %d",
		wxc.Red(), wxc.Green(), wxc.Blue()));

	OnColor2Change(wxc);
}

void VolumePropPanel::OnColorTextFocus(wxCommandEvent& event)
{
	m_color_text->SetSelection(0, -1);
}

void VolumePropPanel::OnColor2TextFocus(wxCommandEvent& event)
{
	m_color2_text->SetSelection(0, -1);
}

void VolumePropPanel::OnInvCheck(wxCommandEvent &event)
{
	bool inv = m_options_toolbar->GetToolState(ID_InvChk);
	if (inv)
		m_options_toolbar->SetToolNormalBitmap(ID_InvChk,
			wxGetBitmapFromMemory(invert));
	else
		m_options_toolbar->SetToolNormalBitmap(ID_InvChk,
			wxGetBitmapFromMemory(invert_off));

	//if (m_sync_group && m_group)
	//	m_group->SetInvert(inv);
	//else if (m_vd)
	//	m_vd->SetInvert(inv);
	m_agent->setValue("invert", inv);

	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnMIPCheck(wxCommandEvent &event)
{
	long val = m_options_toolbar->GetToolState(ID_MipChk) ? 1 : 0;

	//if (m_sync_group && m_group)
	//	m_group->SetMode(val);
	//else if (m_vd)
	//	m_vd->SetMode(val);
	m_agent->setValue("mip mode", val);

	//if (val == 1)
	//{
	//	VRenderFrame* vr_frame = (VRenderFrame*)m_frame;
	//	if (vr_frame)
	//	{
	//		for (int i = 0; i<(int)vr_frame->GetViewList()->size(); i++)
	//		{
	//			VRenderView *vrv = (*vr_frame->GetViewList())[i];
	//			if (vrv && vrv->GetVolMethod() == VOL_METHOD_MULTI)
	//			{
	//				::wxMessageBox("MIP is not supported in Depth mode.");
	//				m_options_toolbar->ToggleTool(ID_MipChk, false);
	//				return;
	//			}
	//		}
	//	}
	//	EnableMip();
	//	if (m_threh_st)
	//		m_threh_st->SetLabel("Shade Threshold : ");
	//}
	//else
	//{
	//	DisableMip();
	//	if (m_threh_st)
	//		m_threh_st->SetLabel("Threshold : ");
	//}

	Layout();

	//RefreshVRenderViews(false, true);
}

//noise reduction
void VolumePropPanel::OnNRCheck(wxCommandEvent &event)
{
	bool val = m_options_toolbar->GetToolState(ID_NRChk);
	if (val)
		m_options_toolbar->SetToolNormalBitmap(ID_NRChk,
			wxGetBitmapFromMemory(smooth));
	else
		m_options_toolbar->SetToolNormalBitmap(ID_NRChk,
			wxGetBitmapFromMemory(smooth_off));

	m_agent->setValue("noise redct", val);
	//if (m_vrv && m_vrv->GetVolMethod() == VOL_METHOD_MULTI)
	//{
	//	//for (int i=0; i<m_vrv->GetAllVolumeNum(); i++)
	//	//{
	//	//	VolumeData* vd = m_vrv->GetAllVolumeData(i);
	//	//	if (vd)
	//	//		vd->SetNR(val);
	//	//}
	//}
	//else
	//{
	//	//if (m_sync_group && m_group)
	//	//	m_group->SetNR(val);
	//	//else if (m_group && m_group->GetBlendMode()==2)
	//	//	m_group->SetNR(val);
	//	//else if (m_vd)
	//	//	m_vd->SetNR(val);
	//	m_vd->setValue("noise redct", val);
	//}

	//RefreshVRenderViews(false, true);
}

//depth mode
void VolumePropPanel::OnDepthCheck(wxCommandEvent &event)
{
	bool val = m_options_toolbar->GetToolState(ID_DepthChk);
	if (val)
		m_options_toolbar->SetToolNormalBitmap(ID_DepthChk, wxGetBitmapFromMemory(depth));
	else
		m_options_toolbar->SetToolNormalBitmap(ID_DepthChk, wxGetBitmapFromMemory(depth_off));

	//if (val)
	//{
	//	if (m_group)
	//	{
	//		m_group->SetBlendMode(2);
	//		if (m_vd)
	//		{
	//			m_group->SetNR(m_vd->GetNR());
	//			m_group->SetSampleRate(m_vd->GetSampleRate());
	//			m_group->SetShadow(m_vd->GetShadow());
	//			double sp;
	//			m_vd->GetShadowParams(sp);
	//			m_group->SetShadowParams(sp);
	//		}
	//	}
	//}
	//else
	//{
	//	if (m_group)
	//		m_group->SetBlendMode(0);
	//}

	//RefreshVRenderViews(false, true);
}

bool VolumePropPanel::SetSpacings()
{
	if (!m_space_x_text || !m_space_y_text || !m_space_z_text)
		return false;

	wxString str, str_new;
	double spcx = 0.0;
	double spcy = 0.0;
	double spcz = 0.0;

	str = m_space_x_text->GetValue();
	str.ToDouble(&spcx);
	if (spcx <= 0.0)
		return false;

	str = m_space_y_text->GetValue();
	str.ToDouble(&spcy);
	if (spcy <= 0.0)
		return false;

	str = m_space_z_text->GetValue();
	str.ToDouble(&spcz);
	if (spcz <= 0.0)
		return false;
	bool override_vox = true;
	//VRenderFrame* vr_frame = (VRenderFrame*)m_frame;
	//if (vr_frame && vr_frame->GetSettingDlg())
	//	override_vox = vr_frame->GetSettingDlg()->GetOverrideVox();

	//if ((m_sync_group || override_vox) && m_group)
	//{
	//	for (int i = 0; i < m_group->GetVolumeNum(); i++)
	//	{
	//		m_group->GetVolumeData(i)->SetSpacings(spcx, spcy, spcz);
	//		m_group->GetVolumeData(i)->SetBaseSpacings(spcx, spcy, spcz);
	//	}
	//}
	//else if (m_vd)
	//{
	//	m_vd->SetSpacings(spcx, spcy, spcz);
	//	m_vd->SetBaseSpacings(spcx, spcy, spcz);
	//}
	//else return false;
	m_agent->setValue("spc x", spcx);
	m_agent->setValue("spc y", spcy);
	m_agent->setValue("spc z", spcz);
	m_agent->setValue("base spc x", spcx);
	m_agent->setValue("base spc y", spcy);
	m_agent->setValue("base spc z", spcz);

	return true;
}

//enable/disable
void VolumePropPanel::EnableAlpha()
{
	long mip_mode;
	m_agent->getValue("mip mode", mip_mode);
	if (mip_mode != 1)
	{
		m_alpha_sldr->Enable();
		m_alpha_text->Enable();
	}
}

void VolumePropPanel::DisableAlpha()
{
	m_alpha_sldr->Disable();
	m_alpha_text->Disable();
}

void VolumePropPanel::EnableShading()
{
	m_low_shading_sldr->Enable();
	m_low_shading_text->Enable();
	m_hi_shading_sldr->Enable();
	m_hi_shading_text->Enable();
	long mip_mode;
	m_agent->getValue("mip mode", mip_mode);
	if (mip_mode == 1)
	{
		m_left_thresh_sldr->Enable();
		m_left_thresh_text->Enable();
		m_right_thresh_sldr->Enable();
		m_right_thresh_text->Enable();
	}
}

void VolumePropPanel::DisableShading()
{
	m_low_shading_sldr->Disable();
	m_low_shading_text->Disable();
	m_hi_shading_sldr->Disable();
	m_hi_shading_text->Disable();
	long mip_mode;
	m_agent->getValue("mip mode", mip_mode);
	bool shadow_enable;
	m_agent->getValue("shadow enable", shadow_enable);
	if (mip_mode == 1 &&
		!shadow_enable)
	{
		m_left_thresh_sldr->Disable();
		m_left_thresh_text->Disable();
		m_right_thresh_sldr->Disable();
		m_right_thresh_text->Disable();
	}
}

void VolumePropPanel::EnableShadow()
{
	m_shadow_sldr->Enable();
	m_shadow_text->Enable();
	long mip_mode;
	m_agent->getValue("mip mode", mip_mode);
	if (mip_mode == 1)
	{
		m_left_thresh_sldr->Enable();
		m_left_thresh_text->Enable();
		m_right_thresh_sldr->Enable();
		m_right_thresh_text->Enable();
	}
}

void VolumePropPanel::DisableShadow()
{
	m_shadow_sldr->Disable();
	m_shadow_text->Disable();
	long mip_mode;
	m_agent->getValue("mip mode", mip_mode);
	bool shading_enable;
	m_agent->getValue("shading enable", shading_enable);
	if (mip_mode == 1 &&
		!shading_enable)
	{
		m_left_thresh_sldr->Disable();
		m_left_thresh_text->Disable();
		m_right_thresh_sldr->Disable();
		m_right_thresh_text->Disable();
	}
}

void VolumePropPanel::EnableColormap()
{
	m_colormap_high_value_sldr->Enable();
	m_colormap_high_value_text->Enable();
	m_colormap_low_value_sldr->Enable();
	m_colormap_low_value_text->Enable();
}

void VolumePropPanel::DisableColormap()
{
	m_colormap_high_value_sldr->Disable();
	m_colormap_high_value_text->Disable();
	m_colormap_low_value_sldr->Disable();
	m_colormap_low_value_text->Disable();
}

void VolumePropPanel::EnableMip()
{
	DisableAlpha();
	m_boundary_sldr->Disable();
	m_boundary_text->Disable();
	m_luminance_sldr->Disable();
	m_luminance_text->Disable();
	bool shading_enable;
	m_agent->getValue("shading enable", shading_enable);
	bool shadow_enable;
	m_agent->getValue("shadow enable", shadow_enable);
	if (shading_enable ||
		shadow_enable)
		EnableShading();
	else
		DisableShading();
}

void VolumePropPanel::DisableMip()
{
	bool alpha_enable;
	m_agent->getValue("alpha enable", alpha_enable);
	if (alpha_enable)
		EnableAlpha();
	else
		DisableAlpha();
	m_boundary_sldr->Enable();
	m_boundary_text->Enable();
	m_luminance_sldr->Enable();
	m_luminance_text->Enable();
	bool shading_enable;
	m_agent->getValue("shading enable", shading_enable);
	bool shadow_enable;
	m_agent->getValue("shadow enable", shadow_enable);
	if (shading_enable ||
		shadow_enable)
		EnableShading();
	else
		DisableShading();
	m_left_thresh_sldr->Enable();
	m_left_thresh_text->Enable();
	m_right_thresh_sldr->Enable();
	m_right_thresh_text->Enable();
}

//update max value
void VolumePropPanel::UpdateMaxVal(double value)
{
	long bits;
	m_agent->getValue("bits", bits);
	if (bits == 8)
		return;
	else if (bits > 8)
	{
		if (value < 255.0)
			value = 255.0;
		if (value > 65535.0)
			value = 65535.0;
	}
	m_max_val = value;
	m_agent->setValue("max int", m_max_val);
	m_agent->setValue("int scale", 65535.0 / m_max_val);
	//GetSettings();
}

void VolumePropPanel::OnSpaceText(wxCommandEvent& event)
{
	//if (SetSpacings())
	//	InitVRenderViews(INIT_BOUNDS | INIT_CENTER);
}

//legend
void VolumePropPanel::OnLegendCheck(wxCommandEvent& event)
{
	bool leg = m_options_toolbar->GetToolState(ID_LegendChk);
	m_agent->setValue("legend", leg);

	//RefreshVRenderViews(false, true);
}

//interpolation
void VolumePropPanel::OnInterpolateCheck(wxCommandEvent& event)
{
	bool inv = m_options_toolbar->GetToolState(ID_InterpolateChk);
	if (inv)
		m_options_toolbar->SetToolNormalBitmap(ID_InterpolateChk,
			wxGetBitmapFromMemory(interpolate));
	else
		m_options_toolbar->SetToolNormalBitmap(ID_InterpolateChk,
			wxGetBitmapFromMemory(interpolate_off));
	//if (m_sync_group && m_group)
	//	m_group->SetInterpolate(inv);
	//else if (m_vd)
	//	m_vd->SetInterpolate(inv);
	m_agent->setValue("interpolate", inv);
	//if (m_vrv)
	//	m_vrv->m_glview->SetIntp(inv);

	//RefreshVRenderViews(false, true);
}

//sync within group
void VolumePropPanel::OnSyncGroupCheck(wxCommandEvent& event)
{
	//m_sync_group = m_options_toolbar->GetToolState(ID_SyncGroupChk);
	/*	if (m_group)
	m_group->SetVolumeSyncProp(m_sync_group);

	if (m_sync_group && m_group)
	{
	wxString str;
	double dVal;
	long iVal;
	bool bVal;

	//gamma
	str = m_gamma_text->GetValue();
	str.ToDouble(&dVal);
	m_group->Set3DGamma(dVal);
	//boundary
	str = m_boundary_text->GetValue();
	str.ToDouble(&dVal);
	m_group->SetBoundary(dVal);
	//saturation
	str = m_saturation_text->GetValue();
	str.ToLong(&iVal);
	dVal = double(iVal) / m_max_val;
	m_group->SetOffset(dVal);
	//left threshold
	str = m_left_thresh_text->GetValue();
	str.ToLong(&iVal);
	dVal = double(iVal) / m_max_val;
	m_group->SetLeftThresh(dVal);
	//right thresh
	str = m_right_thresh_text->GetValue();
	str.ToLong(&iVal);
	dVal = double(iVal) / m_max_val;
	m_group->SetRightThresh(dVal);
	//luminance
	//str = m_luminance_text->GetValue();
	//str.ToLong(&iVal);
	//dVal = double(iVal)/m_max_val;
	//m_group->SetLuminance(dVal);
	//shadow
	bVal = m_shadow_tool->GetToolState(ID_ShadowChk);
	m_group->SetShadow(bVal);
	str = m_shadow_text->GetValue();
	str.ToDouble(&dVal);
	m_group->SetShadowParams(dVal);
	//high shading
	str = m_hi_shading_text->GetValue();
	str.ToDouble(&dVal);
	m_group->SetHiShading(dVal);
	//alpha
	str = m_alpha_text->GetValue();
	str.ToLong(&iVal);
	dVal = double(iVal) / m_max_val;
	m_group->SetAlpha(dVal);
	//sample rate
	str = m_sample_text->GetValue();
	str.ToDouble(&dVal);
	m_group->SetSampleRate(dVal);
	//shading
	bVal = m_shade_tool->GetToolState(ID_ShadingEnableChk);
	m_group->SetShading(bVal);
	str = m_low_shading_text->GetValue();
	str.ToDouble(&dVal);
	m_group->SetLowShading(dVal);
	//colormap low
	str = m_colormap_low_value_text->GetValue();
	str.ToLong(&iVal);
	dVal = double(iVal)/m_max_val;
	m_group->SetColormapValues(dVal, -1);
	//colormap high
	str = m_colormap_high_value_text->GetValue();
	str.ToLong(&iVal);
	dVal = double(iVal)/m_max_val;
	m_group->SetColormapValues(-1, dVal);
	//inversion
	bVal = m_options_toolbar->GetToolState(ID_InvChk);
	m_group->SetInvert(bVal);
	//interpolation
	bVal = m_options_toolbar->GetToolState(ID_InterpolateChk);
	m_group->SetInterpolate(bVal);
	if (m_vrv)
	m_vrv->m_glview->SetIntp(bVal);
	//MIP
	bVal = m_options_toolbar->GetToolState(ID_MipChk);
	m_group->SetMode(bVal?1:0);
	//noise reduction
	bVal = m_options_toolbar->GetToolState(ID_InvChk);
	m_group->SetNR(bVal);
	}
	*/
	//RefreshVRenderViews(false, true);
}

void VolumePropPanel::OnSaveDefault(wxCommandEvent& event)
{
	/*	VRenderFrame* vr_frame = (VRenderFrame*)m_frame;
	if (!vr_frame)
	return;
	DataManager *mgr = vr_frame->GetDataManager();
	if (!mgr)
	return;

	wxString app_name = "FluoRender " +
	wxString::Format("%d.%.1f", VERSION_MAJOR, float(VERSION_MINOR));
	wxString vendor_name = "FluoRender";
	wxString local_name = "default_volume_settings.dft";
	wxFileConfig fconfig(app_name, vendor_name, local_name, "",
	wxCONFIG_USE_LOCAL_FILE);
	wxString str;
	double val;
	int ival;
	//extract boundary
	str = m_boundary_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("extract_boundary", val);
	mgr->m_vol_exb = val;
	//gamma
	str = m_gamma_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("gamma", val);
	mgr->m_vol_gam = val;
	//low offset
	str = m_saturation_text->GetValue();
	str.ToDouble(&val);
	val /= m_max_val;
	fconfig.Write("low_offset", val);
	mgr->m_vol_of1 = val;
	//high offset
	val = 1.0;
	fconfig.Write("high_offset", val);
	mgr->m_vol_of2 = val;
	//low thresholding
	str = m_left_thresh_text->GetValue();
	str.ToDouble(&val);
	val /= m_max_val;
	fconfig.Write("low_thresholding", val);
	mgr->m_vol_lth = val;
	//high thresholding
	str = m_right_thresh_text->GetValue();
	str.ToDouble(&val);
	val /= m_max_val;
	fconfig.Write("high_thresholding", val);
	mgr->m_vol_hth = val;
	//low shading
	str = m_low_shading_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("low_shading", val);
	mgr->m_vol_lsh = val;
	//high shading
	str = m_hi_shading_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("high_shading", val);
	mgr->m_vol_hsh = val;
	//alpha
	str = m_alpha_text->GetValue();
	str.ToDouble(&val);
	val /= m_max_val;
	fconfig.Write("alpha", val);
	mgr->m_vol_alf = val;
	//sample rate
	str = m_sample_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("sample_rate", val);
	mgr->m_vol_spr = val;
	//x spacing
	str = m_space_x_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("x_spacing", val);
	mgr->m_vol_xsp = val;
	//y spacing
	str = m_space_y_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("y_spacing", val);
	mgr->m_vol_ysp = val;
	//z spacing
	str = m_space_z_text->GetValue();
	str.ToDouble(&val);
	fconfig.Write("z_spacing", val);
	mgr->m_vol_zsp = val;
	//luminance
	str = m_luminance_text->GetValue();
	str.ToDouble(&val);
	val /= m_max_val;
	fconfig.Write("luminance", val);
	mgr->m_vol_lum = val;
	//colormap enable
	bool bval = m_colormap_tool->GetToolState(ID_ColormapEnableChk);
	fconfig.Write("colormap_mode", bval);
	mgr->m_vol_cmm = bval;
	//colormap type
	ival = m_colormap_combo->GetCurrentSelection();
	fconfig.Write("colormap", ival);
	mgr->m_vol_cmp = ival;
	//colormap projection
	ival = m_colormap_combo2->GetCurrentSelection();
	fconfig.Write("colormap_proj", ival);
	mgr->m_vol_cmj = ival;
	//colormap low value
	str = m_colormap_low_value_text->GetValue();
	str.ToDouble(&val);
	val /= m_max_val;
	fconfig.Write("colormap_low", val);
	mgr->m_vol_lcm = val;
	//colormap high value
	str = m_colormap_high_value_text->GetValue();
	str.ToDouble(&val);
	val /= m_max_val;
	fconfig.Write("colormap_hi", val);
	mgr->m_vol_hcm = val;
	//alpha
	bool alpha = m_alpha_tool->GetToolState(ID_AlphaChk);
	fconfig.Write("enable_alpha", alpha);
	mgr->m_vol_eap = alpha;
	//enable shading
	bool shading = m_shade_tool->GetToolState(ID_ShadingEnableChk);
	fconfig.Write("enable_shading", shading);
	mgr->m_vol_esh = shading;
	//inversion
	bool interp = m_options_toolbar->GetToolState(ID_InterpolateChk);
	fconfig.Write("enable_interp", interp);
	mgr->m_vol_interp = interp;
	//inversion
	bool inv = m_options_toolbar->GetToolState(ID_InvChk);
	fconfig.Write("enable_inv", inv);
	mgr->m_vol_inv = inv;
	//enable mip
	bool mip = m_options_toolbar->GetToolState(ID_MipChk);
	fconfig.Write("enable_mip", mip);
	mgr->m_vol_mip = mip;
	//noise reduction
	bool nrd = m_options_toolbar->GetToolState(ID_NRChk);
	fconfig.Write("noise_rd", nrd);
	mgr->m_vol_nrd = nrd;
	//shadow
	bool shw = m_shadow_tool->GetToolState(ID_ShadowChk);
	fconfig.Write("enable_shadow", shw);
	mgr->m_vol_shw = shw;
	//shadow intensity
	str = m_shadow_text->GetValue();
	str.ToDouble(&val);
	double swi = val;
	fconfig.Write("shadow_intensity", swi);
	mgr->m_vol_swi = swi;
	wxString expath = wxStandardPaths::Get().GetExecutablePath();
	expath = wxPathOnly(expath);
	wxString dft = expath + "/default_volume_settings.dft";
	wxFileOutputStream os(dft);
	fconfig.Save(os);*/
}

void VolumePropPanel::OnResetDefault(wxCommandEvent &event)
{
	/*	VRenderFrame* vr_frame = (VRenderFrame*)m_frame;
	if (!vr_frame)
	return;
	DataManager *mgr = vr_frame->GetDataManager();
	if (!mgr)
	return;
	if (!m_vd)
	return;

	wxString str;
	double dval;
	int ival;
	bool bval;

	//gamma
	dval = mgr->m_vol_gam;
	str = wxString::Format("%.2f", dval);
	m_gamma_text->ChangeValue(str);
	ival = int(dval*100.0+0.5);
	m_gamma_sldr->SetValue(ival);
	m_vd->Set3DGamma(dval);
	//extract boundary
	dval = mgr->m_vol_exb;
	str = wxString::Format("%.4f", dval);
	m_boundary_text->ChangeValue(str);
	ival = int(dval*2000.0+0.5);
	m_boundary_sldr->SetValue(ival);
	m_vd->SetBoundary(dval);
	//low offset
	dval = mgr->m_vol_of1;
	ival = int(dval*m_max_val+0.5);
	str = wxString::Format("%d", ival);
	m_saturation_text->ChangeValue(str);
	m_saturation_sldr->SetValue(ival);
	m_vd->SetOffset(dval);
	//low thresholding
	dval = mgr->m_vol_lth;
	ival = int(dval*m_max_val+0.5);
	str = wxString::Format("%d", ival);
	m_left_thresh_text->ChangeValue(str);
	m_left_thresh_sldr->SetValue(ival);
	m_vd->SetLeftThresh(dval);
	//high thresholding
	dval = mgr->m_vol_hth;
	ival = int(dval*m_max_val+0.5);
	str = wxString::Format("%d", ival);
	m_right_thresh_text->ChangeValue(str);
	m_right_thresh_sldr->SetValue(ival);
	m_vd->SetRightThresh(dval);
	//low shading
	dval = mgr->m_vol_lsh;
	str = wxString::Format("%.2f", dval);
	m_low_shading_text->ChangeValue(str);
	ival = int(dval*100.0+0.5);
	m_low_shading_sldr->SetValue(ival);
	double amb, diff, spec, shine;
	m_vd->GetMaterial(amb, diff, spec, shine);
	m_vd->SetMaterial(dval, diff, spec, shine);
	//high shading
	dval = mgr->m_vol_hsh;
	str = wxString::Format("%.2f", dval);
	m_hi_shading_text->ChangeValue(str);
	ival = int(dval*10.0+0.5);
	m_hi_shading_sldr->SetValue(ival);
	m_vd->GetMaterial(amb, diff, spec, shine);
	m_vd->SetMaterial(amb, diff, spec, dval);
	//alpha
	dval = mgr->m_vol_alf;
	ival = int(dval*m_max_val+0.5);
	str = wxString::Format("%d", ival);
	m_alpha_text->ChangeValue(str);
	m_alpha_sldr->SetValue(ival);
	m_vd->SetAlpha(dval);
	//sample rate
	dval = mgr->m_vol_spr;
	str = wxString::Format("%.1f", dval);
	m_sample_text->ChangeValue(str);
	ival = int(dval*10.0+0.5);
	m_sample_sldr->SetValue(ival);
	m_vd->SetSampleRate(dval);
	//luminance
	dval = mgr->m_vol_lum;
	ival = int(dval*m_max_val+0.5);
	str = wxString::Format("%d", ival);
	m_luminance_text->ChangeValue(str);
	m_luminance_sldr->SetValue(ival);
	double h, s, v;
	m_vd->GetHSV(h, s, v);
	HSVColor hsv(h, s, dval);
	Color color(hsv);
	m_vd->ResetMaskColorSet();
	m_vd->SetColor(color);
	wxColor wxc((unsigned char)(color.r()*255),
	(unsigned char)(color.g()*255),
	(unsigned char)(color.b()*255));
	m_color_text->ChangeValue(wxString::Format("%d , %d , %d",
	wxc.Red(), wxc.Green(), wxc.Blue()));
	m_color_btn->SetColour(wxc);
	color = m_vd->GetMaskColor();
	wxc = wxColor((unsigned char)(color.r()*255),
	(unsigned char)(color.g()*255),
	(unsigned char)(color.b()*255));
	m_color2_text->ChangeValue(wxString::Format("%d , %d , %d",
	wxc.Red(), wxc.Green(), wxc.Blue()));
	m_color2_btn->SetColour(wxc);
	//colormap mode
	m_vd->SetColormapMode(mgr->m_vol_cmm);
	bool colormap = m_vd->GetColormapMode() == 1;
	m_colormap_tool->ToggleTool(ID_ColormapEnableChk, colormap);
	//colormap
	m_colormap_combo->SetSelection(mgr->m_vol_cmp);
	m_vd->SetColormap(mgr->m_vol_cmp);
	//colormap projection
	m_colormap_combo2->SetSelection(mgr->m_vol_cmj);
	m_vd->SetColormapProj(mgr->m_vol_cmj);
	//colormap low value
	dval = mgr->m_vol_lcm;
	ival = int(dval*m_max_val+0.5);
	str = wxString::Format("%d", ival);
	m_colormap_low_value_text->ChangeValue(str);
	m_colormap_low_value_sldr->SetValue(ival);
	double lcm = dval;
	dval = mgr->m_vol_hcm;
	ival = int(dval*m_max_val+0.5);
	str = wxString::Format("%d", ival);
	m_colormap_high_value_text->ChangeValue(str);
	m_colormap_high_value_sldr->SetValue(ival);
	m_vd->SetColormapValues(lcm, dval);
	//shadow intensity
	dval = mgr->m_vol_swi;
	str = wxString::Format("%.2f", dval);
	ival = int(dval*100.0+0.5);
	m_shadow_text->ChangeValue(str);
	m_shadow_sldr->SetValue(ival);
	m_vd->SetShadowParams(dval);

	//enable alpha
	bval = mgr->m_vol_eap;
	m_alpha_tool->ToggleTool(ID_AlphaChk,bval);
	if (m_sync_group && m_group)
	m_group->SetEnableAlpha(bval);
	else
	m_vd->SetEnableAlpha(bval);
	//enable shading
	bval = mgr->m_vol_esh;
	m_shade_tool->ToggleTool(ID_ShadingEnableChk,bval);
	if (m_sync_group && m_group)
	m_group->SetShading(bval);
	else
	m_vd->SetShading(bval);
	//inversion
	bval = mgr->m_vol_inv;
	m_options_toolbar->ToggleTool(ID_InvChk,bval);
	if (m_sync_group && m_group)
	m_group->SetInvert(bval);
	else
	m_vd->SetInvert(bval);
	//enable mip
	bval = mgr->m_vol_mip;
	m_options_toolbar->ToggleTool(ID_MipChk,bval);
	if (m_sync_group && m_group)
	m_group->SetMode(bval?1:0);
	else
	m_vd->SetMode(bval?1:0);
	//noise reduction
	bval = mgr->m_vol_nrd;
	m_options_toolbar->ToggleTool(ID_NRChk,bval);
	if (m_sync_group && m_group)
	m_group->SetNR(bval);
	else
	m_vd->SetNR(bval);
	//shadow
	bval = mgr->m_vol_shw;
	m_shadow_tool->ToggleTool(ID_ShadowChk,bval);
	if (m_sync_group && m_group)
	m_group->SetShadow(bval);
	else
	m_vd->SetShadow(bval);

	if (m_vd->GetEnableAlpha())
	EnableAlpha();
	else
	DisableAlpha();
	if (m_vd->GetVR()->get_shading())
	EnableShading();
	else
	DisableShading();
	if (m_vd->GetShadow())
	EnableShadow();
	else
	DisableShadow();
	if (m_vd->GetColormapMode() == 1)
	EnableColormap();
	else
	DisableColormap();
	if (m_vd->GetMode() == 1)
	EnableMip();
	else
	DisableMip();

	//apply all
	RefreshVRenderViews(false, true);*/
}