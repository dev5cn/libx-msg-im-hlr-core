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

#ifndef USR_XMSGIMCLIENT_H_
#define USR_XMSGIMCLIENT_H_

#include <libx-msg-im-hlr-pb.h>
#include "../ne/XmsgAp.h"

class XmsgImUsr;

class XmsgImClient
{
public:
	string plat; 
	string type; 
	string did; 
	string apCcid; 
	string ccid; 
	string status; 
	shared_ptr<XmsgAp> apLocal; 
	SptrCgt apForeign; 
	weak_ptr<XmsgImUsr> usr; 
public:
	bool subUsrEvent; 
	bool subSysEvent; 
public:
	void sendNotice(shared_ptr<XmsgImUsr> usr, shared_ptr<Message> notice, SptrXit xit = nullptr ); 
	void sendNotice4local(shared_ptr<XmsgImUsr> usr, shared_ptr<Message> notice, SptrXit xit = nullptr ); 
public:
	void evnEstb(shared_ptr<XmsgImUsr> usr, SptrXit xit = nullptr ); 
	void evnDisc(shared_ptr<XmsgImUsr> usr, shared_ptr<XmsgApClientLostNotice> notice, SptrXit xit = nullptr ); 
	bool isOnline(); 
	bool isLocalAttach(); 
	string toString();
	XmsgImClient(shared_ptr<XmsgImUsr> usr, const string& plat, const string& did, const string& ccid, shared_ptr<XmsgAp> apLocal );
	XmsgImClient(shared_ptr<XmsgImUsr> usr, const string& plat, const string& did, const string& ccid, SptrCgt apForeign );
	virtual ~XmsgImClient();
private:
	void evnEstbNotice2neg(shared_ptr<XmsgImUsr> usr, SptrXit xit); 
	void evnDiscNotice2neg(shared_ptr<XmsgApClientLostNotice> notice, SptrXit xit); 
};

typedef shared_ptr<XmsgImClient> SptrClient;

#endif 
