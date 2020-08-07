// Copyright (c) 2005-2014 Code Synthesis Tools CC
//
// This program was generated by CodeSynthesis XSD, an XML Schema to
// C++ data binding compiler.
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License version 2 as
// published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
//
// In addition, as a special exception, Code Synthesis Tools CC gives
// permission to link this program with the Xerces-C++ library (or with
// modified versions of Xerces-C++ that use the same license as Xerces-C++),
// and distribute linked combinations including the two. You must obey
// the GNU General Public License version 2 in all respects for all of
// the code used other than Xerces-C++. If you modify this copy of the
// program, you may extend this exception to your version of the program,
// but you are not obligated to do so. If you do not wish to do so, delete
// this exception statement from your version.
//
// Furthermore, Code Synthesis Tools CC makes a special exception for
// the Free/Libre and Open Source Software (FLOSS) which is described
// in the accompanying FLOSSE file.
//

// Begin prologue.
//
//
// End prologue.

#include <xsd/cxx/pre.hxx>

#include "SCL.hxx"

namespace SCL
{
  // SCL
  // 

  const SCL::Header_type& SCL::
  Header () const
  {
    return this->Header_.get ();
  }

  SCL::Header_type& SCL::
  Header ()
  {
    return this->Header_.get ();
  }

  void SCL::
  Header (const Header_type& x)
  {
    this->Header_.set (x);
  }

  void SCL::
  Header (::std::auto_ptr< Header_type > x)
  {
    this->Header_.set (x);
  }

  const SCL::Substation_sequence& SCL::
  Substation () const
  {
    return this->Substation_;
  }

  SCL::Substation_sequence& SCL::
  Substation ()
  {
    return this->Substation_;
  }

  void SCL::
  Substation (const Substation_sequence& s)
  {
    this->Substation_ = s;
  }

  const SCL::Communication_optional& SCL::
  Communication () const
  {
    return this->Communication_;
  }

  SCL::Communication_optional& SCL::
  Communication ()
  {
    return this->Communication_;
  }

  void SCL::
  Communication (const Communication_type& x)
  {
    this->Communication_.set (x);
  }

  void SCL::
  Communication (const Communication_optional& x)
  {
    this->Communication_ = x;
  }

  void SCL::
  Communication (::std::auto_ptr< Communication_type > x)
  {
    this->Communication_.set (x);
  }

  const SCL::IED_sequence& SCL::
  IED () const
  {
    return this->IED_;
  }

  SCL::IED_sequence& SCL::
  IED ()
  {
    return this->IED_;
  }

  void SCL::
  IED (const IED_sequence& s)
  {
    this->IED_ = s;
  }

  const SCL::DataTypeTemplates_optional& SCL::
  DataTypeTemplates () const
  {
    return this->DataTypeTemplates_;
  }

  SCL::DataTypeTemplates_optional& SCL::
  DataTypeTemplates ()
  {
    return this->DataTypeTemplates_;
  }

  void SCL::
  DataTypeTemplates (const DataTypeTemplates_type& x)
  {
    this->DataTypeTemplates_.set (x);
  }

  void SCL::
  DataTypeTemplates (const DataTypeTemplates_optional& x)
  {
    this->DataTypeTemplates_ = x;
  }

  void SCL::
  DataTypeTemplates (::std::auto_ptr< DataTypeTemplates_type > x)
  {
    this->DataTypeTemplates_.set (x);
  }
}

#include <xsd/cxx/xml/dom/parsing-source.hxx>

namespace SCL
{
  // SCL
  //

  SCL::
  SCL (const Header_type& Header)
  : ::SCL::tBaseElement (),
    Header_ (Header, this),
    Substation_ (this),
    Communication_ (this),
    IED_ (this),
    DataTypeTemplates_ (this)
  {
  }

  SCL::
  SCL (::std::auto_ptr< Header_type > Header)
  : ::SCL::tBaseElement (),
    Header_ (Header, this),
    Substation_ (this),
    Communication_ (this),
    IED_ (this),
    DataTypeTemplates_ (this)
  {
  }

  SCL::
  SCL (const SCL& x,
       ::xml_schema::flags f,
       ::xml_schema::container* c)
  : ::SCL::tBaseElement (x, f, c),
    Header_ (x.Header_, f, this),
    Substation_ (x.Substation_, f, this),
    Communication_ (x.Communication_, f, this),
    IED_ (x.IED_, f, this),
    DataTypeTemplates_ (x.DataTypeTemplates_, f, this)
  {
  }

  SCL::
  SCL (const ::xercesc::DOMElement& e,
       ::xml_schema::flags f,
       ::xml_schema::container* c)
  : ::SCL::tBaseElement (e, f | ::xml_schema::flags::base, c),
    Header_ (this),
    Substation_ (this),
    Communication_ (this),
    IED_ (this),
    DataTypeTemplates_ (this)
  {
    if ((f & ::xml_schema::flags::base) == 0)
    {
      ::xsd::cxx::xml::dom::parser< char > p (e, true, false, false);
      this->parse (p, f);
    }
  }

  void SCL::
  parse (::xsd::cxx::xml::dom::parser< char >& p,
         ::xml_schema::flags f)
  {
    this->::SCL::tBaseElement::parse (p, f);

    for (; p.more_content (); p.next_content (false))
    {
      const ::xercesc::DOMElement& i (p.cur_element ());
      const ::xsd::cxx::xml::qualified_name< char > n (
        ::xsd::cxx::xml::dom::name< char > (i));

      // Header
      //
      if (n.name () == "Header" && n.namespace_ () == "http://www.iec.ch/61850/2006/SCL")
      {
        ::std::auto_ptr< Header_type > r (
          Header_traits::create (i, f, this));

        if (!Header_.present ())
        {
          this->Header_.set (r);
          continue;
        }
      }

      // Substation
      //
      if (n.name () == "Substation" && n.namespace_ () == "http://www.iec.ch/61850/2006/SCL")
      {
        ::std::auto_ptr< Substation_type > r (
          Substation_traits::create (i, f, this));

        this->Substation_.push_back (r);
        continue;
      }

      // Communication
      //
      if (n.name () == "Communication" && n.namespace_ () == "http://www.iec.ch/61850/2006/SCL")
      {
        ::std::auto_ptr< Communication_type > r (
          Communication_traits::create (i, f, this));

        if (!this->Communication_)
        {
          this->Communication_.set (r);
          continue;
        }
      }

      // IED
      //
      if (n.name () == "IED" && n.namespace_ () == "http://www.iec.ch/61850/2006/SCL")
      {
        ::std::auto_ptr< IED_type > r (
          IED_traits::create (i, f, this));

        this->IED_.push_back (r);
        continue;
      }

      // DataTypeTemplates
      //
      if (n.name () == "DataTypeTemplates" && n.namespace_ () == "http://www.iec.ch/61850/2006/SCL")
      {
        ::std::auto_ptr< DataTypeTemplates_type > r (
          DataTypeTemplates_traits::create (i, f, this));

        if (!this->DataTypeTemplates_)
        {
          this->DataTypeTemplates_.set (r);
          continue;
        }
      }

      break;
    }

    if (!Header_.present ())
    {
      throw ::xsd::cxx::tree::expected_element< char > (
        "Header",
        "http://www.iec.ch/61850/2006/SCL");
    }
  }

  SCL* SCL::
  _clone (::xml_schema::flags f,
          ::xml_schema::container* c) const
  {
    return new class SCL (*this, f, c);
  }

  SCL& SCL::
  operator= (const SCL& x)
  {
    if (this != &x)
    {
      static_cast< ::SCL::tBaseElement& > (*this) = x;
      this->Header_ = x.Header_;
      this->Substation_ = x.Substation_;
      this->Communication_ = x.Communication_;
      this->IED_ = x.IED_;
      this->DataTypeTemplates_ = x.DataTypeTemplates_;
    }

    return *this;
  }

  SCL::
  ~SCL ()
  {
  }
}

#include <istream>
#include <xsd/cxx/xml/sax/std-input-source.hxx>
#include <xsd/cxx/tree/error-handler.hxx>

namespace SCL
{
  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (const ::std::string& u,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::tree::error_handler< char > h;

    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        u, h, p, f));

    h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

    return ::std::auto_ptr< ::SCL::SCL > (
      ::SCL::SCL_ (
        d, f | ::xml_schema::flags::own_dom, p));
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (const ::std::string& u,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        u, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::auto_ptr< ::SCL::SCL > (
      ::SCL::SCL_ (
        d, f | ::xml_schema::flags::own_dom, p));
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (const ::std::string& u,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        u, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::auto_ptr< ::SCL::SCL > (
      ::SCL::SCL_ (
        d, f | ::xml_schema::flags::own_dom, p));
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::std::istream& is,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is);
    return ::SCL::SCL_ (isrc, f, p);
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::std::istream& is,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is);
    return ::SCL::SCL_ (isrc, h, f, p);
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::std::istream& is,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::sax::std_input_source isrc (is);
    return ::SCL::SCL_ (isrc, h, f, p);
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::std::istream& is,
        const ::std::string& sid,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
    return ::SCL::SCL_ (isrc, f, p);
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::std::istream& is,
        const ::std::string& sid,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::auto_initializer i (
      (f & ::xml_schema::flags::dont_initialize) == 0,
      (f & ::xml_schema::flags::keep_dom) == 0);

    ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
    return ::SCL::SCL_ (isrc, h, f, p);
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::std::istream& is,
        const ::std::string& sid,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::xml::sax::std_input_source isrc (is, sid);
    return ::SCL::SCL_ (isrc, h, f, p);
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::xercesc::InputSource& i,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xsd::cxx::tree::error_handler< char > h;

    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        i, h, p, f));

    h.throw_if_failed< ::xsd::cxx::tree::parsing< char > > ();

    return ::std::auto_ptr< ::SCL::SCL > (
      ::SCL::SCL_ (
        d, f | ::xml_schema::flags::own_dom, p));
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::xercesc::InputSource& i,
        ::xml_schema::error_handler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        i, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::auto_ptr< ::SCL::SCL > (
      ::SCL::SCL_ (
        d, f | ::xml_schema::flags::own_dom, p));
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::xercesc::InputSource& i,
        ::xercesc::DOMErrorHandler& h,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
      ::xsd::cxx::xml::dom::parse< char > (
        i, h, p, f));

    if (!d.get ())
      throw ::xsd::cxx::tree::parsing< char > ();

    return ::std::auto_ptr< ::SCL::SCL > (
      ::SCL::SCL_ (
        d, f | ::xml_schema::flags::own_dom, p));
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (const ::xercesc::DOMDocument& doc,
        ::xml_schema::flags f,
        const ::xml_schema::properties& p)
  {
    if (f & ::xml_schema::flags::keep_dom)
    {
      ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d (
        static_cast< ::xercesc::DOMDocument* > (doc.cloneNode (true)));

      return ::std::auto_ptr< ::SCL::SCL > (
        ::SCL::SCL_ (
          d, f | ::xml_schema::flags::own_dom, p));
    }

    const ::xercesc::DOMElement& e (*doc.getDocumentElement ());
    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (e));

    if (n.name () == "SCL" &&
        n.namespace_ () == "http://www.iec.ch/61850/2006/SCL")
    {
      ::std::auto_ptr< ::SCL::SCL > r (
        ::xsd::cxx::tree::traits< ::SCL::SCL, char >::create (
          e, f, 0));
      return r;
    }

    throw ::xsd::cxx::tree::unexpected_element < char > (
      n.name (),
      n.namespace_ (),
      "SCL",
      "http://www.iec.ch/61850/2006/SCL");
  }

  ::std::auto_ptr< ::SCL::SCL >
  SCL_ (::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > d,
        ::xml_schema::flags f,
        const ::xml_schema::properties&)
  {
    ::xml_schema::dom::auto_ptr< ::xercesc::DOMDocument > c (
      ((f & ::xml_schema::flags::keep_dom) &&
       !(f & ::xml_schema::flags::own_dom))
      ? static_cast< ::xercesc::DOMDocument* > (d->cloneNode (true))
      : 0);

    ::xercesc::DOMDocument& doc (c.get () ? *c : *d);
    const ::xercesc::DOMElement& e (*doc.getDocumentElement ());

    const ::xsd::cxx::xml::qualified_name< char > n (
      ::xsd::cxx::xml::dom::name< char > (e));

    if (f & ::xml_schema::flags::keep_dom)
      doc.setUserData (::xml_schema::dom::tree_node_key,
                       (c.get () ? &c : &d),
                       0);

    if (n.name () == "SCL" &&
        n.namespace_ () == "http://www.iec.ch/61850/2006/SCL")
    {
      ::std::auto_ptr< ::SCL::SCL > r (
        ::xsd::cxx::tree::traits< ::SCL::SCL, char >::create (
          e, f, 0));
      return r;
    }

    throw ::xsd::cxx::tree::unexpected_element < char > (
      n.name (),
      n.namespace_ (),
      "SCL",
      "http://www.iec.ch/61850/2006/SCL");
  }
}

#include <xsd/cxx/post.hxx>

// Begin epilogue.
//
//
// End epilogue.

