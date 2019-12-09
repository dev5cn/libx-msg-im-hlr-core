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

#include "XmsgImHlrSysEventColl.h"

XmsgImHlrSysEventColl::XmsgImHlrSysEventColl()
{
	this->ent = XmsgUsrEventNoticeType::TERMINAL_ALL;
	this->ver = 0ULL;
	this->gts = 0ULL;
	this->ets = 0ULL;
}

string XmsgImHlrSysEventColl::toString()
{
	string str;
	SPRINTF_STRING(&str, "ent: %d, ver: %llu, msg: %s, gts: %s, ets: %s", 
			this->ent,
			this->ver,
			this->msg.c_str(),
			DateMisc::to_yyyy_mm_dd_hh_mi_ss_ms(this->gts).c_str(),
			DateMisc::to_yyyy_mm_dd_hh_mi_ss_ms(this->ets).c_str())
	return str;
}

XmsgImHlrSysEventColl::~XmsgImHlrSysEventColl()
{

}

