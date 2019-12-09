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

#include "XmsgImUsrEvent.h"
#include "../usr/XmsgImUsrMgr.h"

XmsgImUsrEvent::XmsgImUsrEvent()
{

}

void XmsgImUsrEvent::onEvent(shared_ptr<XmsgImUsr> usr , shared_ptr<XmsgImHlrUsrEventColl> evn)
{
	shared_ptr<XmsgImHlrEventUsrNotice> notice = this->toNotice(evn);
	this->event.push_back(notice);
	this->event4ver[notice->ver()] = notice;
	if (usr->client.empty()) 
		return;
	list<SptrClient> clients;
	this->chooseClients(usr, evn->ent, clients);
	for (auto& it : clients) 
	{
		if (!it->subUsrEvent) 
			continue;
		it->sendNotice(usr, notice);
	}
}

void XmsgImUsrEvent::getIncEvents(ullong ver, list<shared_ptr<XmsgImHlrEventUsrNotice>>& lis)
{
	for (auto it = this->event.rbegin(); it != this->event.rend(); ++it) 
	{
		shared_ptr<XmsgImHlrEventUsrNotice> notice = *it;
		if (notice->ver() <= ver) 
			continue;
		lis.push_back(notice);
	}
}

shared_ptr<XmsgImHlrEventUsrNotice> XmsgImUsrEvent::getNotice(ullong ver)
{
	auto it = this->event4ver.find(ver);
	return it == this->event4ver.end() ? nullptr : it->second;
}

void XmsgImUsrEvent::eventRead(ullong ver)
{
	for (auto it = this->event.begin(); it != this->event.end();)
	{
		shared_ptr<XmsgImHlrEventUsrNotice> notice = *it;
		if (notice->ver() > ver)
		{
			++it;
			continue;
		}
		this->event.erase(it++); 
		this->event4ver.erase(notice->ver());
	}
}

void XmsgImUsrEvent::setMaxVersion(ullong ver)
{
	this->version.store(ver + 1); 
}

ullong XmsgImUsrEvent::nextVer()
{
	return this->version.fetch_add(1);
}

void XmsgImUsrEvent::loadCb(shared_ptr<XmsgImHlrUsrEventColl> evn)
{
	auto usr = XmsgImUsrMgr::instance()->findXmsgImUsr(evn->cgt->toString());
	if (usr == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found x-msg-im usr for channel global title: %s", evn->cgt->toString().c_str())
		Misc::lazyExit();
		return;
	}
	if (evn->isRead)
	{
		LOG_FAULT("it`s a bug, dat: %s", evn->toString().c_str())
		Misc::lazyExit();
		return;
	}
	auto notice = usr->evn->toNotice(evn);
	usr->evn->event.push_back(notice); 
	usr->evn->event4ver[notice->ver()] = notice;
}

void XmsgImUsrEvent::loadCb4maxVer(SptrCgt cgt, ullong ver)
{
	auto usr = XmsgImUsrMgr::instance()->findXmsgImUsr(cgt->toString());
	if (usr == nullptr)
	{
		LOG_FAULT("it`s a bug, can not found x-msg-im usr for channel global title: %s", cgt->toString().c_str())
		Misc::lazyExit();
		return;
	}
	usr->evn->setMaxVersion(ver);
}

void XmsgImUsrEvent::chooseClients(shared_ptr<XmsgImUsr> usr, XmsgUsrEventNoticeType ent, list<SptrClient>& clients)
{
	if (ent == XmsgUsrEventNoticeType::TERMINAL_ANY_ONE) 
	{
		clients.push_back(Xsc::clock % 2 == 1 ? usr->client.front() : usr->client.back());
		return;
	}
	if (ent == XmsgUsrEventNoticeType::TERMINAL_ALL)
	{
		for (auto& it : usr->client)
			clients.push_back(it);
		return;
	}
	LOG_FAULT("it`s a bug, incomplete.")
}

shared_ptr<XmsgImHlrEventUsrNotice> XmsgImUsrEvent::toNotice(shared_ptr<XmsgImHlrUsrEventColl> evn)
{
	shared_ptr<XmsgImHlrEventUsrNotice> notice(new XmsgImHlrEventUsrNotice());
	notice->set_msg(evn->msg);
	notice->set_dat(evn->dat);
	notice->set_ver(evn->ver);
	notice->set_gts(evn->gts);
	return notice;
}

XmsgImUsrEvent::~XmsgImUsrEvent()
{

}

