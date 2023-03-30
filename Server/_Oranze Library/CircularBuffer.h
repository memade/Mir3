/*
	C++ Exception Processing Class

	Date:
		2008/12/11
*/
#ifndef __ORZ_CIRCULARBUFFER__
#define __ORZ_CIRCULARBUFFER__

#include <windows.h>


class CCircularBuffer  
{
protected:
	int		m_iBufSize;
	char	*m_pBuffer;

	int		m_iHeadPos;
	int		m_iTailPos;


public:
	CCircularBuffer(int size=16384);
	virtual ~CCircularBuffer();

	void	PutData(char *pData, int len);
	void	GetData(char *pData, int len);
	int		GetOutData(char *pData); //HeadPos, ��ȭ
	void	PutData(char& data);
	char&	GetHeadData(){return m_pBuffer[m_iHeadPos];}
	//1 Byte Operation;
	//false : ��絥���� �ٺ���, TRUE: ���������� ������
	BOOL	HeadIncrease(int increasement=1);
	void	SetEmpty() {m_iHeadPos=0; m_iTailPos=0;}

	int&	GetBufferSize() {return m_iBufSize;}
	int&	GetHeadPos() {return m_iHeadPos;}
	int&	GetTailPos() {return m_iTailPos;}
	int		GetValidCount();
protected:
	//over flow ���� ������ �� IndexOverFlow ����
	BOOL	IsOverFlowCondition(int &len) {return (len >= m_iBufSize-GetValidCount()) ? TRUE: FALSE;}
	BOOL	IsIndexOverFlow(int &len) {return (len+m_iTailPos>=m_iBufSize) ? TRUE:FALSE;}
	void	BufferResize(); //overflow condition �϶� size�� ������ �ι�� �ø�
};

inline int CCircularBuffer::GetValidCount()
{
	int count = m_iTailPos-m_iHeadPos;
	if (count<0) count = m_iBufSize+count;
	return count;
}

inline void CCircularBuffer::BufferResize()
{
	int prevBufSize = m_iBufSize;
	m_iBufSize <<= 1;
	char *pNewData = new char[m_iBufSize];
	CopyMemory(pNewData, m_pBuffer, prevBufSize);
	if (m_iTailPos<m_iHeadPos) 
	{
		CopyMemory(pNewData+prevBufSize, m_pBuffer, m_iTailPos);
		m_iTailPos += prevBufSize;
	}
	delete [] m_pBuffer;
	m_pBuffer = pNewData;
}

inline void CCircularBuffer::PutData(char &data)
{
	int len = 1;
	while (IsOverFlowCondition(len)) BufferResize();
	m_pBuffer[m_iTailPos++] = data;
	if (m_iTailPos==m_iBufSize) m_iTailPos = 0;
}

inline void CCircularBuffer::PutData(char *pData, int len)
{
	if (len<=0) 
	{
		OutputDebugString("CCircularBuffer::PutData len is <=0\n"); 
		return;
	}
	while (IsOverFlowCondition(len)) BufferResize();
	if (IsIndexOverFlow(len))
	{
		int FirstCopyLen = m_iBufSize-m_iTailPos;
		int SecondCopyLen = len - FirstCopyLen;
//		ASSERT(FirstCopyLen);
		CopyMemory(m_pBuffer+m_iTailPos, pData, FirstCopyLen);
		if (SecondCopyLen)
		{
			CopyMemory(m_pBuffer, pData+FirstCopyLen, SecondCopyLen);
			m_iTailPos = SecondCopyLen;
		}
		else m_iTailPos = 0;
	}
	else
	{
		CopyMemory(m_pBuffer+m_iTailPos, pData, len);
		m_iTailPos += len;
	}
}

inline int CCircularBuffer::GetOutData(char *pData)
{
	int len = GetValidCount();
	int fc, sc;
	fc = m_iBufSize-m_iHeadPos;
	if (len>fc)
	{
		sc = len - fc;
		CopyMemory(pData, m_pBuffer+m_iHeadPos, fc);
		CopyMemory(pData+fc, m_pBuffer, sc);
		m_iHeadPos = sc;
//		ASSERT(m_iHeadPos==m_iTailPos);
	}
	else
	{
		CopyMemory(pData, m_pBuffer+m_iHeadPos, len);
		m_iHeadPos += len;
		if (m_iHeadPos==m_iBufSize) m_iHeadPos = 0;
	}
	return len;
}

inline void CCircularBuffer::GetData(char *pData, int len)
{
//	ASSERT(len>0&&len<=GetValidCount());
	if (len < m_iBufSize-m_iHeadPos)
		CopyMemory(pData, m_pBuffer+m_iHeadPos, len);
	else
	{
		int fc, sc;
		fc = m_iBufSize-m_iHeadPos;
		sc = len - fc;
		CopyMemory(pData, m_pBuffer+m_iHeadPos, fc);
		if (sc) CopyMemory(pData+fc, m_pBuffer, sc);
	}
}

inline BOOL CCircularBuffer::HeadIncrease(int increasement)
{
//	ASSERT(increasement<=GetValidCount());
	m_iHeadPos += increasement;
	m_iHeadPos %= m_iBufSize;
	return m_iHeadPos!=m_iTailPos;
}

#endif
