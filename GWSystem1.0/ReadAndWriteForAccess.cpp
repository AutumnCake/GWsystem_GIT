#include "stdafx.h"
#include "ReadAndWriteForAccess.h"


CReadAndWriteForAccess::CReadAndWriteForAccess()
{
	m_Conn.OnInitADOAccess();
}


CReadAndWriteForAccess::~CReadAndWriteForAccess()
{
}


bool CReadAndWriteForAccess::WriteToSlideSet(Table_SlideSet slideset)
{
	sql = _T("select  * from ��Ƭ���ñ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(_T("�޷��ҵ���Ƭ���ñ�"));
	}
	try
	{
		slideset.time = COleDateTime::GetCurrentTime();

		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("�ѷ���ͼƬ��"), _variant_t(slideset.ImgSolvedNum));
		m_Conn.m_pRecordset->PutCollect(_T("��ͼƬ��"), _variant_t(slideset.AllImgNum));
		m_Conn.m_pRecordset->PutCollect(_T("��ͼʱ��"), _variant_t(slideset.time));
		m_Conn.m_pRecordset->PutCollect(_T("�Ƿ��з���"), _variant_t(slideset.HaveSolved));
		m_Conn.m_pRecordset->PutCollect(_T("·��"), _variant_t(slideset.path));
		m_Conn.m_pRecordset->PutCollect(_T("��������"), _variant_t(slideset.name));
		m_Conn.m_pRecordset->PutCollect(_T("ģʽ"), _variant_t(slideset.mode));
		m_Conn.m_pRecordset->PutCollect(_T("ɨ�跶Χ"), _variant_t(slideset.range));
		m_Conn.m_pRecordset->PutCollect(_T("���·��"), _variant_t(slideset.resultpath));


		m_Conn.m_pRecordset->Update();

		return false;
	}
	catch (_com_error e)
	{
		AfxMessageBox(_T("��Ƭ���ñ�д�����"));
	}

}


vector<Table_SlideSet> CReadAndWriteForAccess::ReadFromSlideSet()
{
	Table_SlideSet slideset;
	vector<Table_SlideSet>SlideSetVector;
	sql = _T("select * from ��Ƭ���ñ�");
	//sql = _T("select  * from ��Ƭ���ñ� where format(��ͼʱ��, 'yyyy-mm-dd') between format(#2018 - 10 - 16#, 'yyyy-mm-dd') and format(#2019 - 6 - 6#, 'yyyy-mm-dd')");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str = m_Conn.m_pRecordset->GetCollect(_T("·��"));
			slideset.path = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			slideset.name = str;			
			str = m_Conn.m_pRecordset->GetCollect(_T("���·��"));
			slideset.resultpath = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("ģʽ"));
			slideset.mode = str;			
			str = m_Conn.m_pRecordset->GetCollect(_T("ɨ�跶Χ"));
			slideset.range = str;			
			BOOL str1 = m_Conn.m_pRecordset->GetCollect(_T("�Ƿ��з���"));
			slideset.HaveSolved = str1;
			str = m_Conn.m_pRecordset->GetCollect(_T("�ѷ���ͼƬ��"));
			slideset.ImgSolvedNum = str;		
			str = m_Conn.m_pRecordset->GetCollect(_T("��ͼƬ��"));
			slideset.AllImgNum = str;			
			COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("��ͼʱ��"));
			slideset.time = str2;
			SlideSetVector.push_back(slideset);

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;
}


// ��ѯʱ����ڵ�ͼƬ-΢��ģʽ
vector<Table_SlideSet> CReadAndWriteForAccess::QuerySaveTimeMN(COleDateTime begintime, COleDateTime endtime)
{
	vector<Table_SlideSet>SlideSetVector;
	Table_SlideSet slideset;
	CString strDate1 = begintime.Format(_T("%Y-%m-%d"));//yyyy-mm-ddģʽ
	CString strDate2 = endtime.Format(_T("%Y-%m-%d"));//yyyy-mm-ddģʽ
	CString mystr = _T("΢��");
	sql = _T("select  * from ��Ƭ���ñ� where format(��ͼʱ��, 'yyyy-mm-dd') between format(#" + strDate1 + "#, 'yyyy-mm-dd') and format(#" + strDate2 + "#, 'yyyy-mm-dd')  and ��Ƭ���ñ�.[ģʽ] = '" + mystr + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str = m_Conn.m_pRecordset->GetCollect(_T("·��"));
			slideset.path = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			slideset.name = str;			
			str = m_Conn.m_pRecordset->GetCollect(_T("���·��"));
			slideset.resultpath = str;
			COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("��ͼʱ��"));
			slideset.time = str2;
			SlideSetVector.push_back(slideset);

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;
}


// ��ѯʱ����ڵ�ͼƬ-Ⱦɫ��ģʽ
vector<Table_SlideSet> CReadAndWriteForAccess::QuerySaveTimeCHRO(COleDateTime begintime, COleDateTime endtime)
{
	vector<Table_SlideSet>SlideSetVector;
	Table_SlideSet slideset;
	CString strDate1 = begintime.Format(_T("%Y-%m-%d"));//yyyy-mm-ddģʽ
	CString strDate2 = endtime.Format(_T("%Y-%m-%d"));//yyyy-mm-ddģʽ
	CString mystr = _T("Ⱦɫ��");
	sql = _T("select  * from ��Ƭ���ñ� where format(��ͼʱ��, 'yyyy-mm-dd') between format(#" + strDate1 + "#, 'yyyy-mm-dd') and format(#" + strDate2 + "#, 'yyyy-mm-dd')  and ��Ƭ���ñ�.[ģʽ] = '" + mystr + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		while (!m_Conn.m_pRecordset->adoEOF)
		{
			CString str = m_Conn.m_pRecordset->GetCollect(_T("·��"));
			slideset.path = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			slideset.name = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("���·��"));
			slideset.resultpath = str;
			str = m_Conn.m_pRecordset->GetCollect(_T("��ͼʱ��"));

			COleDateTime str2 = m_Conn.m_pRecordset->GetCollect(_T("��ͼʱ��"));
			slideset.time = str2;
			SlideSetVector.push_back(slideset);

			m_Conn.m_pRecordset->MoveNext();
		}

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return SlideSetVector;
}


//��ȡһ�����˱��ε�Ⱦɫ��������  �������ɸ��˱�
// ���ݿ��ж�ȡһ�����˵�Ⱦɫ��������
CHRO_HandleResult CReadAndWriteForAccess::ReadOneResultFromAccess(CString patientname)
{
	CHRO_HandleResult pA;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from Ⱦɫ�����������ݱ����Σ�\
			 			 		 where Ⱦɫ�����������ݱ����Σ�.[�������˻����]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	CString str;
	str = m_Conn.m_pRecordset->GetCollect(_T("�������˻����"));
	pA.patientname = str;

	//1
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���ķ���ϸ������"));
	pA.allcell = _ttoi(str);

	//2
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У����˫��˿������"));
	pA.dic = _ttoi(str);

	//4
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У������˿������"));
	pA.round = _ttoi(str);

	//5
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У��������˿������"));
	pA.ace = _ttoi(str);

	//6
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У�����໥��λ��"));
	pA.t = _ttoi(str);

	//7
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���ĵ�λ��"));
	pA.inv = _ttoi(str);

	//8
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���Ļ�����"));
	pA.Y = _ttof(str);

	//9
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У��������ϸ����"));
	pA.normalcell = _ttoi(str);

	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���Ĳ�����ϸ����"));
	pA.abnormal = _ttoi(str);

	//11
	str = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
	pA.tri = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
	pA.ten = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
	pA.del = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
	pA.ctg = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
	pA.csg = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
	pA.ctb = _ttoi(str);
	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
	pA.cte = _ttoi(str);

	return pA;
}

// һ��ͼƬȾɫ�����������浽���ݿ�
void CReadAndWriteForAccess::SaveOnePicResult(CHRO_HandleResultOnePic resultstruct)
{
	_bstr_t sql;
	_variant_t var;

	sql = _T("select  * from Ⱦɫ��ͼ�����������ݱ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("�������˻����"), _variant_t(resultstruct.patientname));
		m_Conn.m_pRecordset->PutCollect(_T("��Ƭ·��"), _variant_t(resultstruct.picpath));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ����"), _variant_t(resultstruct.normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("������ϸ����"), _variant_t(resultstruct.abnormal));
		m_Conn.m_pRecordset->PutCollect(_T("��ϸ����"), _variant_t(resultstruct.allcell));
		m_Conn.m_pRecordset->PutCollect(_T("ϵͳ�ж���˫��������"), _variant_t(resultstruct.dic));
		m_Conn.m_pRecordset->PutCollect(_T("ϵͳ�ж���Ⱦɫ�廷��"), _variant_t(resultstruct.round));
		m_Conn.m_pRecordset->PutCollect(_T("ϵͳ�ж�������˿����"), _variant_t(resultstruct.ace));
		m_Conn.m_pRecordset->PutCollect(_T("ϵͳ�ж����໥��λ��"), _variant_t(resultstruct.t));
		m_Conn.m_pRecordset->PutCollect(_T("ϵͳ�ж��ĵ�λ�����"), _variant_t(resultstruct.inv));

		m_Conn.m_pRecordset->PutCollect(_T("tri"), _variant_t(resultstruct.tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten"), _variant_t(resultstruct.ten));
		m_Conn.m_pRecordset->PutCollect(_T("del"), _variant_t(resultstruct.del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg"), _variant_t(resultstruct.ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg"), _variant_t(resultstruct.csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb"), _variant_t(resultstruct.ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte"), _variant_t(resultstruct.cte));

		m_Conn.m_pRecordset->PutCollect(_T("tri_check"), _variant_t(resultstruct.tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten_check"), _variant_t(resultstruct.ten));
		m_Conn.m_pRecordset->PutCollect(_T("del_check"), _variant_t(resultstruct.del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg_check"), _variant_t(resultstruct.ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg_check"), _variant_t(resultstruct.csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb_check"), _variant_t(resultstruct.ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte_check"), _variant_t(resultstruct.cte));



		m_Conn.m_pRecordset->PutCollect(_T("�˹�У�Ե�˫��������"), _variant_t(resultstruct.dic));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У�Ե���˿������"), _variant_t(resultstruct.round));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У�Ե�����˿����"), _variant_t(resultstruct.ace));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У�Ե��໥��λ��"), _variant_t(resultstruct.t));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У�Եĵ�λ�����"), _variant_t(resultstruct.inv));

		m_Conn.m_pRecordset->PutCollect(_T("CheckOrNot"), _variant_t(resultstruct.CheckOrNot));
		m_Conn.m_pRecordset->PutCollect(_T("chromosome_num"), _variant_t(resultstruct.chromosome_num));
		m_Conn.m_pRecordset->PutCollect(_T("chromosome_num_check"), _variant_t(resultstruct.chromosome_num));
		m_Conn.m_pRecordset->PutCollect(_T("QuaOrNot"), _variant_t(resultstruct.QuaOrNot));
		m_Conn.m_pRecordset->PutCollect(_T("CheckPath"), _variant_t(resultstruct.CheckPath));

		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}

// һ������Ⱦɫ�����������浽���ݿ�
bool CReadAndWriteForAccess::SaveToAccess(CHRO_HandleResult * pb)
{
	_bstr_t sql;
	_variant_t var;

	//������ʷ����
	sql = _T("select  * from Ⱦɫ�����������ݱ���ʷ���У�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("�������˻����"), _variant_t(pb->patientname));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ������"), _variant_t(pb->allcell));
		m_Conn.m_pRecordset->PutCollect(_T("��Ƭ����"), _variant_t(pb->picturesum));
		m_Conn.m_pRecordset->PutCollect(_T("˫��˿������"), _variant_t(pb->dic));
		m_Conn.m_pRecordset->PutCollect(_T("��˿������"), _variant_t(pb->round));
		m_Conn.m_pRecordset->PutCollect(_T("����˿������"), _variant_t(pb->ace));
		m_Conn.m_pRecordset->PutCollect(_T("�໥��λ��"), _variant_t(pb->t));
		m_Conn.m_pRecordset->PutCollect(_T("��λ��"), _variant_t(pb->inv));
		m_Conn.m_pRecordset->PutCollect(_T("������"), _variant_t(pb->Y));
		m_Conn.m_pRecordset->PutCollect(_T("ͼƬ�����ļ���"), _variant_t(pb->sourcefile));
		m_Conn.m_pRecordset->PutCollect(_T("����洢�ļ���"), _variant_t(pb->resultfile));

		m_Conn.m_pRecordset->PutCollect(_T("tri"), _variant_t(pb->tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten"), _variant_t(pb->ten));
		m_Conn.m_pRecordset->PutCollect(_T("del"), _variant_t(pb->del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg"), _variant_t(pb->ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg"), _variant_t(pb->csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb"), _variant_t(pb->ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte"), _variant_t(pb->cte));


		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//���뱾������
	sql = _T("select  * from Ⱦɫ�����������ݱ����Σ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("�������˻����"), _variant_t(pb->patientname));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ������"), _variant_t(pb->allcell));
		m_Conn.m_pRecordset->PutCollect(_T("��Ƭ����"), _variant_t(pb->picturesum));
		m_Conn.m_pRecordset->PutCollect(_T("˫��˿������"), _variant_t(pb->dic));
		m_Conn.m_pRecordset->PutCollect(_T("��˿������"), _variant_t(pb->round));
		m_Conn.m_pRecordset->PutCollect(_T("����˿������"), _variant_t(pb->ace));
		m_Conn.m_pRecordset->PutCollect(_T("�໥��λ��"), _variant_t(pb->t));
		m_Conn.m_pRecordset->PutCollect(_T("��λ��"), _variant_t(pb->inv));
		m_Conn.m_pRecordset->PutCollect(_T("������"), _variant_t(pb->Y));
		m_Conn.m_pRecordset->PutCollect(_T("ͼƬ�����ļ���"), _variant_t(pb->sourcefile));
		m_Conn.m_pRecordset->PutCollect(_T("����洢�ļ���"), _variant_t(pb->resultfile));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ����"), _variant_t(pb->normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("������ϸ����"), _variant_t(pb->abnormal));

		m_Conn.m_pRecordset->PutCollect(_T("�˹�У���ķ���ϸ������"), _variant_t(pb->allcell));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У����˫��˿������"), _variant_t(pb->dic));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У������˿������"), _variant_t(pb->round));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У��������˿������"), _variant_t(pb->ace));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У�����໥��λ��"), _variant_t(pb->t));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У���ĵ�λ��"), _variant_t(pb->inv));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У���Ļ�����"), _variant_t(pb->Y));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У��������ϸ����"), _variant_t(pb->normalcell));
		m_Conn.m_pRecordset->PutCollect(_T("�˹�У���Ĳ�����ϸ����"), _variant_t(pb->abnormal));

		m_Conn.m_pRecordset->PutCollect(_T("tri"), _variant_t(pb->tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten"), _variant_t(pb->ten));
		m_Conn.m_pRecordset->PutCollect(_T("del"), _variant_t(pb->del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg"), _variant_t(pb->ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg"), _variant_t(pb->csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb"), _variant_t(pb->ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte"), _variant_t(pb->cte));

		m_Conn.m_pRecordset->PutCollect(_T("tri_check"), _variant_t(pb->tri));
		m_Conn.m_pRecordset->PutCollect(_T("ten_check"), _variant_t(pb->ten));
		m_Conn.m_pRecordset->PutCollect(_T("del_check"), _variant_t(pb->del));
		m_Conn.m_pRecordset->PutCollect(_T("ctg_check"), _variant_t(pb->ctg));
		m_Conn.m_pRecordset->PutCollect(_T("csg_check"), _variant_t(pb->csg));
		m_Conn.m_pRecordset->PutCollect(_T("ctb_check"), _variant_t(pb->ctb));
		m_Conn.m_pRecordset->PutCollect(_T("cte_check"), _variant_t(pb->cte));


		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return false;
}

//��ȡѡ�е����в��˵�Ⱦɫ�������� ���������ܱ�
vector<CHRO_HandleResult> CReadAndWriteForAccess::ReadAllResultFromAccess()
{
	vector<CHRO_HandleResult>AllChosePatient;
	CHRO_HandleResult OneChosePatient;
	_bstr_t sql;
	_variant_t var;
	CHRO_HandleResult amendpA;
	sql = _T("select * from Ⱦɫ�����������ݱ����Σ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		CString mystr;
		CString num;
		while (!m_Conn.m_pRecordset->adoEOF)
		{


			mystr = m_Conn.m_pRecordset->GetCollect(_T("�������˻����"));
			OneChosePatient.patientname = mystr;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���ķ���ϸ������"));
			OneChosePatient.allcell = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У����˫��˿������"));
			OneChosePatient.dic = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У������˿������"));
			OneChosePatient.round = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У��������˿������"));
			OneChosePatient.ace = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У�����໥��λ��"));
			OneChosePatient.t = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���ĵ�λ��"));
			OneChosePatient.inv = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���Ļ�����"));
			OneChosePatient.Y = _ttof(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У��������ϸ����"));
			OneChosePatient.normalcell = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("�˹�У���Ĳ�����ϸ����"));
			OneChosePatient.abnormal = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("tri_check"));
			OneChosePatient.tri = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("ten_check"));
			OneChosePatient.ten = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("del_check"));
			OneChosePatient.del = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("ctg_check"));
			OneChosePatient.ctg = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("csg_check"));
			OneChosePatient.csg = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("ctb_check"));
			OneChosePatient.ctb = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("cte_check"));
			OneChosePatient.cte = _ttoi(mystr);

			AllChosePatient.push_back(OneChosePatient);
			m_Conn.m_pRecordset->MoveNext();
		}
		return AllChosePatient;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}
}



// ��һ��ͼƬ�ķ�������浽���ݿ�
void CReadAndWriteForAccess::SaveOnePicMNResult(MN_HandleResultOnePic* resultstruct)
{
	_bstr_t sql;
	_variant_t var;

	sql = _T("select  * from ΢��ͼ�����������ݱ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("��������"), _variant_t(resultstruct->patientname));
		m_Conn.m_pRecordset->PutCollect(_T("��Ƭ·��"), _variant_t(resultstruct->picpath));
		m_Conn.m_pRecordset->PutCollect(_T("ϸ������"), _variant_t(resultstruct->NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("΢����"), _variant_t(resultstruct->NumMicro));
		m_Conn.m_pRecordset->PutCollect(_T("ϸ������_У��"), _variant_t(resultstruct->NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("΢����_У��"), _variant_t(resultstruct->NumMicro));
		m_Conn.m_pRecordset->PutCollect(_T("У����NumCells"), _variant_t(resultstruct->CheckOrNot_NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("У����NumMicro"), _variant_t(resultstruct->CheckOrNot_NumMicro));

		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}


}

// һ�����˵�΢�˷���������浽���ݿ�--У��ʱ
bool CReadAndWriteForAccess::SaveToAccessMN(MN_HandleResult * pb, CString patientname, MN_HandleResultOnePic onepicresult)
{
	_bstr_t sql;
	_variant_t var;

	//������ʷ����
	sql = _T("select * from ΢�˷���������ݱ���ʷ���У�\
			 			 			 		 where ΢�˷���������ݱ���ʷ���У�.[�������˻����]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������

			//m_Conn.m_pRecordset->PutCollect(_T("�������˻����"), _variant_t(patientname));
			////m_Conn.m_pRecordset->PutCollect(_T("����ϸ������"), _variant_t(pb->sumcell));
			//m_Conn.m_pRecordset->PutCollect(_T("��΢����_У��"), _variant_t(pb->sumwh));
			m_Conn.m_pRecordset->PutCollect(_T("����ϸ����_У��"), _variant_t(pb->singlecell));
			m_Conn.m_pRecordset->PutCollect(_T("˫��ϸ����_У��"), _variant_t(pb->doublecell));
			m_Conn.m_pRecordset->PutCollect(_T("���ϸ����_У��"), _variant_t(pb->multiplecell));
			m_Conn.m_pRecordset->PutCollect(_T("����MN_У��"), _variant_t(pb->singleMN));
			m_Conn.m_pRecordset->PutCollect(_T("����MNC_У��"), _variant_t(pb->SingleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("����΢��ϸ����_У��"), _variant_t(pb->SingleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("����΢����_У��"), _variant_t(pb->SingleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("˫��MN_У��"), _variant_t(pb->DoubleMN));
			m_Conn.m_pRecordset->PutCollect(_T("˫��΢��ϸ����_У��"), _variant_t(pb->DoubleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("˫��΢����_У��"), _variant_t(pb->DoubleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("˫��MNC_У��"), _variant_t(pb->DoubleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("���MNC_У��"), _variant_t(pb->multiplecell_wh));
			m_Conn.m_pRecordset->PutCollect(_T("���MN_У��"), _variant_t(pb->MultiMN));
			//m_Conn.m_pRecordset->PutCollect(_T("���΢��ϸ����"), _variant_t(pb->whcellrate));
			//m_Conn.m_pRecordset->PutCollect(_T("���΢����"), _variant_t(pb->whcellrate));
			m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[6]
				+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

			m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[6] +
				pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));

			m_Conn.m_pRecordset->Update();
	}
	catch (_com_error e)
	{
		//AfxMessageBox(e.Description());
	}

	//���뱾������
	sql = _T("select * from ΢�˷���������ݱ����Σ�\
			 			 			 			 		 where ΢�˷���������ݱ����Σ�.[�������˻����]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
			//m_Conn.m_pRecordset->PutCollect(_T("��΢����_У��"), _variant_t(pb->sumwh));
			m_Conn.m_pRecordset->PutCollect(_T("����ϸ����_У��"), _variant_t(pb->singlecell));
			m_Conn.m_pRecordset->PutCollect(_T("˫��ϸ����_У��"), _variant_t(pb->doublecell));
			m_Conn.m_pRecordset->PutCollect(_T("���ϸ����_У��"), _variant_t(pb->multiplecell));
			m_Conn.m_pRecordset->PutCollect(_T("����MN_У��"), _variant_t(pb->singleMN));
			m_Conn.m_pRecordset->PutCollect(_T("����MNC_У��"), _variant_t(pb->SingleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("����΢��ϸ����_У��"), _variant_t(pb->SingleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("����΢����_У��"), _variant_t(pb->SingleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("˫��MN_У��"), _variant_t(pb->DoubleMN));
			m_Conn.m_pRecordset->PutCollect(_T("˫��΢��ϸ����_У��"), _variant_t(pb->DoubleMNC_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("˫��΢����_У��"), _variant_t(pb->DoubleMN_Rate));
			m_Conn.m_pRecordset->PutCollect(_T("˫��MNC_У��"), _variant_t(pb->DoubleMNC_Num));
			m_Conn.m_pRecordset->PutCollect(_T("���MNC_У��"), _variant_t(pb->multiplecell_wh));
			m_Conn.m_pRecordset->PutCollect(_T("���MN_У��"), _variant_t(pb->MultiMN));
			//m_Conn.m_pRecordset->PutCollect(_T("���΢��ϸ����"), _variant_t(pb->whcellrate));
			//m_Conn.m_pRecordset->PutCollect(_T("���΢����"), _variant_t(pb->whcellrate));
			m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[6]
				+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

			m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[1]));
			m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[2]));
			m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[3]));
			m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[4]));
			m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[5]));
			m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));

			m_Conn.m_pRecordset->Update();


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	sql = _T("select * from ΢��ͼ�����������ݱ�\
			 			 			 			 where ΢��ͼ�����������ݱ�.[��Ƭ·��]='" + onepicresult.picpath + "' ");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		m_Conn.m_pRecordset->PutCollect(_T("ϸ������_У��"), _variant_t(onepicresult.NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("΢����_У��"), _variant_t(onepicresult.NumMicro));
		m_Conn.m_pRecordset->PutCollect(_T("У����NumCells"), _variant_t(onepicresult.CheckOrNot_NumCells));
		m_Conn.m_pRecordset->PutCollect(_T("У����NumMicro"), _variant_t(onepicresult.CheckOrNot_NumMicro));

		m_Conn.m_pRecordset->Update();


	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}


	return false;
}

// ���ݿ��ж�ȡһ�����˵��˹�У�����
MN_HandleResult CReadAndWriteForAccess::ReadOneMNResultFromAccess(CString patientname)
{
	MN_HandleResult pA;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from ΢�˷���������ݱ����Σ�\
			 			 			 		 where ΢�˷���������ݱ����Σ�.[�������˻����]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	CString str;
	str = m_Conn.m_pRecordset->GetCollect(_T("�������˻����"));
	pA.patientname = str;


	//2
	str = m_Conn.m_pRecordset->GetCollect(_T("����ϸ����_У��"));
	pA.singlecell = _ttoi(str);


	//3
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��ϸ����_У��"));
	pA.doublecell = _ttoi(str);


	//4
	str = m_Conn.m_pRecordset->GetCollect(_T("���ϸ����_У��"));
	pA.multiplecell = _ttoi(str);


	//5
	str = m_Conn.m_pRecordset->GetCollect(_T("����MN_У��"));
	pA.SingleMN_Num = _ttoi(str);


	//6
	str = m_Conn.m_pRecordset->GetCollect(_T("����΢����_У��"));
	pA.SingleMN_Rate = _ttof(str);


	//7
	str = m_Conn.m_pRecordset->GetCollect(_T("����MNC_У��"));
	pA.SingleMNC_Num = _ttoi(str);

	//8
	str = m_Conn.m_pRecordset->GetCollect(_T("����΢��ϸ����_У��"));
	pA.SingleMNC_Rate = _ttof(str);


	//9
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��MNC_У��"));
	pA.DoubleMNC_Num = _ttoi(str);


	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��΢��ϸ����_У��"));
	pA.DoubleMNC_Rate = _ttof(str);


	//11
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��MN_У��"));
	pA.DoubleMN_Num = _ttof(str);


	//12
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��΢����_У��"));
	pA.DoubleMN_Rate = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("���MN_У��"));
	pA.MultiMN = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("���MNC_У��"));
	pA.multiplecell_wh = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("1MN in doublecell_У��"));
	pA.DoubleCellsWithMN[1] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("2MN in doublecell_У��"));
	pA.DoubleCellsWithMN[2] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("3MN in doublecell_У��"));
	pA.DoubleCellsWithMN[3] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("4MN in doublecell_У��"));
	pA.DoubleCellsWithMN[4] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("5MN in doublecell_У��"));
	pA.DoubleCellsWithMN[5] = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T(">5MN in doublecell_У��"));
	pA.DoubleCellsWithMN[6] = _ttof(str);

	return pA;
}

// ���ݿ��ж�ȡһ�����˵�ϵͳ�������
MN_HandleResult CReadAndWriteForAccess::ReadOneMNSystemResultFromAccess(CString patientname)
{
	MN_HandleResult pA;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from ΢�˷���������ݱ����Σ�\
			 			 			 			 		 where ΢�˷���������ݱ����Σ�.[�������˻����]='" + patientname + "' ");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	CString str;
	str = m_Conn.m_pRecordset->GetCollect(_T("�������˻����"));
	pA.patientname = str;


	//2
	str = m_Conn.m_pRecordset->GetCollect(_T("����ϸ����"));
	pA.singlecell = _ttoi(str);


	//3
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��ϸ����"));
	pA.doublecell = _ttoi(str);


	//4
	str = m_Conn.m_pRecordset->GetCollect(_T("���ϸ����"));
	pA.multiplecell = _ttoi(str);


	//5
	str = m_Conn.m_pRecordset->GetCollect(_T("����MN"));
	pA.SingleMN_Num = _ttoi(str);


	//6
	str = m_Conn.m_pRecordset->GetCollect(_T("����΢����"));
	pA.SingleMN_Rate = _ttof(str);


	//7
	str = m_Conn.m_pRecordset->GetCollect(_T("����MNC"));
	pA.SingleMNC_Num = _ttoi(str);

	//8
	str = m_Conn.m_pRecordset->GetCollect(_T("����΢��ϸ����"));
	pA.SingleMNC_Rate = _ttof(str);


	//9
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��MNC"));
	pA.DoubleMNC_Num = _ttoi(str);


	//10
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��΢��ϸ����"));
	pA.DoubleMNC_Rate = _ttof(str);


	//11
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��MN"));
	pA.DoubleMN_Num = _ttof(str);


	//12
	str = m_Conn.m_pRecordset->GetCollect(_T("˫��΢����"));
	pA.DoubleMN_Rate = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("���MN"));
	pA.MultiMN = _ttof(str);

	str = m_Conn.m_pRecordset->GetCollect(_T("���MNC"));
	pA.multiplecell_wh = _ttof(str);


	return pA;
}
// ���ݿ���������ǰ���˵Ĵ�����
vector<MN_HandleResultOnePic> CReadAndWriteForAccess::FindCurrentPatientResult(CString patientname)
{
	vector<MN_HandleResultOnePic>CurrentPatientResult;
	MN_HandleResultOnePic OneCellResult;
	_bstr_t sql;
	_variant_t var;
	sql = _T("select * from ΢��ͼ�����������ݱ�\
			 			 	   where ΢��ͼ�����������ݱ�.[��������]='" + patientname + "' ");

	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		CString mystr;
		CString num;
		while (!m_Conn.m_pRecordset->adoEOF)
		{

			mystr = m_Conn.m_pRecordset->GetCollect(_T("��������"));
			OneCellResult.patientname = mystr;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("��Ƭ·��"));
			OneCellResult.picpath = mystr;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("ϸ������_У��"));
			OneCellResult.NumCells = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("΢����_У��"));
			OneCellResult.NumMicro = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("У����NumCells"));
			OneCellResult.CheckOrNot_NumCells = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("У����NumMicro"));
			OneCellResult.CheckOrNot_NumMicro = _ttoi(mystr);

			CurrentPatientResult.push_back(OneCellResult);
			m_Conn.m_pRecordset->MoveNext();
		}
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return CurrentPatientResult;
}


//��ȡѡ�еĲ��˵�΢�˷������ ���������ܱ�
vector<MN_HandleResult> CReadAndWriteForAccess::ReadAllMNResultFromAccess()
{
	vector<MN_HandleResult>AllChosePatient;
	MN_HandleResult OneChosePatient;
	_bstr_t sql;
	_variant_t var;
	MN_HandleResult amendpA;
	sql = _T("select * from ΢�˷���������ݱ����Σ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}

	try
	{
		m_Conn.m_pRecordset->MoveFirst();
		CString mystr;
		CString num;
		while (!m_Conn.m_pRecordset->adoEOF)
		{


			mystr = m_Conn.m_pRecordset->GetCollect(_T("�������˻����"));
			OneChosePatient.patientname = mystr;

			mystr = m_Conn.m_pRecordset->GetCollect(_T("����ϸ����_У��"));
			OneChosePatient.singlecell = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("˫��ϸ����_У��"));
			OneChosePatient.doublecell = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("����MN_У��"));
			OneChosePatient.singleMN = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("����MNC_У��"));
			OneChosePatient.SingleMNC_Num = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("����΢��ϸ����_У��"));
			OneChosePatient.SingleMNC_Rate = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("����΢����_У��"));
			OneChosePatient.SingleMN_Rate = _ttof(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("˫��MN_У��"));
			OneChosePatient.DoubleMN = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("˫��΢��ϸ����_У��"));
			OneChosePatient.DoubleMNC_Rate = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("˫��΢����_У��"));
			OneChosePatient.DoubleMN_Rate = _ttoi(mystr);

			mystr = m_Conn.m_pRecordset->GetCollect(_T("˫��MNC_У��"));
			OneChosePatient.DoubleMNC_Num = _ttoi(mystr);

			AllChosePatient.push_back(OneChosePatient);
			m_Conn.m_pRecordset->MoveNext();
		}
		return AllChosePatient;
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

}


// һ�����˵�΢�˷���������浽���ݿ�--����ʱ
bool CReadAndWriteForAccess::SaveToAccessMNAnalysis(MN_HandleResult * pb)
{
	_bstr_t sql;
	_variant_t var;

	//������ʷ����
	sql = _T("select  * from ΢�˷���������ݱ���ʷ���У�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("�������˻����"), _variant_t(pb->patientname));
		//m_Conn.m_pRecordset->PutCollect(_T("����ϸ������"), _variant_t(pb->sumcell));
		//m_Conn.m_pRecordset->PutCollect(_T("��΢����"), _variant_t(pb->sumwh));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ����"), _variant_t(pb->singlecell));
		m_Conn.m_pRecordset->PutCollect(_T("˫��ϸ����"), _variant_t(pb->doublecell));
		m_Conn.m_pRecordset->PutCollect(_T("���ϸ����"), _variant_t(pb->multiplecell));
		m_Conn.m_pRecordset->PutCollect(_T("����MN"), _variant_t(pb->singleMN));
		m_Conn.m_pRecordset->PutCollect(_T("����MNC"), _variant_t(pb->SingleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("����΢��ϸ����"), _variant_t(pb->SingleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("����΢����"), _variant_t(pb->SingleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MN"), _variant_t(pb->DoubleMN));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢��ϸ����"), _variant_t(pb->DoubleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢����"), _variant_t(pb->DoubleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MNC"), _variant_t(pb->DoubleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("���MNC"), _variant_t(pb->multiplecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("���MN"), _variant_t(pb->MultiMN));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢��ϸ����"), _variant_t(pb->whcellrate));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢����"), _variant_t(pb->whcellrate));
		m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell"), _variant_t(pb->SingleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell"), _variant_t(pb->SingleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell"), _variant_t(pb->SingleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell"), _variant_t(pb->SingleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell"), _variant_t(pb->SingleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell"), _variant_t(pb->SingleCellsWithMN[6]
			+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

		m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));


		//m_Conn.m_pRecordset->PutCollect(_T("��΢����_У��"), _variant_t(pb->sumwh));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ����_У��"), _variant_t(pb->singlecell));
		m_Conn.m_pRecordset->PutCollect(_T("˫��ϸ����_У��"), _variant_t(pb->doublecell));
		m_Conn.m_pRecordset->PutCollect(_T("���ϸ����_У��"), _variant_t(pb->multiplecell));
		m_Conn.m_pRecordset->PutCollect(_T("����MN_У��"), _variant_t(pb->singleMN));
		m_Conn.m_pRecordset->PutCollect(_T("����MNC_У��"), _variant_t(pb->SingleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("����΢��ϸ����_У��"), _variant_t(pb->SingleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("����΢����_У��"), _variant_t(pb->SingleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MN_У��"), _variant_t(pb->DoubleMN));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢��ϸ����_У��"), _variant_t(pb->DoubleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢����_У��"), _variant_t(pb->DoubleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MNC_У��"), _variant_t(pb->DoubleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("���MNC_У��"), _variant_t(pb->multiplecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("���MN_У��"), _variant_t(pb->MultiMN));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢��ϸ����"), _variant_t(pb->whcellrate));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢����"), _variant_t(pb->whcellrate));
		m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[6]
			+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

		m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));



		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();
	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	//���뱾������
	sql = _T("select  * from ΢�˷���������ݱ����Σ�");
	try
	{
		m_Conn.GetRecordSet(sql);
	}
	catch (_com_error *e)
	{
		AfxMessageBox(e->ErrorMessage());
	}
	try
	{
		//����Ӧ�Ĳ�Ƭ��ź���Ƭ�����������飬���ڱ�������
		m_Conn.m_pRecordset->AddNew();
		m_Conn.m_pRecordset->PutCollect(_T("�������˻����"), _variant_t(pb->patientname));
		//m_Conn.m_pRecordset->PutCollect(_T("����ϸ������"), _variant_t(pb->sumcell));
		//m_Conn.m_pRecordset->PutCollect(_T("��΢����"), _variant_t(pb->sumwh));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ����"), _variant_t(pb->singlecell));
		m_Conn.m_pRecordset->PutCollect(_T("˫��ϸ����"), _variant_t(pb->doublecell));
		m_Conn.m_pRecordset->PutCollect(_T("���ϸ����"), _variant_t(pb->multiplecell));
		m_Conn.m_pRecordset->PutCollect(_T("����MN"), _variant_t(pb->singleMN));
		m_Conn.m_pRecordset->PutCollect(_T("����MNC"), _variant_t(pb->SingleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("����΢��ϸ����"), _variant_t(pb->SingleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("����΢����"), _variant_t(pb->SingleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MN"), _variant_t(pb->DoubleMN));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢��ϸ����"), _variant_t(pb->DoubleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢����"), _variant_t(pb->DoubleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MNC"), _variant_t(pb->DoubleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("���MNC"), _variant_t(pb->multiplecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("���MN"), _variant_t(pb->MultiMN));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢��ϸ����"), _variant_t(pb->whcellrate));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢����"), _variant_t(pb->whcellrate));
		m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell"), _variant_t(pb->SingleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell"), _variant_t(pb->SingleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell"), _variant_t(pb->SingleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell"), _variant_t(pb->SingleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell"), _variant_t(pb->SingleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell"), _variant_t(pb->SingleCellsWithMN[6]
			+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

		m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));


		//m_Conn.m_pRecordset->PutCollect(_T("��΢����_У��"), _variant_t(pb->sumwh));
		m_Conn.m_pRecordset->PutCollect(_T("����ϸ����_У��"), _variant_t(pb->singlecell));
		m_Conn.m_pRecordset->PutCollect(_T("˫��ϸ����_У��"), _variant_t(pb->doublecell));
		m_Conn.m_pRecordset->PutCollect(_T("���ϸ����_У��"), _variant_t(pb->multiplecell));
		m_Conn.m_pRecordset->PutCollect(_T("����MN_У��"), _variant_t(pb->singleMN));
		m_Conn.m_pRecordset->PutCollect(_T("����MNC_У��"), _variant_t(pb->SingleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("����΢��ϸ����_У��"), _variant_t(pb->SingleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("����΢����_У��"), _variant_t(pb->SingleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MN_У��"), _variant_t(pb->DoubleMN));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢��ϸ����_У��"), _variant_t(pb->DoubleMNC_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��΢����_У��"), _variant_t(pb->DoubleMN_Rate));
		m_Conn.m_pRecordset->PutCollect(_T("˫��MNC_У��"), _variant_t(pb->DoubleMNC_Num));
		m_Conn.m_pRecordset->PutCollect(_T("���MNC_У��"), _variant_t(pb->multiplecell_wh));
		m_Conn.m_pRecordset->PutCollect(_T("���MN_У��"), _variant_t(pb->MultiMN));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢��ϸ����"), _variant_t(pb->whcellrate));
		//m_Conn.m_pRecordset->PutCollect(_T("���΢����"), _variant_t(pb->whcellrate));
		m_Conn.m_pRecordset->PutCollect(_T("1MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in singlecell_У��"), _variant_t(pb->SingleCellsWithMN[6]
			+ pb->SingleCellsWithMN[7] + pb->SingleCellsWithMN[8] + pb->SingleCellsWithMN[9] + pb->SingleCellsWithMN[10]));

		m_Conn.m_pRecordset->PutCollect(_T("1MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[1]));
		m_Conn.m_pRecordset->PutCollect(_T("2MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[2]));
		m_Conn.m_pRecordset->PutCollect(_T("3MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[3]));
		m_Conn.m_pRecordset->PutCollect(_T("4MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[4]));
		m_Conn.m_pRecordset->PutCollect(_T("5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[5]));
		m_Conn.m_pRecordset->PutCollect(_T(">5MN in doublecell_У��"), _variant_t(pb->DoubleCellsWithMN[6] +
			pb->DoubleCellsWithMN[7] + pb->DoubleCellsWithMN[8] + pb->DoubleCellsWithMN[9] + pb->DoubleCellsWithMN[10]));

		m_Conn.m_pRecordset->Update();
		//m_Conn.ExitConnect();

	}
	catch (_com_error e)
	{
		AfxMessageBox(e.Description());
	}

	return false;
}

////������ϣ�����Ƭ��Ϣ��Ϊ�ѷ���
//bool CReadAndWriteForAccess::WriteToSlideSet(Table_SlideSet slideset)
//{
//	sql = _T("select  * from ��Ƭ���ñ�");
//	try
//	{
//		m_Conn.GetRecordSet(sql);
//	}
//	catch (_com_error *e)
//	{
//		AfxMessageBox(_T("�޷��ҵ���Ƭ���ñ�"));
//	}
//	try
//	{
//		slideset.time = COleDateTime::GetCurrentTime();
//
//		m_Conn.m_pRecordset->PutCollect(_T("�Ƿ��з���"), _variant_t(slideset.HaveSolved));
//
//
//
//		m_Conn.m_pRecordset->Update();
//
//		return false;
//	}
//	catch (_com_error e)
//	{
//		AfxMessageBox(_T("��Ƭ���ñ�д�����"));
//	}
//
//}