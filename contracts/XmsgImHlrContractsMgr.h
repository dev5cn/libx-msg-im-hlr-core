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

#ifndef CONTRACTS_XMSGIMHLRCONTRACTSMGR_H_
#define CONTRACTS_XMSGIMHLRCONTRACTSMGR_H_

#include "../coll/XmsgImHlrContractsBlackListColl.h"
#include "../coll/XmsgImHlrContractsColl.h"

class XmsgImHlrContractsMgr
{
public:
	static void loadCb4contracts(shared_ptr<XmsgImHlrContractsColl> dat); 
	static void loadCb4contractsBlackList(shared_ptr<XmsgImHlrContractsBlackListColl> dat); 
	static XmsgImHlrContractsMgr* instance();
private:
	static XmsgImHlrContractsMgr* inst;
	XmsgImHlrContractsMgr();
	virtual ~XmsgImHlrContractsMgr();
};

#endif 
