/*
 * $Id$
 * 
 * Wrapper class for GUID.
 * 
 * Copyright (C) 2008 Dorothea Wachmann
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see http://www.gnu.org/licenses/.
 */
#include "os.h"
#include "util/xmldocument.h"
#include "util/comptr.h"
#include "util/comstring.h"
#include "util/logstream.h"
#include "util/comlogstream.h"
#include "exception/comexception.h"

using namespace bvr20983::COM;

namespace bvr20983
{

  namespace util
  {
    
    const CLSID CLSID_Msxml2_DOMDocument_5_0 = 
    {0x88D969E5,0xF192,0x11D4,{0xA6,0x5F,0x00,0x40,0x96,0x32,0x51,0xE5}};
    
    const IID IID_IXMLDOMDocument = 
    {0x2933BF81,0x7B36,0x11D2,{0xB2,0x0E,0x00,0xC0,0x4F,0x98,0x3E,0x60}};
    
    /**
     *
     */
    XMLDocument::XMLDocument(LPCOLESTR domDocProgId) :
      m_pXmlDoc(domDocProgId,_T("IXMLDOMDocument2"))
    { 
      THROW_COMEXCEPTION( m_pXmlDoc->put_async(VARIANT_FALSE) );
      THROW_COMEXCEPTION( m_pXmlDoc->put_validateOnParse(VARIANT_TRUE) );
      THROW_COMEXCEPTION( m_pXmlDoc->put_resolveExternals(VARIANT_TRUE) );
      
      THROW_COMEXCEPTION( m_pXmlDoc->setProperty(_T("SelectionLanguage"),COVariant(_T("XPath"))) );
      THROW_COMEXCEPTION( m_pXmlDoc->setProperty(_T("SelectionNamespaces"),COVariant( _T("xmlns:v='http://bvr20983.berlios.de'"))) );
    }
    
    /**
     *
     */
    boolean XMLDocument::Load(LPCTSTR fileName)
    { boolean                   result = false;
      VARIANT_BOOL              status;
      COMPtr<IXMLDOMParseError> pXMLErr;
      COMString                 bstr;
      COVariant                 var(fileName);
      
      THROW_COMEXCEPTION( m_pXmlDoc->load(var,&status) );
      
      if( status!=VARIANT_TRUE ) 
      {
        THROW_COMEXCEPTION( m_pXmlDoc->get_parseError(&pXMLErr) );
        THROW_COMEXCEPTION( pXMLErr->get_reason(&bstr) );
        
        LOGGER_ERROR<<_T("Failed to load DOM from file ")<<fileName<<_T(". ")<<bstr<<endl;
      } // of if
      else
        result = true;

/*
      else
      { 
        THROW_COMEXCEPTION(m_pXmlDoc->get_xml(&bstr));

        LOGGER_INFO<<_T("Parsed XML-Document:")<<endl;
        LOGGER_INFO<<bstr<<endl;
      } 
*/
      return result;
    } // of XMLDocument::Load()
    
    /**
     *
     */
    void XMLDocument::DumpSelection(LPCTSTR xpathExpression)
    { if( !m_pXmlDoc.IsNULL() )
      { COMPtr<IXMLDOMElement>  pXMLDocElement;
        COMPtr<IXMLDOMNodeList> pXMLDomNodeList;
        
        THROW_COMEXCEPTION( m_pXmlDoc->get_documentElement(&pXMLDocElement) );
        THROW_COMEXCEPTION( pXMLDocElement->selectNodes(const_cast<LPTSTR>(xpathExpression),&pXMLDomNodeList) );
        
        COMPtr<IXMLDOMNode> node;

        for( HRESULT hr = pXMLDomNodeList->nextNode(&node);hr==S_OK;hr = pXMLDomNodeList->nextNode(&node) )
        { COMString nodeName;
          COVariant nodeValue;
          const VARIANT*  v = nodeValue;
        
          THROW_COMEXCEPTION( node->get_nodeName(&nodeName) );
          THROW_COMEXCEPTION( node->get_nodeValue(const_cast<VARIANT*>(v)) );
          
          LOGGER_INFO<<nodeName<<_T(":")<<nodeValue<<endl;
        } // of for
      } // of if
    } // of XMLDocument::DumpSelection()

    /**
     *
     */
    boolean XMLDocument::GetNodeValue(LPCTSTR xpath,COVariant& value)
    { boolean result = false;
    
      if( !m_pXmlDoc.IsNULL() )
      { COMPtr<IXMLDOMElement>  pXMLDocElement;
        
        THROW_COMEXCEPTION( m_pXmlDoc->get_documentElement(&pXMLDocElement) );
        
        COMPtr<IXMLDOMNode> node(pXMLDocElement);
        
        result = GetNodeValue(node,xpath,value);
      } // of if
      
      return result;
    } // of XMLDocument::GetNodeValue()
    
    /**
     *
     */
    boolean XMLDocument::GetNodeValue(COMPtr<IXMLDOMNode>& node,LPCTSTR xpath,COVariant& value)
    { boolean            result = false;
      COMPtr<IXMLDOMNode> selectedNode;
    
      if( !node.IsNULL() )
      { HRESULT hr = node->selectSingleNode(const_cast<LPTSTR>(xpath),&selectedNode);
        THROW_COMEXCEPTION( hr );
        
        if( hr==S_OK )
        { const VARIANT* v = value;
          
          THROW_COMEXCEPTION( selectedNode->get_nodeValue(const_cast<VARIANT*>(v)) );
          
          GetProperty(selectedNode,value);
  
          result = true;
        } // of if
      } // of if
      
      return result;
    } // of XMLDocument::GetNodeValue()

    /**
     *
     */
    boolean XMLDocument::GetProperty(COMPtr<IXMLDOMNode>& node,COM::COVariant& value)
    { boolean result = false;
    
      if( value.GetType()==VT_BSTR )
      { BSTR    v        = V_BSTR(value);
        UINT    vLen     = ::SysStringLen(v);
        boolean evalExpr = false;
        
        LPCTSTR propStart = _tcsstr(v,_T("${"));
        LPCTSTR exprStart = _tcsstr(v,_T("#{"));
        LPCTSTR begin     = propStart!=NULL && exprStart!=NULL            ? 
                            (propStart<exprStart ? propStart : exprStart) : 
                            (propStart!=NULL     ? propStart : exprStart);
        
        LPCTSTR end       = _tcsrchr(v,_T('}'));
        
        if( NULL!=begin && NULL!=end && end>begin )
        { COVariant v1(begin+2,end-begin-2);
          COVariant v2;
          LPCTSTR   xpath = V_BSTR(v1);
          
          result = GetNodeValue(node,xpath,v2);

          if( result && v2.GetType()==VT_BSTR )
          { BSTR v22          = V_BSTR(v2);
            UINT resultStrLen = (begin-v) + SysStringLen(v22) + ((v+vLen) - (end+1)) + 1;
            
            LPTSTR buffer = (LPTSTR)calloc(resultStrLen,sizeof(TCHAR));
            
            if( begin>v )
              _tcsncat_s(buffer,resultStrLen,v,begin-v);
              
            _tcscat_s(buffer,resultStrLen,v22);
            
            if( end+1<v+vLen-1 )
              _tcscat_s(buffer,resultStrLen,end+1);
              
            value = buffer;
            
            ::free(buffer);
          } // of if
        } // of if
      } // of if
      
      return result;
    } // of XMLDocument::GetProperty()
  } // of namespace util
} // of namespace bvr20983
/*==========================END-OF-FILE===================================*/
