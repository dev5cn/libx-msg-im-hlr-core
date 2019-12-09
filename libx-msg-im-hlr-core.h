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

#ifndef LIBX_MSG_IM_HLR_CORE_H_
#define LIBX_MSG_IM_HLR_CORE_H_

#include "XmsgImHlrCfg.h"
#include "XmsgImHlrMisc.h"
#include "XmsgImHlrTcpLog.h"
#include "coll/XmsgImHlrCfgColl.h"
#include "coll/XmsgImHlrContractsBlackListColl.h"
#include "coll/XmsgImHlrContractsColl.h"
#include "coll/XmsgImHlrSysEventColl.h"
#include "coll/XmsgImHlrUsrDatColl.h"
#include "coll/XmsgImHlrUsrEventColl.h"
#include "contracts/XmsgImHlrContractsMgr.h"
#include "contracts/XmsgImUsrContracts.h"
#include "evn/XmsgImSysEvent.h"
#include "evn/XmsgImUsrEvent.h"
#include "ne/XmsgAp.h"
#include "ne/XmsgImAuth.h"
#include "ne/XmsgNe.h"
#include "ne/XmsgNeUsr.h"
#include "usr/XmsgImClient.h"
#include "usr/XmsgImClientStatusEventQueue.h"
#include "usr/XmsgImUsr.h"
#include "usr/XmsgImUsrMgr.h"

#endif 