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
 * $Id: XMLManagerImpl.h 137 2005-12-06 15:36:31Z daniel $
 * @file                                                                   */
/* ======================================================================= */


#ifndef __STB_XMLMANAGERIMPL__
#define __STB_XMLMANAGERIMPL__

#include "XMLManager.h"

#include <map>
#include <set>

#include <stb/base/string.h>

// Forward declare some XMLDB objects
class Connection;
class Request;
class Reply;

//BEGIN_NAMESPACE_STB

class XMLManagerImpl : public XMLManager
{
public:
    XMLManagerImpl(MUDDLEWARE::Connection* nConnection);
    ~XMLManagerImpl();

    // Implement XMLManager interface
    MUDDLEWARE::Connection* getConnection()  {  return connection;  }
    MUDDLEWARE::Operation::IDENTIFIER registerOperation(MUDDLEWARE::Operation* nOp, bool nAutoDestroy=false);
    MUDDLEWARE::Answer* getAnswer(MUDDLEWARE::Operation::IDENTIFIER nIdentifier);
    TiXmlElement* getElementFromString(const char* nElement);

    bool init(const char* nServerAddr);

    /// Sends out the collected operations to the server
    bool send();

    /// Waits for the server's reply (blocking wait)
    bool receive();

    bool reconnect();

protected:

    void storeAnswers();

    typedef std::map<MUDDLEWARE::Operation::IDENTIFIER, const MUDDLEWARE::Answer*>	AnswersMap;
    typedef std::set<MUDDLEWARE::Operation::IDENTIFIER>							AutoDestroyMap;

    MUDDLEWARE::Connection*	connection;
    stb::string			serverAddr;

    MUDDLEWARE::Request*	request;
    MUDDLEWARE::Reply*		reply;
    unsigned int		opCounter;

    unsigned int		requestHeaderId;

    AnswersMap			answersMap;
    AutoDestroyMap		autoDestroyMap;
};


//BEGIN_NAMESPACE_STB

#endif //__SAM_XMLMANAGERIMPL_HEADERFILE__

//========================================================================
// End of file
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
