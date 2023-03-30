

#ifndef __ORZ_MIR2_GAME_SERVER__
#define __ORZ_MIR2_GAME_SERVER__


#include <netiocp.h>
#include "dbtable.h"


struct sCertUser;
struct sGateUser;


class CGameServer : public CIocpObject
{
public:
	enum { CLASSID = IOCP_OBJECT_CLASSID_0 + 2 };

	sTblSvrIP	m_dbInfo;
	int			m_nCurUserCnt;
	int			m_nMaxUserCnt;
	int			m_nCntInvalidPacket;
	DWORD		m_nLastTick;
	DWORD		m_nCheckTime;

public:
	CGameServer( SOCKET sdClient, char *szIP );
	virtual ~CGameServer();

	//
	// LoginServer -> GameServer
	//
	bool SendCancelAdmissionCert( sCertUser *pUser );
	bool SendAccountExpire(sCertUser *pUser);
	bool SendPasswordSuccess( sGateUser *pUser );
	bool SendTotalUserCount(int nTotalUserCount);

	//
	// GameServer -> LoginServer
	//
	bool OnUserClosed( char *pBody );
	bool OnUserCount( char *pBody );
	bool OnGameTimeOfTimeUser( char *pBody );

	//
	// CIocpObject �����Լ� ����
	//
	void OnError( int nErrCode );
	void OnSend( int nTransferred );
	bool OnRecv( char *pPacket, int nPacketLen );
	bool OnExtractPacket( char *pPacket, int *pPacketLen );

	bool SaveServerUserCount();

};


#endif