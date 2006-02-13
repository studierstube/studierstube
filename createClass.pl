if(@ARGC < 1){
   
}
$argc = 0;
while (<@ARGV>) {
        $argc ++;
}
if ($argc < 1) {
    printf "usage: createClass className";
    exit(1);
}

$className = $ARGV[0];
$classNameUpper=uc($className);
open(header ,">" . $className . ".h");
print header
"/* ======================================================================== 
* Copyright (C) 2005  Graz University of Technology  
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
* <schmalstieg\@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,  
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,  
* Austria.  
* ========================================================================  
* PROJECT: Studierstube  
* ======================================================================== */  
/** The header file for the $className class.  
*  
* \@author Denis Kalkofen  
*  
* \$Id: $className.h 25 2005-11-28 16:11:59Z denis \$  
* \@file                                                                   */  
/* ======================================================================= */  

#ifndef _" . $classNameUpper . "_H_
#define _" . $classNameUpper . "_H_
#include \"common/macros.h\"

BEGIN_NAMESPACE_STB
/**
*	
*/
class " . $className . "
{
public:
    /**
    *     The Constructor	
    */
    " . $className . "();

    /**
    *     The destructor.
    */
    ~" . $className . "();

protected:	

private:
	
};// class 
END_NAMESPACE_STB
#endif//_" . $classNameUpper . "_H_
//========================================================================
// End of " . $className . ".h 
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
";
close(header);


open(cxx ,">" . $className . ".cxx");
printf cxx
"/* ========================================================================
* Copyright (C) 2005  Graz University of Technology
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
* <schmalstieg\@icg.tu-graz.ac.at> or write to Dieter Schmalstieg,
* Graz University of Technology, Inffeldgasse 16a, A8010 Graz,
* Austria.
* ========================================================================
* PROJECT: Studierstube
* ======================================================================== */
/** The header file for the $className class.
*
* \@author Denis Kalkofen
*
* \$Id: $className.cxx 25 2005-11-28 16:11:59Z denis \$
* \@file                                                                   */
/* ======================================================================= */

#include \"" . $className . ".h\"
BEGIN_NAMESPACE_STB


" . $className . "::" . $className . "()
{
   //nil
}

" . $className . "::~" . $className . "()
{
   //nil
}

END_NAMESPACE_STB
//========================================================================
// End of ". $className . ".cxx
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
";
close(cxx);
