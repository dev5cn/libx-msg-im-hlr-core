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

#ifndef COLL_XMSGIMHLRCONTRACTSCOLL_H_
#define COLL_XMSGIMHLRCONTRACTSCOLL_H_

#include <libx-msg-common-dat-struct-cpp.h>
#include <libx-msg-im-hlr-pb.h>

class XmsgImHlrContractsColl
{
public:
	SptrCgt cgt; 
	SptrCgt ctp; 
	shared_ptr<XmsgKv> info; 
	ullong gts; 
	ullong uts; 
public:
	string toString();
	XmsgImHlrContractsColl();
	virtual ~XmsgImHlrContractsColl();
};

#endif 
