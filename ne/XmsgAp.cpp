/*
  Copyright 2019 www.dev5.cn, Inc. dev5@qq.com
 
  This file is part of X-MSG-IM.
 
  X-MSG-IM is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  X-MSG-IM is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
 
  You should have received a copy of the GNU Affero General Public License
  along with X-MSG-IM.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "XmsgAp.h"
#include "../XmsgImHlrCfg.h"
#include "../usr/XmsgImUsrMgr.h"

unordered_map<string, shared_ptr<XmsgImMsgStub>> XmsgAp::__h2nMsgs__;

XmsgAp::XmsgAp(shared_ptr<XscTcpServer> tcpServer, const string &peer, const string& pwd, const string& alg) :
		XmsgNe(tcpServer, peer, X_MSG_AP, pwd, alg)
{
	for (auto& it : XmsgAp::__h2nMsgs__)
		this->msgMgr->msgs[it.first] = it.second;
}

XscMsgItcpRetType XmsgAp::itcp(XscWorker* wk, XscChannel* channel, shared_ptr<XscProtoPdu> pdu)
{
	if (pdu->transm.trans->trans == XSC_TAG_TRANS_BEGIN)
		return this->itcp4begin(wk, channel, pdu);
	if (pdu->transm.trans->trans == XSC_TAG_TRANS_UNIDIRECTION)
		return this->itcp4unidirection(wk, channel, pdu);
	return XscMsgItcpRetType::DISABLE; 
}

XscMsgItcpRetType XmsgAp::itcp4begin(XscWorker* wk, XscChannel* channel, shared_ptr<XscProtoPdu> pdu)
{
	string ccid;
	if (!pdu->transm.getOob(XSC_TAG_UID, ccid) || ccid.empty()) 
	{
		LOG_FAULT("it`s a bug, must be have x-msg-im-client channel id, msg: %s, this: %s", pdu->transm.trans->msg.c_str(), this->toString().c_str())
		return XscMsgItcpRetType::EXCEPTION;
	}
	SptrXitp trans(new XmsgImTransPassive(static_pointer_cast<XscChannel>(this->shared_from_this()), pdu));
	trans->dtid = pdu->transm.trans->stid;
	trans->addOob(XSC_TAG_UID, ccid);
	string itcp;
	if (trans->getOob(XSC_TAG_INTERCEPT, itcp))
		trans->addOob(XSC_TAG_INTERCEPT, itcp);
	shared_ptr<XmsgImMsgStub> stub = this->msgMgr->getMsgStub(pdu->transm.trans->msg);
	if (stub == nullptr)
	{
		LOG_DEBUG("can not found x-msg-im-msg-stub for msg: %s, this: %s", pdu->transm.trans->msg.c_str(), this->toString().c_str())
		XmsgImLog::getLog(trans->channel.get())->transPassStart(trans, pdu);
		trans->endDesc(RET_FORBIDDEN, "unsupported message: %s", pdu->transm.trans->msg.c_str());
		return XscMsgItcpRetType::FORBIDDEN;
	}
	trans->beginMsg = stub->newBegin(pdu->transm.trans->dat, pdu->transm.trans->dlen);
	if (trans->beginMsg == nullptr)
	{
		LOG_DEBUG("can not reflect a begin message from data, msg: %s, this: %s", pdu->transm.trans->msg.c_str(), this->toString().c_str())
		XmsgImLog::getLog(trans->channel.get())->transPassStart(trans, pdu);
		trans->endDesc(RET_EXCEPTION, "request message format error: %s", pdu->transm.trans->msg.c_str());
		return XscMsgItcpRetType::FORBIDDEN;
	}
	XmsgImLog::getLog(trans->channel.get())->transPassStart(trans, pdu);
	if (!stub->auth)
	{
		((void (*)(shared_ptr<XscUsr> ap, const string& ccid, SptrXitp trans, shared_ptr<Message> req)) (stub->cb))(this->usr.lock(), ccid, trans, trans->beginMsg);
		return XscMsgItcpRetType::SUCCESS;
	}
	auto ap = static_pointer_cast<XmsgNeUsr>(this->usr.lock());
	string apCcid = XmsgMisc::getApCcid(ap->uid , ccid);
	shared_ptr<XmsgImClient> client = XmsgImUsrMgr::instance()->findXmsgImClient(apCcid);
	if (client == nullptr) 
	{
		LOG_DEBUG("can not found x-msg-im-client channel id, ap-ccid: %s, msg: %s, this: %s", apCcid.c_str(), pdu->transm.trans->msg.c_str(), this->toString().c_str())
		trans->endDesc(RET_FORBIDDEN, "can not found usr data");
		return XscMsgItcpRetType::FORBIDDEN;
	}
	auto usr = client->usr.lock();
	if (usr == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found usr data from x-msg-im-client: %s", client->toString().c_str())
		trans->endDesc(RET_EXCEPTION, "system exception, can not found usr data");
		return XscMsgItcpRetType::FORBIDDEN;
	}
	usr->future([stub, ap, client, trans, usr] 
	{
		((void (*)(shared_ptr<XmsgNeUsr> nu, SptrUsr usr, SptrClient client, SptrXitp trans, shared_ptr<Message> req)) (stub->cb))(ap, usr, client, trans, trans->beginMsg);
	});
	return XscMsgItcpRetType::SUCCESS;
}

XscMsgItcpRetType XmsgAp::itcp4unidirection(XscWorker* wk, XscChannel* channel, shared_ptr<XscProtoPdu> pdu)
{
	string ccid;
	if (!pdu->transm.getOob(XSC_TAG_UID, ccid) || ccid.empty()) 
	{
		LOG_FAULT("it`s a bug, must be have x-msg-im-client channel id, msg: %s, this: %s", pdu->transm.trans->msg.c_str(), this->toString().c_str())
		return XscMsgItcpRetType::EXCEPTION;
	}
	SptrXitup trans(new XmsgImTransUnidirectionPass(static_pointer_cast<XscChannel>(this->shared_from_this()), pdu));
	shared_ptr<XmsgImMsgStub> stub = this->msgMgr->getMsgStub(pdu->transm.trans->msg);
	if (stub == nullptr)
	{
		XmsgImLog::getLog(trans->channel.get())->transPassUni(trans, pdu);
		LOG_DEBUG("can not found x-msg-im-msg-stub for msg: %s, this: %s", pdu->transm.trans->msg.c_str(), this->toString().c_str())
		return XscMsgItcpRetType::FORBIDDEN;
	}
	trans->uniMsg = stub->newUnidirection(pdu->transm.trans->dat, pdu->transm.trans->dlen);
	if (trans->uniMsg == nullptr)
	{
		XmsgImLog::getLog(trans->channel.get())->transPassUni(trans, pdu);
		LOG_DEBUG("can not reflect a unidirection message from data, msg: %s, this: %s", pdu->transm.trans->msg.c_str(), this->toString().c_str())
		return XscMsgItcpRetType::FORBIDDEN;
	}
	XmsgImLog::getLog(trans->channel.get())->transPassUni(trans, pdu);
	if (!stub->auth)
	{
		((void (*)(shared_ptr<XscUsr> ap, const string& ccid, SptrXitup trans, shared_ptr<Message> msg)) (stub->cb))(this->usr.lock(), ccid, trans, trans->uniMsg);
		return XscMsgItcpRetType::SUCCESS;
	}
	auto ap = static_pointer_cast<XmsgNeUsr>(this->usr.lock());
	string apCcid = XmsgMisc::getApCcid(ap->uid , ccid);
	SptrClient client = XmsgImUsrMgr::instance()->findXmsgImClient(apCcid);
	if (client == nullptr) 
	{
		LOG_DEBUG("can not found x-msg-im-client channel id, ap-ccid: %s, msg: %s, this: %s", apCcid.c_str(), pdu->transm.trans->msg.c_str(), this->toString().c_str())
		return XscMsgItcpRetType::FORBIDDEN;
	}
	auto usr = client->usr.lock();
	if (usr == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found usr data from x-msg-im-client: %s", client->toString().c_str())
		return XscMsgItcpRetType::FORBIDDEN;
	}
	usr->future([stub, ap, client, usr, trans] 
	{
		((void (*)(shared_ptr<XmsgNeUsr> nu, SptrUsr usr, SptrClient client, SptrXitup trans, shared_ptr<Message> msg)) (stub->cb))(ap, usr, client, trans, trans->uniMsg);
	});
	return XscMsgItcpRetType::SUCCESS;
}

bool XmsgAp::regH2N(const Descriptor* begin, const Descriptor* end, const Descriptor* uni, void* cb, bool auth, ForeignAccessPermission foreign)
{
	shared_ptr<XmsgImMsgStub> stub(new XmsgImMsgStub(begin, end, uni, cb, auth, foreign));
	if (XmsgAp::__h2nMsgs__.find(stub->msg) != XmsgAp::__h2nMsgs__.end())
	{
		LOG_ERROR("duplicate message: %s", stub->msg.c_str())
		return false;
	}
	XmsgAp::__h2nMsgs__[stub->msg] = stub;
	return true;
}

XmsgAp::~XmsgAp()
{

}

