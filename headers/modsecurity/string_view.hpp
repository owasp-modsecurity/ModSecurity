/**
 * \file string_view.hpp
 *
 * \brief This header contains the definition of the string_view type, as
 *        described by the C++17 standard.
 *
 * \author Matthew Rodusek (matthew.rodusek@gmail.com)
 * \copyright Matthew Rodusek
 */

/*
 * The MIT License (MIT)
 *
 * Licensed under the MIT License <http://opensource.org/licenses/MIT>.
 * Copyright (c) 2016 Matthew Rodusek <http://rodusek.me>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
#ifndef BPSTD_STRING_VIEW_HPP
#define BPSTD_STRING_VIEW_HPP

#include <algorithm>  // std::
#include <string>     // std::char_traits
#include <ostream>    // std::basic_ostream
#include <cstddef>    // std::size_t
#include <memory>     // std::allocator
#include <stdexcept>  // std::out_of_range
#include <iterator>   // std::reverse_iterator
namespace bpstd { // back-port std

  ////////////////////////////////////////////////////////////////////////////
  /// \brief A wrapper around non-owned strings.
  ///
  /// This is an implementation of the C++17 string_view proposal
  ///
  /// \ingroup core
  ////////////////////////////////////////////////////////////////////////////
  template<
    typename CharT,
    typename Traits = std::char_traits<CharT>
  >
  class basic_string_view final
  {
    //------------------------------------------------------------------------
    // Public Member Types
    //------------------------------------------------------------------------
  public:

    using char_type   = CharT;
    using traits_type = Traits;
    using size_type   = std::size_t;

    using value_type      = CharT;
    using reference       = value_type&;
    using const_reference = const value_type&;
    using pointer         = value_type*;
    using const_pointer   = const value_type*;

    using iterator       = const CharT*;
    using const_iterator = const CharT*;
    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    //------------------------------------------------------------------------
    // Public Members
    //------------------------------------------------------------------------
  public:

    static constexpr size_type npos = size_type(-1);

    //------------------------------------------------------------------------
    // Constructors
    //------------------------------------------------------------------------
  public:

    /// \brief Default constructs a basic_string_view without any content
    constexpr basic_string_view() noexcept;

    /// \brief Constructs a basic_string_view by copying another one
    ///
    /// \param other the string view being copied
    constexpr basic_string_view(const basic_string_view& other) noexcept = default;

    /// \brief Constructs a basic_string_view by moving anothe rone
    ///
    /// \param other the string view being moved
    constexpr basic_string_view(basic_string_view&& other) noexcept = default;

    /// \brief Constructs a basic_string_view from a std::basic_string
    ///
    /// \param str the string to view
    template<typename Allocator>
    basic_string_view(const std::basic_string<CharT,Traits,Allocator>& str) noexcept;

    /// \brief Constructs a basic_string_view from an ansi-string
    ///
    /// \param str the string to view
    constexpr basic_string_view(const char_type* str) noexcept;

    /// \brief Constructs a basic_string_view from an ansi string of a given size
    ///
    /// \param str the string to view
    /// \param count the size of the string
    constexpr basic_string_view(const char_type* str, size_type count) noexcept;

    //------------------------------------------------------------------------
    // Assignment
    //------------------------------------------------------------------------
  public:

    /// \brief Assigns a basic_string_view from an ansi-string
    ///
    /// \param view the string to view
    /// \return reference to \c (*this)
    basic_string_view& operator=(const basic_string_view& view) = default;

    //------------------------------------------------------------------------
    // Capacity
    //------------------------------------------------------------------------
  public:

    /// \brief Returns the length of the string, in terms of bytes
    ///
    /// \return the length of the string, in terms of bytes
    constexpr size_type size() const noexcept;

    /// \copydoc basic_string_view::size
    constexpr size_type length() const noexcept;

    /// \brief The largest possible number of char-like objects that can be
    ///        referred to by a basic_string_view.
    /// \return Maximum number of characters
    constexpr size_type max_size() const noexcept;

    /// \brief Returns whether the basic_string_view is empty
    ///        (i.e. whether its length is 0).
    ///
    /// \return whether the basic_string_view is empty
    constexpr bool empty() const noexcept;

    //------------------------------------------------------------------------
    // Element Access
    //------------------------------------------------------------------------
  public:

    /// \brief Gets the ansi-string of the current basic_string_view
    ///
    /// \return the ansi-string pointer
    constexpr const char_type* c_str() const noexcept;

    /// \brief Gets the data of the current basic_string_view
    ///
    /// \note This is an alias of #c_str
    ///
    /// \return the data this basic_string_view contains
    constexpr const char_type* data() const noexcept;

    /// \brief Accesses the element at index \p pos
    ///
    /// \param pos the index to access
    /// \return const reference to the character
    constexpr const_reference operator[](size_type pos) const noexcept;

    /// \brief Accesses the element at index \p pos
    ///
    /// \param pos the index to access
    /// \return const reference to the character
    constexpr const_reference at(size_type pos) const;

    /// \brief Access the first character of the string
    ///
    /// \note Undefined behavior if basic_string_view is empty
    ///
    /// \return reference to the first character of the string
    constexpr const_reference front() const noexcept;

    /// \brief References the last character of the string
    ///
    /// \note Undefined behavior if basic_string_view is empty
    ///
    /// \return reference to the last character of the string
    constexpr const_reference back() const noexcept;

    //------------------------------------------------------------------------
    // Modifiers
    //------------------------------------------------------------------------
  public:

    /// \brief Moves the start of the view forward by n characters.
    ///
    /// The behavior is undefined if n > size().
    ///
    /// \param n number of characters to remove from the start of the view
    void remove_prefix(size_type n) noexcept;

    /// \brief Moves the end of the view back by n characters.
    ///
    /// The behavior is undefined if n > size().
    ///
    /// \param n number of characters to remove from the end of the view
    void remove_suffix(size_type n) noexcept;

    /// \brief Exchanges the view with that of v.
    ///
    /// \param v view to swap with
    void swap(basic_string_view& v) noexcept;

    //------------------------------------------------------------------------
    // Conversions
    //------------------------------------------------------------------------
  public:

    /// \brief Creates a basic_string with a copy of the content of the current view.
    ///
    /// \tparam Allocator type used to allocate internal storage
    /// \param a Allocator instance to use for allocating the new string
    ///
    /// \return A basic_string containing a copy of the characters of the current view.
    template<class Allocator = std::allocator<CharT>>
    constexpr std::basic_string<CharT, Traits, Allocator>
      to_string(const Allocator& a = Allocator()) const;

    /// \copydoc basic_string_view::to_string
    template<class Allocator>
    explicit constexpr operator std::basic_string<CharT, Traits, Allocator>() const;

    //------------------------------------------------------------------------
    // Operations
    //------------------------------------------------------------------------
  public:

    /// \brief Copies the substring [pos, pos + rcount) to the character string pointed
    ///        to by dest, where rcount is the smaller of count and size() - pos.
    ///
    /// \param dest pointer to the destination character string
    /// \param count requested substring length
    /// \param pos position of the first character
    size_type copy( char_type* dest,
                    size_type count = npos,
                    size_type pos = 0 ) const;

    /// \brief Returns a substring of this viewed string
    ///
    /// \param pos the position of the first character in the substring
    /// \param len the length of the substring
    /// \return the created substring
    basic_string_view substr(size_t pos = 0, size_t len = npos) const;

    //------------------------------------------------------------------------

    /// \brief Compares two character sequences
    ///
    /// \param v view to compare
    /// \return negative value if this view is less than the other character
    ///         sequence, zero if the both character sequences are equal, positive
    ///         value if this view is greater than the other character sequence.
    int compare(basic_string_view v) const noexcept;

    /// \brief Compares two character sequences
    ///
    /// \param pos   position of the first character in this view to compare
    /// \param count number of characters of this view to compare
    /// \param v     view to compare
    /// \return negative value if this view is less than the other character
    ///         sequence, zero if the both character sequences are equal, positive
    ///         value if this view is greater than the other character sequence.
    int compare(size_type pos, size_type count, basic_string_view v) const;

    /// \brief Compares two character sequences
    ///
    /// \param pos1   position of the first character in this view to compare
    /// \param count1 number of characters of this view to compare
    /// \param v      view to compare
    /// \param pos2   position of the second character in this view to compare
    /// \param count2 number of characters of the given view to compare
    /// \return negative value if this view is less than the other character
    ///         sequence, zero if the both character sequences are equal, positive
    ///         value if this view is greater than the other character sequence.
    int compare( size_type pos1, size_type count1, basic_string_view v,
                 size_type pos2, size_type count2 ) const;

    /// \brief Compares two character sequences
    ///
    /// \param s pointer to the character string to compare to
    /// \return negative value if this view is less than the other character
    ///         sequence, zero if the both character sequences are equal, positive
    ///         value if this view is greater than the other character sequence.
    int compare(const char_type* s) const;

    /// \brief Compares two character sequences
    ///
    /// \param pos   position of the first character in this view to compare
    /// \param count number of characters of this view to compare
    /// \param s pointer to the character string to compare to
    /// \return negative value if this view is less than the other character
    ///         sequence, zero if the both character sequences are equal, positive
    ///         value if this view is greater than the other character sequence.
    int compare(size_type pos, size_type count, const char_type* s) const;

    /// \brief Compares two character sequences
    ///
    /// \param pos   position of the first character in this view to compare
    /// \param count1 number of characters of this view to compare
    /// \param s pointer to the character string to compare to
    /// \param count2 number of characters of the given view to compare
    /// \return negative value if this view is less than the other character
    ///         sequence, zero if the both character sequences are equal, positive
    ///         value if this view is greater than the other character sequence.
    int compare( size_type pos, size_type count1, const char_type* s,
                 size_type count2 ) const;

    //------------------------------------------------------------------------

    size_type find(basic_string_view v, size_type pos = 0) const;

    size_type find(char_type c, size_type pos = 0) const;

    size_type find(const char_type* s, size_type pos, size_type count) const;

    size_type find(const char_type* s, size_type pos = 0) const;

    //------------------------------------------------------------------------

    size_type rfind(basic_string_view v, size_type pos = npos) const;

    size_type rfind(char_type c, size_type pos = npos) const;

    size_type rfind(const char_type* s, size_type pos, size_type count) const;

    size_type rfind(const char_type* s, size_type pos = npos) const;

    //------------------------------------------------------------------------

    size_type find_first_of(basic_string_view v, size_type pos = 0) const;

    size_type find_first_of(char_type c, size_type pos = 0) const;

    size_type find_first_of(const char_type* s, size_type pos, size_type count) const;

    size_type find_first_of(const char_type* s, size_type pos = 0) const;

    //------------------------------------------------------------------------

    size_type find_last_of(basic_string_view v, size_type pos = npos) const;

    size_type find_last_of(char_type c, size_type pos = npos) const;

    size_type find_last_of(const char_type* s, size_type pos, size_type count) const;

    size_type find_last_of(const char_type* s, size_type pos = npos) const;

    //------------------------------------------------------------------------

    size_type find_first_not_of(basic_string_view v, size_type pos = 0) const;

    size_type find_first_not_of(char_type c, size_type pos = 0) const;

    size_type find_first_not_of(const char_type* s, size_type pos, size_type count) const;

    size_type find_first_not_of(const char_type* s, size_type pos = 0) const;

    //------------------------------------------------------------------------

    size_type find_last_not_of(basic_string_view v, size_type pos = npos) const;

    size_type find_last_not_of(char_type c, size_type pos = npos) const;

    size_type find_last_not_of(const char_type* s, size_type pos, size_type count) const;

    size_type find_last_not_of(const char_type* s, size_type pos = npos) const;

    //------------------------------------------------------------------------
    // Iterators
    //------------------------------------------------------------------------
  public:

    /// \{
    /// \brief Retrieves the begin iterator for this basic_string_view
    ///
    /// \return the begin iterator
    const_iterator begin() const noexcept;
    const_iterator cbegin() const noexcept;
    /// \}

    /// \{
    /// \brief Retrieves the end iterator for this basic_string_view
    ///
    /// \return the end iterator
    const_iterator end() const noexcept;
    const_iterator cend() const noexcept;
    /// \}

    /// \{
    /// \brief Retrieves the reverse begin iterator for this basic_string_view
    ///
    /// \return the reverse begin iterator
    const_reverse_iterator rbegin() const noexcept;
    const_reverse_iterator rend() const noexcept;
    /// \}

    /// \{
    /// \brief Retrieves the reverse end iterator for this basic_string_view
    ///
    /// \return the reverse end iterator
    const_reverse_iterator crbegin() const noexcept;
    const_reverse_iterator crend() const noexcept;
    /// \}

    //------------------------------------------------------------------------
    // Private Member
    //------------------------------------------------------------------------
  private:

    const char_type* m_str;  ///< The internal string type
    size_type        m_size; ///< The size of this string

    /// \brief Checks whether \p c is one of the characters in \p str
    ///
    /// \param c the character to check
    /// \param str the characters to compare against
    /// \return true if \p c is one of the characters in \p str
    static bool is_one_of(CharT c, basic_string_view str);
  };

  template <typename CharT, typename Traits>
  const typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::npos;

  //--------------------------------------------------------------------------
  // Public Functions
  //--------------------------------------------------------------------------

  /// \brief Overload for ostream output of basic_string_view
  ///
  /// \param o   The output stream to print to
  /// \param str the string to print
  /// \return reference to the output stream
  template<typename CharT, typename Traits>
  std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>& o,
                                               const basic_string_view<CharT,Traits>& str);

  template<typename CharT, typename Traits>
  void swap(basic_string_view<CharT,Traits>& lhs,
            basic_string_view<CharT,Traits>& rhs) noexcept;

  //--------------------------------------------------------------------------
  // Comparison Functions
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  bool operator==(const basic_string_view<CharT,Traits>& lhs,
                  const basic_string_view<CharT,Traits>& rhs) noexcept;
  template<typename CharT, typename Traits>
  bool operator!=(const basic_string_view<CharT,Traits>& lhs,
                  const basic_string_view<CharT,Traits>& rhs) noexcept;
  template<typename CharT, typename Traits>
  bool operator<(const basic_string_view<CharT,Traits>& lhs,
                 const basic_string_view<CharT,Traits>& rhs) noexcept;
  template<typename CharT, typename Traits>
  bool operator>(const basic_string_view<CharT,Traits>& lhs,
                 const basic_string_view<CharT,Traits>& rhs) noexcept;
  template<typename CharT, typename Traits>
  bool operator<=(const basic_string_view<CharT,Traits>& lhs,
                  const basic_string_view<CharT,Traits>& rhs) noexcept;
  template<typename CharT, typename Traits>
  bool operator>=(const basic_string_view<CharT,Traits>& lhs,
                  const basic_string_view<CharT,Traits>& rhs) noexcept;

  //--------------------------------------------------------------------------
  // Type Aliases
  //--------------------------------------------------------------------------

  using string_view    = basic_string_view<char>;
  using wstring_view   = basic_string_view<wchar_t>;
  using u16string_view = basic_string_view<char16_t>;
  using u32string_view = basic_string_view<char32_t>;

} // namespace bpstd

#ifndef BPSTD_DETAIL_STRING_VIEW_INL
#define BPSTD_DETAIL_STRING_VIEW_INL

namespace bpstd {

  //--------------------------------------------------------------------------
  // Constructor
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline constexpr basic_string_view<CharT,Traits>::basic_string_view()
    noexcept
    : m_str(nullptr),
      m_size(0)
  {

  }

  template<typename CharT, typename Traits>
  template<typename Allocator>
  inline basic_string_view<CharT,Traits>::basic_string_view(const std::basic_string<CharT,Traits,Allocator>& str)
    noexcept
    : m_str(str.c_str()),
      m_size(str.size())
  {

  }

  template<typename CharT, typename Traits>
  inline constexpr basic_string_view<CharT,Traits>::basic_string_view(const char_type* str)
    noexcept
    : m_str(str),
      m_size(traits_type::length(str))
  {

  }

  template<typename CharT, typename Traits>
  inline constexpr basic_string_view<CharT,Traits>::basic_string_view(const char_type* str, size_type count)
    noexcept
    : m_str(str),
      m_size(count)
  {

  }

  //--------------------------------------------------------------------------
  // Capacity
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline constexpr typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::size()
    const noexcept
  {
    return m_size;
  }

  template<typename CharT, typename Traits>
  inline constexpr typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::length()
    const noexcept
  {
    return size();
  }

  template<typename CharT, typename Traits>
  inline constexpr typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::max_size()
    const noexcept
  {
    return npos - 1;
  }

  template<typename CharT, typename Traits>
  inline constexpr bool basic_string_view<CharT,Traits>::empty()
    const noexcept
  {
    return m_size == 0;
  }

  //--------------------------------------------------------------------------
  // Element Access
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline constexpr const typename basic_string_view<CharT,Traits>::char_type*
    basic_string_view<CharT,Traits>::c_str()
    const noexcept
  {
    return m_str;
  }

  template<typename CharT, typename Traits>
  inline constexpr const typename basic_string_view<CharT,Traits>::char_type*
    basic_string_view<CharT,Traits>::data()
    const noexcept
  {
    return m_str;
  }

  template<typename CharT, typename Traits>
  inline constexpr typename basic_string_view<CharT,Traits>::const_reference
    basic_string_view<CharT,Traits>::operator[](size_type pos)
    const noexcept
  {
    return m_str[pos];
  }

  template<typename CharT, typename Traits>
  inline constexpr typename basic_string_view<CharT,Traits>::const_reference
    basic_string_view<CharT,Traits>::at(size_type pos)
    const
  {
    return pos < m_size ? m_str[pos] : throw std::out_of_range("Input out of range in basic_string_view::at"), m_str[pos];
  }

  template<typename CharT, typename Traits>
  inline constexpr typename basic_string_view<CharT,Traits>::const_reference
    basic_string_view<CharT,Traits>::front( )
    const noexcept
  {
    return *m_str;
  }

  template<typename CharT, typename Traits>
  inline constexpr typename basic_string_view<CharT,Traits>::const_reference
    basic_string_view<CharT,Traits>::back( )
    const noexcept
  {
    return m_str[m_size-1];
  }

  //--------------------------------------------------------------------------
  // Modifiers
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline void
    basic_string_view<CharT,Traits>::remove_prefix(size_type n)
    noexcept
  {
    m_str += n, m_size -= n;
  }

  template<typename CharT, typename Traits>
  inline void
    basic_string_view<CharT,Traits>::remove_suffix(size_type n)
    noexcept
  {
    m_size -= n;
  }

  template<typename CharT, typename Traits>
  inline void
    basic_string_view<CharT,Traits>::swap(basic_string_view& v)
    noexcept
  {
    using std::swap;
    swap(m_size,v.m_size);
    swap(m_str,v.m_str);
  }

  //--------------------------------------------------------------------------
  // Conversions
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  template<class Allocator>
  inline constexpr std::basic_string<CharT, Traits, Allocator>
    basic_string_view<CharT,Traits>::to_string(const Allocator& a)
    const
  {
    return std::basic_string<CharT,Traits,Allocator>(m_str, m_size, a);
  }

  template<typename CharT, typename Traits>
  template<class Allocator>
  inline constexpr basic_string_view<CharT,Traits>::operator
    std::basic_string<CharT, Traits, Allocator>()
    const
  {
    return std::basic_string<CharT,Traits,Allocator>(m_str, m_size);
  }

  //--------------------------------------------------------------------------
  // String Operations
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::copy(char_type* dest,
                                          size_type count,
                                          size_type pos)
    const
  {
    if(pos >= m_size) {
      throw std::out_of_range("Index out of range in basic_string_view::copy");
    }

    const size_type rcount = std::min(m_size - pos,count+1);
    std::copy( m_str + pos, m_str + pos + rcount, dest);
    return rcount;
  }

  template<typename CharT, typename Traits>
  inline basic_string_view<CharT,Traits>
    basic_string_view<CharT,Traits>::substr(size_type pos,
                                            size_type len)
    const
  {
    const size_type max_length = pos > m_size ? 0 : m_size - pos;

    if (pos > size()) {
      throw std::out_of_range("Index out of range in basic_string_view::substr");
    }

    return basic_string_view(m_str + pos, std::min(len, max_length) );
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline int basic_string_view<CharT,Traits>::compare(basic_string_view v)
    const noexcept
  {
    const size_type rlen = std::min(m_size,v.m_size);
    const int compare = Traits::compare(m_str,v.m_str,rlen);

    return (compare ? compare : (m_size < v.m_size ? -1 : (m_size > v.m_size ? 1 : 0)));
  }

  template<typename CharT, typename Traits>
  inline int basic_string_view<CharT,Traits>::compare(size_type pos,
                                                      size_type count,
                                                      basic_string_view v)
    const
  {
    return substr(pos,count).compare(v);
  }

  template<typename CharT, typename Traits>
  inline int basic_string_view<CharT,Traits>::compare(size_type pos1,
                                                      size_type count1,
                                                      basic_string_view v,
                                                      size_type pos2,
                                                      size_type count2)
    const
  {
    return substr(pos1,count1).compare(v.substr(pos2,count2));
  }

  template<typename CharT, typename Traits>
  inline int basic_string_view<CharT,Traits>::compare(const char_type* s)
    const
  {
    return compare(basic_string_view<CharT,Traits>(s));
  }

  template<typename CharT, typename Traits>
  inline int basic_string_view<CharT,Traits>::compare(size_type pos,
                                                      size_type count,
                                                      const char_type* s)
    const
  {
    return substr(pos, count).compare(basic_string_view<CharT,Traits>(s));
  }

  template<typename CharT, typename Traits>
  inline int basic_string_view<CharT,Traits>::compare(size_type pos,
                                                      size_type count1,
                                                      const char_type* s,
                                                      size_type count2)
    const
  {
    return substr(pos, count1).compare(basic_string_view<CharT,Traits>(s, count2));
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find(basic_string_view v,
                                          size_type pos)
    const
  {
    // Can't find a substring if the substring is bigger than this
    if (pos > size()) {
      return npos;
    }
    if ((pos + v.size()) > size()) {
      return npos;
    }

    const auto offset = pos;
    const auto increments = size() - v.size();

    for (auto i = 0u; i <= increments; ++i) {
      const auto j = i + offset;
      if (substr(j, v.size()) == v) {
        return j;
      }
    }
    return npos;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find(char_type c,
                                          size_type pos)
    const
  {
    return find(basic_string_view<CharT,Traits>(&c, 1), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find(const char_type* s, size_type pos,
                                          size_type count)
    const
  {
    return find(basic_string_view<CharT,Traits>(s, count), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find(const char_type* s,
                                          size_type pos)
    const
  {
    return find(basic_string_view<CharT,Traits>(s), pos);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::rfind(basic_string_view v,
                                           size_type pos)
    const
  {
    if (empty()) {
      return v.empty() ? 0u : npos;
    }
    if (v.empty()) {
      return std::min(size() - 1, pos);
    }
    if (v.size() > size()) {
      return npos;
    }

    auto i = std::min(pos, (size() - v.size()));
    while (i != npos) {
      if (substr(i, v.size()) == v) {
        return i;
      }
      --i;
    }

    return npos;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::rfind(char_type c,
                                           size_type pos)
    const
  {
    return rfind(basic_string_view<CharT,Traits>(&c, 1), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::rfind(const char_type* s, size_type pos,
                                           size_type count)
    const
  {
    return rfind(basic_string_view<CharT,Traits>(s, count), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::rfind(const char_type* s,
                                           size_type pos)
    const
  {
    return rfind(basic_string_view<CharT,Traits>(s), pos);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_of(basic_string_view v,
                                                   size_type pos)
    const
  {
    const auto max_index = size();
    for (auto i = pos; i < max_index;  ++i) {
      if (is_one_of(m_str[i],v)) {
        return i;
      }
    }

    return npos;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_of(char_type c,
                                                   size_type pos)
    const
  {
    return find_first_of(basic_string_view<CharT,Traits>(&c, 1), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_of(const char_type* s, size_type pos,
                                                   size_type count)
    const
  {
    return find_first_of(basic_string_view<CharT,Traits>(s, count), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_of(const char_type* s,
                                                   size_type pos)
    const
  {
    return find_first_of(basic_string_view<CharT,Traits>(s), pos);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_of(basic_string_view v,
                                                  size_type pos)
    const
  {
    if (empty()) {
      return npos;
    }
    const auto max_index = std::min(size() - 1, pos);
    for (auto i = 0u; i <= max_index;  ++i) {
      const auto j = max_index - i;

      if (is_one_of(m_str[j],v)) {
        return j;
      }
    }

    return npos;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_of(char_type c,
                                                  size_type pos)
    const
  {
    return find_last_of(basic_string_view<CharT,Traits>(&c, 1), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_of(const char_type* s, size_type pos,
                                                  size_type count)
    const
  {
    return find_last_of(basic_string_view<CharT,Traits>(s, count), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_of(const char_type* s,
                                                  size_type pos)
    const
  {
    return find_last_of(basic_string_view<CharT,Traits>(s), pos);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_not_of(basic_string_view v,
                                                       size_type pos)
    const
  {
    const auto max_index = size();
    for (auto i = pos; i < max_index;  ++i) {
      if (!is_one_of(m_str[i],v)) {
        return i;
      }
    }

    return npos;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_not_of(char_type c,
                                                       size_type pos)
    const
  {
    return find_first_not_of(basic_string_view<CharT,Traits>(&c, 1), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_not_of(const char_type* s,
                                                       size_type pos,
                                                       size_type count)
    const
  {
    return find_first_not_of(basic_string_view<CharT,Traits>(s, count), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_first_not_of(const char_type* s,
                                                       size_type pos)
    const
  {
    return find_first_not_of(basic_string_view<CharT,Traits>(s), pos);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_not_of(basic_string_view v,
                                                      size_type pos)
    const
  {
    if (empty()) {
      return npos;
    }
    const auto max_index = std::min(size() - 1, pos);
    for (auto i = 0u; i <= max_index;  ++i) {
      const auto j = max_index - i;

      if (!is_one_of(m_str[j],v)) {
        return j;
      }
    }

    return npos;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_not_of(char_type c,
                                                      size_type pos)
    const
  {
    return find_last_not_of(basic_string_view<CharT,Traits>(&c, 1), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_not_of(const char_type* s,
                                                      size_type pos,
                                                      size_type count)
    const
  {
    return find_last_not_of(basic_string_view<CharT,Traits>(s, count), pos);
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::size_type
    basic_string_view<CharT,Traits>::find_last_not_of(const char_type* s,
                                                      size_type pos)
    const
  {
    return find_last_not_of(basic_string_view<CharT,Traits>(s), pos);
  }

  //--------------------------------------------------------------------------
  // Iterator
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_iterator
    basic_string_view<CharT,Traits>::begin()
    const noexcept
  {
    return m_str;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_iterator
    basic_string_view<CharT,Traits>::cbegin()
    const noexcept
  {
    return begin();
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_iterator
    basic_string_view<CharT,Traits>::end()
    const noexcept
  {
    return m_str + m_size;
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_iterator
    basic_string_view<CharT,Traits>::cend()
    const noexcept
  {
    return cend();
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_reverse_iterator
    basic_string_view<CharT,Traits>::rbegin()
    const noexcept
  {
    return const_reverse_iterator{end()};
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_reverse_iterator
    basic_string_view<CharT,Traits>::crbegin()
    const noexcept
  {
    return rbegin();
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_reverse_iterator
    basic_string_view<CharT,Traits>::rend()
    const noexcept
  {
    return const_reverse_iterator{begin()};
  }

  template<typename CharT, typename Traits>
  inline typename basic_string_view<CharT,Traits>::const_reverse_iterator
    basic_string_view<CharT,Traits>::crend()
    const noexcept
  {
    return crend();
  }

  template <typename CharT, typename Traits>
  inline bool basic_string_view<CharT,Traits>::is_one_of(CharT c,
                                                         basic_string_view str)
  {
    for (auto s : str) {
      if (c == s) {
        return true;
      }
    }
    return false;
  }

  //--------------------------------------------------------------------------
  // Public Functions
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  std::basic_ostream<CharT,Traits>& operator<<(std::basic_ostream<CharT,Traits>& o,
                                               const basic_string_view<CharT,Traits>& str)
  {
    o.write(str.data(),str.size());
    return o;
  }

  template<typename CharT, typename Traits>
  inline void swap(basic_string_view<CharT,Traits>& lhs,
                   basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    lhs.swap(rhs);
  }

  //--------------------------------------------------------------------------
  // Comparison Functions
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline bool operator==(const basic_string_view<CharT,Traits>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return lhs.compare(rhs) == 0;
  }

  template<typename CharT, typename Traits>
  inline bool operator==(basic_string_view<CharT,Traits> lhs,
                         const CharT* rhs)
    noexcept
  {
    return lhs == basic_string_view<CharT,Traits>(rhs);
  }

  template<typename CharT, typename Traits>
  inline bool operator==(const CharT* lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return basic_string_view<CharT,Traits>(lhs) == rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator==(const std::basic_string<CharT,Traits,Allocator>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
  {
    return basic_string_view<CharT,Traits>(lhs) == rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator==(const basic_string_view<CharT,Traits>& lhs,
                         const std::basic_string<CharT,Traits,Allocator>& rhs)
  {
    return lhs == basic_string_view<CharT,Traits>(rhs);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline bool operator!=(const basic_string_view<CharT,Traits>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return lhs.compare(rhs) != 0;
  }

  template<typename CharT, typename Traits>
  inline bool operator!=(const basic_string_view<CharT,Traits>& lhs,
                         const CharT* rhs)
    noexcept
  {
    return lhs != basic_string_view<CharT,Traits>(rhs);
  }

  template<typename CharT, typename Traits>
  inline bool operator!=(const CharT* lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return basic_string_view<CharT,Traits>(lhs) != rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator!=(const std::basic_string<CharT,Traits,Allocator>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
  {
    return basic_string_view<CharT,Traits>(lhs) != rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator!=(const basic_string_view<CharT,Traits>& lhs,
                         const std::basic_string<CharT,Traits,Allocator>& rhs)
  {
    return lhs != basic_string_view<CharT,Traits>(rhs);
  }
  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline bool operator<(const basic_string_view<CharT,Traits>& lhs,
                        const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return lhs.compare(rhs) < 0;
  }

  template<typename CharT, typename Traits>
  inline bool operator<(const basic_string_view<CharT,Traits>& lhs,
                        const CharT* rhs)
    noexcept
  {
    return lhs < basic_string_view<CharT,Traits>(rhs);
  }

  template<typename CharT, typename Traits>
  inline bool operator<(const CharT* lhs,
                        const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return basic_string_view<CharT,Traits>(lhs) < rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator<(const std::basic_string<CharT,Traits,Allocator>& lhs,
                        const basic_string_view<CharT,Traits>& rhs)
  {
    return basic_string_view<CharT,Traits>(lhs) < rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator<(const basic_string_view<CharT,Traits>& lhs,
                        const std::basic_string<CharT,Traits,Allocator>& rhs)
  {
    return lhs < basic_string_view<CharT,Traits>(rhs);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline bool operator>(const basic_string_view<CharT,Traits>& lhs,
                        const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return lhs.compare(rhs) > 0;
  }

  template<typename CharT, typename Traits>
  inline bool operator>(const basic_string_view<CharT,Traits>& lhs,
                        const CharT* rhs)
    noexcept
  {
    return lhs > basic_string_view<CharT,Traits>(rhs);
  }

  template<typename CharT, typename Traits>
  inline bool operator>(const CharT* lhs,
                        const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return basic_string_view<CharT,Traits>(lhs) > rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator>(const std::basic_string<CharT,Traits,Allocator>& lhs,
                        const basic_string_view<CharT,Traits>& rhs)
  {
    return basic_string_view<CharT,Traits>(lhs) > rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator>(const basic_string_view<CharT,Traits>& lhs,
                        const std::basic_string<CharT,Traits,Allocator>& rhs)
  {
    return lhs > basic_string_view<CharT,Traits>(rhs);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline bool operator<=(const basic_string_view<CharT,Traits>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return lhs.compare(rhs) <= 0;
  }

  template<typename CharT, typename Traits>
  inline bool operator<=(const basic_string_view<CharT,Traits>& lhs,
                         const CharT* rhs)
    noexcept
  {
    return lhs <= basic_string_view<CharT,Traits>(rhs);
  }

  template<typename CharT, typename Traits>
  inline bool operator<=(const CharT* lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return basic_string_view<CharT,Traits>(lhs) <= rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator<=(const std::basic_string<CharT,Traits,Allocator>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
  {
    return basic_string_view<CharT,Traits>(lhs) <= rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator<=(const basic_string_view<CharT,Traits>& lhs,
                         const std::basic_string<CharT,Traits,Allocator>& rhs)
  {
    return lhs <= basic_string_view<CharT,Traits>(rhs);
  }

  //--------------------------------------------------------------------------

  template<typename CharT, typename Traits>
  inline bool operator>=(const basic_string_view<CharT,Traits>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return lhs.compare(rhs) >= 0;
  }

  template<typename CharT, typename Traits>
  inline bool operator>=(const basic_string_view<CharT,Traits>& lhs,
                         const CharT* rhs)
    noexcept
  {
    return lhs >= basic_string_view<CharT,Traits>(rhs);
  }

  template<typename CharT, typename Traits>
  inline bool operator>=(const CharT* lhs,
                         const basic_string_view<CharT,Traits>& rhs)
    noexcept
  {
    return basic_string_view<CharT,Traits>(lhs) >= rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator>=(const std::basic_string<CharT,Traits,Allocator>& lhs,
                         const basic_string_view<CharT,Traits>& rhs)
  {
    return basic_string_view<CharT,Traits>(lhs) >= rhs;
  }

  template<typename CharT, typename Traits, typename Allocator>
  inline bool operator>=(const basic_string_view<CharT,Traits>& lhs,
                         const std::basic_string<CharT,Traits,Allocator>& rhs)
  {
    return lhs >= basic_string_view<CharT,Traits>(rhs);
  }

} // namespace bpstd

#endif /* BPSTD_DETAIL_STRING_VIEW_INL */

#endif /* BPSTD_STRING_VIEW_HPP */
