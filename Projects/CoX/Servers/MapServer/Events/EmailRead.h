/*
 * Super Entity Game Server Project
 * http://github.com/Segs
 * Copyright (c) 2006 - 2017 Super Entity Game Server Team (see Authors.txt)
 * This software is licensed! (See License.txt for details)
 *
 */

#pragma once
#include "GameCommandList.h"

#include <QtCore/QString>

class EmailRead final : public GameCommand
{
public:
    EmailRead(const int id, const QString &message, const QString recipient) : GameCommand(MapEventTypes::evEmailReadCmd),
        m_id(id), m_message(message), m_recipient(recipient)
    {
    }

    void    serializeto(BitStream &bs) const override {
        bs.StorePackedBits(1, type()-MapEventTypes::evFirstServerToClient);
        bs.StoreBits(32, m_id);
        bs.StoreString(m_message);
        bs.StorePackedBits(1, m_count);
        bs.StoreString(m_recipient);
    }

    void    serializefrom(BitStream &src);

protected:
    int m_id;
    QString m_message;
    int m_count = 1; //Doesn't do anything in Issue 0, seemingly, so hardcoding as 1
    QString m_recipient; //Possible misnamed variable, as this is actually the sender in the email's read tab
};
