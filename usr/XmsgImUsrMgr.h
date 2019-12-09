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

#ifndef XMSGIMUSRMGR_H_
#define XMSGIMUSRMGR_H_

#include "XmsgImUsr.h"
#include "XmsgImClient.h"
#include "../coll/XmsgImHlrUsrDatColl.h"

class XmsgImUsrMgr
{
public:
	void addXmsgImUsr(shared_ptr<XmsgImUsr> usr); 
	void delXmsgImUsr(const string& cgt); 
	shared_ptr<XmsgImUsr> findXmsgImUsr(const string& cgt); 
	size_t size4usr(); 
public:
	void addXmsgImClient(SptrClient client); 
	SptrClient removeXmsgImClient(const string& ccid); 
	SptrClient findXmsgImClient(const string& ccid); 
	size_t size4client(); 
public:
	void search(const string& keyword, list<shared_ptr<XmsgImUsr>>& lis); 
	static void loadCb(shared_ptr<XmsgImHlrUsrDatColl> dat); 
	static XmsgImUsrMgr* instance();
public:
	unordered_map<string , shared_ptr<XmsgImUsr>> usr4cgt; 
	unordered_map<string , SptrClient> client4apCcid; 
	mutex lock4usr4cgt;
	mutex lock4client4apCcid;
private:
	static XmsgImUsrMgr* inst;
	XmsgImUsrMgr();
	virtual ~XmsgImUsrMgr();
};

#endif 
