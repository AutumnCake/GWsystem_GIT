#include <afx.h>
#include <stdio.h>
#include <Winsock2.h>
#include <string>
//#include <sstream>
#include <iostream>
#include <vector>
#include <windows.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#include "Control_Dev.h"

//************************************  
// Method:    AsciiToByte ��ASCII�ַ�ת��Ϊ����  
// Returns:   BYTE ���ص�����  
// Parameter: BYTE b ��ת�����ַ�  
//************************************  
BYTE AsciiToByte(BYTE b)
{
	BYTE ret = 0;
	if (b >= '0' && b <= '9')
		ret = b - '0';
	else if (b >= 'A' && b <= 'F')
		ret = b - 'A' + 10;
	else if (b >= 'a' && b <= 'f')
		ret = b - 'a' + 10;
	else
		ret = 0;
	return ret;
}

//*********************
/*
���ٶȻ�λ��ת��Ϊ16���Ƶ��ַ����飬����תΪ��λ��ǰ��λ�ں�
Ȼ�󽫽������ַ�ƴ�ӳɷ��͵�can֡
string pro_part��֡�̶��Ĳ���
char * whole_F��ƴ�Ӻ��can֡
long num���ٶȻ�λ��
*///
int long_to_char(string pro_part, char * whole_F, long num)
{
	TCHAR sz[9] = { 0 };//ת��Ϊ32λ��16���Ƶ��ַ�����
	_stprintf_s(sz, TEXT("%08X"), num);
	for (int j = 0; j < 8; j++)//����֡��ǰ4�ֽ�
	{
		whole_F[j] = pro_part[j];
	}

	for (int i = 0; i < 8; i++)//����֡��ǰ4�ֽ�
	{
		switch (i)
		{
		case 0:whole_F[8] = sz[6]; break;
		case 1:whole_F[9] = sz[7]; break;
		case 2:whole_F[10] = sz[4]; break;
		case 3:whole_F[11] = sz[5]; break;
		case 4:whole_F[12] = sz[2]; break;
		case 5:whole_F[13] = sz[3]; break;
		case 6:whole_F[14] = sz[0]; break;
		case 7:whole_F[15] = sz[1]; break;
		case 8:whole_F[16] = sz[8]; break;
		default:break;
		}
	}
	return 0;
}

SOCKET Control_Dev::m_sockClient = NULL;
bool Control_Dev::Status_Ref = false;
bool Control_Dev::Communication_Success = false;

Control_Dev::Control_Dev()
{
	Status_Ref = false;
	m_sockClient = NULL;
	m_CAN_num = 0;
	//m_devtype = VCI_CANETTCP;
	//m_devind = 0;
	
}
/*
CANET_Init(char *Dev_addr, int Dev_port)
����֧���ڳ�ʼ��Socket����Canת��̫�������أ�TCP_Serverģʽ������ͨ�ţ�
Dev_addr�� ���ص���ַ��Ĭ��Ϊ��192.168.1.5
Dev_port�� ���صĶ˿ںţ�Ĭ��Ϊ��5000
������ֵΪ 0����������ʧ��
������ֵΪ -1��������������ʧ��
*/
int Control_Dev::CANET_Init(char *Dev_addr, int Dev_port)
{
	closesocket(m_sockClient);
	WSACleanup();


	WORD wVersionRequested;
	WSADATA wsaData;
	int err;                           
	wVersionRequested = MAKEWORD(2, 2);  
	err = WSAStartup(wVersionRequested, &wsaData); 
	if (err != 0)
	{ 
		Communication_Success = false;
		return -1;
	}
	//SOCKET sockClient;
	if (LOBYTE(wsaData.wVersion) != 2 ||
		HIBYTE(wsaData.wVersion) != 2)
	{
		WSACleanup();
		Communication_Success = false;
		return -2;
	}
	m_sockClient = socket(AF_INET, SOCK_STREAM, 0);  //�½��ͻ���socket
	SOCKADDR_IN addrSrv;								  
	memset(&addrSrv, 0, sizeof(SOCKADDR_IN));                 
	addrSrv.sin_addr.S_un.S_addr = inet_addr(Dev_addr);  //����Ҫ���ӵķ���˵�ַ
	addrSrv.sin_family = AF_INET;
	addrSrv.sin_port = htons(Dev_port);                  //���Ӷ˿�5000
	//m_sockClient = sockClient;
	err = connect(m_sockClient, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR)); //���ӵ������
	if (err)
	{
		Communication_Success = false;
		return -1; 
	}											//����ʧ��
	
	Communication_Success = true;
	Dev_heart_beat(1, false);
	Dev_heart_beat(2, false);
	Dev_heart_beat(3, false);
	Dev_heart_beat(4, false);
	Dev_heart_beat(5, false);
	return 0;    // ���ӳɹ�
}
/*
int Control_Dev::CANET_Init(char *Dev_addr, int Dev_port)
{
	if (Communication_Success)
	{
		Communication_Success = false;
		Sleep(500);
		VCI_CloseDevice(m_devtype, m_devind);
		return 0;
	}
	/*
	char a[10] = "asdf";
	//  	itoa(189,a,10);
	char b[2] = "s";
	// 	b[0]='.';
	// 	b[1]='\0';
	strcat(a, b);
	char c[10];
	itoa(199, c, 10);
	*/
/*
	if (VCI_OpenDevice(m_devtype, m_devind, 0) != STATUS_OK)
	{
		return -1;
	}
	VCI_SetReference(m_devtype, m_devind, 0, CMD_DESIP, (PVOID)Dev_addr);
	VCI_SetReference(m_devtype, m_devind, 0, CMD_DESPORT, (PVOID)&Dev_port);

	if (VCI_StartCAN(m_devtype, m_devind, 0) != STATUS_OK)
	{
		return -1;
	}

	Communication_Success = true;
	return 0;
}
*/
/*
Send_command(char*pData, char*F_id, int iStandorExtern, int iRemoteOrData)
char*pData�����͵�can֡�������ַ������飬16���Ƹ�ʽ����8λ�����磺"2B40600006000000"
char*F_id�����͵�can֡��ID�ţ�16���ƣ���6λ  ���磺 "00000601"
*/
int Control_Dev::Send_command(char*pData, char*F_id, int iStandorExtern, int iRemoteOrData)
{
	if (pData == NULL)return -2;

	if (m_sockClient == NULL)return -1;

	if (F_id == NULL)return -3;

	DWORD iTickStart = GetTickCount(); // ??????
	// ֡ID�Ĵ���
	//char F_id[9];   //������can֡ID
	//F_id = F_id;
	char F_id_X[9] = { '0' }; //ת��֡ID����ַ�����
	int len_id = strlen(F_id);
	for (int i = 0; i < len_id / 2; i++)   //ת��Ϊÿ2λת��Ϊ16���Ƶ�ASCII�ַ�
	{
		char temF[9] = "0";
		strncpy_s(temF, F_id + i * 2, 2);
		sscanf_s(temF, "%X", &F_id_X[i]);
	}
	//can֡����ת��

	char F_data_X[17] = { '0' };//ת��֡���ݴ���ַ�����
	int len_data = strlen(pData);
	for (int i = 0; i < len_data / 2; i++)
	{
		char temF[17] = "0";
		strncpy_s(temF, pData + i * 2, 2);
		sscanf_s(temF, "%X", &F_data_X[i]);
	}

	// ������������ƴ��
	int iSendLen = (len_data + len_id+2) / 2;
	//int iSendLen = strlen();
	char*pSend = new char[iSendLen];
	memset(pSend, 0, iSendLen);

	int  j = 0;//����������

	BYTE btSig = 0;
	if (iStandorExtern)  //�Ƿ�Ϊ��׼֡
		btSig |= (1 << 7);
	if (iRemoteOrData)
		btSig |= (1 << 6);//�Ƿ�ΪԶ��֡
	BYTE btLen = 8;
	btSig |= btLen;

	pSend[j++] = btSig;   //��ʾλ+6��can���ı�־λ
	for (int i = 0; i < 4; i++)  //����can֡ID 
	{
		pSend[j] = F_id_X[i];
		j++;
	}

	for (int h = 0; h < btLen; h++)//����8���ֽڵ�����
	{
		pSend[j] = F_data_X[h];
		j++;
	}

	//int iRes = 0;
	int send_Result = send(m_sockClient, pSend, iSendLen, 0);
	//int hh = send_Result;
	//while(send_Result == SOCKET_ERROR);
	//{
	//	//if (m_sockClient != NULL)
	//	//{
	//	//	closesocket(m_sockClient);
	//	//	WSACleanup();
	//	//}
	//	//Sleep(1000);
	//	if (m_sockClient == NULL)
	//	{
	//		 CANET_Init("192.168.1.5", 5000);
	//		 Sleep(20);
	//	}

	//}

	Sleep(5);
	delete pSend;
	return 0;
}

/*
int Control_Dev::Send_command(char*pData, char*F_id, int iStandorExtern, int iRemoteOrData)
{
	if (!Communication_Success)
		return -1;
	VCI_CAN_OBJ frameinfo;
	BYTE datalen = 8;

	frameinfo.DataLen = datalen;
	memcpy(&frameinfo.Data, pData, datalen);

	frameinfo.RemoteFlag = 0;//����֡
	frameinfo.ExternFlag = 0;//��׼֡
	if (frameinfo.ExternFlag == 1)
	{
		frameinfo.ID = ((DWORD)F_id[0] << 24) + ((DWORD)F_id[1] << 16) + ((DWORD)F_id[2] << 8) +
			((DWORD)F_id[3]);
	}
	else
	{
		frameinfo.ID = ((DWORD)F_id[2] << 8) + ((DWORD)F_id[3]);
	}

	if (!VCI_Transmit(m_devtype, m_devind, m_CAN_num, &frameinfo, 1) == 1)
	{
		return -1;
	}
	return 0;
}*/
/*
Recv_message(char *pRecv,int &iRLen)

char *pRecv���յ��ı��ģ�����ֵ
int &iRLen�����ܵı��ĳ��ȣ�ÿ������13���ַ�������ֵ�ɶ���������ɡ�

*/
int Control_Dev::Recv_message(char *pRecv, int &iRLen, int Timeout)
{
	int iRes = 0;
	//ULONG iLen = (iRLen + 7) / 8 * 13;
	int iLen = iRLen  / 13 * 13;
	char*pActvRecv = new char[iLen];// 
	memset(pActvRecv, 0, iLen);  

	/*���ó�ʱ*///**********************************
	int nNetTimeout =10;//0.5�룬 
	//���÷��ͳ�ʱ 
	//setsockopt(socket��SOL_SOCKET, SO_SNDTIMEO��(char *)&nNetTimeout, sizeof(int));
	//���ý��ճ�ʱ 
	setsockopt(m_sockClient, SOL_SOCKET, SO_RCVTIMEO, (char *)&nNetTimeout, sizeof(int));
	//****************************************

	iRes = recv(m_sockClient, pActvRecv, iLen, 0);
	//iRes = recv(m_sockClient, pActvRecv, iLen, MSG_WAITALL);

	Sleep(1);
	/*******��ȡ��ȡ���ı��ĵĳ���********************/
	int n = _mbslen((unsigned char*)pActvRecv);
	/*************************************************/


	if (iRes == 0 || iRes == -1) //����ʧ��
	{
		iRLen = 0;
		delete[] pActvRecv;

		if (m_sockClient != NULL)
		{
			closesocket(m_sockClient);
			WSACleanup();
			Sleep(10);
			CANET_Init("192.168.1.5", 5000);
			Sleep(20);
		}

		return -1;
	}
	CString Byte_hex;
	BYTE ghg;
	for (int i = 0; i < iLen; i++)
	{
		pRecv[i] = pActvRecv[i];
	}
	pRecv = pActvRecv;

	/*
	int j = 0;
	UINT cv = 1;
	//char cv3;
	UINT imod = 0;
	for (int i = 0; i < iLen; i++)
	{
		ghg = pActvRecv[i];
		Byte_hex.Format(_T("%02x"), ghg);
		//Byte_hex.Format(_T("%s"), ghg);
		//cout << Byte_hex;
		imod = i % 13;
		if (imod)
		{
			pRecv[j] = pActvRecv[i];
			cv = pRecv[j];
			j++;
			iRLen = i-i/13;
		}
		else
		{
			cv = pActvRecv[i];
			if (!cv)
			{
				iRLen = i;
				break;
			}
		}

	}
	*/
	delete[] pActvRecv;
	//delete arrt;

	return 0;
}
/*
int Control_Dev::Recv_message(char *pRecv, int &iRLen, int Timeout)
{
	//VCI_CAN_OBJ frameinfo[50];
	VCI_ERR_INFO errinfo;
	int len = 1, length;
	int i = 0;
	CString str, tmpstr;

		if (!Communication_Success)
			return -1;
		//len=VCI_Receive(dlg->m_devtype,dlg->m_devind,dlg->m_cannum,frameinfo,50,200);
		length = VCI_GetReceiveNum(m_devtype, m_devind, m_CAN_num);
		if (length <= 0)
		{
			pRecv = NULL;
			return -1;
		}
		PVCI_CAN_OBJ frameinfo = new VCI_CAN_OBJ[length];
		len = VCI_Receive(m_devtype, m_devind, m_CAN_num, frameinfo, length, 200);
		if (len <= 0)
		{
			//ע�⣺���û�ж��������������ô˺�������ȡ����ǰ�Ĵ����룬
			//ǧ����ʡ����һ������ʹ����ܲ���֪����������ʲô��
			VCI_ReadErrInfo(m_devtype,m_devind,m_CAN_num, &errinfo);
			return -1;
		}
		else
		{
			for (i = 0; i<len; i++)
			{
				tmpstr.Format(_T("֡ID:%08x"), frameinfo[i].ID);
				if (frameinfo[i].RemoteFlag == 0)
				{
					str = "����:";
					if (frameinfo[i].DataLen>8)
						frameinfo[i].DataLen = 8;
					for (int j = 0; j<frameinfo[i].DataLen; j++)
					{
						pRecv[j] = frameinfo[i].Data[j];
					}

				}
			}
			
		}
	delete[] frameinfo;
	return 0;

}*/
/*
Dev_ENA(UINT Dev_id, bool Dev_Switch)
���ʹ�ܣ��Ͽ�ʹ�ܺ���

UINT Dev_id�� �����ID�ţ�
bool Dev_Switch�� ΪTrueʱ����ʾ����ʹ�ܣ�Ϊfalseʱ��ʾ�Ͽ�ʹ�ܣ�

*/
int Control_Dev::Dev_Reset_Communication()
{
	send(m_sockClient, "0008200", 8, 0);
	Sleep(5);
	return 0;
}
int Control_Dev::Dev_ENA(UINT Dev_id, bool Dev_Switch)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString Dev_id_s;
	Dev_id_s.Format(_T("%02x"),Dev_id);
	CString Dev_F_id = "000006" + Dev_id_s;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	//���ȶϿ�ʹ��
	char pSend[17] = "2B40600006000000";
	//Send_command(pSend, F_id, 0, 0);

	if (Dev_Switch)
	{
		Send_command(pSend, F_id, 0, 0);
		strcpy_s(pSend, "2B40600007000000");
		Send_command(pSend, F_id, 0, 0);
		strcpy_s(pSend, "2B4060000F000000");
		Send_command(pSend, F_id, 0, 0);

	}
	else
	{
		Send_command(pSend, F_id, 0, 0);
	}
	return 0;
}
int Control_Dev::Dev_PUSIRobot_ENA(UINT Dev_id, bool Dev_Switch)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString Dev_id_s;
	Dev_id_s.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + Dev_id_s;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}

	if (Dev_Switch)
	{
		Send_command("2F0E600001000000", F_id, 0, 0);

	}
	else
	{
		Send_command("2F0E600000000000", F_id, 0, 0);
	}
	return 0;
}
/*
Dev_clear_alarm(UINT Dev_id);  �������
UINT Dev_id�� �����ID�ţ�
*/
int Control_Dev::Dev_clear_alarm(UINT Dev_id)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString Dev_id_s;
	Dev_id_s.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + Dev_id_s;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	char pSend[17] = "2B40600080000000";
	//Sleep(30);
	Send_command(pSend, F_id, 0, 0);//�������
	Send_command("2B4060001F010000", F_id, 0, 0);//��������
	//Sleep(30);
	return 0;

}

/*
���λ�ÿ���ģʽ��
Dev_relatice_Pos(UINT Dev_id, long Relative_Pos, int Dev_Pro_Velocity = 5)
UINT Dev_id�� �����ID�ţ�
long Relative_Pos �����λ�ã���Ŀǰ��λ����������20000����/ת��
int Dev_Pro_Velocity �������˶��ٶ�
*/
int Control_Dev::Dev_Single_Mov(UINT Dev_id, long Move_Pos, float Dev_Pro_Velocity, bool abs_or_rel)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	//����������ٶ�ת��Ϊ16���Ƶ��ַ���can֡data  ************
	long F_Veloc_s = Dev_Pro_Velocity * 240;
	char F_Velocity[17] = "0";
	long_to_char("23816000", F_Velocity, F_Veloc_s);
	/*
	longtoHex.Format(_T("%08x"), F_Veloc_s);
	CString Dev_F_data = "23816000" + longtoHex.Right(2) + longtoHex.Mid(4, 2) + longtoHex.Mid(2, 2) + longtoHex.Left(2);
	for (int i = 0; i < Dev_F_data.GetLength(); i++)
	{
		F_Velocity[i] = Dev_F_data[i];
	}
	*/

	//���������λ��ת��Ϊ16���Ƶ��ַ���can֡data ************
	char F_Postion[17] = "0";
	long_to_char("237A6000", F_Postion, Move_Pos);
	/*
	longtoHex.Format(_T("%08x"), Relative_Pos);
	Dev_F_data = "237A6000" + longtoHex.Right(2) + longtoHex.Mid(4, 2) + longtoHex.Mid(2, 2) + longtoHex.Left(2);
	for (int i = 0; i < Dev_F_data.GetLength(); i++)
	{
		F_Postion[i] = Dev_F_data[i];
	}
	*/

	Send_command(F_Velocity, F_id, 0, 0);//�ٶ�

	Send_command(F_Postion, F_id, 0, 0);//λ��

	if (abs_or_rel)                //����λ��
	{
		//Send_command("2B4060000F020000", F_id, 0, 0);//�����λ
		//Send_command("2B4060001F020000", F_id, 0, 0);//��λ��λ��ָ��
		Send_command("2B4060001F000000", F_id, 0, 0);//��λ��λ��ָ��
		Send_command("2B4060000F000000", F_id, 0, 0);//�����λ
		//Sleep(5);

	}
	else                          //���λ��
	{
		//Send_command("2B4060004F020000", F_id, 0, 0);//�����λ
	
		Send_command("2B4060005F020000", F_id, 0, 0);//��λ��λ��ָ��
	
		Send_command("2B4060004F020000", F_id, 0, 0);//�����λ
		//Sleep(5);
	}
	Status_Ref = false;//�·��˶��󽫣��·�ָ��ı�־λ���㣬���¶�ȡ��
	return 1;
}
int Control_Dev::Dev_PUSIRobot_Single_Mov(UINT Dev_id, long Move_Pos, float Dev_Pro_Velocity, bool abs_or_rel)
{

	Dev_PUSIRobot_ENA(Dev_id,true);
	Sleep(500);
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	//����������ٶ�ת��Ϊ16���Ƶ��ַ���can֡data  ************
	long F_Veloc_s = Dev_Pro_Velocity;
	char F_Velocity[17] = "0";
	long_to_char("23036000", F_Velocity, F_Veloc_s);

	Send_command("2F006000FF000000", F_id, 0, 0);//�������״̬
	Send_command("2F016000FF000000", F_id, 0, 0);//�������״̬
	Send_command("2F05600000000000", F_id, 0, 0);//����λ�ÿ���ģʽ
	if (Dev_id == 4)
	{
		Send_command("2F08600006000000", F_id, 0, 0);//���ü��ٶ�ϵ��1-8,06
		Send_command("2F09600006000000", F_id, 0, 0);//���ü��ٶ�ϵ��1-8,06
	}
	else
	{
		Send_command("2F08600002000000", F_id, 0, 0);//���ü��ٶ�ϵ��1-8,06
		Send_command("2F09600002000000", F_id, 0, 0);//���ü��ٶ�ϵ��1-8,06
	}

	Send_command("2F02600001000000", F_id, 0, 0);//����ת������
	Send_command(F_Velocity, F_id, 0, 0);//�ٶ�


	if (abs_or_rel)                //����λ��
	{
		char F_Postion[17] = "0";
		long_to_char("231C6000", F_Postion, Move_Pos);
		Send_command(F_Postion, F_id, 0, 0);//λ��
	}
	else                          //���λ��
	{
		char F_Postion[17] = "0";
		long_to_char("23046000", F_Postion, Move_Pos);
		Send_command(F_Postion, F_id, 0, 0);//λ��
	}
	Status_Ref = false;//�·��˶��󽫣��·�ָ��ı�־λ���㣬���¶�ȡ��
	return 1;
}
int Control_Dev::Dev_Single_Mov_Para(UINT Dev_id, unsigned int Control_Model)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	switch (Control_Model)
	{
	case 0:

		//Send_command("2B40600006000000", F_id, 0, 0);//�������ÿ�����/  ʹ��
		Send_command("2B40600007000000", F_id, 0, 0);//����Switch on/  ʹ��
		Send_command("2B4060000F000000", F_id, 0, 0);//���ÿ�����10F���������/  ʹ��

		Send_command("2F60600001000000", F_id, 0, 0);//λ�ÿ���ģʽ����

		//Send_command("2383600058020000", F_id, 0, 0);//���ü��ٶ�100r/min

		//Send_command("2384600058020000", F_id, 0, 0);//���ü��ٶ�100r/min
		//Send_command("2383600058020000", F_id, 0, 0);//���ü��ٶ�100r/min
		switch (Dev_id)

		{
		case 1:
			Send_command("23836000B0040000", F_id, 0, 0);//���ü��ٶ�200��B004��r/min��300��0807��100��5802��60��6801��

			Send_command("23846000B0040000", F_id, 0, 0);//���ü��ٶ�200r/min
			break;
		case 2:
			//Send_command("2383600008070000", F_id, 0, 0);//���ü��ٶ�200��B004��r/min��300��0807��

			//Send_command("2384600008070000", F_id, 0, 0);//���ü��ٶ�200r/min

			Send_command("2383600058020000", F_id, 0, 0);//���ü��ٶ�200��B004��r/min��300��0807��100��5802��60��6801��

			Send_command("2384600058020000", F_id, 0, 0);//���ü��ٶ�200r/min

			break;
		case 3:
			Send_command("2383600058020000", F_id, 0, 0);//���ü��ٶ�50r/min(2C01) 60��6801��100��5802��

			Send_command("2384600058020000", F_id, 0, 0);//���ü��ٶ�50r/min(2C01)
			//Send_command("238360003C000000", F_id, 0, 0);//���ü��ٶ�10r/min(3C00)

			//Send_command("238460003C000000", F_id, 0, 0);//���ü��ٶ�10r/min(3C00)
			//Send_command("2383600096000000", F_id, 0, 0);//���ü��ٶ�25r/min(9600)

			//Send_command("2384600096000000", F_id, 0, 0);//���ü��ٶ�25r/min(9600)
			break;
		case 4:
			Send_command("238360002C010000", F_id, 0, 0);//���ü��ٶ�50r/min
			Send_command("238460002C010000", F_id, 0, 0);//���ü��ٶ�50r/min
			break;
		case 5:
			//������־can����������
			Send_command("238360002C010000", F_id, 0, 0);//���ü��ٶ�50r/min
			Send_command("238460002C010000", F_id, 0, 0);//���ü��ٶ�50r/min

			break;
		default:
			break;
		}
		break;
	case 1:
		break;
	case 2:
		Send_command("2B40600006000000", F_id, 0, 0);//�������ÿ�����/  ʹ��
		Send_command("2B40600007000000", F_id, 0, 0);//����Switch on/  ʹ��
		Send_command("2B4060000F010000", F_id, 0, 0);//���ÿ�����10F���������/  ʹ��

		Send_command("2F60600003000000", F_id, 0, 0);//�ٶȿ���ģʽ��

		Send_command("2383600008070000", F_id, 0, 0);//���ü��ٶ�200��B004��r/min��300��0807��

		Send_command("2384600008070000", F_id, 0, 0);//���ü��ٶ�200r/min
		break;
	default:
		break;
	}
	
	return 1;
}

int Control_Dev::Dev_Velocity_Model(UINT Dev_id, int START_STOP, float Dev_Pro_Velocity, bool Continue_Model)
{

	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	//����������ٶ�ת��Ϊ16���Ƶ��ַ���can֡data  ************
	long F_Veloc_s = (long)(Dev_Pro_Velocity * 240);
	char F_Velocity[17] = "0";
	long_to_char("23FF6000", F_Velocity, F_Veloc_s);
	/*
	longtoHex.Format(_T("%08x"), F_Veloc_s);
	CString Dev_F_data = "23816000" + longtoHex.Right(2) + longtoHex.Mid(4, 2) + longtoHex.Mid(2, 2) + longtoHex.Left(2);

	for (int i = 0; i < Dev_F_data.GetLength(); i++)
	{
	F_Velocity[i] = Dev_F_data[i];
	}
	*/
		switch (START_STOP)

		{
		case 1:
			if (Continue_Model)
			{
				Send_command("2B4060000F000000", F_id, 0, 0);//���ÿ�����10F���������
				Send_command(F_Velocity, F_id, 0, 0);		//�����ٶ�
			}
			else
			{
				Send_command(F_Velocity, F_id, 0, 0);		//�����ٶ�
			}

			break;

		case 2:
			Send_command("2B4060000F010000", F_id, 0, 0);//���ÿ�����10F���������/  ʹ��
			break;
		default:
			break;

		}

	Status_Ref = false;//�·��˶��󽫣��·�ָ��ı�־λ���㣬���¶�ȡ��
	return 1;
}

/*
ֹͣ�˶���
Dev_stop(UINT Dev_id)
UINT Dev_id�� �����ID�ţ�
*/
int Control_Dev::Dev_stop(UINT Dev_id)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	Send_command("2B4060004F010000", F_id, 0, 0);//ֹͣ
	return 0;
}
/***************************************
���λ�ã�Ҳ��������λ����㣺
Dev_Set_Zero_Postion(UINT Dev_id)
UINT Dev_id�� �����ID�� */
int Control_Dev::Dev_Set_Zero_Postion(UINT Dev_id)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	Send_command("2F0C700001000000", F_id, 0, 0);//  λ�����
	return 0;
}

int Control_Dev::Dev_PUSIRobot_Set_Zero_Postion(UINT Dev_id)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	Send_command("230C600000000000", F_id, 0, 0);//  ����ǰλ������Ϊ��
	return 0;
}
/***************************************
ֹͣ�˶���
Dev_stop(UINT Dev_id)
UINT Dev_id�� �����ID��
*/

int Control_Dev::Dev_heart_beat(UINT Dev_id, bool open_close)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	if (open_close)
	{
		Send_command("2B171000E8030000", F_id, 0, 0);//��������1000msһ��
		//Sleep(30);
	}
	else
	{
		Send_command("2B17100000000000", F_id, 0, 0);//�رձ��ģ�
		//Sleep(30);
	}

	return 0;
}

/***************************************
���üӼ��٣�
Dev_stop(UINT Dev_id)
UINT Dev_id�� �����ID�� */
int Control_Dev::Dev_acc_dec(UINT Dev_id, long Dev_Acc, long Dev_Dec)
{
	//���������IDת��Ϊ16���Ƶ��ַ���can֡ID
	CString longtoHex;
	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int i = 0; i < Dev_F_id.GetLength(); i++)
	{
		F_id[i] = Dev_F_id[i];
	}
	//����������ٶ�ת��Ϊ16���Ƶ��ַ���can֡data
	int F_Acc = Dev_Acc * 6;
	longtoHex.Format(_T("%08x"), F_Acc);
	CString Dev_F_data = "23836000" + longtoHex.Right(2) + longtoHex.Mid(4, 2) + longtoHex.Mid(2, 2) + longtoHex.Left(2);
	char F_Acc_s[17] = "0";
	for (int i = 0; i < Dev_F_data.GetLength(); i++)
	{
		F_Acc_s[i] = Dev_F_data[i];
	}
	//���������λ��ת��Ϊ16���Ƶ��ַ���can֡data
	int F_Dec = Dev_Dec * 6;
	longtoHex.Format(_T("%08x"), F_Dec);
	Dev_F_data = " 23846000" + longtoHex.Right(2) + longtoHex.Mid(4, 2) + longtoHex.Mid(2, 2) + longtoHex.Left(2);
	char F_Dec_s[17] = "0";
	for (int i = 0; i < Dev_F_data.GetLength(); i++)
	{
		F_Dec_s[i] = Dev_F_data[i];
	}

	Send_command(F_Acc_s, F_id, 0, 0);//���ٶ�
	//Sleep(30);
	Send_command(F_Dec_s, F_id, 0, 0);//���ٶ�
	//Sleep(30);

	return 0;
}

/*��ѯ״̬�֡�λ�õȣ�
Dev_Read_Status(UINT *Dev_id, BOOL *Dev_Status, int recv_len = 100);
UINT *Dev_id�� �����ID������
INT *Dev_Status : ��Ų�ѯ����״̬����id����һ����С����ĳһ��Ԫ��Ϊ0��ʾ��ǰ�˶�/����δ��ɣ�Ϊ1��ʾ�˶�/�������
					Ϊ-1��ʾ��ǰ�˶�/������ִ���
int recv_len = 100����������ʱ�Ļ����С��Ĭ��Ϊ100������ķ�ʽ�ǣ��Ƚ������б��ģ����������ٷ��Ͳ�ѯָ�
Ȼ���ٽ��ղ�ѯ���ģ�ƥ����Ҫ�ı������
int model=0 :�˶�ģʽ��������0ʱ��ʾ��ǰֵ�ǻ���ģʽ����ѯ�Ƿ���㵽λ��������1ʱ��ʾ��ǰ��λ�ÿ���ģʽ����ѯ�Ƿ��˶���λ*/
int Control_Dev::Dev_Read_Status(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model, int recv_len)
{
	
	//��ջ���
	
	int recv_real_len = recv_len;
	char *pRecv = new char[recv_len];
	//��ջ���***************************
	/*
	while (recv_real_len >(recv_len / 13 * 12))
	{
		recv_real_len = recv_len;
		Recv_message(pRecv, recv_real_len);
	}
	*///**********************************

	//Recv_message(pRecv, recv_real_len);  //���Ƚ���һ��recv������


	//��ѯ����
	int id_len = Dev_id.size();
	CString longtoHex;

	for (int i = 0; i < id_len; i++)
	{
		longtoHex.Format(_T("%02x"), Dev_id[i]);
		CString Dev_F_id = _T("000006") + longtoHex;
		char F_id[9] = "0";
		for (int m = 0; m < Dev_F_id.GetLength(); m++)
		{
			F_id[m] = Dev_F_id[m];
		}
		//����״̬�ֲ�ѯָ��
		Send_command("4B41600000000000", F_id, 0, 0);
		//Send_command("4B41600000000000", F_id, 0, 0);
		//Send_command("4B41600000000000", F_id, 0, 0);
		//���½Ӳ�ѯ��ǰ״̬
		///Sleep(20);
		memset(pRecv, 0, recv_len);
		BOOL target_reached = false;
		int k = 0;
		while ((!target_reached) & (k < 2))//����֤���Ƿ�ʹ��K���жϣ�������������
		{
			recv_real_len = recv_len;
			/*Recv_message(pRecv, recv_real_len);*/

			if (Recv_message(pRecv, recv_real_len) == -1)
				Send_command("4B41600000000000", F_id, 0, 0);

			//	continue;
			
			//��ȡÿһλ�ı��ģ�ÿ�����Ĺ�12����
			/*
			for (int m = 0; m < recv_real_len; m++)
			{
				char uu = pRecv[m];
			}

			*/
			int j = 0;
			while ((!target_reached) && (j < recv_real_len ))
			{
				char temp_p, temp_p1, temp_p2, temp_p3, temp_p4, temp_p5, temp_p6;
				if (pRecv[j] != 8)
				{
					char shtr = pRecv[j];
					j++;
					continue;
				}
				temp_p = pRecv[j + 3];
				temp_p1 = pRecv[j + 4];
				temp_p2 = pRecv[j + 5];
				temp_p3 = pRecv[j + 6];
				temp_p4 = pRecv[j + 7];
				temp_p5 = pRecv[j + 8];
				temp_p6 = pRecv[j + 9];
				//if ((bool)(((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x40')) || (bool)(pRecv[j * 12 + 5] == '\x7c'))
				bool ID_bool = ((((char)pRecv[j+3] & '\x0f') == '\x05')) && ((((char)pRecv[j+4] & '\x0f') == Dev_id[i]));
				Status_Ref = ID_bool && ((j + 12) < recv_real_len);
				if (!Status_Ref)
				{
					j++;
					continue;
				}
				//�ж����˶��Ƿ�λ��״̬�Ĵ����ĵ�10λ��(״̬�� 0 �仯��1),�ж�״̬�Ƿ�Ϊ1��
				if (Status_Ref && (bool)((pRecv[j+4] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j + 6] == '\x41'))
				{
					switch (model)
					{
					case 0: if (pRecv[j+ 10] == '\x20')// ��12λΪ1ʱ��ʾ��λ���̳��ִ���
							{
								Dev_Status[i] = -1;//�����˶�����
								target_reached = TRUE;
							}
							else
							{
								Dev_Status[i] = pRecv[j + 10] & '\x04';// 16��ʾ״̬�ֵĸ�8λ��16=00010000����12λ��ʾ��λ��ɱ�־
								target_reached = TRUE;
							}

							break;
					case 1:if (pRecv[j+ 10] == '\x20')// ��12λΪ1ʱ��ʾλ�ÿ����˶����̳��ִ���
							{
							   Dev_Status[i] = -1;//λ���˶�����
							   target_reached = TRUE;

							}
						   else     ////ע���������е�������12λ�ģ�
						   {
							   Dev_Status[i] = pRecv[j+ 10] & '\x04';// 20��ʾ״̬�ֵĸ�8λ��20=00010100����10λ��ʾλ�õ�λ
							   target_reached = TRUE;
						   }
						   break;
					default:
						break;
					}
					j += 12;
				}
				else
					j += 12;
			}
			/*
			int j = 0;
			while ((!target_reached) & (j < recv_real_len / 12))
			{
				j++;
				//if ((bool)(((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x40')) || (bool)(pRecv[j * 12 + 5] == '\x7c'))
				if ((bool)((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]))
				{
					Status_Ref = true;
				}
				//�ж����˶��Ƿ�λ��״̬�Ĵ����ĵ�10λ��(״̬�� 0 �仯��1),�ж�״̬�Ƿ�Ϊ1��
				if (Status_Ref && (bool)((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x41'))
				{
					switch (model)
					{
					case 0: if (pRecv[j * 12 + 9]== '\x20')// ��12λΪ1ʱ��ʾ��λ���̳��ִ���
							{
								Dev_Status[i] = -1;//�����˶�����
								target_reached = TRUE;
							}
							else
							{
								Dev_Status[i] = pRecv[j * 12 + 9] & '\x04';// 16��ʾ״̬�ֵĸ�8λ��16=00010000����12λ��ʾ��λ��ɱ�־
								target_reached = TRUE;
							}

						   break;
					case 1:if (pRecv[j * 12 + 9] == '\x20')// ��12λΪ1ʱ��ʾλ�ÿ����˶����̳��ִ���
							{
							   Dev_Status[i] = -1;//λ���˶�����
							   target_reached = TRUE;

							}
						   else     ////ע���������е�������12λ�ģ�
						   {
							   Dev_Status[i] = pRecv[j * 12 + 9] & '\x04';// 20��ʾ״̬�ֵĸ�8λ��20=00010100����10λ��ʾλ�õ�λ
							   target_reached = TRUE;
						   }
						   break;
					default:
						break;
					}

				}
			}*/
			k++;
		}
	}
	delete pRecv;
	return 0;
}
int Control_Dev::Dev_Read_PosControl_Response(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model, int recv_len)
{

	//��ջ���

	int recv_real_len = recv_len;
	char *pRecv = new char[recv_len];
	//��ջ���***************************
	/*
	while (recv_real_len >(recv_len / 13 * 12))
	{
	recv_real_len = recv_len;
	Recv_message(pRecv, recv_real_len);
	}
	*///**********************************

	//Recv_message(pRecv, recv_real_len);  //���Ƚ���һ��recv������


	//��ѯ����
	int id_len = Dev_id.size();
	CString longtoHex;

	for (int i = 0; i < id_len; i++)
	{
		longtoHex.Format(_T("%02x"), Dev_id[i]);
		CString Dev_F_id = _T("000006") + longtoHex;
		char F_id[9] = "0";
		for (int m = 0; m < Dev_F_id.GetLength(); m++)
		{
			F_id[m] = Dev_F_id[m];
		}
		//����״̬�ֲ�ѯָ��
		Send_command("4B41600000000000", F_id, 0, 0);
		//Send_command("4B41600000000000", F_id, 0, 0);
		//Send_command("4B41600000000000", F_id, 0, 0);
		//���½Ӳ�ѯ��ǰ״̬
		///Sleep(20);
		memset(pRecv, 0, recv_len);
		BOOL target_reached = false;
		int k = 0;
		while ((!target_reached) & (k < 2))//����֤���Ƿ�ʹ��K���жϣ�������������
		{
			recv_real_len = recv_len;
			/*Recv_message(pRecv, recv_real_len);*/

			if (Recv_message(pRecv, recv_real_len) == -1)
				Send_command("4B41600000000000", F_id, 0, 0);

			//	continue;

			//��ȡÿһλ�ı��ģ�ÿ�����Ĺ�12����
			/*
			for (int m = 0; m < recv_real_len; m++)
			{
			char uu = pRecv[m];
			}

			*/
			int j = 0;
			while ((!target_reached) && (j < recv_real_len))
			{
				//if ((bool)(((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x40')) || (bool)(pRecv[j * 12 + 5] == '\x7c'))
				bool ID_bool = ((((char)pRecv[j] & '\xf0') == '\x80')) && ((((char)pRecv[j] & '\x0f') == Dev_id[i]));
				if (ID_bool && ((j + 6) < recv_real_len))//(bool)((pRecv[j] & '\x0f') == Dev_id[i])
				{
					Status_Ref = true;
				}
				//�ж����˶��Ƿ�λ��״̬�Ĵ����ĵ�10λ��(״̬�� 0 �仯��1),�ж�״̬�Ƿ�Ϊ1��
				if (Status_Ref && (bool)((pRecv[j] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j + 2] == '\x41'))
				{
					switch (model)
					{
					case 0: if (pRecv[j + 6] == '\x20')// ��12λΪ1ʱ��ʾ��λ���̳��ִ���
					{
								Dev_Status[i] = -1;//�����˶�����
								target_reached = TRUE;
					}
							else
							{
								Dev_Status[i] = pRecv[j + 6] & '\x04';// 16��ʾ״̬�ֵĸ�8λ��16=00010000����12λ��ʾ��λ��ɱ�־
								target_reached = TRUE;
							}

							break;
					case 1:if (pRecv[j + 6] == '\x20')// ��12λΪ1ʱ��ʾλ�ÿ����˶����̳��ִ���
					{
							   Dev_Status[i] = -1;//λ���˶�����
							   target_reached = TRUE;

					}
						   else     ////ע���������е�������12λ�ģ�
						   {
							   Dev_Status[i] = pRecv[j + 6] & '\x04';// 20��ʾ״̬�ֵĸ�8λ��20=00010100����10λ��ʾλ�õ�λ
							   target_reached = TRUE;
						   }
						   break;
					default:
						break;
					}
					j += 9;
				}
				else
					j++;
			}
			/*
			int j = 0;
			while ((!target_reached) & (j < recv_real_len / 12))
			{
			j++;
			//if ((bool)(((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x40')) || (bool)(pRecv[j * 12 + 5] == '\x7c'))
			if ((bool)((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]))
			{
			Status_Ref = true;
			}
			//�ж����˶��Ƿ�λ��״̬�Ĵ����ĵ�10λ��(״̬�� 0 �仯��1),�ж�״̬�Ƿ�Ϊ1��
			if (Status_Ref && (bool)((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x41'))
			{
			switch (model)
			{
			case 0: if (pRecv[j * 12 + 9]== '\x20')// ��12λΪ1ʱ��ʾ��λ���̳��ִ���
			{
			Dev_Status[i] = -1;//�����˶�����
			target_reached = TRUE;
			}
			else
			{
			Dev_Status[i] = pRecv[j * 12 + 9] & '\x04';// 16��ʾ״̬�ֵĸ�8λ��16=00010000����12λ��ʾ��λ��ɱ�־
			target_reached = TRUE;
			}

			break;
			case 1:if (pRecv[j * 12 + 9] == '\x20')// ��12λΪ1ʱ��ʾλ�ÿ����˶����̳��ִ���
			{
			Dev_Status[i] = -1;//λ���˶�����
			target_reached = TRUE;

			}
			else     ////ע���������е�������12λ�ģ�
			{
			Dev_Status[i] = pRecv[j * 12 + 9] & '\x04';// 20��ʾ״̬�ֵĸ�8λ��20=00010100����10λ��ʾλ�õ�λ
			target_reached = TRUE;
			}
			break;
			default:
			break;
			}

			}
			}*/
			k++;
		}
	}
	delete pRecv;
	return 0;
}
int Control_Dev::Dev_PUSIRobot_Read_Status(vector<UINT> Dev_id, vector<bool> &Dev_Status, int model, int recv_len)
{

	//��ջ���

	int recv_real_len = recv_len;
	char *pRecv = new char[recv_len];
	//��ջ���***************************
	/*
	while (recv_real_len >(recv_len / 13 * 12))
	{
	recv_real_len = recv_len;
	Recv_message(pRecv, recv_real_len);
	}
	*///**********************************

	//Recv_message(pRecv, recv_real_len);  //���Ƚ���һ��recv������

	//��ѯ����
	int id_len = Dev_id.size();
	CString longtoHex;

	for (int i = 0; i < id_len; i++)
	{
		longtoHex.Format(_T("%02x"), Dev_id[i]);
		CString Dev_F_id = _T("000006") + longtoHex;
		char F_id[9] = "0";
		for (int m = 0; m < Dev_F_id.GetLength(); m++)
		{
			F_id[m] = Dev_F_id[m];
		}
		//����״̬�ֲ�ѯָ��
		Send_command("4001600000000000", F_id, 0, 0);//������״̬
		memset(pRecv, 0, recv_len);
		BOOL target_reached = false;
		int k = 0;
		while ((!target_reached) & (k < 2))//����֤���Ƿ�ʹ��K���жϣ�������������
		{
			recv_real_len = recv_len;
			/*Recv_message(pRecv, recv_real_len);*/

			if (Recv_message(pRecv, recv_real_len) == -1)
			{
				Send_command("4001600000000000", F_id, 0, 0);//������״̬
			}

			//int whole_NUM = 0;
			//unsigned int jjj = *pRecv;
			//if (jjj == 8)
			//{
			//	jjj = jjj + 100;
			//	while (jjj)
			//	{
			//		char shtr = pRecv[whole_NUM];
			//		jjj--;
			//		whole_NUM++;
			//	}

			//}
			int j = 0;
			while ((!target_reached) && (j < recv_real_len))
			{
				char temp_p, temp_p1, temp_p2, temp_p3, temp_p4, temp_p5, temp_p6;
				if (pRecv[j] != 8)
				{
					char shtr = pRecv[j];
					j++;
					continue;
				}

				//if ((bool)(((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x40')) || (bool)(pRecv[j * 12 + 5] == '\x7c'))
				bool ID_bool = ((((char)pRecv[j+3] & '\x0f') == '\x05')) && ((((char)pRecv[j+4] & '\x0f') == Dev_id[i]));
				if (ID_bool && ((j + 6) < recv_real_len))//(bool)((pRecv[j] & '\x0f') == Dev_id[i])
				{
					Status_Ref = true;
				}
				//�ж����˶��Ƿ�λ��״̬�Ĵ����ĵ�10λ��(״̬�� 0 �仯��1),�ж�״̬�Ƿ�Ϊ1��
				if (Status_Ref && (bool)((pRecv[j+4] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j + 6] == '\x01'))
				{
					switch (model)
					{
					case 0: 
							Dev_Status[i] = pRecv[j + 9] & '\x01';// ��ʾ�ⲿֹͣλ������
							target_reached = TRUE;
						break;
					case 1:
							//Dev_Status[i] = !(pRecv[j + 9] & '\x00');// ��ʾ�������ֹͣ״̬��Ϊ08��ʾ����������
						temp_p = pRecv[j + 3];
						temp_p1 = pRecv[j + 4];
						temp_p2 = pRecv[j + 5];
						temp_p3 = pRecv[j + 6];
						temp_p4 = pRecv[j + 7];
						temp_p5 = pRecv[j + 8];
						temp_p6 = pRecv[j + 9];
						Dev_Status[i] = (pRecv[j + 9] == '\xe0');// ����Ƿ���ɣ�Ϊ08��ʾ����������
							target_reached = TRUE;
						break;
					default:
						break;
					}
					//j += 9;
				}
				//else
					//j++;
				j += 12;
			}
			k++;
		}
	}
	delete pRecv;
	return 0;
}
int Control_Dev::Dev_Read_InputIO_Status(UINT Dev_id, int recv_len)
{
	int recv_real_len = recv_len;
	char *pRecv = new char[recv_len];

	//��ѯ����

		CString longtoHex;
		longtoHex.Format(_T("%02x"), Dev_id);
		CString Dev_F_id = _T("000006") + longtoHex;
		char F_id[9] = "0";
		for (int m = 0; m < Dev_F_id.GetLength(); m++)
		{
			F_id[m] = Dev_F_id[m];
		}
		//����״̬�ֲ�ѯָ��
		Send_command("4B03700000000000", F_id, 0, 0);
		///Sleep(20);
		memset(pRecv, 0, recv_len);
		BOOL target_reached = false;
		int k = 0;
		while ((!target_reached) & (k < 2))//����֤���Ƿ�ʹ��K���жϣ�������������
		{
			recv_real_len = recv_len;
			/*Recv_message(pRecv, recv_real_len);*/

			if (Recv_message(pRecv, recv_real_len) == -1)
				Send_command("4B03700000000000", F_id, 0, 0);

			int j = 0;
			while ((!target_reached) && (j < recv_real_len))
			{
				char temp_p, temp_p1, temp_p2, temp_p3, temp_p4, temp_p5, temp_p6;
				if (pRecv[j] != 8)
				{
					char shtr = pRecv[j];
					j++;
					continue;
				}
				temp_p = pRecv[j + 3];
				temp_p1 = pRecv[j + 4];
				temp_p2 = pRecv[j + 5];
				temp_p3 = pRecv[j + 6];
				temp_p4 = pRecv[j + 7];
				temp_p5 = pRecv[j + 8];
				temp_p6 = pRecv[j + 9];
				//if ((bool)(((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x40')) || (bool)(pRecv[j * 12 + 5] == '\x7c'))
				bool ID_bool = ((((char)pRecv[j + 3] & '\x0f') == '\x05')) && ((((char)pRecv[j + 4] & '\x0f') == Dev_id));
				Status_Ref = ID_bool && ((j + 12) < recv_real_len);
				if (!Status_Ref)
				{
					j++;
					continue;
				}
				//�ж����˶��Ƿ�λ��״̬�Ĵ����ĵ�10λ��(״̬�� 0 �仯��1),�ж�״̬�Ƿ�Ϊ1��
				if (Status_Ref && (bool)((char)pRecv[j + 7] == '\x70') && (bool)((char)pRecv[j + 6] == '\x03'))
				{
					target_reached = TRUE;
					//��9����ֵ����8λ�ֱ��ʾ1-8����������ڵ�״̬
					if (!(pRecv[j + 9] & '\x08'))//����X4������λ
					{
						delete pRecv;
						return Motor_UpperEnd_STOP;
					}
					if (!(pRecv[j + 9] & '\x04'))//����X3������λ
					{
						delete pRecv;
						return Motor_LowerEnd_STOP;
					}
					delete pRecv;
					return Motor_Move_In_Range;
				}

				j += 12;

			}

			k++;
		}
	delete pRecv;
	return -1;
}

/***************************************
��ѯλ�õȣ�
Dev_Read_Pos(UINT *Dev_id, BOOL *Dev_Status, long *Real_Pos, int recv_len)
UINT *Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID��
long *Real_Pos :��Ų�ѯ����λ�ã�
int recv_len = 100����������ʱ�Ļ����С��Ĭ��Ϊ100������ķ�ʽ�ǣ��Ƚ������б��ģ����������ٷ��Ͳ�ѯָ�
Ȼ���ٽ��ղ�ѯ���ģ�ƥ����Ҫ�ı������*/
bool Control_Dev::Dev_Read_Pos(UINT Dev_id, long &Real_Pos, int recv_len)
{
	if (Dev_id != 3)
		return Dev_Read_Pos_Theory(Dev_id, Real_Pos, recv_len);

	//��ջ���
	int recv_real_len = recv_len;
	BYTE *pRecv = new BYTE[recv_len];
	/*
	while (recv_real_len >(recv_len / 13 * 12))
	{
		recv_real_len = recv_len;
		Recv_message(pRecv, recv_real_len);
	}
	*/
	//Recv_message(pRecv, recv_real_len);

	//��ѯ����
	CString longtoHex;

		longtoHex.Format(_T("%02x"), Dev_id);
		CString Dev_F_id = "000006" + longtoHex;
		char F_id[9] = "0";
		for (int n = 0; n < Dev_F_id.GetLength(); n++)
		{
			F_id[n] = Dev_F_id[n];
		}

		//����λ�ò�ѯָ��
		//Send_command("4B41600000000000", F_id, 0, 0);//״̬��ѯ
		//Send_command("4364600000000000", F_id, 0, 0);
		Send_command("430A700000000000", F_id, 0, 0);//  
		//Send_command("430A700000000000", F_id, 0, 0);//  
		
		//Send_command("437A600000000000", F_id, 0, 0);//  Target Position
		//���½Ӳ�ѯ��ǰ״̬

		BYTE *pRecv_real = new BYTE[50];
		int count_real = 0;
		int k = 1;
		BOOL target_reached = false;
		bool Get_mes=false;
		while ((!target_reached) && (k<30))//����֤���Ƿ�ʹ��I���жϣ�������������
		{
			if ((k % 3) == 0)
			{
				//����λ�ò�ѯָ��
				//Send_command("4B41600000000000", F_id, 0, 0);//״̬��ѯ
				//Send_command("4364600000000000", F_id, 0, 0);
				Send_command("430A700000000000", F_id, 0, 0);//  
			}
			recv_real_len = recv_len;
			memset(pRecv, 0, recv_len);
			Recv_message((char*)pRecv, recv_real_len,50);
			int real_Size = strlen((char*)pRecv);// / sizeof(pRecv[0]);
			for (int i = 0; i < recv_len; i++)
			{
				char temp_p, temp_p1, temp_p2, temp_p3, temp_p4, temp_p5, temp_p6;
				if (pRecv[i] != 8)
				{
					char shtr = pRecv[i];
					i++;
					continue;
				}
				temp_p = pRecv[i + 3];
				temp_p1 = pRecv[i + 4];
				temp_p2 = pRecv[i + 5];
				temp_p3 = pRecv[i + 6];
				temp_p4 = pRecv[i + 7];
				temp_p5 = pRecv[i + 8];
				temp_p6 = pRecv[i + 9];

				char jj2 = pRecv[i];
				bool ID_bool = ((((char)pRecv[i+3] & '\x0f') == '\x05')) && ((((char)pRecv[i+4] & '\x0f') == Dev_id));

				//if (((char)pRecv[i] == '\xff'))
				//	i += 10;

				if (Get_mes || ID_bool)
				{
					if ((i + 12 > recv_real_len))
						return false;
					while (count_real<13)
					{
						pRecv_real[count_real] = pRecv[i];
						count_real++;
						i++;
					}
					Get_mes = true;
					if (count_real>12)
					{
						bool call_bool = ((char)pRecv_real[7] == '\x70') && ((char)pRecv_real[6] == '\x0a');
					
							//�жϲ�ѯ���ı��ģ���ȡλ�ã�
							if (call_bool)       //���ö�ȡλ�õ�
							{
								CString Pos_hex;
								unsigned long Pos_int;
								//Pos_int = (long)pRecv[j * 12 + 8] + ((long)pRecv[j * 12 + 9] << 8) + ((long)pRecv[j * 12 + 10] << 16) + ((long)pRecv[j * 12 + 11] << 24);
								CString Pos_H_1, Pos_H_2, Pos_L_1, Pos_L_2;
								//char jkl1 = pRecv_real[7];
								//int tt = (int)pRecv_real[7];
								Pos_H_2.Format(_T("%02x"), pRecv_real[12]);
								Pos_H_1.Format(_T("%02x"), pRecv_real[11]);
								Pos_L_2.Format(_T("%02x"), pRecv_real[10]);
								Pos_L_1.Format(_T("%02x"), pRecv_real[9]);
								//Pos_hex = pRecv[j * 12 + 11] + pRecv[j * 12 + 10] + pRecv[j * 12 + 9] + pRecv[j * 12 + 8];
								Pos_hex = Pos_H_2 + Pos_H_1 + Pos_L_2 + Pos_L_1;
								//��Cstringת��Ϊstring��
								wstring ws(Pos_hex);
								string s_Pos_hex;
								s_Pos_hex.assign(ws.begin(), ws.end());
								stringstream Pos_stream;
								Pos_stream << std::hex << s_Pos_hex;
								Pos_stream >> Pos_int;
								//cout << Pos_int<<endl;
								//cout << static_cast<long>(Pos_int)<<endl;
								Real_Pos = static_cast<long>(Pos_int);
								target_reached = true;
								i = recv_len;
							}

							Get_mes = false;
							count_real = 0;
							memset(pRecv_real, 0, 50);
						}

					}
				i += 12;
			}

			k++;
		}
		delete pRecv;
		delete pRecv_real;
		if (target_reached)
		{
			//Real_Pos = (double)Real_Pos;//���������ֵ�Ǳ�����������������ֻ���28�������������4096��
			//һȦ��4096����Ч��һȦ��������20000��������Ĭ�ϣ�

			return true;
		}
		else
		{
			Real_Pos = 0;

			return false;
		}


	return 0;
}

bool Control_Dev::Dev_Read_Pos_Theory(UINT Dev_id, long &Real_Pos, int recv_len)
{

	//��ջ���
	int recv_real_len = recv_len;
	BYTE *pRecv = new BYTE[recv_len];
	/*
	while (recv_real_len >(recv_len / 13 * 12))
	{
	recv_real_len = recv_len;
	Recv_message(pRecv, recv_real_len);
	}
	*/
	//Recv_message(pRecv, recv_real_len);

	//��ѯ����
	CString longtoHex;

	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int n = 0; n < Dev_F_id.GetLength(); n++)
	{
		F_id[n] = Dev_F_id[n];
	}

	//����λ�ò�ѯָ��
	//Send_command("4B41600000000000", F_id, 0, 0);//״̬��ѯ
	//Send_command("4364600000000000", F_id, 0, 0);
	Send_command("4364600000000000", F_id, 0, 0);//  
	//Send_command("430A700000000000", F_id, 0, 0);//  

	//Send_command("437A600000000000", F_id, 0, 0);//  Target Position
	//���½Ӳ�ѯ��ǰ״̬

	BYTE *pRecv_real = new BYTE[50];
	int count_real = 0;
	int k = 1;
	BOOL target_reached = false;
	bool Get_mes = false;
	while ((!target_reached) && (k<30))//����֤���Ƿ�ʹ��I���жϣ�������������
	{
		if ((k % 3) == 0)
		{
			//����λ�ò�ѯָ��
			//Send_command("4B41600000000000", F_id, 0, 0);//״̬��ѯ
			//Send_command("4364600000000000", F_id, 0, 0);
			Send_command("4364600000000000", F_id, 0, 0);//  
		}
		recv_real_len = recv_len;
		memset(pRecv, 0, recv_len);
		Recv_message((char*)pRecv, recv_real_len, 50);
		//int real_Size = sizeof(pRecv) / sizeof(pRecv[0]);
		for (int i = 0; i < recv_len; i++)
		{
			char temp_p, temp_p1, temp_p2, temp_p3, temp_p4, temp_p5, temp_p6;
			if (pRecv[i] != 8)
			{
				char shtr = pRecv[i];
				i++;
				continue;
			}
			temp_p = pRecv[i + 3];
			temp_p1 = pRecv[i + 4];
			temp_p2 = pRecv[i + 5];
			temp_p3 = pRecv[i + 6];
			temp_p4 = pRecv[i + 7];
			temp_p5 = pRecv[i + 8];
			temp_p6 = pRecv[i + 9];

			char jj2 = pRecv[i];
			bool ID_bool = ((((char)pRecv[i + 3] & '\x0f') == '\x05')) && ((((char)pRecv[i + 4] & '\x0f') == Dev_id));

			//if (((char)pRecv[i] == '\xff'))
			//	i += 10;

			if (Get_mes || ID_bool)
			{
				if ((i + 12 > recv_real_len))
					return false;
				while (count_real<13)
				{
					pRecv_real[count_real] = pRecv[i];
					count_real++;
					i++;
				}
				Get_mes = true;
				if (count_real>12)
				{
					bool call_bool = ((char)pRecv_real[7] == '\x60') && ((char)pRecv_real[6] == '\x64');

					//�жϲ�ѯ���ı��ģ���ȡλ�ã�
					if (call_bool)       //���ö�ȡλ�õ�
					{
						CString Pos_hex;
						unsigned long Pos_int;
						//Pos_int = (long)pRecv[j * 12 + 8] + ((long)pRecv[j * 12 + 9] << 8) + ((long)pRecv[j * 12 + 10] << 16) + ((long)pRecv[j * 12 + 11] << 24);
						CString Pos_H_1, Pos_H_2, Pos_L_1, Pos_L_2;
						//char jkl1 = pRecv_real[7];
						//int tt = (int)pRecv_real[7];
						Pos_H_2.Format(_T("%02x"), pRecv_real[12]);
						Pos_H_1.Format(_T("%02x"), pRecv_real[11]);
						Pos_L_2.Format(_T("%02x"), pRecv_real[10]);
						Pos_L_1.Format(_T("%02x"), pRecv_real[9]);
						//Pos_hex = pRecv[j * 12 + 11] + pRecv[j * 12 + 10] + pRecv[j * 12 + 9] + pRecv[j * 12 + 8];
						Pos_hex = Pos_H_2 + Pos_H_1 + Pos_L_2 + Pos_L_1;
						//��Cstringת��Ϊstring��
						wstring ws(Pos_hex);
						string s_Pos_hex;
						s_Pos_hex.assign(ws.begin(), ws.end());
						stringstream Pos_stream;
						Pos_stream << std::hex << s_Pos_hex;
						Pos_stream >> Pos_int;
						//cout << Pos_int<<endl;
						//cout << static_cast<long>(Pos_int)<<endl;
						Real_Pos = static_cast<long>(Pos_int);
						target_reached = true;
						i = recv_len;
					}

					Get_mes = false;
					count_real = 0;
					memset(pRecv_real, 0, 50);
				}
				i += 12;
			}

		}

		k++;
	}
	delete pRecv;
	delete pRecv_real;
	if (target_reached)
	{
		//Real_Pos = (double)Real_Pos;//���������ֵ�Ǳ�����������������ֻ���28�������������4096��
		//һȦ��4096����Ч��һȦ��������20000��������Ĭ�ϣ�

		return true;
	}
	else
	{
		Real_Pos = 0;

		return false;
	}


	return 0;
}
bool Control_Dev::Dev_PUSIRobot_Read_Pos_Theory(UINT Dev_id, long &Real_Pos, int recv_len)
{

	//��ջ���
	int recv_real_len = recv_len;
	BYTE *pRecv = new BYTE[recv_len];
	//��ѯ����
	CString longtoHex;

	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int n = 0; n < Dev_F_id.GetLength(); n++)
	{
		F_id[n] = Dev_F_id[n];
	}

	//����λ�ò�ѯָ��
	Send_command("400C600000000000", F_id, 0, 0);//  

	BYTE *pRecv_real = new BYTE[50];
	int count_real = 0;
	int k = 1;
	BOOL target_reached = false;
	bool Get_mes = false;
	while ((!target_reached) && (k<30))//����֤���Ƿ�ʹ��I���жϣ�������������
	{
		if ((k % 3) == 0)
			Send_command("400C600000000000", F_id, 0, 0);//   
		recv_real_len = recv_len;
		memset(pRecv, 0, recv_len);
		Recv_message((char*)pRecv, recv_real_len, 50);
		for (int i = 0; i < recv_len; i++)
		{
			char temp_p, temp_p1, temp_p2, temp_p3, temp_p4, temp_p5, temp_p6;
			if (pRecv[i] != 8)
			{
				char shtr = pRecv[i];
				i++;
				continue;
			}
			temp_p = pRecv[i + 3];
			temp_p1 = pRecv[i + 4];
			temp_p2 = pRecv[i + 5];
			temp_p3 = pRecv[i + 6];
			temp_p4 = pRecv[i + 7];
			temp_p5 = pRecv[i + 8];
			temp_p6 = pRecv[i + 9];

			char jj2 = pRecv[i];
			bool ID_bool = ((((char)pRecv[i + 3] & '\x0f') == '\x05')) && ((((char)pRecv[i + 4] & '\x0f') == Dev_id));

			//if (((char)pRecv[i] == '\xff'))
			//	i += 10;

			if (Get_mes || ID_bool)
			{
				if ((i + 12 > recv_real_len))
					return false;
				while (count_real<13)
				{
					pRecv_real[count_real] = pRecv[i];
					count_real++;
					i++;
				}
				Get_mes = true;
				if (count_real>12)
				{
					bool call_bool = ((char)pRecv_real[7] == '\x60') && ((char)pRecv_real[6] == '\x0C');

					//�жϲ�ѯ���ı��ģ���ȡλ�ã�
					if (call_bool)       //���ö�ȡλ�õ�
					{
						CString Pos_hex;
						unsigned long Pos_int;
						//Pos_int = (long)pRecv[j * 12 + 8] + ((long)pRecv[j * 12 + 9] << 8) + ((long)pRecv[j * 12 + 10] << 16) + ((long)pRecv[j * 12 + 11] << 24);
						CString Pos_H_1, Pos_H_2, Pos_L_1, Pos_L_2;
						//char jkl1 = pRecv_real[7];
						//int tt = (int)pRecv_real[7];
						Pos_H_2.Format(_T("%02x"), pRecv_real[12]);
						Pos_H_1.Format(_T("%02x"), pRecv_real[11]);
						Pos_L_2.Format(_T("%02x"), pRecv_real[10]);
						Pos_L_1.Format(_T("%02x"), pRecv_real[9]);
						//Pos_hex = pRecv[j * 12 + 11] + pRecv[j * 12 + 10] + pRecv[j * 12 + 9] + pRecv[j * 12 + 8];
						Pos_hex = Pos_H_2 + Pos_H_1 + Pos_L_2 + Pos_L_1;
						//��Cstringת��Ϊstring��
						wstring ws(Pos_hex);
						string s_Pos_hex;
						s_Pos_hex.assign(ws.begin(), ws.end());
						stringstream Pos_stream;
						Pos_stream << std::hex << s_Pos_hex;
						Pos_stream >> Pos_int;
						//cout << Pos_int<<endl;
						//cout << static_cast<long>(Pos_int)<<endl;
						Real_Pos = static_cast<long>(Pos_int);
						target_reached = true;
						i = recv_len;
					}

					Get_mes = false;
					count_real = 0;
					memset(pRecv_real, 0, 50);
				}

			}

		}

		k++;
	}
	delete pRecv;
	delete pRecv_real;
	if (target_reached)
	{
		return true;
	}
	else
	{
		Real_Pos = 0;

		return false;
	}


	return 0;
}
bool Control_Dev::Dev_Read_Analog_Input1(UINT Dev_id, long &Analog_Input, int recv_len)
{
	//��ջ���
	int recv_real_len = recv_len;
	BYTE *pRecv = new BYTE[recv_len];

	//while (recv_real_len >(recv_len / 13 * 12))
	//{
	//recv_real_len = recv_len;
	//Recv_message((char*)pRecv, recv_real_len);
	//}

	//Recv_message((char*)pRecv, recv_real_len);

	//��ѯ����
	CString longtoHex;

	longtoHex.Format(_T("%02x"), Dev_id);
	CString Dev_F_id = "000006" + longtoHex;
	char F_id[9] = "0";
	for (int n = 0; n < Dev_F_id.GetLength(); n++)
	{
		F_id[n] = Dev_F_id[n];
	}

	//����λ�ò�ѯָ��
	//Send_command("4B41600000000000", F_id, 0, 0);//״̬��ѯ
	//Send_command("4364600000000000", F_id, 0, 0);
	Send_command("430E700000000000", F_id, 0, 0);//  
	//Send_command("430A700000000000", F_id, 0, 0);//  

	//Send_command("437A600000000000", F_id, 0, 0);//  Target Position
	//���½Ӳ�ѯ��ǰ״̬

	BYTE *pRecv_real = new BYTE[50];
	int count_real = 0;
	int k = 1;
	BOOL target_reached = false;
	bool Get_mes = false;
	while ((!target_reached) && (k<20))//����֤���Ƿ�ʹ��I���жϣ�������������
	{
		if ((k % 10) == 0)
		{
			//����λ�ò�ѯָ��
			//Send_command("4B41600000000000", F_id, 0, 0);//״̬��ѯ
			//Send_command("4364600000000000", F_id, 0, 0);
			Send_command("430E700000000000", F_id, 0, 0);//  
		}
		recv_real_len = recv_len;
		memset(pRecv, 0, recv_len);
		Recv_message((char*)pRecv, recv_real_len, 50);
		//int real_Size = sizeof(pRecv) / sizeof(pRecv[0]);
		for (int i = 0; i < recv_len; i++)
		{
			char temp_p, temp_p1, temp_p2, temp_p3, temp_p4, temp_p5, temp_p6;
			if (pRecv[i] != 8)
			{
				char shtr = pRecv[i];
				i++;
				continue;
			}
			temp_p = pRecv[i + 3];
			temp_p1 = pRecv[i + 4];
			temp_p2 = pRecv[i + 5];
			temp_p3 = pRecv[i + 6];
			temp_p4 = pRecv[i + 7];
			temp_p5 = pRecv[i + 8];
			temp_p6 = pRecv[i + 9];

			char jj2 = pRecv[i];
			bool ID_bool = ((((char)pRecv[i + 3] & '\x0f') == '\x05')) && ((((char)pRecv[i + 4] & '\x0f') == Dev_id));

			//if (((char)pRecv[i] == '\xff'))
			//	i += 10;

			if (Get_mes || ID_bool)
			{
				if ((i + 12 > recv_real_len))
					return false;
				while (count_real<13)
				{
					pRecv_real[count_real] = pRecv[i];
					count_real++;
					i++;
				}
				Get_mes = true;
				if (count_real>12)
				{
					bool call_bool = ((char)pRecv_real[7] == '\x70') && ((char)pRecv_real[6] == '\x0E');

					//�жϲ�ѯ���ı��ģ���ȡλ�ã�
					if (call_bool)       //���ö�ȡλ�õ�
					{
						CString Pos_hex;
						unsigned long Pos_int;
						//Pos_int = (long)pRecv[j * 12 + 8] + ((long)pRecv[j * 12 + 9] << 8) + ((long)pRecv[j * 12 + 10] << 16) + ((long)pRecv[j * 12 + 11] << 24);
						CString Pos_H_1, Pos_H_2, Pos_L_1, Pos_L_2;
						char jkl1 = pRecv_real[7];
						int tt = (int)pRecv_real[7];
						Pos_H_2.Format(_T("%02x"), pRecv_real[12]);
						Pos_H_1.Format(_T("%02x"), pRecv_real[11]);
						Pos_L_2.Format(_T("%02x"), pRecv_real[10]);
						Pos_L_1.Format(_T("%02x"), pRecv_real[9]);
						//Pos_hex = pRecv[j * 12 + 11] + pRecv[j * 12 + 10] + pRecv[j * 12 + 9] + pRecv[j * 12 + 8];
						Pos_hex = Pos_H_2 + Pos_H_1 + Pos_L_2 + Pos_L_1;
						//��Cstringת��Ϊstring��
						wstring ws(Pos_hex);
						string s_Pos_hex;
						s_Pos_hex.assign(ws.begin(), ws.end());
						stringstream Pos_stream;
						Pos_stream << std::hex << s_Pos_hex;
						Pos_stream >> Pos_int;
						//cout << Pos_int<<endl;
						//cout << static_cast<long>(Pos_int)<<endl;
						Analog_Input = static_cast<long>(Pos_int);
						target_reached = true;
						i = recv_len;
					}

					Get_mes = false;
					count_real = 0;
					memset(pRecv_real, 0, 50);
				}

			}

		}

		k++;
	}
	delete pRecv;
	delete pRecv_real;
	if (target_reached)
	{
		//Real_Pos = (double)Real_Pos;//���������ֵ�Ǳ�����������������ֻ���28�������������4096��
		//һȦ��4096����Ч��һȦ��������20000��������Ĭ�ϣ�

		return true;
	}
	else
	{
		Analog_Input = 0;

		return false;
	}


	return 0;

}

/***************************************
�����˶���
Dev_Home(UINT *Dev_id,bool Star_or_Stop);
UINT *Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID��
bool Star_or_Stop :������ֹͣ���㣬Ϊ1��ʾ�������㣬Ϊ0��ʾֹͣ����*/
int Control_Dev::Dev_Home(vector<UINT> Dev_id, vector<long> Home_offset, bool Star_or_Stop,bool fast_slow)
{
	int id_len = Dev_id.size();
	CString longtoHex;
	if (Star_or_Stop)
	{
		//��ѯ����
		CString longtoHex_offset;
		CString Dev_F_id = "0" ;
		char F_id[9] = "0";
		CString Dev_offset = "0";
		char F_offset[17] = "0";
		for (int i = 0; i < id_len; i++)
		{

			longtoHex.Format(_T("%02x"), Dev_id[i]);
			Dev_F_id = "000006" + longtoHex;
			for (int j = 0; j < Dev_F_id.GetLength(); j++)
			{
				F_id[j] = Dev_F_id[j];
			}

			long_to_char("237C6000", F_offset, Home_offset[i]);

			/*
			longtoHex_offset.Format(_T("%08x"), Home_offset[i]);
			Dev_offset = "237C6000"+longtoHex_offset.Right(2) + longtoHex_offset.Mid(4, 2)
						+ longtoHex_offset.Mid(2, 2) + longtoHex_offset.Left(2);
			for (int k = 0; k < Dev_offset.GetLength(); k++)
			{
				F_offset[k] = Dev_offset[k];
			}
			*/
			//����״̬�ֲ�ѯָ��

			Send_command("2B40600006000000", F_id, 0, 0);//�������ÿ�����/  ʹ��
			Send_command("2B40600007000000", F_id, 0, 0);//����Switch on/  ʹ��
			Send_command("2B4060000F010000", F_id, 0, 0);//���ÿ�����10F���������/  ʹ��

			Send_command("2F60600006000000", F_id, 0, 0);//���ÿ���ģʽΪ������ƣ�6060/0 ��Ϊ06��
			//Sleep(30);
			switch (Dev_id[i])
			{
			case 1:
				Send_command("2F98600001000000", F_id, 0, 0);//���û���ģʽ��������÷�ʽ1��ʹ�õ���X3��
				Send_command("239A600058020000", F_id, 0, 0);//���ü��ٶ�100r/min
				//Send_command("239A60002C010000", F_id, 0, 0);//���ü��ٶ�50r/min
				//Sleep(30);
				//Send_command("23996001F0000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ1rps
				Send_command("23996001B0040000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ5rps
				//Sleep(30);
				//Send_command("2399600278000000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ0.5rps
				Send_command("2399600258020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
				//Sleep(30);
				break;
			case 2:
				Send_command("2F98600002000000", F_id, 0, 0);//���û���ģʽ��������÷�ʽ2;ʹ�õ���X3��
				//Sleep(30);
				Send_command("239A600058020000", F_id, 0, 0);//���ü��ٶ�100r/min
				//Send_command("239A60002C010000", F_id, 0, 0);//���ü��ٶ�50r/min
				//Sleep(30);
				//Send_command("23996001F0000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ1rps
				//Send_command("23996001B0040000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ5rps
				Send_command("2399600160090000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ10rps
				//Sleep(30);
				//Send_command("2399600278000000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ0.5rps
				Send_command("2399600258020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
				//Send_command("23996002F0000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ1rps
				//Sleep(30);
				break;
			case 3:
				Send_command("2F01700004000000", F_id, 0, 0);//������㴫������X3
				Sleep(10);
				Send_command("2F98600004000000", F_id, 0, 0);//���û���ģʽ��������÷�ʽ3
				//Sleep(30);
				Send_command("239A60002C010000", F_id, 0, 0);//���ü��ٶ�50r/min

				if (fast_slow)
				{
					//Send_command("2399600158020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
					////Sleep(30);
					//Send_command("2399600278000000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ0.5rps
					Send_command("2399600160090000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ10rps
					Send_command("2399600258020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
				}
				else
				{
					Send_command("2399600178000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ0.5rps
					Send_command("2399600248000000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ0.3rps
					
				}
				//Send_command("239A60002C010000", F_id, 0, 0);//���ü��ٶ�50r/min
				//Sleep(30);
				//Send_command("23996001F0000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ1rps
				//Send_command("23996001B0040000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ5rps
				//Send_command("2399600160090000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ10rps
				//Send_command("2399600158020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
				//Sleep(30);
				//Send_command("2399600278000000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ0.5rps
				//Send_command("2399600258020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
				//Send_command("23996002F0000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ1rps
				break;

			case 4:
				Send_command("2F01700001000000", F_id, 0, 0);//������㴫������X1
				Sleep(10);
				Send_command("2F98600013000000", F_id, 0, 0);//���û���ģʽ��������÷�ʽ19
				//Sleep(30);
				//Send_command("239A600058020000", F_id, 0, 0);//���ü��ٶ�100r/min
				Send_command("239A60002C010000", F_id, 0, 0);//���ü��ٶ�50r/min
				//Sleep(30);
				Send_command("23996001F0000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ1rps
				//Send_command("23996001B0040000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ5rps
				//Send_command("2399600160090000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ10rps
				//Send_command("2399600158020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
				//Sleep(30);
				Send_command("2399600278000000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ0.5rps
				//Send_command("2399600258020000", F_id, 0, 0);//���û����ٶȣ�search for zero��Ϊ2.5rps
				//Send_command("23996002F0000000", F_id, 0, 0);//���û����ٶȣ�search for switch��Ϊ1rps
				break;
			default:
				break;

			}

			//Send_command("237C600080841E00", F_id, 0, 0);
			Send_command(F_offset, F_id, 0, 0);			 //���û���ƫ������������
			//Sleep(30);
			Send_command("2B4060001F000000", F_id, 0, 0);//��ʼ����
			//Sleep(30);
		}
	}
	else
	{
		//��ѯ����
		int id_len = sizeof(Dev_id);
		for (int i = 0; i < id_len; i++)
		{

			longtoHex.Format(_T("%02x"), Dev_id[i]);
			CString Dev_F_id = "000006" + longtoHex;
			char F_id[9] = "0";
			for (int i = 0; i < Dev_F_id.GetLength(); i++)
			{
				F_id[i] = Dev_F_id[i];
			}

			//����״̬�ֲ�ѯָ��
			Send_command("2B4060001F010000", F_id, 0, 0);//��������
			//Sleep(20);
		}


	}
		return 0;
}

/***************************************
�̶��������ã�
Dev_Para_set(vector<UINT> Dev_id);
vector<UINT> Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID������
*/
int Control_Dev::Dev_Para_set(vector<UINT> Dev_id)
{
	int id_len = Dev_id.size();
	CString longtoHex;
	CString Dev_F_id = "0";
	char F_id[9] = "0";
		for (int i = 0; i < id_len; i++)
		{

			longtoHex.Format(_T("%02x"), Dev_id[i]);
			Dev_F_id = "000006" + longtoHex;
			for (int i = 0; i < Dev_F_id.GetLength(); i++)
			{
				F_id[i] = Dev_F_id[i];
			}

			Send_command("2B17100000000000", F_id, 0, 0);//�ر�����
			//Sleep(20);


		}
		return 0;
}

/***************************************
��ѯ�˶�ָ���Ƿ���ȷ�·����鿴���������ر������ͣ�
bool Dev_Read_Respond_Message(UINT Dev_id);
UINT Dev_id�� ��Ҫ��ѯ�����Ӧ�ĵ����ID��
*/
bool Control_Dev::Dev_Read_Respond_Message(UINT Dev_id,int recv_len)
{
	//��ջ���

	int recv_real_len = recv_len;
	char *pRecv = new char[recv_len];
		//���½Ӳ�ѯ��ǰ״̬
		memset(pRecv, 0, recv_len);
		bool Respond_reached = false;
		bool Status_Ref = false;
		recv_real_len = recv_len;
		Recv_message(pRecv, recv_real_len);
		int j = 0;
		while ((!Respond_reached) & (j < recv_real_len))//����֤���Ƿ�ʹ��K���жϣ�������������
		{

					//if ((bool)(((pRecv[j * 12 + 3] & '\x0f') == Dev_id[i]) && (bool)(pRecv[j * 12 + 5] == '\x40')) || (bool)(pRecv[j * 12 + 5] == '\x7c'))
					if ((bool)((pRecv[j] & '\x0f') == Dev_id) && ((j + 2) < recv_real_len))
					{
						Status_Ref = true;
					}
					//�ж����˶��Ƿ�λ��״̬�Ĵ����ĵ�10λ��(״̬�� 0 �仯��1),�ж�״̬�Ƿ�Ϊ1��
					if (Status_Ref && (bool)(pRecv[j + 2] == '\x40'))
					{

						Respond_reached = true;
						delete pRecv;
						return true;
					}

					j++;

		}
		delete pRecv;
		return true;
}

/***************************************
Dev_Set_Status:����ĳ������ĸ�λ���״̬
int Dev_ID�������ID
bool Status����ǰ�ĵ����λ���״̬��ΪTRUEʱ��ʾ��λ��ɣ�
*/
void Control_Dev::Dev_Set_Status(int Dev_ID, bool Status)
{
	ResetCompleteStatus[Dev_ID] = Status;
}

/***************************************
�ر�ͨ��
 */
void Control_Dev::closeSocket()
{
	closesocket(m_sockClient);
	WSACleanup();
}

Control_Dev::~Control_Dev()
{

}
