// CameraParaSetDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GWSystem1.0.h"
#include "CameraParaSetDlg.h"
#include "afxdialogex.h"


// CameraParaSetDlg �Ի���

IMPLEMENT_DYNAMIC(CameraParaSetDlg, CDialogEx)

CameraParaSetDlg::CameraParaSetDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CameraParaSetDlg::IDD, pParent)
{

}

CameraParaSetDlg::~CameraParaSetDlg()
{
}

void CameraParaSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_TABLENUM, m_comboxtablenum);
}


BEGIN_MESSAGE_MAP(CameraParaSetDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TABLENUM, &CameraParaSetDlg::OnCbnSelchangeComboTablenum)
	ON_BN_CLICKED(IDC_BTN_SAVECAMERAPARA, &CameraParaSetDlg::OnBnClickedBtnSavecamerapara)
	ON_EN_CHANGE(IDC_EDIT_EXPOSURETIME, &CameraParaSetDlg::OnEnChangeEditExposuretime)
	ON_EN_CHANGE(IDC_EDIT_GAIN, &CameraParaSetDlg::OnEnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_REDGAIN, &CameraParaSetDlg::OnEnChangeEditRedgain)
	ON_EN_CHANGE(IDC_EDIT_GREENGAIN1, &CameraParaSetDlg::OnEnChangeEditGreengain1)
	ON_EN_CHANGE(IDC_EDIT_GREENGAIN2, &CameraParaSetDlg::OnEnChangeEditGreengain2)
	ON_EN_CHANGE(IDC_EDIT_BLUEGAIN, &CameraParaSetDlg::OnEnChangeEditBluegain)
	ON_EN_CHANGE(IDC_EDIT_GAMMA, &CameraParaSetDlg::OnEnChangeEditGamma)
	ON_EN_CHANGE(IDC_EDIT_CONTRAST, &CameraParaSetDlg::OnEnChangeEditContrast)
	ON_EN_CHANGE(IDC_EDIT_BRIGHTNESS, &CameraParaSetDlg::OnEnChangeEditBrightness)
	ON_EN_CHANGE(IDC_EDIT_HUE, &CameraParaSetDlg::OnEnChangeEditHue)
	ON_EN_CHANGE(IDC_EDIT_SATURATION, &CameraParaSetDlg::OnEnChangeEditSaturation)
	ON_EN_CHANGE(IDC_EDIT_EXPOSUREMAX, &CameraParaSetDlg::OnEnChangeEditExposuremax)
	ON_BN_CLICKED(IDC_RADIO_BIT8, &CameraParaSetDlg::OnBnClickedRadioBit8)
	ON_BN_CLICKED(IDC_RADIO_BIT16, &CameraParaSetDlg::OnBnClickedRadioBit16)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_DEFAULTCAMERA, &CameraParaSetDlg::OnBnClickedBtnDefaultcamera)
END_MESSAGE_MAP()


// CameraParaSetDlg ��Ϣ�������


BOOL CameraParaSetDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//----------������Ͽ���Ŀ------------//
	m_comboxtablenum.AddString(_T("4 X"));
	m_comboxtablenum.AddString(_T("10 X"));
	m_comboxtablenum.AddString(_T("20 X"));
	m_comboxtablenum.AddString(_T("40 X"));
	m_comboxtablenum.AddString(_T("60 X"));
	m_comboxtablenum.AddString(_T("100 X"));

	//m_Camera_Database_Scanning.ExitConnect();
	m_Camera_Database_Scanning.OnInitADOAccess();
	m_comboxtablenum.SetCurSel(m_Table_Index);

	Camera_Paras_read();
	// �쳣:  OCX ����ҳӦ���� FALSE


	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣:  OCX ����ҳӦ���� FALSE
}

BOOL CameraParaSetDlg::Camera_Paras_read()
{
	Camera_Paras_Get(m_comboxtablenum.GetCurSel());

	return true;
}

BOOL CameraParaSetDlg::Camera_Paras_write()
{
	//m_comboxtablenum.SetCurSel(m_Table_Index);
	Camera_Paras_Set(m_comboxtablenum.GetCurSel());
	/*
	switch (m_Table_Index)
	{
	case 0:
	Camera_Paras_Set(_T("Table_4X"));
	break;
	case 1:
	Camera_Paras_Set(_T("Table_10X"));
	break;
	case 2:
	Camera_Paras_Set(_T("Table_20X"));
	break;
	case 3:
	Camera_Paras_Set(_T("Table_40X"));
	break;
	case 4:
	Camera_Paras_Set(_T("Table_60X"));
	break;
	case 5:
	Camera_Paras_Set(_T("Table_100X"));
	break;
	default:
	break;

	}*/
	return true;
}

void CameraParaSetDlg::PassHandleCameera(HANDLE hCamera, BOOL bPreviewing, int Table_Index)
{
	m_hCamera = hCamera;
	//m_bConnected = bConnected;
	m_bPreviewing = bPreviewing;
	m_Table_Index = Table_Index;
	//m_comboxtablenum.SetCurSel(Table_Index);
	//Camera_Paras_Get(m_Table_Index);
	m_fFrameRate = 100000;
	if (!LucamGetFormat(m_hCamera, &m_lffFormat, &m_fFrameRate))
	{
		AfxMessageBox(_T("Unable to get camera video format. Capture frames may not work properly."));
	}



}

void CameraParaSetDlg::Camera_Paras_Get(int Table_Index)
{
	tableIndex.Format(_T("%d"), Table_Index);

	sql = _T("select * from ���������\
			 			 where ���������.[�ﾵID]='" + tableIndex + "' ");

	//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
	try
	{
		m_Camera_Database_Scanning.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());

	}
	try
	{

		UpdateData(TRUE);

		CString  ReadString;
		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("�ع�ʱ��"));
		GetDlgItem(IDC_EDIT_EXPOSURETIME)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("������"));
		GetDlgItem(IDC_EDIT_GAIN)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����"));
		GetDlgItem(IDC_EDIT_REDGAIN)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����1"));
		GetDlgItem(IDC_EDIT_GREENGAIN1)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����2"));
		GetDlgItem(IDC_EDIT_GREENGAIN2)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����"));
		GetDlgItem(IDC_EDIT_BLUEGAIN)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("Gamma"));
		GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("�Աȶ�"));
		GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("����"));
		GetDlgItem(IDC_EDIT_BRIGHTNESS)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("ɫ��"));
		GetDlgItem(IDC_EDIT_HUE)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("���Ͷ�"));
		GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("����ع�"));
		GetDlgItem(IDC_EDIT_EXPOSUREMAX)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("ͼ����"));
		GetDlgItem(IDC_EDIT_IMAGEWIDTH)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ʼX"));
		GetDlgItem(IDC_EDIT_X_BEGIN)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("ͼ��߶�"));
		GetDlgItem(IDC_EDIT_IMAGEHIGHT)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ʼY"));
		GetDlgItem(IDC_EDIT_Y_BEGIN)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("Xƫ��"));
		GetDlgItem(IDC_EDIT_X_OFFSET)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("X����"));
		GetDlgItem(IDC_EDIT_X_CHILD)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("Yƫ��"));
		GetDlgItem(IDC_EDIT_Y_OFFSET)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("Y����"));
		GetDlgItem(IDC_EDIT_Y_CHILD)->SetWindowText(ReadString);

		ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("����"));

		if (ReadString == "LUCAM_PF_16")
			CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT16);
		else
			CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT8);
		UpdateData(FALSE);


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}




}


void CameraParaSetDlg::Camera_Paras_Set(int Table_Index)
{

	tableIndex.Format(_T("%d"), Table_Index);
	sql = _T("select * from ���������\ where ���������.[�ﾵID]='" + tableIndex + "' ");

	try
	{
		m_Camera_Database_Scanning.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());

	}
	try
	{
		m_Camera_Database_Scanning.m_pRecordset->MoveFirst();
		while (!m_Camera_Database_Scanning.m_pRecordset->adoEOF)
		{
			//m_Conn.m_pRecordset->AddNew();
			CString CStrTem;
			GetDlgItem(IDC_EDIT_EXPOSURETIME)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("�ع�ʱ��"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_GAIN)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("������"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_REDGAIN)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("��ɫ����"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_GREENGAIN1)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("��ɫ����1"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_GREENGAIN2)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("��ɫ����2"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_BLUEGAIN)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("��ɫ����"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_GAMMA)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("Gamma"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_CONTRAST)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("�Աȶ�"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_BRIGHTNESS)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("����"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_HUE)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("ɫ��"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_SATURATION)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("���Ͷ�"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_EXPOSUREMAX)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("����ع�"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_IMAGEHIGHT)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("ͼ����"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_X_BEGIN)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("��ʼX"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_IMAGEHIGHT)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("ͼ��߶�"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_Y_BEGIN)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("��ʼY"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_X_OFFSET)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("Xƫ��"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_X_CHILD)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("X����"), _variant_t(CStrTem));

			GetDlgItem(IDC_EDIT_Y_OFFSET)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("Yƫ��"), _variant_t(CStrTem));


			GetDlgItem(IDC_EDIT_Y_CHILD)->GetWindowText(CStrTem);
			m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("Y����"), _variant_t(CStrTem));


			if (IDC_RADIO_BIT8 == GetCheckedRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16))
				m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("����"), _variant_t("LUCAM_PF_8"));
			else
				m_Camera_Database_Scanning.m_pRecordset->PutCollect(_T("����"), _variant_t("LUCAM_PF_16"));

			UpdateData(FALSE);
			m_Camera_Database_Scanning.m_pRecordset->MoveNext();

		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}

//ѡ���ﾵ
void CameraParaSetDlg::OnCbnSelchangeComboTablenum()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Camera_Paras_read();
}


void CameraParaSetDlg::OnBnClickedBtnSavecamerapara()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	Camera_Paras_write();
}


void CameraParaSetDlg::OnEnChangeEditExposuretime()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_EXPOSURETIME)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_EXPOSURE, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 			where ���������.[�ﾵID]='" + tableIndex + "' ");

		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{
			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("�ع�ʱ��"));
			GetDlgItem(IDC_EDIT_EXPOSURETIME)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current exposure."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());
		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditGain()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_GAIN)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			where ���������.[�ﾵID]='" + tableIndex + "' ");

		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);

			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("������"));
			GetDlgItem(IDC_EDIT_GAIN)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current gain."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditRedgain()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_REDGAIN)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_RED, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);

			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����"));
			GetDlgItem(IDC_EDIT_REDGAIN)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current RedGain."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}
	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditGreengain1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_GREENGAIN1)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_GREEN1, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);

			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����1"));
			GetDlgItem(IDC_EDIT_GREENGAIN1)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current GreenGain1."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditGreengain2()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_GREENGAIN2)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_GREEN2, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����2"));
			GetDlgItem(IDC_EDIT_GREENGAIN2)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current GreenGain2."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditBluegain()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_BLUEGAIN)->GetWindowText(csTemp);
	//CT2A strExpo(csTemp, CP_UTF8);
	if (csTemp == "") return;
	FLOAT fValue = (FLOAT)atof(CT2A(csTemp));
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAIN_BLUE, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("��ɫ����"));
			GetDlgItem(IDC_EDIT_BLUEGAIN)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current BlueGain."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditGamma()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_GAMMA)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_GAMMA, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("Gamma"));
			GetDlgItem(IDC_EDIT_GAMMA)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current Gmama."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditContrast()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_CONTRAST)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_CONTRAST, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 	 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("�Աȶ�"));
			GetDlgItem(IDC_EDIT_CONTRAST)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current Contrast."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditBrightness()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_BRIGHTNESS)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_BRIGHTNESS, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("����"));
			GetDlgItem(IDC_EDIT_BRIGHTNESS)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current Brightness."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditHue()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_HUE)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_HUE, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("ɫ��"));
			GetDlgItem(IDC_EDIT_HUE)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current Hue."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditSaturation()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_SATURATION)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_SATURATION, fValue, 0))
	{

		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("���Ͷ�"));
			GetDlgItem(IDC_EDIT_SATURATION)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current Saturation."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnEnChangeEditExposuremax()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	CString csTemp;
	GetDlgItem(IDC_EDIT_EXPOSUREMAX)->GetWindowText(csTemp);
	CT2A strExpo(csTemp, CP_UTF8);
	FLOAT fValue = (FLOAT)atof(strExpo);
	if (!LucamSetProperty(m_hCamera, LUCAM_PROP_AUTO_EXP_MAXIMUM, fValue, 0))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("����ع�"));
			GetDlgItem(IDC_EDIT_EXPOSUREMAX)->SetWindowText(ReadString);
			AfxMessageBox(_T("Unable to set current ExposureMax."));

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}

	}

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CameraParaSetDlg::OnBnClickedRadioBit8()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT8);
	m_lffFormat.pixelFormat = LUCAM_PF_8;
	if (!LucamSetFormat(m_hCamera, &m_lffFormat, m_fFrameRate))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("����"));
			if (ReadString == "LUCAM_PF_16")
				CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT16);
			else
				CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT8);

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}


	}

}


void CameraParaSetDlg::OnBnClickedRadioBit16()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT16);
	m_lffFormat.pixelFormat = LUCAM_PF_16;
	if (!LucamSetFormat(m_hCamera, &m_lffFormat, 100000))
	{
		UpdateData(false);
		tableIndex.Format(_T("%d"), m_comboxtablenum.GetCurSel());
		sql = _T("select * from ���������\
				 				 				 			 			 			 			 					 	where ���������.[�ﾵID]='" + tableIndex + "' ");
		//where Ⱦɫ��ͼ�����������ݱ�.[��Ƭ���]='" + SlideNumber + "'and Ⱦɫ��ͼ�����������ݱ�.[��Ƭ����]='" + PicName + "' ");
		try
		{
			m_Camera_Database_Scanning.GetRecordSet(sql);
		}
		catch (_com_error *e)
		{
			AfxMessageBox(e->ErrorMessage());

		}
		try
		{

			UpdateData(TRUE);
			CString  ReadString = m_Camera_Database_Scanning.m_pRecordset->GetCollect(_T("����"));
			if (ReadString == "LUCAM_PF_16")
				CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT16);
			else
				CheckRadioButton(IDC_RADIO_BIT8, IDC_RADIO_BIT16, IDC_RADIO_BIT8);

		}
		catch (_com_error e)
		{
			AfxMessageBox(e.Description());

		}


	}

}


void CameraParaSetDlg::OnClose()
{
	// TODO:  �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//���ݿ��˳�����
	m_Camera_Database_Scanning.ExitConnect();
	CDialogEx::OnClose();
}


void CameraParaSetDlg::OnBnClickedBtnDefaultcamera()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
