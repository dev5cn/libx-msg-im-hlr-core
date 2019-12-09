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

#ifndef EVN_XMSGIMUSREVENT_H_
#define EVN_XMSGIMUSREVENT_H_

#include <libx-msg-im-hlr-pb.h>
#include "../coll/XmsgImHlrSysEventColl.h"

class XmsgImUsr;
class XmsgImClient;

class XmsgImUsrEvent
{
public:
	void onEvent(shared_ptr<XmsgImUsr> usr , shared_ptr<XmsgImHlrUsrEventColl> evn); 
public:
	void getIncEvents(ullong ver, list<shared_ptr<XmsgImHlrEventUsrNotice>>& lis); 
	shared_ptr<XmsgImHlrEventUsrNotice> getNotice(ullong ver); 
	void eventRead(ullong ver); 
	void setMaxVersion(ullong ver); 
	ullong nextVer(); 
public:
	static void loadCb(shared_ptr<XmsgImHlrUsrEventColl> evn); 
	static void loadCb4maxVer(SptrCgt cgt, ullong ver); 
	XmsgImUsrEvent();
	virtual ~XmsgImUsrEvent();
public:
	atomic_ullong version; 
	list<shared_ptr<XmsgImHlrEventUsrNotice>> event; 
	unordered_map<ullong , shared_ptr<XmsgImHlrEventUsrNotice>> event4ver; 
	void chooseClients(shared_ptr<XmsgImUsr> usr, XmsgUsrEventNoticeType ent, list<shared_ptr<XmsgImClient>>& clients); 
	shared_ptr<XmsgImHlrEventUsrNotice> toNotice(shared_ptr<XmsgImHlrUsrEventColl> evn); 
};

#endif 
