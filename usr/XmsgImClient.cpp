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

#include "XmsgImClient.h"
#include "XmsgImUsr.h"
#include "XmsgImUsrMgr.h"
#include "../XmsgImHlrCfg.h"
#include "../evn/XmsgImSysEvent.h"

XmsgImClient::XmsgImClient(shared_ptr<XmsgImUsr> usr, const string& plat, const string& did, const string& ccid, shared_ptr<XmsgAp> apLocal )
{
	this->plat = plat;
	this->did = did;
	this->usr = usr;
	this->apLocal = apLocal;
	this->apForeign = nullptr;
	this->status = "online";
	this->type = XmsgMisc::getPlatType(plat);
	this->apCcid = XmsgMisc::getApCcid(apLocal->usr.lock()->uid, ccid);
	this->ccid = ccid;
	this->subUsrEvent = false;
	this->subSysEvent = false;
}

XmsgImClient::XmsgImClient(shared_ptr<XmsgImUsr> usr, const string& plat, const string& did, const string& ccid, SptrCgt apForeign )
{
	this->plat = plat;
	this->did = did;
	this->usr = usr;
	this->apLocal = nullptr;
	this->apForeign = apForeign;
	this->status = "online";
	this->type = XmsgMisc::getPlatType(plat);
	this->apCcid = XmsgMisc::getApCcid(apForeign->toString(), ccid);
	this->ccid = ccid;
	this->subUsrEvent = false;
	this->subSysEvent = false;
}

void XmsgImClient::evnEstb(shared_ptr<XmsgImUsr> usr, SptrXit xit )
{
	this->evnEstbNotice2neg(usr, xit);
}

void XmsgImClient::evnDisc(shared_ptr<XmsgImUsr> usr, shared_ptr<XmsgApClientLostNotice> notice, SptrXit xit)
{
	auto c = XmsgImUsrMgr::instance()->removeXmsgImClient(this->apCcid); 
	if (c == NULL)
	{
		LOG_FAULT("it`s a bug, client: %s", this->toString().c_str())
	}
	usr->delClient(this->apCcid); 
	this->evnDiscNotice2neg(notice, xit); 
	XmsgImSysEvent::instance()->future([usr, c]
	{
		XmsgImSysEvent::instance()->unSub(usr, c);
	});
}

void XmsgImClient::evnEstbNotice2neg(shared_ptr<XmsgImUsr> usr, SptrXit xit)
{
	list<shared_ptr<XmsgNeUsr>> lis;
	XmsgNeMgr::instance()->getSubClientEstbEventGroup(lis);
	if (lis.empty())
	{
		LOG_DEBUG("no network element group subscribe x-msg-client established event, this: %s", this->toString().c_str())
		return;
	}
	string apCgt;
	string ccid;
	XmsgMisc::parseApCcid(this->apCcid, apCgt, ccid);
	shared_ptr<XmsgApClientEstbNotice> notice(new XmsgApClientEstbNotice());
	notice->set_apcgt(apCgt);
	notice->set_cgt(usr->dat->cgt->toString());
	notice->set_plat(this->plat);
	notice->set_did(this->did);
	notice->set_ccid(ccid);
	for (auto& it : lis)
	{
		XmsgImChannel::cast(it->channel)->unidirection(notice, nullptr, xit);
	}
}

void XmsgImClient::evnDiscNotice2neg(shared_ptr<XmsgApClientLostNotice> notice, SptrXit xit)
{
	list<shared_ptr<XmsgNeUsr>> lis;
	XmsgNeMgr::instance()->getSubClientEstbEventGroup(lis);
	if (lis.empty())
	{
		LOG_DEBUG("no network element group subscribe x-msg-client lost event, this: %s", this->toString().c_str())
		return;
	}
	for (auto& it : lis)
	{
		XmsgImChannel::cast(it->channel)->unidirection(notice, nullptr, xit);
	}
}

bool XmsgImClient::isOnline()
{
	return this->apLocal != nullptr || this->apForeign != nullptr;
}

bool XmsgImClient::isLocalAttach()
{
	return this->apLocal != nullptr;
}

void XmsgImClient::sendNotice(SptrUsr usr, shared_ptr<Message> notice, SptrXit xit)
{
	this->sendNotice4local(usr, notice);
}

void XmsgImClient::sendNotice4local(SptrUsr usr, shared_ptr<Message> notice, SptrXit xit)
{
	SptrOob oob(new list<pair<uchar, string>>());
	oob->push_back(make_pair<>(XSC_TAG_UID, this->ccid));
	this->apLocal->unidirection(notice, oob);
}

string XmsgImClient::toString()
{
	string str;
	auto usr = this->usr.lock();
	SPRINTF_STRING(&str, "plat: %s, type: %s, did: %s, status: %s, apCcid: %s, usr: %s, ne: %s", 
			this->plat.c_str(),
			this->type.c_str(),
			this->did.c_str(),
			this->status.c_str(),
			this->apCcid.c_str(),
			usr == nullptr ? "null" : usr->toString().c_str(),
			this->apLocal == nullptr ? this->apForeign->toString().c_str() : this->apLocal->toString().c_str())
	return str;
}

XmsgImClient::~XmsgImClient()
{

}

