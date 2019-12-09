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

#include "XmsgImUsrMgr.h"
#include "XmsgImClientStatusEventQueue.h"

XmsgImUsrMgr* XmsgImUsrMgr::inst = new XmsgImUsrMgr();

XmsgImUsrMgr::XmsgImUsrMgr()
{

}

XmsgImUsrMgr* XmsgImUsrMgr::instance()
{
	return XmsgImUsrMgr::inst;
}

void XmsgImUsrMgr::addXmsgImUsr(shared_ptr<XmsgImUsr> usr)
{
	unique_lock<mutex> lock(this->lock4usr4cgt);
	this->usr4cgt[usr->dat->cgt->toString()] = usr;
}

void XmsgImUsrMgr::delXmsgImUsr(const string& cgt)
{
	unique_lock<mutex> lock(this->lock4usr4cgt);
	this->usr4cgt.erase(cgt);
}

shared_ptr<XmsgImUsr> XmsgImUsrMgr::findXmsgImUsr(const string& cgt)
{
	unique_lock<mutex> lock(this->lock4usr4cgt);
	auto it = this->usr4cgt.find(cgt);
	return it == this->usr4cgt.end() ? nullptr : it->second;
}

size_t XmsgImUsrMgr::size4usr()
{
	unique_lock<mutex> lock(this->lock4usr4cgt);
	return this->usr4cgt.size();
}

void XmsgImUsrMgr::addXmsgImClient(SptrClient client)
{
	unique_lock<mutex> lock(this->lock4client4apCcid);
	this->client4apCcid[client->apCcid] = client;
	lock.unlock(); 
	XmsgImClientStatusEventQueue::instance()->xMsgImClientStatusChanged(client, "online");
}

SptrClient XmsgImUsrMgr::removeXmsgImClient(const string& apCcid)
{
	unique_lock<mutex> lock(this->lock4client4apCcid);
	auto it = this->client4apCcid.find(apCcid);
	if (it == this->client4apCcid.end())
		return nullptr;
	SptrClient client = it->second;
	this->client4apCcid.erase(it);
	lock.unlock(); 
	XmsgImClientStatusEventQueue::instance()->xMsgImClientStatusChanged(client, "offline");
	return client;
}

SptrClient XmsgImUsrMgr::findXmsgImClient(const string& apCcid)
{
	unique_lock<mutex> lock(this->lock4client4apCcid);
	auto it = this->client4apCcid.find(apCcid);
	return it == this->client4apCcid.end() ? nullptr : it->second;
}

size_t XmsgImUsrMgr::size4client()
{
	unique_lock<mutex> lock(this->lock4client4apCcid);
	return this->client4apCcid.size();
}

void XmsgImUsrMgr::search(const string& keyword, list<shared_ptr<XmsgImUsr>>& lis)
{
	unique_lock<mutex> lock(this->lock4usr4cgt);
	for (auto& it : this->usr4cgt)
	{
		auto usr = it.second;
		if (usr->dat->pub->name().find(keyword) != string::npos)
			lis.push_back(usr);
	}
}

void XmsgImUsrMgr::loadCb(shared_ptr<XmsgImHlrUsrDatColl> dat)
{
	shared_ptr<XmsgImUsr> usr(new XmsgImUsr(dat));
	XmsgImUsrMgr::instance()->addXmsgImUsr(usr);
}

XmsgImUsrMgr::~XmsgImUsrMgr()
{

}

