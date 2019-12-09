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

#ifndef EVN_XMSGIMSYSEVENT_H_
#define EVN_XMSGIMSYSEVENT_H_

#include <libx-msg-im-hlr-pb.h>
#include "../coll/XmsgImHlrSysEventColl.h"
#include "../usr/XmsgImUsr.h"

class XmsgImSysEvent: public ActorBlockingSingleThread
{
public:
	void setMaxVersion(ullong ver); 
	ullong nextVer(); 
	void sub(SptrUsr usr, SptrClient client, SptrXitp trans, shared_ptr<XmsgImHlrEventSysSubReq> req); 
	void pub(shared_ptr<XmsgImHlrSysEventColl> evn); 
	void unSub(SptrUsr usr, SptrClient client); 
public:
	static void loadCb(shared_ptr<XmsgImHlrSysEventColl> evn); 
	static XmsgImSysEvent* instance();
	static void init();
	XmsgImSysEvent();
	virtual ~XmsgImSysEvent();
private:
	atomic_ullong version; 
	list<shared_ptr<XmsgImHlrEventSysNotice>> event; 
	unordered_map<string , shared_ptr<list<SptrClient>>> subClient; 
	static XmsgImSysEvent* inst;
	shared_ptr<XmsgImHlrEventSysNotice> toNotice(shared_ptr<XmsgImHlrSysEventColl> evn); 
};

#endif 
