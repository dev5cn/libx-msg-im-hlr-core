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

#include "XmsgImUsr.h"
#include "../XmsgImHlrCfg.h"

XmsgImUsr::XmsgImUsr(shared_ptr<XmsgImHlrUsrDatColl> dat) :
		Actor(ActorType::ACTOR_ITC, XmsgImUsr::assignXscWorker(dat->cgt))
{
	this->dat = dat;
	this->contracts.reset(new XmsgImUsrContracts());
	this->evn.reset(new XmsgImUsrEvent());
}

void XmsgImUsr::addClient(SptrClient client)
{
	this->client.push_back(client);
}

SptrClient XmsgImUsr::delClient(const string& apCcid)
{
	for (auto it = this->client.begin(); it != this->client.end(); ++it)
	{
		if ((*it)->apCcid != apCcid)
			continue;
		auto c = *it;
		this->client.erase(it);
		return c;
	}
	return nullptr;
}

SptrClient XmsgImUsr::findClientByPlat(const string& plat)
{
	for (auto& it : this->client)
	{
		if (it->plat == plat)
			return it;
	}
	return nullptr;
}

SptrClient XmsgImUsr::removeClientByPlat(const string& plat)
{
	for (auto it = this->client.begin(); it != this->client.end(); ++it)
	{
		if ((*it)->plat != plat)
			continue;
		auto c = *it;
		this->client.erase(it);
		return c;
	}
	return nullptr;
}

SptrClient XmsgImUsr::removeClientByType(const string& type)
{
	for (auto it = this->client.begin(); it != this->client.end(); ++it)
	{
		if ((*it)->type != type)
			continue;
		auto c = *it;
		this->client.erase(it);
		return c;
	}
	return nullptr;
}

int XmsgImUsr::assignXscWorker(SptrCgt cgt)
{
	return (((int) (cgt->uid.data()[cgt->uid.length() - 1])) & 0x0000FF) % XmsgImHlrCfg::instance()->cfgPb->xsctcpcfg().worker();
}

string XmsgImUsr::toString()
{
	return this->dat->toString().c_str();
}

XmsgImUsr::~XmsgImUsr()
{

}

