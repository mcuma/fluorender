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
#include "ComponentDlg.h"
#include "VRenderFrame.h"
#include "Components/CompGenerator.h"
#include <wx/valnum.h>
#include <wx/stdpaths.h>

BEGIN_EVENT_TABLE(ComponentDlg, wxPanel)
	EVT_COLLAPSIBLEPANE_CHANGED(wxID_ANY, ComponentDlg::OnPaneChange)
	//initial grow
	EVT_CHECKBOX(ID_InitialGrowCheck, ComponentDlg::OnInitialGrowCheck)
	EVT_CHECKBOX(ID_IGParamTransitionCheck, ComponentDlg::OnIGParamTransitionCheck)
	EVT_COMMAND_SCROLL(ID_IGIterationsSldr, ComponentDlg::OnIGIterationsSldr)
	EVT_TEXT(ID_IGIterationsText, ComponentDlg::OnIGIterationsText)
	//translate
	EVT_COMMAND_SCROLL(ID_IGTranslateSldr, ComponentDlg::OnIGTranslateSldr)
	EVT_TEXT(ID_IGTranslateText, ComponentDlg::OnIGTranslateText)
	EVT_COMMAND_SCROLL(ID_IGTranslate2Sldr, ComponentDlg::OnIGTranslate2Sldr)
	EVT_TEXT(ID_IGTranslate2Text, ComponentDlg::OnIGTranslate2Text)
	//scalar falloff
	EVT_COMMAND_SCROLL(ID_IGScalarFalloffSldr, ComponentDlg::OnIGScalarFalloffSldr)
	EVT_TEXT(ID_IGScalarFalloffText, ComponentDlg::OnIGScalarFalloffText)
	EVT_COMMAND_SCROLL(ID_IGScalarFalloff2Sldr, ComponentDlg::OnIGScalarFalloff2Sldr)
	EVT_TEXT(ID_IGScalarFalloff2Text, ComponentDlg::OnIGScalarFalloff2Text)
	//grad falloff
	EVT_COMMAND_SCROLL(ID_IGGradFalloffSldr, ComponentDlg::OnIGGradFalloffSldr)
	EVT_TEXT(ID_IGGradFalloffText, ComponentDlg::OnIGGradFalloffText)
	EVT_COMMAND_SCROLL(ID_IGGradFalloff2Sldr, ComponentDlg::OnIGGradFalloff2Sldr)
	EVT_TEXT(ID_IGGradFalloff2Text, ComponentDlg::OnIGGradFalloff2Text)
	//variance falloff
	EVT_COMMAND_SCROLL(ID_IGVarFalloffSldr, ComponentDlg::OnIGVarFalloffSldr)
	EVT_TEXT(ID_IGVarFalloffText, ComponentDlg::OnIGVarFalloffText)
	EVT_COMMAND_SCROLL(ID_IGVarFalloff2Sldr, ComponentDlg::OnIGVarFalloff2Sldr)
	EVT_TEXT(ID_IGVarFalloff2Text, ComponentDlg::OnIGVarFalloff2Text)
	//angle falloff
	EVT_COMMAND_SCROLL(ID_IGAngleFalloffSldr, ComponentDlg::OnIGAngleFalloffSldr)
	EVT_TEXT(ID_IGAngleFalloffText, ComponentDlg::OnIGAngleFalloffText)
	EVT_COMMAND_SCROLL(ID_IGAngleFalloff2Sldr, ComponentDlg::OnIGAngleFalloff2Sldr)
	EVT_TEXT(ID_IGAngleFalloff2Text, ComponentDlg::OnIGAngleFalloff2Text)

	//sized grow
	EVT_CHECKBOX(ID_SizedGrowCheck, ComponentDlg::OnSizedGrowCheck)
	EVT_CHECKBOX(ID_SGParamTransitionCheck, ComponentDlg::OnSGParamTransitionCheck)
	EVT_COMMAND_SCROLL(ID_SGIterationsSldr, ComponentDlg::OnSGIterationsSldr)
	EVT_TEXT(ID_SGIterationsText, ComponentDlg::OnSGIterationsText)
	//size limiter
	EVT_COMMAND_SCROLL(ID_SGSizeLimiterSldr, ComponentDlg::OnSGSizeLimiterSldr)
	EVT_TEXT(ID_SGSizeLimiterText, ComponentDlg::OnSGSizeLimiterText)
	EVT_COMMAND_SCROLL(ID_SGSizeLimiter2Sldr, ComponentDlg::OnSGSizeLimiter2Sldr)
	EVT_TEXT(ID_SGSizeLimiter2Text, ComponentDlg::OnSGSizeLimiter2Text)
	//translate
	EVT_COMMAND_SCROLL(ID_SGTranslateSldr, ComponentDlg::OnSGTranslateSldr)
	EVT_TEXT(ID_SGTranslateText, ComponentDlg::OnSGTranslateText)
	EVT_COMMAND_SCROLL(ID_SGTranslate2Sldr, ComponentDlg::OnSGTranslate2Sldr)
	EVT_TEXT(ID_SGTranslate2Text, ComponentDlg::OnSGTranslate2Text)
	//scalar falloff
	EVT_COMMAND_SCROLL(ID_SGScalarFalloffSldr, ComponentDlg::OnSGScalarFalloffSldr)
	EVT_TEXT(ID_SGScalarFalloffText, ComponentDlg::OnSGScalarFalloffText)
	EVT_COMMAND_SCROLL(ID_SGScalarFalloff2Sldr, ComponentDlg::OnSGScalarFalloff2Sldr)
	EVT_TEXT(ID_SGScalarFalloff2Text, ComponentDlg::OnSGScalarFalloff2Text)
	//grad falloff
	EVT_COMMAND_SCROLL(ID_SGGradFalloffSldr, ComponentDlg::OnSGGradFalloffSldr)
	EVT_TEXT(ID_SGGradFalloffText, ComponentDlg::OnSGGradFalloffText)
	EVT_COMMAND_SCROLL(ID_SGGradFalloff2Sldr, ComponentDlg::OnSGGradFalloff2Sldr)
	EVT_TEXT(ID_SGGradFalloff2Text, ComponentDlg::OnSGGradFalloff2Text)
	//variance falloff
	EVT_COMMAND_SCROLL(ID_SGVarFalloffSldr, ComponentDlg::OnSGVarFalloffSldr)
	EVT_TEXT(ID_SGVarFalloffText, ComponentDlg::OnSGVarFalloffText)
	EVT_COMMAND_SCROLL(ID_SGVarFalloff2Sldr, ComponentDlg::OnSGVarFalloff2Sldr)
	EVT_TEXT(ID_SGVarFalloff2Text, ComponentDlg::OnSGVarFalloff2Text)
	//angle falloff
	EVT_COMMAND_SCROLL(ID_SGAngleFalloffSldr, ComponentDlg::OnSGAngleFalloffSldr)
	EVT_TEXT(ID_SGAngleFalloffText, ComponentDlg::OnSGAngleFalloffText)
	EVT_COMMAND_SCROLL(ID_SGAngleFalloff2Sldr, ComponentDlg::OnSGAngleFalloff2Sldr)
	EVT_TEXT(ID_SGAngleFalloff2Text, ComponentDlg::OnSGAngleFalloff2Text)

	//cleanup
	EVT_CHECKBOX(ID_CleanupCheck, ComponentDlg::OnCleanupCheck)
	EVT_COMMAND_SCROLL(ID_CLIterationsSldr, ComponentDlg::OnCLIterationsSldr)
	EVT_TEXT(ID_CLIterationsText, ComponentDlg::OnCLIterationsText)
	EVT_COMMAND_SCROLL(ID_CLSizeLimiterSldr, ComponentDlg::OnCLSizeLimiterSldr)
	EVT_TEXT(ID_CLSizeLimiterText, ComponentDlg::OnCLSizeLimiterText)

	//match slices
	EVT_CHECKBOX(ID_MatchSlicesCheck, ComponentDlg::OnMatchSlicesCheck)
	EVT_COMMAND_SCROLL(ID_SizeThreshSldr, ComponentDlg::OnSizeThreshSldr)
	EVT_TEXT(ID_SizeThreshText, ComponentDlg::OnSizeThreshText)
	EVT_COMMAND_SCROLL(ID_SizeRatioSldr, ComponentDlg::OnSizeRatioSldr)
	EVT_TEXT(ID_SizeRatioText, ComponentDlg::OnSizeRatioText)
	EVT_COMMAND_SCROLL(ID_DistThreshSldr, ComponentDlg::OnDistThreshSldr)
	EVT_TEXT(ID_DistThreshText, ComponentDlg::OnDistThreshText)
	EVT_COMMAND_SCROLL(ID_AngleThreshSldr, ComponentDlg::OnAngleThreshSldr)
	EVT_TEXT(ID_AngleThreshText, ComponentDlg::OnAngleThreshText)

	//execute
	EVT_BUTTON(ID_ExecuteBtn, ComponentDlg::OnExecute)
END_EVENT_TABLE()

wxWindow* ComponentDlg::Create3DAnalysisPage(wxWindow *parent)
{
	wxPanel *page = new wxPanel(parent);
	wxStaticText *st = 0;

	return page;
}

wxWindow* ComponentDlg::Create2DAnalysisPage(wxWindow *parent)
{
	m_adv_page = new wxScrolledWindow(parent);
	wxStaticText *st = 0;

	//initial grow
	wxBoxSizer* sizer1 = new wxBoxSizer(wxVERTICAL);
	m_initial_grow_pane = CreateInitialGrowPane(m_adv_page);
	sizer1->Add(m_initial_grow_pane, 0, wxEXPAND);
	//sized grow
	wxBoxSizer* sizer2 = new wxBoxSizer(wxVERTICAL);
	m_sized_grow_pane = CreateSizedGrowPane(m_adv_page);
	sizer2->Add(m_sized_grow_pane, 0, wxEXPAND);
	//cleanup
	wxBoxSizer* sizer3 = new wxBoxSizer(wxVERTICAL);
	m_cleanup_pane = CreateCleanupPane(m_adv_page);
	sizer3->Add(m_cleanup_pane, 0, wxEXPAND);
	//match slices
	wxBoxSizer* sizer4 = new wxBoxSizer(wxVERTICAL);
	m_match_slices_pane = CreateMatchSlicesPane(m_adv_page);
	sizer4->Add(m_match_slices_pane, 0, wxEXPAND);

	wxBoxSizer* sizerv = new wxBoxSizer(wxVERTICAL);
	sizerv->Add(10, 10);
	sizerv->Add(sizer1, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer2, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer3, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer4, 0, wxEXPAND);
	sizerv->Add(10, 10);

	m_adv_page->SetScrollRate(10, 10);
	m_adv_page->SetSizer(sizerv);

	return m_adv_page;
}

wxCollapsiblePane* ComponentDlg::CreateInitialGrowPane(wxWindow *parent)
{
	wxCollapsiblePane* collpane = new wxCollapsiblePane(parent,
		ID_InitialGrowPane, "Initial Grow",
		wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
	collpane->SetBackgroundColour(m_notebook->GetThemeBackgroundColour());
	wxWindow *pane = collpane->GetPane();
	wxStaticText* st;
	//validator: integer
	wxIntegerValidator<unsigned int> vald_int;
	//validator: floating point 3
	wxFloatingPointValidator<double> vald_fp3(3);

	//enable
	wxBoxSizer *sizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_initial_grow_check = new wxCheckBox(pane, ID_InitialGrowCheck, "Enable",
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	m_ig_param_transition_check = new wxCheckBox(pane, ID_IGParamTransitionCheck, "Parameter Transition",
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	sizer1->Add(5, 5);
	sizer1->Add(m_initial_grow_check, 0, wxALIGN_CENTER);
	sizer1->Add(5, 5);
	sizer1->Add(m_ig_param_transition_check, 0, wxALIGN_CENTER);
	//iterations
	wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Iterations:",
		wxDefaultPosition, wxSize(100, 23));
	m_ig_iterations_sldr = new wxSlider(pane, ID_IGIterationsSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_iterations_text = new wxTextCtrl(pane, ID_IGIterationsText, "0",
		wxDefaultPosition, wxSize(60, 20), 0, vald_int);
	sizer2->Add(5, 5);
	sizer2->Add(st, 0, wxALIGN_CENTER);
	sizer2->Add(m_ig_iterations_sldr, 1, wxEXPAND);
	sizer2->Add(m_ig_iterations_text, 0, wxALIGN_CENTER);
	//translate
	wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Translation:",
		wxDefaultPosition, wxSize(100, 23));
	m_ig_translate_sldr = new wxSlider(pane, ID_IGTranslateSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_translate_text = new wxTextCtrl(pane, ID_IGTranslateText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_ig_translate_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_ig_translate2_sldr = new wxSlider(pane, ID_IGTranslate2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_translate2_text = new wxTextCtrl(pane, ID_IGTranslate2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer3->Add(5, 5);
	sizer3->Add(st, 0, wxALIGN_CENTER);
	sizer3->Add(m_ig_translate_sldr, 1, wxEXPAND);
	sizer3->Add(m_ig_translate_text, 0, wxALIGN_CENTER);
	sizer3->Add(m_ig_translate_st, 0, wxALIGN_CENTER);
	sizer3->Add(m_ig_translate2_sldr, 1, wxEXPAND);
	sizer3->Add(m_ig_translate2_text, 0, wxALIGN_CENTER);
	//scalar falloff
	wxBoxSizer *sizer4 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Scalar Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_ig_scalar_falloff_sldr = new wxSlider(pane, ID_IGScalarFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_scalar_falloff_text = new wxTextCtrl(pane, ID_IGScalarFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_ig_scalar_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_ig_scalar_falloff2_sldr = new wxSlider(pane, ID_IGScalarFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_scalar_falloff2_text = new wxTextCtrl(pane, ID_IGScalarFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer4->Add(5, 5);
	sizer4->Add(st, 0, wxALIGN_CENTER);
	sizer4->Add(m_ig_scalar_falloff_sldr, 1, wxEXPAND);
	sizer4->Add(m_ig_scalar_falloff_text, 0, wxALIGN_CENTER);
	sizer4->Add(m_ig_scalar_falloff_st, 0, wxALIGN_CENTER);
	sizer4->Add(m_ig_scalar_falloff2_sldr, 1, wxEXPAND);
	sizer4->Add(m_ig_scalar_falloff2_text, 0, wxALIGN_CENTER);
	//grad falloff
	wxBoxSizer *sizer5 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Grad Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_ig_grad_falloff_sldr = new wxSlider(pane, ID_IGGradFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_grad_falloff_text = new wxTextCtrl(pane, ID_IGGradFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_ig_grad_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_ig_grad_falloff2_sldr = new wxSlider(pane, ID_IGGradFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_grad_falloff2_text = new wxTextCtrl(pane, ID_IGGradFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer5->Add(5, 5);
	sizer5->Add(st, 0, wxALIGN_CENTER);
	sizer5->Add(m_ig_grad_falloff_sldr, 1, wxEXPAND);
	sizer5->Add(m_ig_grad_falloff_text, 0, wxALIGN_CENTER);
	sizer5->Add(m_ig_grad_falloff_st, 0, wxALIGN_CENTER);
	sizer5->Add(m_ig_grad_falloff2_sldr, 1, wxEXPAND);
	sizer5->Add(m_ig_grad_falloff2_text, 0, wxALIGN_CENTER);
	//variance falloff
	wxBoxSizer *sizer6 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Var Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_ig_var_falloff_sldr = new wxSlider(pane, ID_IGVarFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_var_falloff_text = new wxTextCtrl(pane, ID_IGVarFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_ig_var_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_ig_var_falloff2_sldr = new wxSlider(pane, ID_IGVarFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_var_falloff2_text = new wxTextCtrl(pane, ID_IGVarFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer6->Add(5, 5);
	sizer6->Add(st, 0, wxALIGN_CENTER);
	sizer6->Add(m_ig_var_falloff_sldr, 1, wxEXPAND);
	sizer6->Add(m_ig_var_falloff_text, 0, wxALIGN_CENTER);
	sizer6->Add(m_ig_var_falloff_st, 0, wxALIGN_CENTER);
	sizer6->Add(m_ig_var_falloff2_sldr, 1, wxEXPAND);
	sizer6->Add(m_ig_var_falloff2_text, 0, wxALIGN_CENTER);
	//angle falloff
	wxBoxSizer *sizer7 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Angle Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_ig_angle_falloff_sldr = new wxSlider(pane, ID_IGAngleFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_angle_falloff_text = new wxTextCtrl(pane, ID_IGAngleFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_ig_angle_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_ig_angle_falloff2_sldr = new wxSlider(pane, ID_IGAngleFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_ig_angle_falloff2_text = new wxTextCtrl(pane, ID_IGAngleFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer7->Add(5, 5);
	sizer7->Add(st, 0, wxALIGN_CENTER);
	sizer7->Add(m_ig_angle_falloff_sldr, 1, wxEXPAND);
	sizer7->Add(m_ig_angle_falloff_text, 0, wxALIGN_CENTER);
	sizer7->Add(m_ig_angle_falloff_st, 0, wxALIGN_CENTER);
	sizer7->Add(m_ig_angle_falloff2_sldr, 1, wxEXPAND);
	sizer7->Add(m_ig_angle_falloff2_text, 0, wxALIGN_CENTER);

	wxBoxSizer* sizerv = new wxBoxSizer(wxVERTICAL);
	sizerv->Add(10, 10);
	sizerv->Add(sizer1, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer2, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer3, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer4, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer5, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer6, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer7, 0, wxEXPAND);
	sizerv->Add(10, 10);

	pane->SetSizer(sizerv);
	sizerv->SetSizeHints(pane);

	return collpane;
}

wxCollapsiblePane* ComponentDlg::CreateSizedGrowPane(wxWindow *parent)
{
	wxCollapsiblePane* collpane = new wxCollapsiblePane(parent,
		ID_SizedGrowPane, "Sized Grow",
		wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
	collpane->SetBackgroundColour(m_notebook->GetThemeBackgroundColour());
	wxWindow *pane = collpane->GetPane();
	wxStaticText* st;
	//validator: integer
	wxIntegerValidator<unsigned int> vald_int;
	//validator: floating point 3
	wxFloatingPointValidator<double> vald_fp3(3);

	//enable
	wxBoxSizer *sizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_sized_grow_check = new wxCheckBox(pane, ID_SizedGrowCheck, "Enable",
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	m_sg_param_transition_check = new wxCheckBox(pane, ID_SGParamTransitionCheck, "Parameter Transition",
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	sizer1->Add(5, 5);
	sizer1->Add(m_sized_grow_check, 0, wxALIGN_CENTER);
	sizer1->Add(5, 5);
	sizer1->Add(m_sg_param_transition_check, 0, wxALIGN_CENTER);
	//iterations
	wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Iterations:",
		wxDefaultPosition, wxSize(100, 23));
	m_sg_iterations_sldr = new wxSlider(pane, ID_SGIterationsSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_iterations_text = new wxTextCtrl(pane, ID_SGIterationsText, "0",
		wxDefaultPosition, wxSize(60, 20), 0, vald_int);
	sizer2->Add(5, 5);
	sizer2->Add(st, 0, wxALIGN_CENTER);
	sizer2->Add(m_sg_iterations_sldr, 1, wxEXPAND);
	sizer2->Add(m_sg_iterations_text, 0, wxALIGN_CENTER);
	//size limiter
	wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Size Limit:",
		wxDefaultPosition, wxSize(100, 23));
	m_sg_size_limiter_sldr = new wxSlider(pane, ID_SGSizeLimiterSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_size_limiter_text = new wxTextCtrl(pane, ID_SGSizeLimiterText, "0",
		wxDefaultPosition, wxSize(60, 20), 0, vald_int);
	m_sg_size_limiter_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_sg_size_limiter2_sldr = new wxSlider(pane, ID_SGSizeLimiter2Sldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_size_limiter2_text = new wxTextCtrl(pane, ID_SGSizeLimiter2Text, "0",
		wxDefaultPosition, wxSize(60, 20), 0, vald_int);
	sizer3->Add(5, 5);
	sizer3->Add(st, 0, wxALIGN_CENTER);
	sizer3->Add(m_sg_size_limiter_sldr, 1, wxEXPAND);
	sizer3->Add(m_sg_size_limiter_text, 0, wxALIGN_CENTER);
	sizer3->Add(m_sg_size_limiter_st, 0, wxALIGN_CENTER);
	sizer3->Add(m_sg_size_limiter2_sldr, 1, wxEXPAND);
	sizer3->Add(m_sg_size_limiter2_text, 0, wxALIGN_CENTER);
	//translate
	wxBoxSizer *sizer4 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Translation:",
		wxDefaultPosition, wxSize(100, 23));
	m_sg_translate_sldr = new wxSlider(pane, ID_SGTranslateSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_translate_text = new wxTextCtrl(pane, ID_SGTranslateText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_sg_translate_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_sg_translate2_sldr = new wxSlider(pane, ID_SGTranslate2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_translate2_text = new wxTextCtrl(pane, ID_SGTranslate2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer4->Add(5, 5);
	sizer4->Add(st, 0, wxALIGN_CENTER);
	sizer4->Add(m_sg_translate_sldr, 1, wxEXPAND);
	sizer4->Add(m_sg_translate_text, 0, wxALIGN_CENTER);
	sizer4->Add(m_sg_translate_st, 0, wxALIGN_CENTER);
	sizer4->Add(m_sg_translate2_sldr, 1, wxEXPAND);
	sizer4->Add(m_sg_translate2_text, 0, wxALIGN_CENTER);
	//scalar falloff
	wxBoxSizer *sizer5 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Scalar Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_sg_scalar_falloff_sldr = new wxSlider(pane, ID_SGScalarFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_scalar_falloff_text = new wxTextCtrl(pane, ID_SGScalarFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_sg_scalar_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_sg_scalar_falloff2_sldr = new wxSlider(pane, ID_SGScalarFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_scalar_falloff2_text = new wxTextCtrl(pane, ID_SGScalarFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer5->Add(5, 5);
	sizer5->Add(st, 0, wxALIGN_CENTER);
	sizer5->Add(m_sg_scalar_falloff_sldr, 1, wxEXPAND);
	sizer5->Add(m_sg_scalar_falloff_text, 0, wxALIGN_CENTER);
	sizer5->Add(m_sg_scalar_falloff_st, 0, wxALIGN_CENTER);
	sizer5->Add(m_sg_scalar_falloff2_sldr, 1, wxEXPAND);
	sizer5->Add(m_sg_scalar_falloff2_text, 0, wxALIGN_CENTER);
	//grad falloff
	wxBoxSizer *sizer6 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Grad Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_sg_grad_falloff_sldr = new wxSlider(pane, ID_SGGradFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_grad_falloff_text = new wxTextCtrl(pane, ID_SGGradFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_sg_grad_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_sg_grad_falloff2_sldr = new wxSlider(pane, ID_SGGradFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_grad_falloff2_text = new wxTextCtrl(pane, ID_SGGradFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer6->Add(5, 5);
	sizer6->Add(st, 0, wxALIGN_CENTER);
	sizer6->Add(m_sg_grad_falloff_sldr, 1, wxEXPAND);
	sizer6->Add(m_sg_grad_falloff_text, 0, wxALIGN_CENTER);
	sizer6->Add(m_sg_grad_falloff_st, 0, wxALIGN_CENTER);
	sizer6->Add(m_sg_grad_falloff2_sldr, 1, wxEXPAND);
	sizer6->Add(m_sg_grad_falloff2_text, 0, wxALIGN_CENTER);
	//variance falloff
	wxBoxSizer *sizer7 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Var Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_sg_var_falloff_sldr = new wxSlider(pane, ID_SGVarFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_var_falloff_text = new wxTextCtrl(pane, ID_SGVarFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_sg_var_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_sg_var_falloff2_sldr = new wxSlider(pane, ID_SGVarFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_var_falloff2_text = new wxTextCtrl(pane, ID_SGVarFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer7->Add(5, 5);
	sizer7->Add(st, 0, wxALIGN_CENTER);
	sizer7->Add(m_sg_var_falloff_sldr, 1, wxEXPAND);
	sizer7->Add(m_sg_var_falloff_text, 0, wxALIGN_CENTER);
	sizer7->Add(m_sg_var_falloff_st, 0, wxALIGN_CENTER);
	sizer7->Add(m_sg_var_falloff2_sldr, 1, wxEXPAND);
	sizer7->Add(m_sg_var_falloff2_text, 0, wxALIGN_CENTER);
	//angle falloff
	wxBoxSizer *sizer8 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Angle Falloff:",
		wxDefaultPosition, wxSize(100, 23));
	m_sg_angle_falloff_sldr = new wxSlider(pane, ID_SGAngleFalloffSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_angle_falloff_text = new wxTextCtrl(pane, ID_SGAngleFalloffText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	m_sg_angle_falloff_st = new wxStaticText(pane, 0, L" \u27a0 ");
	m_sg_angle_falloff2_sldr = new wxSlider(pane, ID_SGAngleFalloff2Sldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_sg_angle_falloff2_text = new wxTextCtrl(pane, ID_SGAngleFalloff2Text, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer8->Add(5, 5);
	sizer8->Add(st, 0, wxALIGN_CENTER);
	sizer8->Add(m_sg_angle_falloff_sldr, 1, wxEXPAND);
	sizer8->Add(m_sg_angle_falloff_text, 0, wxALIGN_CENTER);
	sizer8->Add(m_sg_angle_falloff_st, 0, wxALIGN_CENTER);
	sizer8->Add(m_sg_angle_falloff2_sldr, 1, wxEXPAND);
	sizer8->Add(m_sg_angle_falloff2_text, 0, wxALIGN_CENTER);

	wxBoxSizer* sizerv = new wxBoxSizer(wxVERTICAL);
	sizerv->Add(10, 10);
	sizerv->Add(sizer1, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer2, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer3, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer4, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer5, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer6, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer7, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer8, 0, wxEXPAND);
	sizerv->Add(10, 10);

	pane->SetSizer(sizerv);
	sizerv->SetSizeHints(pane);

	return collpane;
}

wxCollapsiblePane* ComponentDlg::CreateCleanupPane(wxWindow *parent)
{
	wxCollapsiblePane* collpane = new wxCollapsiblePane(parent,
		ID_CleanupPane, "Cleanup",
		wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
	collpane->SetBackgroundColour(m_notebook->GetThemeBackgroundColour());
	wxWindow *pane = collpane->GetPane();
	wxStaticText* st;
	//validator: integer
	wxIntegerValidator<unsigned int> vald_int;

	//enable
	wxBoxSizer *sizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_cleanup_check = new wxCheckBox(pane, ID_CleanupCheck, "Enable",
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	sizer1->Add(5, 5);
	sizer1->Add(m_cleanup_check, 0, wxALIGN_CENTER);
	//iterations
	wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Iterations:",
		wxDefaultPosition, wxSize(100, 23));
	m_cl_iterations_sldr = new wxSlider(pane, ID_CLIterationsSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_cl_iterations_text = new wxTextCtrl(pane, ID_CLIterationsText, "0",
		wxDefaultPosition, wxSize(60, 20), 0, vald_int);
	sizer2->Add(5, 5);
	sizer2->Add(st, 0, wxALIGN_CENTER);
	sizer2->Add(m_cl_iterations_sldr, 1, wxEXPAND);
	sizer2->Add(m_cl_iterations_text, 0, wxALIGN_CENTER);
	//size limiter
	wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Size Limit:",
		wxDefaultPosition, wxSize(100, 23));
	m_cl_size_limiter_sldr = new wxSlider(pane, ID_CLSizeLimiterSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_cl_size_limiter_text = new wxTextCtrl(pane, ID_CLSizeLimiterText, "0",
		wxDefaultPosition, wxSize(60, 20), 0, vald_int);
	sizer3->Add(5, 5);
	sizer3->Add(st, 0, wxALIGN_CENTER);
	sizer3->Add(m_cl_size_limiter_sldr, 1, wxEXPAND);
	sizer3->Add(m_cl_size_limiter_text, 0, wxALIGN_CENTER);

	wxBoxSizer* sizerv = new wxBoxSizer(wxVERTICAL);
	sizerv->Add(10, 10);
	sizerv->Add(sizer1, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer2, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer3, 0, wxEXPAND);
	sizerv->Add(10, 10);

	pane->SetSizer(sizerv);
	sizerv->SetSizeHints(pane);

	return collpane;
}

wxCollapsiblePane* ComponentDlg::CreateMatchSlicesPane(wxWindow *parent)
{
	wxCollapsiblePane* collpane = new wxCollapsiblePane(parent,
		ID_MatchSlicesPane, "Match Slices",
		wxDefaultPosition, wxDefaultSize, wxCP_NO_TLW_RESIZE);
	collpane->SetBackgroundColour(m_notebook->GetThemeBackgroundColour());
	wxWindow *pane = collpane->GetPane();
	wxStaticText* st;
	//validator: integer
	wxIntegerValidator<unsigned int> vald_int;
	//validator: floating point 3
	wxFloatingPointValidator<double> vald_fp3(3);

	//enable
	wxBoxSizer *sizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_match_slices_check = new wxCheckBox(pane, ID_MatchSlicesCheck, "Enable",
		wxDefaultPosition, wxDefaultSize, wxALIGN_RIGHT);
	sizer1->Add(5, 5);
	sizer1->Add(m_match_slices_check, 0, wxALIGN_CENTER);
	//size thresh
	wxBoxSizer *sizer2 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Size Threshold:",
		wxDefaultPosition, wxSize(100, 23));
	m_size_thresh_sldr = new wxSlider(pane, ID_SizeThreshSldr, 0, 0, 100,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_size_thresh_text = new wxTextCtrl(pane, ID_SizeThreshText, "0",
		wxDefaultPosition, wxSize(60, 20), 0, vald_int);
	sizer2->Add(5, 5);
	sizer2->Add(st, 0, wxALIGN_CENTER);
	sizer2->Add(m_size_thresh_sldr, 1, wxEXPAND);
	sizer2->Add(m_size_thresh_text, 0, wxALIGN_CENTER);
	//size ratio
	wxBoxSizer *sizer3 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Size Ratio:",
		wxDefaultPosition, wxSize(100, 23));
	m_size_ratio_sldr = new wxSlider(pane, ID_SizeRatioSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_size_ratio_text = new wxTextCtrl(pane, ID_SizeRatioText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer3->Add(5, 5);
	sizer3->Add(st, 0, wxALIGN_CENTER);
	sizer3->Add(m_size_ratio_sldr, 1, wxEXPAND);
	sizer3->Add(m_size_ratio_text, 0, wxALIGN_CENTER);
	//dist thresh
	wxBoxSizer *sizer4 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Dist Threshold:",
		wxDefaultPosition, wxSize(100, 23));
	m_dist_thresh_sldr = new wxSlider(pane, ID_DistThreshSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_dist_thresh_text = new wxTextCtrl(pane, ID_DistThreshText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer4->Add(5, 5);
	sizer4->Add(st, 0, wxALIGN_CENTER);
	sizer4->Add(m_dist_thresh_sldr, 1, wxEXPAND);
	sizer4->Add(m_dist_thresh_text, 0, wxALIGN_CENTER);
	//angle thresh
	wxBoxSizer *sizer5 = new wxBoxSizer(wxHORIZONTAL);
	st = new wxStaticText(pane, 0, "Ang Threshold:",
		wxDefaultPosition, wxSize(100, 23));
	m_angle_thresh_sldr = new wxSlider(pane, ID_AngleThreshSldr, 0, 0, 1000,
		wxDefaultPosition, wxDefaultSize, wxSL_HORIZONTAL);
	m_angle_thresh_text = new wxTextCtrl(pane, ID_AngleThreshText, "0.000",
		wxDefaultPosition, wxSize(60, 20), 0, vald_fp3);
	sizer5->Add(5, 5);
	sizer5->Add(st, 0, wxALIGN_CENTER);
	sizer5->Add(m_angle_thresh_sldr, 1, wxEXPAND);
	sizer5->Add(m_angle_thresh_text, 0, wxALIGN_CENTER);

	wxBoxSizer* sizerv = new wxBoxSizer(wxVERTICAL);
	sizerv->Add(10, 10);
	sizerv->Add(sizer1, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer2, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer3, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer4, 0, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer5, 0, wxEXPAND);
	sizerv->Add(10, 10);

	pane->SetSizer(sizerv);
	sizerv->SetSizeHints(pane);

	return collpane;
}

ComponentDlg::ComponentDlg(wxWindow *frame, wxWindow *parent)
	: wxPanel(parent, wxID_ANY,
		wxDefaultPosition,
		wxSize(450, 500),
		0, "ComponentDlg"),
	m_frame(parent),
	m_view(0)
{
	//notebook
	m_notebook = new wxNotebook(this, wxID_ANY);
	m_notebook->AddPage(Create3DAnalysisPage(m_notebook), "Basic");
	m_notebook->AddPage(Create2DAnalysisPage(m_notebook), "Advanced");

	wxBoxSizer* sizer1 = new wxBoxSizer(wxHORIZONTAL);
	m_execute_btn = new wxButton(this, ID_ExecuteBtn, "Execute",
		wxDefaultPosition, wxSize(50, 20));
	sizer1->AddStretchSpacer();
	sizer1->Add(m_execute_btn, 0, wxALIGN_CENTER);
	sizer1->Add(10, 10);

	//all controls
	wxBoxSizer* sizerv = new wxBoxSizer(wxVERTICAL);
	sizerv->Add(10, 10);
	sizerv->Add(m_notebook, 1, wxEXPAND);
	sizerv->Add(10, 10);
	sizerv->Add(sizer1, 0, wxEXPAND);
	sizerv->Add(10, 10);

	SetSizer(sizerv);
	Layout();

	GetSettings();
}

ComponentDlg::~ComponentDlg()
{
	SaveSettings();
}

void ComponentDlg::GetSettings()
{
	//defaults
	//initial grow
	m_initial_grow = false;
	m_ig_param_transition = false;
	m_ig_iterations = 50;
	m_ig_translate = m_ig_translate2 = 1.0;
	m_ig_scalar_falloff = m_ig_scalar_falloff2 = 0.15;
	m_ig_grad_falloff = m_ig_grad_falloff2 = 0.1;
	m_ig_var_falloff = m_ig_var_falloff2 = 0.2;
	m_ig_angle_falloff = m_ig_angle_falloff2 = 0.2;

	//sized grow
	m_sized_grow = false;
	m_sg_param_transition = false;
	m_sg_iterations = 40;
	m_sg_size_limiter = m_sg_size_limiter2 = 20;
	m_sg_translate = m_sg_translate2 = 0.5;
	m_sg_scalar_falloff = m_sg_scalar_falloff2 = 0.25;
	m_sg_grad_falloff = m_sg_grad_falloff2 = 0.25;
	m_sg_var_falloff = m_sg_var_falloff2 = 0.35;
	m_sg_angle_falloff = m_sg_angle_falloff2 = 0.35;

	//cleanup
	m_cleanup = false;
	m_cl_iterations = 10;
	m_cl_size_limiter = 5;
	
	//match slices
	m_match_slices = false;
	m_size_thresh = 25;
	m_size_ratio = 0.6;
	m_dist_thresh = 2.5;
	m_angle_thresh = 0.7;

	//read values
	LoadSettings();

	//update ui
	//initial grow
	m_initial_grow_check->SetValue(m_initial_grow);
	EnableInitialGrow(m_initial_grow);
	if (m_initial_grow)
		m_initial_grow_pane->Expand();
	else
		m_initial_grow_pane->Collapse();
	m_ig_param_transition_check->SetValue(m_ig_param_transition);
	EnableIGParamTransition(m_ig_param_transition);
	m_ig_iterations_text->SetValue(wxString::Format("%d", m_ig_iterations));
	m_ig_translate_text->SetValue(wxString::Format("%.3f", m_ig_translate));
	m_ig_scalar_falloff_text->SetValue(wxString::Format("%.3f", m_ig_scalar_falloff));
	m_ig_grad_falloff_text->SetValue(wxString::Format("%.3f", m_ig_grad_falloff));
	m_ig_var_falloff_text->SetValue(wxString::Format("%.3f", m_ig_var_falloff));
	m_ig_angle_falloff_text->SetValue(wxString::Format("%.3f", m_ig_angle_falloff));
	m_ig_translate2_text->SetValue(wxString::Format("%.3f", m_ig_translate2));
	m_ig_scalar_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_scalar_falloff2));
	m_ig_grad_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_grad_falloff2));
	m_ig_var_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_var_falloff2));
	m_ig_angle_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_angle_falloff2));

	//sized grow
	m_sized_grow_check->SetValue(m_sized_grow);
	EnableSizedGrow(m_sized_grow);
	if (m_sized_grow)
		m_sized_grow_pane->Expand();
	else
		m_sized_grow_pane->Collapse();
	m_sg_param_transition_check->SetValue(m_sg_param_transition);
	EnableSGParamTransition(m_sg_param_transition);
	m_sg_iterations_text->SetValue(wxString::Format("%d", m_sg_iterations));
	m_sg_size_limiter_text->SetValue(wxString::Format("%d", m_sg_size_limiter));
	m_sg_translate_text->SetValue(wxString::Format("%.3f", m_sg_translate));
	m_sg_scalar_falloff_text->SetValue(wxString::Format("%.3f", m_sg_scalar_falloff));
	m_sg_grad_falloff_text->SetValue(wxString::Format("%.3f", m_sg_grad_falloff));
	m_sg_var_falloff_text->SetValue(wxString::Format("%.3f", m_sg_var_falloff));
	m_sg_angle_falloff_text->SetValue(wxString::Format("%.3f", m_sg_angle_falloff));
	m_sg_size_limiter2_text->SetValue(wxString::Format("%d", m_sg_size_limiter2));
	m_sg_translate2_text->SetValue(wxString::Format("%.3f", m_sg_translate2));
	m_sg_scalar_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_scalar_falloff2));
	m_sg_grad_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_grad_falloff2));
	m_sg_var_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_var_falloff2));
	m_sg_angle_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_angle_falloff2));

	//cleanup
	m_cleanup_check->SetValue(m_cleanup);
	EnableCleanup(m_cleanup);
	if (m_cleanup)
		m_cleanup_pane->Expand();
	else
		m_cleanup_pane->Collapse();
	m_cl_iterations_text->SetValue(wxString::Format("%d", m_cl_iterations));
	m_cl_size_limiter_text->SetValue(wxString::Format("%d", m_cl_size_limiter));

	//match slices
	m_match_slices_check->SetValue(m_match_slices);
	EnableMatchSlices(m_match_slices);
	if (m_match_slices)
		m_match_slices_pane->Expand();
	else
		m_match_slices_pane->Collapse();
	m_size_thresh_text->SetValue(wxString::Format("%d", m_size_thresh));
	m_size_ratio_text->SetValue(wxString::Format("%.3f", m_size_ratio));
	m_dist_thresh_text->SetValue(wxString::Format("%.3f", m_dist_thresh));
	m_angle_thresh_text->SetValue(wxString::Format("%.3f", m_angle_thresh));
}

void ComponentDlg::LoadSettings()
{
	wxString expath = wxStandardPaths::Get().GetExecutablePath();
	expath = expath.BeforeLast(GETSLASH(), NULL);
#ifdef _WIN32
	wxString dft = expath + "\\default_component_settings.dft";
#else
	wxString dft = expath + "/../Resources/default_component_settings.dft";
#endif
	wxFileInputStream is(dft);
	if (!is.IsOk())
		return;
	wxFileConfig fconfig(is);

	//initial grow
	fconfig.Read("initial_grow", &m_initial_grow);
	fconfig.Read("ig_param_transition", &m_ig_param_transition);
	fconfig.Read("ig_iterations", &m_ig_iterations);
	//translate
	fconfig.Read("ig_translate", &m_ig_translate);
	fconfig.Read("ig_translate2", &m_ig_translate2);
	//scalar falloff
	fconfig.Read("ig_scalar_falloff", &m_ig_scalar_falloff);
	fconfig.Read("ig_scalar_falloff2", &m_ig_scalar_falloff2);
	//grad falloff
	fconfig.Read("ig_grad_falloff", &m_ig_grad_falloff);
	fconfig.Read("ig_grad_falloff2", &m_ig_grad_falloff2);
	//variance falloff
	fconfig.Read("ig_var_falloff", &m_ig_var_falloff);
	fconfig.Read("ig_var_falloff2", &m_ig_var_falloff2);
	//angle falloff
	fconfig.Read("ig_angle_falloff", &m_ig_angle_falloff);
	fconfig.Read("ig_angle_falloff2", &m_ig_angle_falloff2);

	//sized grow
	fconfig.Read("sized_grow", &m_sized_grow);
	fconfig.Read("sg_param_transition", &m_sg_param_transition);
	fconfig.Read("sg_iterations", &m_sg_iterations);
	//size limiter
	fconfig.Read("sg_size_limiter", &m_sg_size_limiter);
	fconfig.Read("sg_size_limiter2", &m_sg_size_limiter2);
	//translate
	fconfig.Read("sg_translate", &m_sg_translate);
	fconfig.Read("sg_translate2", &m_sg_translate2);
	//scalar falloff
	fconfig.Read("sg_scalar_falloff", &m_sg_scalar_falloff);
	fconfig.Read("sg_scalar_falloff2", &m_sg_scalar_falloff2);
	//grad falloff
	fconfig.Read("sg_grad_falloff", &m_sg_grad_falloff);
	fconfig.Read("sg_grad_falloff2", &m_sg_grad_falloff2);
	//variance falloff
	fconfig.Read("sg_var_falloff", &m_sg_var_falloff);
	fconfig.Read("sg_var_falloff2", &m_sg_var_falloff2);
	//angle falloff
	fconfig.Read("sg_angle_falloff", &m_sg_angle_falloff);
	fconfig.Read("sg_angle_falloff2", &m_sg_angle_falloff2);

	//cleanup
	fconfig.Read("cleanup", &m_cleanup);
	fconfig.Read("cl_iterations", &m_cl_iterations);
	fconfig.Read("cl_size_limiter", &m_cl_size_limiter);

	//match slices
	fconfig.Read("match_slices", &m_match_slices);
	fconfig.Read("size_thresh", &m_size_thresh);
	fconfig.Read("size_ratio", &m_size_ratio);
	fconfig.Read("dist_thresh", &m_dist_thresh);
	fconfig.Read("angle_thresh", &m_angle_thresh);
}

void ComponentDlg::SaveSettings()
{
	wxFileConfig fconfig("FluoRender default component settings");

	//initial grow
	fconfig.Write("initial_grow", m_initial_grow);
	fconfig.Write("ig_param_transition", m_ig_param_transition);
	fconfig.Write("ig_iterations", m_ig_iterations);
	//translate
	fconfig.Write("ig_translate", m_ig_translate);
	fconfig.Write("ig_translate2", m_ig_translate2);
	//scalar falloff
	fconfig.Write("ig_scalar_falloff", m_ig_scalar_falloff);
	fconfig.Write("ig_scalar_falloff2", m_ig_scalar_falloff2);
	//grad falloff
	fconfig.Write("ig_grad_falloff", m_ig_grad_falloff);
	fconfig.Write("ig_grad_falloff2", m_ig_grad_falloff2);
	//variance falloff
	fconfig.Write("ig_var_falloff", m_ig_var_falloff);
	fconfig.Write("ig_var_falloff2", m_ig_var_falloff2);
	//angle falloff
	fconfig.Write("ig_angle_falloff", m_ig_angle_falloff);
	fconfig.Write("ig_angle_falloff2", m_ig_angle_falloff2);

	//sized grow
	fconfig.Write("sized_grow", m_sized_grow);
	fconfig.Write("sg_param_transition", m_sg_param_transition);
	fconfig.Write("sg_iterations", m_sg_iterations);
	//size limiter
	fconfig.Write("sg_size_limiter", m_sg_size_limiter);
	fconfig.Write("sg_size_limiter2", m_sg_size_limiter2);
	//translate
	fconfig.Write("sg_translate", m_sg_translate);
	fconfig.Write("sg_translate2", m_sg_translate2);
	//scalar falloff
	fconfig.Write("sg_scalar_falloff", m_sg_scalar_falloff);
	fconfig.Write("sg_scalar_falloff2", m_sg_scalar_falloff2);
	//grad falloff
	fconfig.Write("sg_grad_falloff", m_sg_grad_falloff);
	fconfig.Write("sg_grad_falloff2", m_sg_grad_falloff2);
	//variance falloff
	fconfig.Write("sg_var_falloff", m_sg_var_falloff);
	fconfig.Write("sg_var_falloff2", m_sg_var_falloff2);
	//angle falloff
	fconfig.Write("sg_angle_falloff", m_sg_angle_falloff);
	fconfig.Write("sg_angle_falloff2", m_sg_angle_falloff2);

	//cleanup
	fconfig.Write("cleanup", m_cleanup);
	fconfig.Write("cl_iterations", m_cl_iterations);
	fconfig.Write("cl_size_limiter", m_cl_size_limiter);

	//match slices
	fconfig.Write("match_slices", m_match_slices);
	fconfig.Write("size_thresh", m_size_thresh);
	fconfig.Write("size_ratio", m_size_ratio);
	fconfig.Write("dist_thresh", m_dist_thresh);
	fconfig.Write("angle_thresh", m_angle_thresh);

	wxString expath = wxStandardPaths::Get().GetExecutablePath();
	expath = expath.BeforeLast(GETSLASH(), NULL);
#ifdef _WIN32
	wxString dft = expath + "\\default_component_settings.dft";
#else
	wxString dft = expath + "/../Resources/default_component_settings.dft";
#endif
	wxFileOutputStream os(dft);
	fconfig.Save(os);
}

void ComponentDlg::OnPaneChange(wxCollapsiblePaneEvent& event)
{
	if (m_adv_page)
		m_adv_page->SendSizeEvent();
}

void ComponentDlg::EnableInitialGrow(bool value)
{
	m_initial_grow = value;
	if (m_initial_grow)
	{
		m_ig_param_transition_check->Enable();
		m_ig_iterations_sldr->Enable();
		m_ig_iterations_text->Enable();
		m_ig_translate_sldr->Enable();
		m_ig_translate_text->Enable();
		m_ig_translate2_sldr->Enable();
		m_ig_translate2_text->Enable();
		m_ig_scalar_falloff_sldr->Enable();
		m_ig_scalar_falloff_text->Enable();
		m_ig_scalar_falloff2_sldr->Enable();
		m_ig_scalar_falloff2_text->Enable();
		m_ig_grad_falloff_sldr->Enable();
		m_ig_grad_falloff_text->Enable();
		m_ig_grad_falloff2_sldr->Enable();
		m_ig_grad_falloff2_text->Enable();
		m_ig_var_falloff_sldr->Enable();
		m_ig_var_falloff_text->Enable();
		m_ig_var_falloff2_sldr->Enable();
		m_ig_var_falloff2_text->Enable();
		m_ig_angle_falloff_sldr->Enable();
		m_ig_angle_falloff_text->Enable();
		m_ig_angle_falloff2_sldr->Enable();
		m_ig_angle_falloff2_text->Enable();
	}
	else
	{
		m_ig_param_transition_check->Disable();
		m_ig_iterations_sldr->Disable();
		m_ig_iterations_text->Disable();
		m_ig_translate_sldr->Disable();
		m_ig_translate_text->Disable();
		m_ig_translate2_sldr->Disable();
		m_ig_translate2_text->Disable();
		m_ig_scalar_falloff_sldr->Disable();
		m_ig_scalar_falloff_text->Disable();
		m_ig_scalar_falloff2_sldr->Disable();
		m_ig_scalar_falloff2_text->Disable();
		m_ig_grad_falloff_sldr->Disable();
		m_ig_grad_falloff_text->Disable();
		m_ig_grad_falloff2_sldr->Disable();
		m_ig_grad_falloff2_text->Disable();
		m_ig_var_falloff_sldr->Disable();
		m_ig_var_falloff_text->Disable();
		m_ig_var_falloff2_sldr->Disable();
		m_ig_var_falloff2_text->Disable();
		m_ig_angle_falloff_sldr->Disable();
		m_ig_angle_falloff_text->Disable();
		m_ig_angle_falloff2_sldr->Disable();
		m_ig_angle_falloff2_text->Disable();
	}
}

void ComponentDlg::EnableIGParamTransition(bool value)
{
	m_ig_param_transition = value;
	if (m_ig_param_transition)
	{
		m_ig_translate_st->Show();
		m_ig_translate2_sldr->Show();
		m_ig_translate2_text->Show();
		m_ig_scalar_falloff_st->Show();
		m_ig_scalar_falloff2_sldr->Show();
		m_ig_scalar_falloff2_text->Show();
		m_ig_grad_falloff_st->Show();
		m_ig_grad_falloff2_sldr->Show();
		m_ig_grad_falloff2_text->Show();
		m_ig_var_falloff_st->Show();
		m_ig_var_falloff2_sldr->Show();
		m_ig_var_falloff2_text->Show();
		m_ig_angle_falloff_st->Show();
		m_ig_angle_falloff2_sldr->Show();
		m_ig_angle_falloff2_text->Show();
	}
	else
	{
		m_ig_translate_st->Hide();
		m_ig_translate2_sldr->Hide();
		m_ig_translate2_text->Hide();
		m_ig_scalar_falloff_st->Hide();
		m_ig_scalar_falloff2_sldr->Hide();
		m_ig_scalar_falloff2_text->Hide();
		m_ig_grad_falloff_st->Hide();
		m_ig_grad_falloff2_sldr->Hide();
		m_ig_grad_falloff2_text->Hide();
		m_ig_var_falloff_st->Hide();
		m_ig_var_falloff2_sldr->Hide();
		m_ig_var_falloff2_text->Hide();
		m_ig_angle_falloff_st->Hide();
		m_ig_angle_falloff2_sldr->Hide();
		m_ig_angle_falloff2_text->Hide();
	}

	if (m_adv_page)
		m_adv_page->SendSizeEvent();
}

void ComponentDlg::OnInitialGrowCheck(wxCommandEvent &event)
{
	EnableInitialGrow(m_initial_grow_check->GetValue());
}

void ComponentDlg::OnIGParamTransitionCheck(wxCommandEvent &event)
{
	EnableIGParamTransition(m_ig_param_transition_check->GetValue());

	if (m_ig_param_transition)
	{
		//copy values
		m_ig_translate2_text->SetValue(wxString::Format("%.3f", m_ig_translate));
		m_ig_scalar_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_scalar_falloff));
		m_ig_grad_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_grad_falloff));
		m_ig_var_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_var_falloff));
		m_ig_angle_falloff2_text->SetValue(wxString::Format("%.3f", m_ig_angle_falloff));
	}
}

void ComponentDlg::OnIGIterationsSldr(wxScrollEvent &event)
{
	int val = event.GetPosition();
	m_ig_iterations_text->SetValue(wxString::Format("%d", val));
}

void ComponentDlg::OnIGIterationsText(wxCommandEvent &event)
{
	long val = 0;
	m_ig_iterations_text->GetValue().ToLong(&val);
	m_ig_iterations = val;
	m_ig_iterations_sldr->SetValue(m_ig_iterations);
}

//translate
void ComponentDlg::OnIGTranslateSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_translate_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGTranslateText(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_translate_text->GetValue().ToDouble(&val);
	m_ig_translate = val;
	m_ig_translate_sldr->SetValue(int(m_ig_translate * 1000.0 + 0.5));
}

void ComponentDlg::OnIGTranslate2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_translate2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGTranslate2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_translate2_text->GetValue().ToDouble(&val);
	m_ig_translate2 = val;
	m_ig_translate2_sldr->SetValue(int(m_ig_translate2 * 1000.0 + 0.5));
}

//scalar falloff
void ComponentDlg::OnIGScalarFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_scalar_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGScalarFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_scalar_falloff_text->GetValue().ToDouble(&val);
	m_ig_scalar_falloff = val;
	m_ig_scalar_falloff_sldr->SetValue(int(m_ig_scalar_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnIGScalarFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_scalar_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGScalarFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_scalar_falloff2_text->GetValue().ToDouble(&val);
	m_ig_scalar_falloff2 = val;
	m_ig_scalar_falloff2_sldr->SetValue(int(m_ig_scalar_falloff2 * 1000.0 + 0.5));
}

//grad falloff
void ComponentDlg::OnIGGradFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_grad_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGGradFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_grad_falloff_text->GetValue().ToDouble(&val);
	m_ig_grad_falloff = val;
	m_ig_grad_falloff_sldr->SetValue(int(m_ig_grad_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnIGGradFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_grad_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGGradFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_grad_falloff2_text->GetValue().ToDouble(&val);
	m_ig_grad_falloff2 = val;
	m_ig_grad_falloff2_sldr->SetValue(int(m_ig_grad_falloff2 * 1000.0 + 0.5));
}

//variance falloff
void ComponentDlg::OnIGVarFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_var_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGVarFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_var_falloff_text->GetValue().ToDouble(&val);
	m_ig_var_falloff = val;
	m_ig_var_falloff_sldr->SetValue(int(m_ig_var_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnIGVarFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_var_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGVarFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_var_falloff2_text->GetValue().ToDouble(&val);
	m_ig_var_falloff2 = val;
	m_ig_var_falloff2_sldr->SetValue(int(m_ig_var_falloff2 * 1000.0 + 0.5));
}

//angle falloff
void ComponentDlg::OnIGAngleFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_angle_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGAngleFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_angle_falloff_text->GetValue().ToDouble(&val);
	m_ig_angle_falloff = val;
	m_ig_angle_falloff_sldr->SetValue(int(m_ig_angle_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnIGAngleFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_ig_angle_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnIGAngleFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_ig_angle_falloff2_text->GetValue().ToDouble(&val);
	m_ig_angle_falloff2 = val;
	m_ig_angle_falloff2_sldr->SetValue(int(m_ig_angle_falloff2 * 1000.0 + 0.5));
}

//sized grow
void ComponentDlg::EnableSizedGrow(bool value)
{
	m_sized_grow = value;
	if (m_sized_grow)
	{
		m_sg_param_transition_check->Enable();
		m_sg_iterations_sldr->Enable();
		m_sg_iterations_text->Enable();
		m_sg_size_limiter_sldr->Enable();
		m_sg_size_limiter_text->Enable();
		m_sg_size_limiter2_sldr->Enable();
		m_sg_size_limiter2_text->Enable();
		m_sg_translate_sldr->Enable();
		m_sg_translate_text->Enable();
		m_sg_translate2_sldr->Enable();
		m_sg_translate2_text->Enable();
		m_sg_scalar_falloff_sldr->Enable();
		m_sg_scalar_falloff_text->Enable();
		m_sg_scalar_falloff2_sldr->Enable();
		m_sg_scalar_falloff2_text->Enable();
		m_sg_grad_falloff_sldr->Enable();
		m_sg_grad_falloff_text->Enable();
		m_sg_grad_falloff2_sldr->Enable();
		m_sg_grad_falloff2_text->Enable();
		m_sg_var_falloff_sldr->Enable();
		m_sg_var_falloff_text->Enable();
		m_sg_var_falloff2_sldr->Enable();
		m_sg_var_falloff2_text->Enable();
		m_sg_angle_falloff_sldr->Enable();
		m_sg_angle_falloff_text->Enable();
		m_sg_angle_falloff2_sldr->Enable();
		m_sg_angle_falloff2_text->Enable();
	}
	else
	{
		m_sg_param_transition_check->Disable();
		m_sg_iterations_sldr->Disable();
		m_sg_iterations_text->Disable();
		m_sg_size_limiter_sldr->Disable();
		m_sg_size_limiter_text->Disable();
		m_sg_size_limiter2_sldr->Disable();
		m_sg_size_limiter2_text->Disable();
		m_sg_translate_sldr->Disable();
		m_sg_translate_text->Disable();
		m_sg_translate2_sldr->Disable();
		m_sg_translate2_text->Disable();
		m_sg_scalar_falloff_sldr->Disable();
		m_sg_scalar_falloff_text->Disable();
		m_sg_scalar_falloff2_sldr->Disable();
		m_sg_scalar_falloff2_text->Disable();
		m_sg_grad_falloff_sldr->Disable();
		m_sg_grad_falloff_text->Disable();
		m_sg_grad_falloff2_sldr->Disable();
		m_sg_grad_falloff2_text->Disable();
		m_sg_var_falloff_sldr->Disable();
		m_sg_var_falloff_text->Disable();
		m_sg_var_falloff2_sldr->Disable();
		m_sg_var_falloff2_text->Disable();
		m_sg_angle_falloff_sldr->Disable();
		m_sg_angle_falloff_text->Disable();
		m_sg_angle_falloff2_sldr->Disable();
		m_sg_angle_falloff2_text->Disable();
	}
}

void ComponentDlg::EnableSGParamTransition(bool value)
{
	m_sg_param_transition = value;
	if (m_sg_param_transition)
	{
		m_sg_size_limiter_st->Show();
		m_sg_size_limiter2_sldr->Show();
		m_sg_size_limiter2_text->Show();
		m_sg_translate_st->Show();
		m_sg_translate2_sldr->Show();
		m_sg_translate2_text->Show();
		m_sg_scalar_falloff_st->Show();
		m_sg_scalar_falloff2_sldr->Show();
		m_sg_scalar_falloff2_text->Show();
		m_sg_grad_falloff_st->Show();
		m_sg_grad_falloff2_sldr->Show();
		m_sg_grad_falloff2_text->Show();
		m_sg_var_falloff_st->Show();
		m_sg_var_falloff2_sldr->Show();
		m_sg_var_falloff2_text->Show();
		m_sg_angle_falloff_st->Show();
		m_sg_angle_falloff2_sldr->Show();
		m_sg_angle_falloff2_text->Show();
	}
	else
	{
		m_sg_size_limiter_st->Hide();
		m_sg_size_limiter2_sldr->Hide();
		m_sg_size_limiter2_text->Hide();
		m_sg_translate_st->Hide();
		m_sg_translate2_sldr->Hide();
		m_sg_translate2_text->Hide();
		m_sg_scalar_falloff_st->Hide();
		m_sg_scalar_falloff2_sldr->Hide();
		m_sg_scalar_falloff2_text->Hide();
		m_sg_grad_falloff_st->Hide();
		m_sg_grad_falloff2_sldr->Hide();
		m_sg_grad_falloff2_text->Hide();
		m_sg_var_falloff_st->Hide();
		m_sg_var_falloff2_sldr->Hide();
		m_sg_var_falloff2_text->Hide();
		m_sg_angle_falloff_st->Hide();
		m_sg_angle_falloff2_sldr->Hide();
		m_sg_angle_falloff2_text->Hide();
	}

	if (m_adv_page)
		m_adv_page->SendSizeEvent();
}

void ComponentDlg::OnSizedGrowCheck(wxCommandEvent &event)
{
	EnableSizedGrow(m_sized_grow_check->GetValue());
}

void ComponentDlg::OnSGParamTransitionCheck(wxCommandEvent &event)
{
	EnableSGParamTransition(m_sg_param_transition_check->GetValue());

	if (m_sg_param_transition)
	{
		//copy values
		m_sg_size_limiter2_text->SetValue(wxString::Format("%d", m_sg_size_limiter));
		m_sg_translate2_text->SetValue(wxString::Format("%.3f", m_sg_translate));
		m_sg_scalar_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_scalar_falloff));
		m_sg_grad_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_grad_falloff));
		m_sg_var_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_var_falloff));
		m_sg_angle_falloff2_text->SetValue(wxString::Format("%.3f", m_sg_angle_falloff));
	}
}

void ComponentDlg::OnSGIterationsSldr(wxScrollEvent &event)
{
	int val = event.GetPosition();
	m_sg_iterations_text->SetValue(wxString::Format("%d", val));
}

void ComponentDlg::OnSGIterationsText(wxCommandEvent &event)
{
	long val = 0;
	m_sg_iterations_text->GetValue().ToLong(&val);
	m_sg_iterations = val;
	m_sg_iterations_sldr->SetValue(m_sg_iterations);
}

//size limiter
void ComponentDlg::OnSGSizeLimiterSldr(wxScrollEvent &event)
{
	int val = event.GetPosition();
	m_sg_size_limiter_text->SetValue(wxString::Format("%d", val));
}

void ComponentDlg::OnSGSizeLimiterText(wxCommandEvent &event)
{
	long val = 0;
	m_sg_size_limiter_text->GetValue().ToLong(&val);
	m_sg_size_limiter = val;
	m_sg_size_limiter_sldr->SetValue(m_sg_size_limiter);
}

void ComponentDlg::OnSGSizeLimiter2Sldr(wxScrollEvent &event)
{
	int val = event.GetPosition();
	m_sg_size_limiter2_text->SetValue(wxString::Format("%d", val));
}

void ComponentDlg::OnSGSizeLimiter2Text(wxCommandEvent &event)
{
	long val = 0;
	m_sg_size_limiter2_text->GetValue().ToLong(&val);
	m_sg_size_limiter2 = val;
	m_sg_size_limiter2_sldr->SetValue(m_sg_size_limiter2);
}

//translate
void ComponentDlg::OnSGTranslateSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_translate_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGTranslateText(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_translate_text->GetValue().ToDouble(&val);
	m_sg_translate = val;
	m_sg_translate_sldr->SetValue(int(m_sg_translate * 1000.0 + 0.5));
}

void ComponentDlg::OnSGTranslate2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_translate2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGTranslate2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_translate2_text->GetValue().ToDouble(&val);
	m_sg_translate2 = val;
	m_sg_translate2_sldr->SetValue(int(m_sg_translate2 * 1000.0 + 0.5));
}

//scalar falloff
void ComponentDlg::OnSGScalarFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_scalar_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGScalarFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_scalar_falloff_text->GetValue().ToDouble(&val);
	m_sg_scalar_falloff = val;
	m_sg_scalar_falloff_sldr->SetValue(int(m_sg_scalar_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnSGScalarFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_scalar_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGScalarFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_scalar_falloff2_text->GetValue().ToDouble(&val);
	m_sg_scalar_falloff2 = val;
	m_sg_scalar_falloff2_sldr->SetValue(int(m_sg_scalar_falloff2 * 1000.0 + 0.5));
}

//grad falloff
void ComponentDlg::OnSGGradFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_grad_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGGradFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_grad_falloff_text->GetValue().ToDouble(&val);
	m_sg_grad_falloff = val;
	m_sg_grad_falloff_sldr->SetValue(int(m_sg_grad_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnSGGradFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_grad_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGGradFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_grad_falloff2_text->GetValue().ToDouble(&val);
	m_sg_grad_falloff2 = val;
	m_sg_grad_falloff2_sldr->SetValue(int(m_sg_grad_falloff2 * 1000.0 + 0.5));
}

//variance falloff
void ComponentDlg::OnSGVarFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_var_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGVarFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_var_falloff_text->GetValue().ToDouble(&val);
	m_sg_var_falloff = val;
	m_sg_var_falloff_sldr->SetValue(int(m_sg_var_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnSGVarFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_var_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGVarFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_var_falloff2_text->GetValue().ToDouble(&val);
	m_sg_var_falloff2 = val;
	m_sg_var_falloff2_sldr->SetValue(int(m_sg_var_falloff2 * 1000.0 + 0.5));
}

//angle falloff
void ComponentDlg::OnSGAngleFalloffSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_angle_falloff_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGAngleFalloffText(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_angle_falloff_text->GetValue().ToDouble(&val);
	m_sg_angle_falloff = val;
	m_sg_angle_falloff_sldr->SetValue(int(m_sg_angle_falloff * 1000.0 + 0.5));
}

void ComponentDlg::OnSGAngleFalloff2Sldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_sg_angle_falloff2_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSGAngleFalloff2Text(wxCommandEvent &event)
{
	double val = 0.0;
	m_sg_angle_falloff2_text->GetValue().ToDouble(&val);
	m_sg_angle_falloff2 = val;
	m_sg_angle_falloff2_sldr->SetValue(int(m_sg_angle_falloff2 * 1000.0 + 0.5));
}

//cleanup
void ComponentDlg::EnableCleanup(bool value)
{
	m_cleanup = value;
	if (m_cleanup)
	{
		m_cl_iterations_sldr->Enable();
		m_cl_iterations_text->Enable();
		m_cl_size_limiter_sldr->Enable();
		m_cl_size_limiter_text->Enable();
	}
	else
	{
		m_cl_iterations_sldr->Disable();
		m_cl_iterations_text->Disable();
		m_cl_size_limiter_sldr->Disable();
		m_cl_size_limiter_text->Disable();
	}
}

void ComponentDlg::OnCleanupCheck(wxCommandEvent &event)
{
	EnableCleanup(m_cleanup_check->GetValue());
}

void ComponentDlg::OnCLIterationsSldr(wxScrollEvent &event)
{
	int val = event.GetPosition();
	m_cl_iterations_text->SetValue(wxString::Format("%d", val));
}

void ComponentDlg::OnCLIterationsText(wxCommandEvent &event)
{
	long val = 0;
	m_cl_iterations_text->GetValue().ToLong(&val);
	m_cl_iterations = val;
	m_cl_iterations_sldr->SetValue(m_cl_iterations);
}

void ComponentDlg::OnCLSizeLimiterSldr(wxScrollEvent &event)
{
	int val = event.GetPosition();
	m_cl_size_limiter_text->SetValue(wxString::Format("%d", val));
}

void ComponentDlg::OnCLSizeLimiterText(wxCommandEvent &event)
{
	long val = 0;
	m_cl_size_limiter_text->GetValue().ToLong(&val);
	m_cl_size_limiter = val;
	m_cl_size_limiter_sldr->SetValue(m_cl_size_limiter);
}

//match slices
void ComponentDlg::EnableMatchSlices(bool value)
{
	m_match_slices = value;
	if (m_match_slices)
	{
		m_size_thresh_sldr->Enable();
		m_size_thresh_text->Enable();
		m_size_ratio_sldr->Enable();
		m_size_ratio_text->Enable();
		m_dist_thresh_sldr->Enable();
		m_dist_thresh_text->Enable();
		m_angle_thresh_sldr->Enable();
		m_angle_thresh_text->Enable();
	}
	else
	{
		m_size_thresh_sldr->Disable();
		m_size_thresh_text->Disable();
		m_size_ratio_sldr->Disable();
		m_size_ratio_text->Disable();
		m_dist_thresh_sldr->Disable();
		m_dist_thresh_text->Disable();
		m_angle_thresh_sldr->Disable();
		m_angle_thresh_text->Disable();
	}
}

void ComponentDlg::OnMatchSlicesCheck(wxCommandEvent &event)
{
	EnableMatchSlices(m_match_slices_check->GetValue());
}

void ComponentDlg::OnSizeThreshSldr(wxScrollEvent &event)
{
	int val = event.GetPosition();
	m_size_thresh_text->SetValue(wxString::Format("%d", val));
}

void ComponentDlg::OnSizeThreshText(wxCommandEvent &event)
{
	long val = 0;
	m_size_thresh_text->GetValue().ToLong(&val);
	m_size_thresh = val;
	m_size_thresh_sldr->SetValue(m_size_thresh);
}

void ComponentDlg::OnSizeRatioSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_size_ratio_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnSizeRatioText(wxCommandEvent &event)
{
	double val = 0.0;
	m_size_ratio_text->GetValue().ToDouble(&val);
	m_size_ratio = val;
	m_size_ratio_sldr->SetValue(int(m_size_ratio * 1000.0 + 0.5));
}

void ComponentDlg::OnDistThreshSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 100.0;
	m_dist_thresh_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnDistThreshText(wxCommandEvent &event)
{
	double val = 0.0;
	m_dist_thresh_text->GetValue().ToDouble(&val);
	m_dist_thresh = val;
	m_dist_thresh_sldr->SetValue(int(m_dist_thresh * 100.0 + 0.5));
}

void ComponentDlg::OnAngleThreshSldr(wxScrollEvent &event)
{
	double val = (double)event.GetPosition() / 1000.0;
	m_angle_thresh_text->SetValue(wxString::Format("%.3f", val));
}

void ComponentDlg::OnAngleThreshText(wxCommandEvent &event)
{
	double val = 0.0;
	m_angle_thresh_text->GetValue().ToDouble(&val);
	m_angle_thresh = val;
	m_angle_thresh_sldr->SetValue(int(m_angle_thresh * 1000.0 + 0.5));
}

void ComponentDlg::OnExecute(wxCommandEvent &event)
{
	if (!m_view)
		return;
	VolumeData* vd = m_view->m_glview->m_cur_vol;
	if (!vd)
		return;

	FL::ComponentGenerator cg(vd, KernelProgram::get_device_id());
	cg.OrderID_2D();
	cg.InitialGrow();
}