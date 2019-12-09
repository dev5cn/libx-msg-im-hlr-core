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

#ifndef XMSGIMUSR_H_
#define XMSGIMUSR_H_

#include "XmsgImClient.h"
#include "../coll/XmsgImHlrUsrDatColl.h"
#include "../contracts/XmsgImUsrContracts.h"
#include "../evn/XmsgImUsrEvent.h"

class XmsgImUsr: public Actor
{
public:
	list<SptrClient> client; 
	shared_ptr<XmsgImHlrUsrDatColl> dat; 
	shared_ptr<XmsgImUsrContracts> contracts; 
	shared_ptr<XmsgImUsrEvent> evn; 
public:
	void addClient(SptrClient client); 
	SptrClient delClient(const string& apCcid); 
	SptrClient findClientByPlat(const string& plat); 
	SptrClient removeClientByPlat(const string& plat); 
	SptrClient removeClientByType(const string& type); 
public:
	string toString();
	XmsgImUsr(shared_ptr<XmsgImHlrUsrDatColl> dat);
	virtual ~XmsgImUsr();
private:
	static int assignXscWorker(SptrCgt cgt); 
};

typedef shared_ptr<XmsgImUsr> SptrUsr;

#endif 
