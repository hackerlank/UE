#include "MyProject.h"
#include "ClientBuffer.h"
#include "MsgBuffer.h"
#include "DynBuffer.h"
#include "ByteBuffer.h"
#include "MCircularBuffer.h"
#include "MMutex.h"
#include "MLock.h"
#include "Common.h"
#include "UtilStr.h"
#include "UtilMsg.h"

ClientBuffer::ClientBuffer()
{
	m_rawBuffer = new MsgBuffer();
	m_msgBuffer = new MsgBuffer();
	//m_sendTmpBA = new ByteBuffer();
	m_sendTmpBuffer = new MsgBuffer();
	m_socketSendBA = new ByteBuffer();
	//m_socketSendBA.m_id = 1000;

	//m_dynBuff = new DynamicBuffer<byte>(8096);
	m_unCompressHeaderBA = new ByteBuffer();
	m_sendData = new ByteBuffer();
	m_tmpData = new ByteBuffer(4);
	m_tmp1fData = new ByteBuffer(4);

	m_readMutex = new MMutex();
	m_writeMutex = new MMutex();

#if MSG_ENCRIPT
	m_cryptContext = new CryptContext();
#endif
}

ClientBuffer::~ClientBuffer()
{
	delete m_rawBuffer;
	delete m_msgBuffer;
	delete m_sendTmpBuffer;

	delete m_socketSendBA;
	delete m_unCompressHeaderBA;
	delete m_sendData;
	delete m_tmpData;

	delete m_readMutex;
	delete m_writeMutex;

#if MSG_ENCRIPT
	delete m_cryptContext;
#endif
}

DynBuffer<char>* ClientBuffer::getDynBuff()
{
	return m_dynBuff;
}


MsgBuffer* ClientBuffer::getSendTmpBuffer()
{
	return m_sendTmpBuffer;
}

ByteBuffer* ClientBuffer::getSendBuffer()
{
	return m_socketSendBA;
}

ByteBuffer* ClientBuffer::getSendData()
{
	return m_sendData;
}

#if MSG_ENCRIPT
public void ClientBuffer::setCryptKey(byte[] encrypt)
{
	//m_cryptContext.cryptAlgorithm = CryptAlgorithm.DES;
	m_cryptContext.m_cryptKey = encrypt;
	Dec.DES_set_key_unchecked(m_cryptContext.m_cryptKey, m_cryptContext.m_cryptKeyArr[(int)CryptAlgorithm.DES] as DES_key_schedule);
}

public void ClientBuffer::checkDES()
{
	if (m_cryptContext.m_cryptKey != null && m_cryptContext.m_cryptAlgorithm != CryptAlgorithm.DES)
	{
		m_cryptContext.m_cryptAlgorithm = CryptAlgorithm.DES;
	}
}
#endif

MsgBuffer* ClientBuffer::getRawBuffer()
{
	return m_rawBuffer;
}

void ClientBuffer::SetRevBufferSize(int32 size)
{
	m_dynBuff = new DynBuffer<char>(size);
}

void ClientBuffer::moveDyn2Raw()
{
	g_pLogSys->log(UtilStr::Format("移动动态数据消息数据到原始数据队列，消息长度　{0}", m_dynBuff->getSize()));
	UtilMsg::formatBytes2Array(m_dynBuff->getBuff(), m_dynBuff->getSize());

#if MSG_ENCRIPT
	checkDES();
#endif
	// 接收到一个socket数据，就被认为是一个数据包，这个地方可能会有问题，服务器是这么发送的，只能这么处理，自己写入包的长度
	m_tmp1fData->clear();
	m_tmp1fData->writeUnsignedInt32(m_dynBuff->getSize());      // 填充长度
	m_rawBuffer->getCircularBuffer()->pushBackBA(m_tmp1fData);
	// 写入包的数据
	m_rawBuffer->getCircularBuffer()->pushBackArr(m_dynBuff->getBuff(), 0, m_dynBuff->getSize());
}

void ClientBuffer::moveRaw2Msg()
{
	while (m_rawBuffer->popFront())  // 如果有数据
	{
		//UnCompressAndDecryptAllInOne();
		UnCompressAndDecryptEveryOne();
	}
}

void ClientBuffer::send(bool bnet)
{
	m_tmpData->clear();
	m_tmpData->writeUnsignedInt32(m_sendData->getLength());      // 填充长度

	if (bnet)       // 从 socket 发送出去
	{
#if NET_MULTHREAD
		using (MLock mlock = new MLock(m_writeMutex))
#endif
		{
			//m_sendTmpBA.writeUnsignedInt(m_sendData.length);                            // 写入头部长度
			//m_sendTmpBA.writeBytes(m_sendData.dynBuff.buff, 0, m_sendData.length);      // 写入内容

			m_sendTmpBuffer->getCircularBuffer()->pushBackBA(m_tmpData);
			m_sendTmpBuffer->getCircularBuffer()->pushBackBA(m_sendData);
		}
	}
	else        // 直接放入接收消息缓冲区
	{
#if NET_MULTHREAD

#endif
		{
			//m_tmpData.clear();
			//m_tmpData.writeUnsignedInt(m_sendData.length);      // 填充长度

			m_msgBuffer->getCircularBuffer()->pushBackBA(m_tmpData);              // 保存消息大小字段
			m_msgBuffer->getCircularBuffer()->pushBackBA(m_sendData);             // 保存消息大小字段
		}
	}
}

ByteBuffer* ClientBuffer::getMsg()
{
	MLock mlock(m_readMutex);
	{
		if (m_msgBuffer->popFront())
		{
			return m_msgBuffer->getMsgBodyBA();
		}
	}

	return nullptr;
}

// 获取数据，然后压缩加密
void ClientBuffer::getSocketSendData()
{
	//m_socketSendBA.m_startTest = false;

	m_socketSendBA->clear();

	// 获取完数据，就解锁
#if NET_MULTHREAD
	using (MLock mlock = new MLock(m_writeMutex))
#endif
	{
		//m_socketSendBA.writeBytes(m_sendTmpBA.dynBuff.buff, 0, (uint)m_sendTmpBA.length);
		//m_sendTmpBA.clear();
		// 一次全部取出来发送出去
		//m_socketSendBA.writeBytes(m_sendTmpBuffer.circuleBuffer.buff, 0, (uint)m_sendTmpBuffer.circuleBuffer.size);
		//m_sendTmpBuffer.circuleBuffer.clear();
		// 一次仅仅获取一个消息发送出去，因为每一个消息的长度要填写加密补位后的长度
		if (m_sendTmpBuffer->popFront())     // 弹出一个消息，如果只有一个消息，内部会重置变量
		{
			m_socketSendBA->writeBytes(m_sendTmpBuffer->getHeaderBA()->getDynBuff()->getBuff(), 0, m_sendTmpBuffer->getHeaderBA()->getLength());       // 写入头
			m_socketSendBA->writeBytes(m_sendTmpBuffer->getMsgBodyBA()->getDynBuff()->getBuff(), 0, m_sendTmpBuffer->getMsgBodyBA()->getLength());             // 写入消息体
		}
	}

#if MSG_COMPRESS || MSG_ENCRIPT
	m_socketSendBA.setPos(0);
	CompressAndEncryptEveryOne();
	// CompressAndEncryptAllInOne();
#endif
	m_socketSendBA->setPos(0);        // 设置指针 pos

	//m_socketSendBA.m_startTest = true;
}

// 压缩加密每一个包
void ClientBuffer::CompressAndEncryptEveryOne()
{
	uint32 origMsgLen = 0;    // 原始的消息长度，后面判断头部是否添加压缩标志
	uint32 compressMsgLen = 0;
#if MSG_ENCRIPT
	uint cryptLen = 0;
#endif
#if MSG_COMPRESS && !MSG_ENCRIPT
	bool bHeaderChange = false; // 消息内容最前面的四个字节中消息的长度是否需要最后修正
#endif
	while (m_socketSendBA->getBytesAvailable() > 0)
	{
#if MSG_COMPRESS && !MSG_ENCRIPT
		bHeaderChange = false;
#endif

		m_socketSendBA->readUnsignedInt32(origMsgLen);    // 读取一个消息包头

#if MSG_COMPRESS
		if (origMsgLen > MsgCV::PACKET_ZIP_MIN)
		{
			compressMsgLen = m_socketSendBA->compress(origMsgLen);
		}
		else
		{
			m_socketSendBA->incPosDelta(origMsgLen);
			compressMsgLen = origMsgLen;
		}
#endif
		// 只加密消息 body
		//#if MSG_ENCRIPT
		//                m_socketSendBA.position -= compressMsgLen;      // 移动加密指针位置
		//                cryptLen = m_socketSendBA.encrypt(m_cryptKeyArr[(int)m_cryptAlgorithm], compressMsgLen, m_cryptAlgorithm);
		//                if (compressMsgLen != cryptLen)
		//                {
		//                    bHeaderChange = true;
		//                }
		//                compressMsgLen = cryptLen;
		//#endif

		// 加密如果系统补齐字节，长度可能会变成 8 字节的证书倍，因此需要等加密完成后再写入长度
#if MSG_COMPRESS && !MSG_ENCRIPT
		if (origMsgLen > DataCV::PACKET_ZIP_MIN)    // 如果原始长度需要压缩
		{
			bHeaderChange = true;
			origMsgLen = compressMsgLen;                // 压缩后的长度
			origMsgLen |= DataCV::PACKET_ZIP;            // 添加
		}
		//#endif
		//#if !MSG_ENCRIPT
		if (bHeaderChange)
		{
			m_socketSendBA->position -= (compressMsgLen + 4);        // 移动到头部位置
			m_socketSendBA->writeUnsignedInt32(origMsgLen, false);     // 写入压缩或者加密后的消息长度
			m_socketSendBA->incPosDelta(compressMsgLen);              // 移动到下一个位置
		}
#endif

		// 整个消息压缩后，包括 4 个字节头的长度，然后整个加密
#if MSG_ENCRIPT
		cryptLen = ((compressMsgLen + 4 + 7) / 8) * 8 - 4;      // 计算加密后，不包括 4 个头长度的 body 长度
		if (origMsgLen > MsgCV::PACKET_ZIP_MIN)    // 如果原始长度需要压缩
		{
			origMsgLen = cryptLen;                // 压缩后的长度
			origMsgLen |= MsgCV::PACKET_ZIP;            // 添加
		}
		else
		{
			origMsgLen = cryptLen;                // 压缩后的长度
		}

		m_socketSendBA->decPosDelta((compressMsgLen + 4));        // 移动到头部位置
		m_socketSendBA->writeUnsignedInt32(origMsgLen, false);     // 写入压缩或者加密后的消息长度

		m_socketSendBA.decPosDelta(4);      // 移动到头部
		m_socketSendBA.encrypt(m_cryptContext, 0);  // 加密
#endif
	}

	// 整个消息压缩后，包括 4 个字节头的长度，然后整个加密
	//#if MSG_ENCRIPT
	//m_socketSendBA.position = 0;      // 移动到头部
	//m_socketSendBA.encrypt(m_cryptKeyArr[(int)m_cryptAlgorithm], 0, m_cryptAlgorithm);
	//#endif
}

// 压缩解密作为一个包
void ClientBuffer::CompressAndEncryptAllInOne()
{
#if MSG_COMPRESS
	uint origMsgLen = m_socketSendBA.length;       // 原始的消息长度，后面判断头部是否添加压缩标志
	uint compressMsgLen = 0;
	if (origMsgLen > MsgCV.PACKET_ZIP_MIN)
	{
		compressMsgLen = m_socketSendBA.compress();
	}
#endif

#if MSG_ENCRIPT
	else
	{
		compressMsgLen = origMsgLen;
		m_socketSendBA.position += origMsgLen;
	}

	m_socketSendBA.position -= compressMsgLen;
	compressMsgLen = m_socketSendBA.encrypt(m_cryptContext, 0);
#endif

#if MSG_COMPRESS || MSG_ENCRIPT             // 如果压缩或者加密，需要再次添加压缩或者加密后的头长度
	if (origMsgLen > MsgCV.PACKET_ZIP_MIN)    // 如果原始长度需要压缩
	{
		origMsgLen = compressMsgLen;
		origMsgLen |= MsgCV.PACKET_ZIP;            // 添加
	}
	else
	{
		origMsgLen = compressMsgLen;
	}

	m_socketSendBA.position = 0;
	m_socketSendBA.insertUnsignedInt32(origMsgLen);            // 写入压缩或者加密后的消息长度
#endif
}

// 消息格式
// |------------- 加密的整个消息  -------------------------------------|
// |----4 Header----|-压缩的 body----|----4 Header----|-压缩的 body----|
// |                |                |                |                |
void ClientBuffer::UnCompressAndDecryptEveryOne()
{
#if MSG_ENCRIPT
	m_rawBuffer.msgBodyBA.decrypt(m_cryptContext, 0);
#endif
	//#if MSG_COMPRESS
	//m_rawBuffer.headerBA.setPos(0); // 这个头目前没有用，是客户端自己添加的，服务器发送一个包，就认为是一个完整的包
	//m_rawBuffer.msgBodyBA.setPos(0);
	//uint msglen = m_rawBuffer.headerBA.readUnsignedInt();
	//if ((msglen & DataCV.PACKET_ZIP) > 0)
	//{
	//    m_rawBuffer.msgBodyBA.uncompress();
	//}
	//#endif

	m_rawBuffer->getMsgBodyBA()->setPos(0);

	uint32 msglen = 0;
	while (m_rawBuffer->getMsgBodyBA()->getBytesAvailable() >= 4)
	{
		m_rawBuffer->getMsgBodyBA()->readUnsignedInt32(msglen);    // 读取一个消息包头
		if (msglen == 0)     // 如果是 0 ，就说明最后是由于加密补齐的数据
		{
			break;
		}
#if MSG_COMPRESS
		if ((msglen & MsgCV::PACKET_ZIP) > 0)
		{
			msglen &= (~MsgCV.PACKET_ZIP);         // 去掉压缩标志位
			Ctx.m_instance.m_logSys.log(string.Format("消息需要解压缩，消息未解压长度　{0}", msglen));
			msglen = m_rawBuffer->getMsgBodyBA->uncompress(msglen);
			Ctx.m_instance.m_logSys.log(string.Format("消息需要解压缩，消息解压后长度　{0}", msglen));
		}
		else
#endif
		{
			g_pLogSys->log(UtilStr::Format("消息不需要解压缩，消息原始长度　{0}", msglen));
			m_rawBuffer->getMsgBodyBA()->incPosDelta(msglen);
		}

		m_unCompressHeaderBA->clear();
		m_unCompressHeaderBA->writeUnsignedInt32(msglen);        // 写入解压后的消息的长度，不要写入 msglen ，如果压缩，再加密，解密后，再解压后的长度才是真正的长度
		m_unCompressHeaderBA->setPos(0);

#if NET_MULTHREAD
		using (MLock mlock = new MLock(m_readMutex))
#endif
		{
			m_msgBuffer->getCircularBuffer()->pushBackBA(m_unCompressHeaderBA);             // 保存消息大小字段
			m_msgBuffer->getCircularBuffer()->pushBackArr(m_rawBuffer->getMsgBodyBA()->getDynBuff()->getBuff(), m_rawBuffer->getMsgBodyBA()->getPos() - msglen, msglen);      // 保存消息大小字段
		}

		g_pLogSys->log(UtilStr::Format("解压解密后消息起始索引 {0}, 消息长度　{1}, 消息 position 位置 {2}, 消息 size {3}", m_rawBuffer->getMsgBodyBA()->getPos() - msglen, msglen, m_rawBuffer->getMsgBodyBA()->getPos(), m_rawBuffer->getMsgBodyBA()->getLength()));
		g_pNetDispList->addOneRevMsg();

		// Test 读取消息头
		// ByteBuffer buff = getMsg();
		// stNullUserCmd cmd = new stNullUserCmd();
		// cmd.derialize(buff);
		// Ctx.m_instance.m_logSys.log(string.Format("测试打印消息: byCmd = {0}, byParam = {1}", cmd.byCmd, cmd.byParam));
	}
}

void ClientBuffer::UnCompressAndDecryptAllInOne()
{
#if MSG_ENCRIPT
	m_rawBuffer.msgBodyBA.decrypt(m_cryptContext, 0);
#endif
#if MSG_COMPRESS
	m_rawBuffer.headerBA.setPos(0);
	uint msglen = 0;
	m_rawBuffer.headerBA.readUnsignedInt32(ref msglen);
	if ((msglen & MsgCV.PACKET_ZIP) > 0)
	{
		m_rawBuffer.msgBodyBA.uncompress();
	}
#endif

#if !MSG_COMPRESS && !MSG_ENCRIPT
	m_unCompressHeaderBA->clear();
	m_unCompressHeaderBA->writeUnsignedInt32(m_rawBuffer->getMsgBodyBA()->getLength());
	m_unCompressHeaderBA->setPos(0);
#endif
#if NET_MULTHREAD
	using (MLock mlock = new MLock(m_readMutex))
#endif
	{
#if !MSG_COMPRESS && !MSG_ENCRIPT
		m_msgBuffer->getCircularBuffer()->pushBackBA(m_unCompressHeaderBA);             // 保存消息大小字段
#endif
		m_msgBuffer->getCircularBuffer()->pushBackBA(m_rawBuffer->getMsgBodyBA());      // 保存消息大小字段
	}
}