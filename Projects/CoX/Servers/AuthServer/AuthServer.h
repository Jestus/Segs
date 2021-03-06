/*
 * Super Entity Game Server Project
 * http://segs.sf.net/
 * Copyright (c) 2006 - 2016 Super Entity Game Server Team (see Authors.txt)
 * This software is licensed! (See License.txt for details)
 *
 */

#pragma once
// segs includes
#include "AuthDatabase/AuthDBSyncEvents.h"
#include "EventProcessor.h"

// ACE includes
#include <ace/INET_Addr.h>
#include <ace/Synch.h>

class AuthLink;
class AuthHandler;
struct ClientAcceptor;
class AuthServer : public EventProcessor
{
public:
                                    AuthServer();
                                    ~AuthServer();

        bool                        ShutDown();
        bool                        ReadConfigAndRestart();

protected:
        bool                        Run();
        ClientAcceptor *            m_acceptor;     //!< ace acceptor wrapping AuthClientService
        ACE_INET_Addr               m_location;     //!< address this server will bind at.
        bool                        m_running;      //!< true if this server is running
        ACE_Thread_Mutex            m_mutex;        //!< used to prevent multiple threads accessing config reload function
        std::unique_ptr<AuthHandler> m_handler;     //!< holds the AuthHandler

        // EventProcessor interface
        void dispatch(SEGSEvent *ev);
};
