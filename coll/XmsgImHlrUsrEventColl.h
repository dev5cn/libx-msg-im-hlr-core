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

#ifndef COLL_XMSGIMHLRUSREVENTCOLL_H_
#define COLL_XMSGIMHLRUSREVENTCOLL_H_

#include <libx-msg-common-dat-struct-cpp.h>

typedef enum XmsgUsrEventNoticeType
{
	TERMINAL_ANY_ONE = 0x01, 
	TERMINAL_PC_ONLY = 0x02, 
	TERMINAL_MOBILE_ONLY = 0x03, 
	TERMINAL_WEB_ONLY = 0x04, 
	TERMINAL_ALL = 0x05, 
} XmsgUsrEventNoticeType;

class XmsgImHlrUsrEventColl
{
public:
	SptrCgt cgt; 
	bool isRead; 
	XmsgUsrEventNoticeType ent; 
	ullong ver; 
	string msg; 
	string dat; 
	ullong gts; 
	ullong uts; 
	ullong ets; 
public:
	static bool isValidEnt(int ent); 
	string toString();
	XmsgImHlrUsrEventColl();
	virtual ~XmsgImHlrUsrEventColl();
};

#endif 