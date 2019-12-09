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

#ifndef CONTRACTS_XMSGIMUSRCONTRACTS_H_
#define CONTRACTS_XMSGIMUSRCONTRACTS_H_

#include "../coll/XmsgImHlrContractsBlackListColl.h"
#include "../coll/XmsgImHlrContractsColl.h"

#define X_MSG_IM_HLR_CONTRACTS_REPLY_OPER_IGNORE																		"ignore"	
#define X_MSG_IM_HLR_CONTRACTS_REPLY_OPER_AGREE																		"agree"	
#define X_MSG_IM_HLR_CONTRACTS_REPLY_OPER_REJECT																		"reject"	

class XmsgImUsr;

class XmsgImUsrContracts
{
public:
	void addCtp(shared_ptr<XmsgImHlrContractsColl> ctp); 
	shared_ptr<XmsgImHlrContractsColl> getCtp(const string& cgt); 
	bool delCtp(SptrCgt cgt); 
	void addBlackList(shared_ptr<XmsgImHlrContractsBlackListColl> ctp); 
	shared_ptr<XmsgImHlrContractsBlackListColl> getBlackList(const string& cgt); 
	bool delBlackList(SptrCgt cgt); 
public:
	XmsgImUsrContracts();
	virtual ~XmsgImUsrContracts();
private:
	unordered_map<string, shared_ptr<XmsgImHlrContractsColl>> contracts; 
	unordered_map<string, shared_ptr<XmsgImHlrContractsBlackListColl>> blackList; 
};

#endif 
