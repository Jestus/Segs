/*
* Super Entity Game Server Project
* http://segs.sf.net/
* Copyright (c) 2009 Super Entity Game Server Team (see Authors.txt)
* This software is licensed! (See License.txt for details)
*

*/
#include "HashStorage.h"
#include "GameEvents.h"
#include "NetStructures/Character.h"
#include "NetStructures/Costume.h"
#include "GameDatabase/GameDBSyncEvents.h"
#include "GameData/playerdata_definitions.h"

#include <QtCore/QDebug>

// SpecHash<std::string,val>
//
// get hash index of given key
//uint32_t get_hash_idx(const std::string &)
//{
//    return 0;
//}

void UpdateServer::dependent_dump() const
{
    qDebug() << "Game:pktCS_ServerUpdate";
    qDebug() << "{";

    qDebug() << "    buildDate"<<m_build_date;
    //uint8_t clientInfo[16];
    qDebug() << "    localMapServer"<<localMapServer;
    qDebug() << "    currentVersion"<<currentVersion;
    //uint8_t segsHash[16];
    qDebug().nospace() << "    authId 0x"<<QString::number(authID,16);
    qDebug().nospace() << "    authCookie 0x"<<QString::number(authCookie,16);
    qDebug() << "    accountName"<<accountName;
    qDebug() << "}";
}

void UpdateServer::serializeto( BitStream &tgt ) const
{
    tgt.StorePackedBits(1, 2); //opcode
    tgt.StorePackedBits(1, m_build_date);
    tgt.StorePackedBits(1, 0); // flags
    tgt.StoreString(currentVersion);
    tgt.StoreBitArray(clientInfo,sizeof(clientInfo)*8);
    tgt.StorePackedBits(1, authID);
    tgt.StoreBits(32, authCookie);
    tgt.StoreString(accountName);
}

void UpdateServer::serializefrom( BitStream &src )
{
    m_build_date = src.GetPackedBits(1);
    /*uint32_t t =*/ src.GetPackedBits(1);
    src.GetString(currentVersion);
    src.GetBitArray(clientInfo,sizeof(clientInfo)*8);
    authID = src.GetPackedBits(1);
    authCookie = src.GetBits(32);
    src.GetString(accountName);
}

void GameEntryError::serializeto( BitStream &tgt ) const
{
    tgt.StorePackedBits(1, 3); //opcode
    tgt.StoreString(m_error);
}
void GameEntryError::serializefrom( BitStream &tgt )
{
    tgt.GetString(m_error);
}

void CharacterSlots::serializeto( BitStream &tgt ) const
{
    tgt.StorePackedBits(1, 2); //opcode
    tgt.StorePackedBits(1,static_cast<uint32_t>(m_data->m_max_slots));
    assert(m_data->m_max_slots>0);
    for(size_t i=0; i<m_data->m_max_slots; i++)
    {
        Character converted;
        PlayerData player_data;
        toActualCharacter(m_data->m_characters[i],converted,player_data);
        converted.serializetoCharsel(tgt);
    }
    //tgt.StoreBitArray(m_clientinfo,128);
}

void CharacterSlots::serializefrom( BitStream &)
{

}

void UpdateCharacter::serializeto( BitStream &bs ) const
{
    bs.StorePackedBits(1,5); // opcode
    bs.StorePackedBits(1,m_index);
}

void UpdateCharacter::serializefrom( BitStream &bs )
{
    m_index = bs.GetPackedBits(1);
}

void CharacterResponse::serializeto( BitStream &bs ) const
{
    GameAccountResponseCharacterData indexed_character(m_data->get_character(m_index));
    Character converted;
    PlayerData player_data;
    toActualCharacter(indexed_character,converted,player_data);
    bs.StorePackedBits(1,6); // opcode

    if(indexed_character.m_name.compare("EMPTY")!=0)// actual character was read from db
    {
        bs.StorePackedBits(1,m_index);
        converted.getCurrentCostume()->storeCharselParts(bs);
    }
    else
    {
        bs.StorePackedBits(1,-1);
        bs.StorePackedBits(1,0); // 0 parts
    }
}

void CharacterResponse::serializefrom( BitStream &bs )
{
    bs.GetPackedBits(1);
    assert(!"TODO");
}

void DeletionAcknowledged::serializeto( BitStream &tgt ) const
{
    tgt.StorePackedBits(1,5); // opcode 5
}
