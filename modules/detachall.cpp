/*
 * Copyright (C) 2004-2009  See the AUTHORS file for details.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */
 
#include <znc/User.h>
#include <znc/Modules.h>
#include <znc/Chan.h>
#include <znc/IRCNetwork.h>

class CDetachAllMod : public CModule {
public:
    MODCONSTRUCTOR(CDetachAllMod) {}
    ~CDetachAllMod() {}

    EModRet OnStatusCommand(CString& sCommand)
    {
        if(sCommand.Token(0).Equals("detachall"))
        {
            DetachAll();
            return HALT;
        }
        return CONTINUE;
    }

    EModRet OnUserRaw(CString& sLine)
    {
        if(sLine.Token(0).Equals("detachall"))
        {
            DetachAll();
            return HALT;
        }
        return CONTINUE;
    }

protected:
    void DetachAll()
    {
        
        CIRCNetwork* pNetwork = GetNetwork();

        const vector<CChan *>& Channels = pNetwork->GetChans();

        for (unsigned int c = 0; c < Channels.size(); c++)
        {
            CChan* chan = Channels[c];
            if (chan->IsDetached()) {
                m_pUser->PutStatusNotice(Channels[c]->GetName() + " already detached", m_pClient);
            }
            else {
                chan->DetachUser();
                m_pUser->PutStatusNotice(Channels[c]->GetName() + " DETACHED", m_pClient);
            }
        }
    }
};

USERMODULEDEFS(CDetachAllMod, "Adds /detachall and /znc detachall commands.")
