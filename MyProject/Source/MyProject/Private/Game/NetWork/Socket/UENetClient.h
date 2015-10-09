#ifndef __UENETCLIENT_H
#define __UENETCLIENT_H

#include "MyProject.h"
//#include "IPv4Address.h"		// FIPv4Address
//#include "IPv4Endpoint.h"		// ֻ���� IPv4Endpoint.h ����벻�� // FIPv4Endpoint
#include "Networking.h"			// ����һ����ȫ��������ֹֻ�������ֵ��±��벻��
#include <string>

class FSocket;
class ClientBuffer;
class MEvent;
class MMutex;

class UENetClient
{
protected:
	FString	m_strDesc;
	FIPv4Endpoint m_boundEndpoint;

	FSocket* m_socket;
	ClientBuffer* m_clientBuffer;

	bool m_brecvThreadStart;      // �����߳��Ƿ�����
	bool m_isConnected;

	MEvent* m_msgSendEndEvent;
	MMutex* m_sendMutex;

protected:
	void testSendData();

public:
	std::string m_ip;
	int m_port;

	UENetClient();
	~UENetClient();

	bool getRecvThreadStart();
	void setRecvThreadStart(bool value);

	bool getIsConnected();
	void setIsConnected(bool value);

	ClientBuffer* getClientBuffer();
	void sendMsg();

	bool connect(FString ip = "127.0.0.1", uint32 port = 5000);	// ���ӷ�����
	void Send();
	void Receive();
	bool checkAndUpdateConnect();
	void Disconnect();
};

#endif