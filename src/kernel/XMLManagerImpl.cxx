/* ========================================================================
 * Copyright (C) 2004-2005  Graz University of Technology
 *
 * This framework is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this framework; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * For further information please contact Dieter Schmalstieg under
 * <schmalstieg@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
 * Graz University of Technology, Institut für Maschinelles Sehen und Darstellen,
 * Inffeldgasse 16a, 8010 Graz, Austria.
 * ========================================================================
 * PROJECT: Studierstube
 * ======================================================================== */
/** @author   Daniel Wagner, modified by Bernhard Reitinger for STB4
 *
 * $Id: XMLManagerImpl.cpp 137 2005-12-06 15:36:31Z daniel $
 * @file                                                                   */
/* ======================================================================= */

#ifdef ENABLE_MUDDLEWARE

#include <stb/kernel/XMLManagerImpl.h>
#include <tinyxml/tinyxml.h>

BEGIN_NAMESPACE_STB

XMLManagerImpl::XMLManagerImpl(MUDDLEWARE::Connection* nConnection) : connection(nConnection)
{
    request = MUDDLEWARE::Request::create();
    reply = MUDDLEWARE::Reply::create();
    opCounter = 0;
    requestHeaderId = 0xffffffff;
}


XMLManagerImpl::~XMLManagerImpl()
{
    if(request)
        MUDDLEWARE::Request::destroy(request);

    if(reply)
    {
        reply->removeAllAnswersWithoutDeleting();
        MUDDLEWARE::Reply::destroy(reply);
    }

    // delete all operations
    for(AnswersMap::iterator iter=answersMap.begin(); iter!=answersMap.end(); iter++)
    {
        MUDDLEWARE::Answer* answer = const_cast<MUDDLEWARE::Answer*>(iter->second);
        MUDDLEWARE::Answer::destroy(answer);
    }

    if(connection)
    {
        connection->shutdown();
        MUDDLEWARE::Connection::destroy(connection);
    }
}


MUDDLEWARE::Operation::IDENTIFIER
XMLManagerImpl::registerOperation(MUDDLEWARE::Operation* nOp, bool nAutoDestroy)
{
    if(nOp->getOperation() != MUDDLEWARE::Operation::OP_UNREGISTER_WATCHDOG)
        nOp->setClientData(++opCounter);

    request->addOperation(nOp);

    if(nAutoDestroy)
        autoDestroyMap.insert(nOp->getClientData());

    return nOp->getClientData();
}


MUDDLEWARE::Answer*
XMLManagerImpl::getAnswer(MUDDLEWARE::Operation::IDENTIFIER nIdentifier)
{
    if(nIdentifier==MUDDLEWARE::Message::INVALID_CLIENTID)
        return NULL;

    // unbind the answer. it is now no longer
    // stored in the hash-map. the calling instance now has
    // to take care for it!
    //
    AnswersMap::iterator it = answersMap.find(nIdentifier);
    if(it != answersMap.end())
    {
        const MUDDLEWARE::Answer* answer = it->second;
        answersMap.erase(it);
        return const_cast<MUDDLEWARE::Answer*>(answer);
    }
    else
        return NULL;
}


bool
XMLManagerImpl::init(const char* nServerAddr)
{
    if(!connection)
        return false;

    serverAddr = nServerAddr;

    return connection->init(nServerAddr);
}


bool
XMLManagerImpl::reconnect()
{
    if(connection)
    {
        connection->shutdown();
        MUDDLEWARE::Connection::destroy(connection);
    }

    connection = MUDDLEWARE::Connection::create();
    return connection->init(serverAddr.c_str());
}


bool
XMLManagerImpl::send()
{
    if(!connection)
        return false;

    //if(request->getNumOperations()==0)
    //	registerOperation(new MUDDLEWARE::Operation(MUDDLEWARE::Operation::OP_EMPTY));

    bool sendOk = connection->send(*request);
    requestHeaderId = request->getHeaderId();

    if(sendOk)
    {
        MUDDLEWARE::Request::destroy(request);
        request = MUDDLEWARE::Request::create();
    }

    return sendOk;
}


bool
XMLManagerImpl::receive()
{
    if(!connection)
        return false;

    bool ret = connection->receive(requestHeaderId, *reply);

    // since we can not garantie that all answer will be read
    // before the next reply arrives we store them in a hash map.
    // the hash map always contains the latest version of an
    // answer for a specific clientdata.
    storeAnswers();

    MUDDLEWARE::Reply::destroy(reply);
    reply = MUDDLEWARE::Reply::create();

    return ret;
}


void
XMLManagerImpl::storeAnswers()
{
    for(int i=0; i<reply->getNumAnswers(); i++)
    {
        MUDDLEWARE::Answer		*oldAnswer = NULL;
        MUDDLEWARE::Answer		*newAnswer = const_cast<MUDDLEWARE::Answer*>(reply->getAnswerByIndex(i));

        AutoDestroyMap::iterator itDestroy = autoDestroyMap.find(newAnswer->getClientData());
        if(itDestroy != autoDestroyMap.end())
        {
            autoDestroyMap.erase(itDestroy);
            MUDDLEWARE::Answer::destroy(newAnswer);
            continue;
        }

        // we only store the latest version. if an entry already
        // exists we have to find and delete it.
        //
        AnswersMap::iterator it = answersMap.find(newAnswer->getClientData());
        if(it != answersMap.end())
        {
            oldAnswer = const_cast<MUDDLEWARE::Answer*>(it->second);
            MUDDLEWARE::Answer::destroy(oldAnswer);
            it->second = newAnswer;
        }
        else
            answersMap[newAnswer->getClientData()] = newAnswer;
    }

    reply->removeAllAnswersWithoutDeleting();
}


TiXmlElement*
XMLManagerImpl::getElementFromString(const char* nElement)
{
    TiXmlElement* newElem = new TiXmlElement("New Element");

    if(!newElem->Parse(nElement, NULL, TIXML_DEFAULT_ENCODING))
    {
        delete newElem;
        return NULL;
    }

    return newElem;
}

END_NAMESPACE_STB

#endif

//========================================================================
// End of $FILENAME$
//========================================================================
// Local Variables:
// mode: c++
// c-basic-offset: 4
// eval: (c-set-offset 'substatement-open 0)
// eval: (c-set-offset 'case-label '+)
// eval: (c-set-offset 'statement 'c-lineup-runin-statements)
// eval: (setq indent-tabs-mode nil)
// End:
//========================================================================
