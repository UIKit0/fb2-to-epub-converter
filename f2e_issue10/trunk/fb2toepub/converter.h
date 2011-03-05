//
//  Copyright (C) 2010, 2011 Alexey Bobkov
//
//  This file is part of Fb2toepub converter.
//
//  Fb2toepub converter is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Fb2toepub converter is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Fb2toepub converter.  If not, see <http://www.gnu.org/licenses/>.
//


#ifndef FB2TOEPUB__CONVERTER_H
#define FB2TOEPUB__CONVERTER_H

#include <vector>
#include <set>
#include "streamzip.h"
#include "scanner.h"
#include "translit.h"

namespace Fb2ToEpub
{

    // Different threshold file sizes for splitting, depending on next element
    // (see implementation of pass1 and pass2 section())
    const std::size_t MAX_UNIT_SIZE     = (FB2TOEPUB_MAX_TEXT_FILE_SIZE);
    //const std::size_t MAX_UNIT_SIZE     = 25000UL;
    const std::size_t UNIT_SIZE0        = MAX_UNIT_SIZE*1/2;
    const std::size_t UNIT_SIZE1        = MAX_UNIT_SIZE*3/4;
    const std::size_t UNIT_SIZE2        = MAX_UNIT_SIZE*5/6;

    //-----------------------------------------------------------------------
    struct Unit
    {
        enum BodyType
        {
            BODY_NONE,
            MAIN,
            NOTES,
            COMMENTS
        };
        enum Type
        {
            UNIT_NONE,
            COVERPAGE,
            ANNOTATION,
            IMAGE,
            TITLE,
            SECTION
        };

        // common data
        BodyType                    bodyType_;      // body type
        Type                        type_;          // unit type
        int                         id_;            // inique id (among same type units)
        String                      title_;         // unit title from fb2 book, if any (e.g. section title)

        // pass 1 data
        std::size_t                 size_;          // approx. size of unit
        int                         parent_;        // paremt unit index, of -1 in no parent
        strvector                   refIds_;        // refernce ids collected in this unit
        std::set<String>            refs_;          // refernces from this unit to another place
        String                      noteRefId_;     // if it is note or comment section and is has an id, it should have anchor

        // pass 2 data
        String                      file_;          // file name to store all unit text
#if !FB2TOEPUB_TOC_REFERS_FILES_ONLY
        String                      fileId_;        // reference id inside file (for toc)
#endif
        int                         level_;         // toc level

        Unit() : type_(UNIT_NONE), size_(0), parent_(0), level_(0) {}
        Unit(BodyType bodyType, Type type, int id, int parent) : bodyType_(bodyType), type_(type), id_(id), size_(0), parent_(parent), level_(0) {}
    };
    typedef std::vector<Unit>       UnitArray;


    //-----------------------------------------------------------------------
    typedef std::map<String, String>  ReferenceMap;   // (refid -> file) or (refid -> refid)


    //-----------------------------------------------------------------------
    // PRINT INFO
    //-----------------------------------------------------------------------
    void FB2TOEPUB_DECL DoPrintInfo(const String &in);

    //-----------------------------------------------------------------------
    // CONVERTION PASS 1 (DETERMINE DOCUMENT STRUCTURE AND COLLECT ALL CROSS-REFERENCES INSIDE THE FB2 FILE)
    //-----------------------------------------------------------------------
    void FB2TOEPUB_DECL DoConvertionPass1(LexScanner *scanner, UnitArray *units);

    //-----------------------------------------------------------------------
    // CONVERTER PASS 2 (CREATE EPUB DOCUMENT)
    //-----------------------------------------------------------------------
    void FB2TOEPUB_DECL DoConvertionPass2  (LexScanner *scanner,
                                            const strvector &css,
                                            const strvector &fonts,
                                            const strvector &mfonts,
                                            XlitConv *xlitConv,
                                            UnitArray *units,
                                            OutPackStm *pout);


};  //namespace Fb2ToEpub

#endif
