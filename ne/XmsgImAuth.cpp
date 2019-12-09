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

#include "XmsgImAuth.h"
#include "../XmsgImHlrCfg.h"
#include "../usr/XmsgImUsrMgr.h"

unordered_map<string, shared_ptr<XmsgImMsgStub>> XmsgImAuth::__h2nMsgs__;

XmsgImAuth::XmsgImAuth(shared_ptr<XscTcpServer> tcpServer, const string &peer, const string& pwd, const string& alg) :
		XmsgNe(tcpServer, peer, X_MSG_IM_AUTH, pwd, alg)
{
	for (auto& it : XmsgImAuth::__h2nMsgs__)
		this->msgMgr->msgs[it.first] = it.second;
}

XscMsgItcpRetType XmsgImAuth::itcp(XscWorker* wk, XscChannel* channel, shared_ptr<XscProtoPdu> pdu)
{
	return XscMsgItcpRetType::DISABLE;
}

bool XmsgImAuth::regH2N(const Descriptor* begin, const Descriptor* end, const Descriptor* uni, void* cb, bool auth, ForeignAccessPermission foreign)
{
	shared_ptr<XmsgImMsgStub> stub(new XmsgImMsgStub(begin, end, uni, cb, auth, foreign));
	if (XmsgImAuth::__h2nMsgs__.find(stub->msg) != XmsgImAuth::__h2nMsgs__.end())
	{
		LOG_ERROR("duplicate message: %s", stub->msg.c_str())
		return false;
	}
	LOG_TRACE("reg h2n messsage: %s", stub->toString().c_str())
	XmsgImAuth::__h2nMsgs__[stub->msg] = stub;
	return true;
}

XmsgImAuth::~XmsgImAuth()
{

}

