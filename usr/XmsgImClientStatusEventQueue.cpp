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

#include "XmsgImClientStatusEventQueue.h"
#include "XmsgImUsrMgr.h"
#include "../XmsgImHlrCfg.h"

XmsgImClientStatusEventQueue* XmsgImClientStatusEventQueue::inst = NULL;

XmsgImClientStatusEventQueue::XmsgImClientStatusEventQueue() :
		ActorBlockingSingleThread("x-msg-im-client-status-event-queue")
{

}

XmsgImClientStatusEventQueue* XmsgImClientStatusEventQueue::instance()
{
	return XmsgImClientStatusEventQueue::inst;
}

void XmsgImClientStatusEventQueue::init()
{
	XmsgImClientStatusEventQueue::inst = new XmsgImClientStatusEventQueue();
}

void XmsgImClientStatusEventQueue::pubAll(shared_ptr<XmsgNeUsr> xMsgChannelStatus)
{
	this->future([xMsgChannelStatus]
	{
		ullong sts = DateMisc::dida();
		list<SptrClient> clients;
		unique_lock<mutex> lock(XmsgImUsrMgr::instance()->lock4client4apCcid);
		for (auto& it : XmsgImUsrMgr::instance()->client4apCcid)
		{
			clients.push_back(it.second);
		}
		lock.unlock();
		XmsgImClientStatusEventQueue::instance()->pubBatch(xMsgChannelStatus, clients);
		XmsgImClientStatusEventQueue::instance()->xMsgChannelStatus[xMsgChannelStatus->uid] = xMsgChannelStatus;
		LOG_INFO("pub all x-msg-im-client status to x-msg-channel-status successful, elap: %dms, size: %zu, x-msg-channel-status: %s", DateMisc::elapDida(sts), clients.size(), xMsgChannelStatus->uid.c_str())
	});
}

void XmsgImClientStatusEventQueue::xMsgImClientStatusChanged(SptrClient client, const string& status)
{
	this->future([client, status]
	{
		if (XmsgImClientStatusEventQueue::instance()->xMsgChannelStatus.empty()) 
		{
			return;
		}
		shared_ptr<XmsgImUsr> usr = client->usr.lock();
		if (usr == nullptr)
		{
			LOG_FAULT("it`s a bug, client: %s", client->toString().c_str())
			return;
		}
		shared_ptr<XmsgImHlrChannelStatusChangedNotice> notice(new XmsgImHlrChannelStatusChangedNotice());
		notice->set_cgt(usr->dat->cgt->toString());
		notice->set_plat(client->plat);
		notice->set_did(client->did);
		notice->set_status(status);
		for (auto& it : XmsgImClientStatusEventQueue::instance()->xMsgChannelStatus) 
		{
			XmsgImChannel::cast(it.second->channel)->unidirection(notice);
		}
	});
}

void XmsgImClientStatusEventQueue::pubBatch(shared_ptr<XmsgNeUsr> xMsgChannelStatus, const list<SptrClient>& lis)
{
	shared_ptr<XmsgImHlrChannelStatusPubReq> req(new XmsgImHlrChannelStatusPubReq());
	int pageSize = 0x80; 
	int size = 0;
	for (auto& it : lis)
	{
		SptrClient client = it;
		shared_ptr<XmsgImUsr> usr = client->usr.lock();
		if (usr == nullptr)
		{
			LOG_FAULT("it`s a bug, client: %s", client->toString().c_str())
			continue;
		}
		XmsgImHlrChannelStatusChangedNotice* notice = req->add_notice();
		notice->set_cgt(usr->dat->cgt->toString());
		notice->set_plat(client->plat);
		notice->set_did(client->did);
		notice->set_status(client->status);
		++size;
		if (size < pageSize) 
			continue;
		XmsgImChannel::cast(xMsgChannelStatus->channel)->begin(req, [size](SptrXiti itrans)
		{
			if (itrans->ret != RET_SUCCESS)
			{
				LOG_ERROR("pub x-msg-im-client status to x-msg-channel-status failed, ret: %04X, desc: %s", itrans->ret, itrans->desc.c_str()) 
				return;
			}
			LOG_INFO("send x-msg-im-client status to x-msg-channel-status successful, size: %d", size)
		});
		size = 0;
		req = shared_ptr<XmsgImHlrChannelStatusPubReq>();
	}
	if (size < 1) 
		return;
	XmsgImChannel::cast(xMsgChannelStatus->channel)->begin(req, [size](SptrXiti itrans)
	{
		if (itrans->ret != RET_SUCCESS)
		{
			LOG_ERROR("pub x-msg-im-client status to x-msg-channel-status failed, ret: %04X, desc: %s", itrans->ret, itrans->desc.c_str()) 
			return;
		}
		LOG_INFO("send x-msg-im-client status to x-msg-channel-status successful, size: %d", size)
	});
}

XmsgImClientStatusEventQueue::~XmsgImClientStatusEventQueue()
{

}

