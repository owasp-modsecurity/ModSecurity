// A Bison parser, made by GNU Bison 3.0.2.

// Locations for Bison parsers in C++

// Copyright (C) 2002-2013 Free Software Foundation, Inc.

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

// As a special exception, you may create a larger work that contains
// part or all of the Bison parser skeleton and distribute that work
// under terms of your choice, so long as that work isn't itself a
// parser generator using the skeleton or a modified version thereof
// as a parser skeleton.  Alternatively, if you modify or redistribute
// the parser skeleton itself, you may (at your option) remove this
// special exception, which will cause the skeleton and the resulting
// Bison output files to be licensed under the GNU General Public
// License without this special exception.

// This special exception was added by the Free Software Foundation in
// version 2.2 of Bison.

/**
 ** \file location.hh
 ** Define the yy::location class.
 */

#ifndef YY_YY_LOCATION_HH_INCLUDED
# define YY_YY_LOCATION_HH_INCLUDED

# include "position.hh"


namespace yy {
#line 46 "location.hh" // location.cc:291
  /// Abstract a location.
  class location
  {
  public:

    /// Construct a location from \a b to \a e.
    location (const position& b, const position& e)
      : begin (b)
      , end (e)
    {
    }

    /// Construct a 0-width location in \a p.
    explicit location (const position& p = position ())
      : begin (p)
      , end (p)
    {
    }

    /// Construct a 0-width location in \a f, \a l, \a c.
    explicit location (std::string* f,
                       unsigned int l = 1u,
                       unsigned int c = 1u)
      : begin (f, l, c)
      , end (f, l, c)
    {
    }


    /// Initialization.
    void initialize (std::string* f = YY_NULLPTR,
                     unsigned int l = 1u,
                     unsigned int c = 1u)
    {
      begin.initialize (f, l, c);
      end = begin;
    }

    /** \name Line and Column related manipulators
     ** \{ */
  public:
    /// Reset initial location to final location.
    void step ()
    {
      begin = end;
    }

    /// Extend the current location to the COUNT next columns.
    void columns (int count = 1)
    {
      end += count;
    }

    /// Extend the current location to the COUNT next lines.
    void lines (int count = 1)
    {
      end.lines (count);
    }
    /** \} */


  public:
    /// Beginning of the located region.
    position begin;
    /// End of the located region.
    position end;
  };

  /// Join two location objects to create a location.
  inline location operator+ (location res, const location& end)
  {
    res.end = end.end;
    return res;
  }

  /// Change end position in place.
  inline location& operator+= (location& res, int width)
  {
    res.columns (width);
    return res;
  }

  /// Change end position.
  inline location operator+ (location res, int width)
  {
    return res += width;
  }

  /// Change end position in place.
  inline location& operator-= (location& res, int width)
  {
    return res += -width;
  }

  /// Change end position.
  inline location operator- (const location& begin, int width)
  {
    return begin + -width;
  }

  /// Compare two location objects.
  inline bool
  operator== (const location& loc1, const location& loc2)
  {
    return loc1.begin == loc2.begin && loc1.end == loc2.end;
  }

  /// Compare two location objects.
  inline bool
  operator!= (const location& loc1, const location& loc2)
  {
    return !(loc1 == loc2);
  }

  /** \brief Intercept output stream redirection.
   ** \param ostr the destination output stream
   ** \param loc a reference to the location to redirect
   **
   ** Avoid duplicate information.
   */
  template <typename YYChar>
  inline std::basic_ostream<YYChar>&
  operator<< (std::basic_ostream<YYChar>& ostr, const location& loc)
  {
    unsigned int end_col = 0 < loc.end.column ? loc.end.column - 1 : 0;
    ostr << loc.begin// << "(" << loc.end << ") "
;
    if (loc.end.filename
        && (!loc.begin.filename
            || *loc.begin.filename != *loc.end.filename))
      ostr << '-' << loc.end.filename << ':' << loc.end.line << '.' << end_col;
    else if (loc.begin.line < loc.end.line)
      ostr << '-' << loc.end.line << '.' << end_col;
    else if (loc.begin.column < end_col)
      ostr << '-' << end_col;
    return ostr;
  }


} // yy
#line 187 "location.hh" // location.cc:291
#endif // !YY_YY_LOCATION_HH_INCLUDED
