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
 * $Id: XMLManager.h 137 2005-12-06 15:36:31Z daniel $
 * @file                                                                   */
/* ======================================================================= */


#ifndef __STB_XMLMANAGER__
#define __STB_XMLMANAGER__

#include <Muddleware/Client/Connection.h>

class TiXmlElement;

//BEGIN_NAMESPACE_STB

/// High-level interface for messaging with the XML Server
/**
 *  Applications should not use getConnection() (which exposes the low-level
 *  interface) unless the programmer is absolutely sure about what he is
 *  doing with the connection.
 *  Requests to the database can be registered via registerOperation(), which
 *  returns a client wide unique identifier. The application must use call getAnswer()
 *  using this identifier until it gets a reply - otherwise the XMLManager will
 *  store the answer and never delete it (memory leak). It is the applications
 *  duty to delete retrieved Answers.
 */
class  XMLManager
{
public:
    virtual ~XMLManager()
    {}

    /// Returns the internal Connection object
    /**
     * The Connection object should usually not be used
     * directly. In special cases such as startup or shutdown
     * it nevertheless can be handy to have more low-level
     * access to the XML database.
     */
    virtual MUDDLEWARE::Connection* getConnection() = 0;

    /// Registers an operation to be sent to the XML server
    /**
     * The message will be sent to the server during the next sent out
     * together with all other messages that were registered. The method
     * returns an identifier which can be used to query the result with
     * getAnswerForIdentifier(). It is not required to set an identifier
     * manually since registerXMLDbOperation() will do that automatically.
     * XML requests are sent out and retrieved once per frame. Do not expect
     * a reply before the next frame.
     */
    virtual MUDDLEWARE::Operation::IDENTIFIER registerOperation(MUDDLEWARE::Operation* nOp, bool nAutoDestroy=false) = 0;


    /// Retrieves an answer for a previously registered operation
    /**
     * Use the value registerXMLDbOperation() returned for nIdentifier.
     * The ownership of the Answer object moves from the XMLManager to the
     * calling instance. It is the duty of the calling instance to free
     * memory lateron by calling delete onto the Answer object.
     * NOTE: it is important that answers are queried for all identifiers
     * that were returned by registerOperation() lateron because memory
     * leaks can happen otherwise!
     */
    virtual MUDDLEWARE::Answer* getAnswer(MUDDLEWARE::Operation::IDENTIFIER nIdentifier) = 0;


    /// Takes a string and returns a TiXmlElement node created from that string
    /**
     * Will return NULL if anything goes wrong (parsing, etc).
     * The calling instance takes over owenship and has to destroy the element
     * by calling delete when the element is no longer needed.
     */
    virtual TiXmlElement* getElementFromString(const char* nElement) = 0;
};


//END_NAMESPACE_STB

#endif //__STB_XMLMANAGER__

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
