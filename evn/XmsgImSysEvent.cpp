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

#include <libx-msg-im-hlr-pb.h>
#include "XmsgImSysEvent.h"
#include "../XmsgImHlrCfg.h"

XmsgImSysEvent* XmsgImSysEvent::inst = NULL;

XmsgImSysEvent::XmsgImSysEvent() :
		ActorBlockingSingleThread("sys-evn")
{

}

XmsgImSysEvent* XmsgImSysEvent::instance()
{
	return XmsgImSysEvent::inst;
}

void XmsgImSysEvent::init()
{
	XmsgImSysEvent::inst = new XmsgImSysEvent();
}

void XmsgImSysEvent::sub(SptrUsr usr, SptrClient client, SptrXitp trans, shared_ptr<XmsgImHlrEventSysSubReq> req)
{
	list<shared_ptr<XmsgImHlrEventSysNotice>> lis;
	for (auto it = this->event.rbegin(); it != this->event.rend(); ++it) 
	{
		shared_ptr<XmsgImHlrEventSysNotice> notice = *it;
		if (notice->ver() <= req->ver()) 
			continue;
		lis.push_back(notice);
	}
	shared_ptr<XmsgImHlrEventSysSubRsp> rsp(new XmsgImHlrEventSysSubRsp());
	rsp->set_latestver(this->version - 1); 
	trans->end(rsp); 
	if (lis.empty()) 
	{
		usr->future([client]
		{
			client->subSysEvent = true; 
		});
		return;
	}
	for (auto& it : lis)
	{
		shared_ptr<XmsgImHlrEventSysNotice> notice = it;
		usr->future([usr, client, notice]
		{
			client->sendNotice(usr, notice);
		});
	}
	usr->future([client]
	{
		client->subSysEvent = true; 
	});
}

void XmsgImSysEvent::pub(shared_ptr<XmsgImHlrSysEventColl> evn)
{
	auto notice = this->toNotice(evn);
	this->event.push_back(this->toNotice(evn));
	for (auto& it : this->subClient)
	{
		shared_ptr<list<SptrClient>> lis = it.second;
		for (auto& iter : *lis)
		{
			SptrClient client = iter;
			auto usr = client->usr.lock();
			if (usr == nullptr)
			{
				LOG_FAULT("it`s a bug, x-msg-im-usr dat is null, client: %s", iter->toString().c_str())
				continue;
			}
			usr->future([usr, client, notice]
			{
				client->sendNotice(usr, notice);
			});
		}
	}
}

void XmsgImSysEvent::unSub(SptrUsr usr, SptrClient client)
{
	auto it = this->subClient.find(usr->dat->cgt->toString());
	if (it == this->subClient.end())
	{
		LOG_DEBUG("can not found in subscribe client map, client: %s", client->toString().c_str())
		usr->future([client]
		{
			client->subSysEvent = false; 
		});
		return;
	}
	shared_ptr<list<SptrClient>> lis = it->second;
	for (auto iter = lis->begin(); iter != lis->end();)
	{
		if ((*iter)->apCcid == client->apCcid)
		{
			lis->erase(iter++);
			break;
		}
		++iter;
	}
	if (lis->empty())
		this->subClient.erase(it);
	usr->future([client]
	{
		client->subSysEvent = false; 
	});
}

void XmsgImSysEvent::loadCb(shared_ptr<XmsgImHlrSysEventColl> evn)
{
	XmsgImSysEvent::instance()->event.push_back(XmsgImSysEvent::instance()->toNotice(evn));
}

void XmsgImSysEvent::setMaxVersion(ullong ver)
{
	this->version.store(ver + 1); 
}

ullong XmsgImSysEvent::nextVer()
{
	return this->version.fetch_add(1);
}

shared_ptr<XmsgImHlrEventSysNotice> XmsgImSysEvent::toNotice(shared_ptr<XmsgImHlrSysEventColl> evn)
{
	shared_ptr<XmsgImHlrEventSysNotice> notice(new XmsgImHlrEventSysNotice());
	notice->set_msg(evn->msg);
	notice->set_dat(evn->dat);
	notice->set_ver(evn->ver);
	notice->set_gts(evn->gts);
	return notice;
}

XmsgImSysEvent::~XmsgImSysEvent()
{

}

