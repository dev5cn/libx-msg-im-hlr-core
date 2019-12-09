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

#include "XmsgImUsrContracts.h"

XmsgImUsrContracts::XmsgImUsrContracts()
{

}

void XmsgImUsrContracts::addCtp(shared_ptr<XmsgImHlrContractsColl> ctp)
{
	this->contracts[ctp->cgt->toString()] = ctp;
}

shared_ptr<XmsgImHlrContractsColl> XmsgImUsrContracts::getCtp(const string& cgt)
{
	auto it = this->contracts.find(cgt);
	return it == this->contracts.end() ? nullptr : it->second;
}

bool XmsgImUsrContracts::delCtp(SptrCgt cgt)
{
	return this->contracts.erase(cgt->toString()) == 1;
}

void XmsgImUsrContracts::addBlackList(shared_ptr<XmsgImHlrContractsBlackListColl> ctp)
{
	this->blackList[ctp->ctp->toString()] = ctp;
}

shared_ptr<XmsgImHlrContractsBlackListColl> XmsgImUsrContracts::getBlackList(const string& cgt)
{
	auto it = this->blackList.find(cgt);
	return it == this->blackList.end() ? nullptr : it->second;
}

bool XmsgImUsrContracts::delBlackList(SptrCgt cgt)
{
	return this->blackList.erase(cgt->toString()) == 1;
}

XmsgImUsrContracts::~XmsgImUsrContracts()
{

}

