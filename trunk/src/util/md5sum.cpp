/*
 * $Id: $
 * 
 * A class for download files using the windows BITS service.
 * 
 * Copyright (C) 2009 Dorothea Wachmann
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
#include "util/md5sum.h"
#include "util/logstream.h"
#include "util/handle.h"
#include "exception/lasterrorexception.h"

using namespace bvr20983;

namespace bvr20983
{
  namespace util
  {
    /**
     *
     */
    CryptoContext::CryptoContext(DWORD dwProvType,DWORD dwFlags,LPCTSTR pszProvider) :
      m_hCryptProv(NULL)
    { THROW_LASTERROREXCEPTION1( ::CryptAcquireContext(&m_hCryptProv,NULL,pszProvider,dwProvType,dwFlags) );
    }

    /**
     *
     */
    CryptoContext::~CryptoContext()
    { if( NULL!=m_hCryptProv) 
        ::CryptReleaseContext(m_hCryptProv, 0);

      m_hCryptProv = NULL;
    }

    /**
     *
     */
    CryptoHash::CryptoHash(const CryptoContext& cryptoContext,ALG_ID algId) : 
      m_hHash(NULL)
    { THROW_LASTERROREXCEPTION1( ::CryptCreateHash(*cryptoContext, algId, 0, 0, &m_hHash) );
    }

    /**
     *
     */
    CryptoHash::~CryptoHash()
    { if( NULL!=m_hHash )
        ::CryptDestroyHash(m_hHash);

      m_hHash = NULL;
    }

    /**
     *
     */
    void CryptoHash::Put(BYTE* pBuffer,DWORD bufferLen)
    { if( NULL!=m_hHash && NULL!=pBuffer )
      { THROW_LASTERROREXCEPTION1( ::CryptHashData(m_hHash, pBuffer, bufferLen, 0) ); }
    } // of CryptoHash::Put()

    /**
     *
     */
    DWORD CryptoHash::Get(auto_ptr<BYTE>& pBuffer) const
    { DWORD dwHashLen = 0;

      if( NULL!=m_hHash )
      { THROW_LASTERROREXCEPTION1( ::CryptGetHashParam(m_hHash,HP_HASHVAL,NULL,&dwHashLen,0) );

        if( dwHashLen>0 )
        { BYTE* hashBuffer = new BYTE[dwHashLen];

          THROW_LASTERROREXCEPTION1( ::CryptGetHashParam(m_hHash,HP_HASHVAL,hashBuffer,&dwHashLen,0) );

          pBuffer.reset(hashBuffer);
        } // of if
      } // of if

      return dwHashLen;
    } // of CryptoHash::Get()


  #ifdef _UNICODE
    wostream& CryptoHash::Dump(wostream& os) const
  #else
    ostream& CryptoHash::Dump(ostream& os) const
  #endif  
    { ios_base::iostate err = 0;
    
  #ifdef _UNICODE
      wostream::sentry opfx(os);
  #else
      ostream::sentry opfx(os);
  #endif  

      try
      {
        if( opfx && NULL!=m_hHash )
        { os<<setfill(_T('0'))<<setw(2)<<hex;
        
          auto_ptr<BYTE> hashValue;

          DWORD hashLen = this->Get(hashValue);

          BYTE* pBuffer = hashValue.get();

          if( NULL!=pBuffer )
            for( DWORD i=0;i<hashLen;i++ )
              os<<pBuffer[i];
          
          os.width(0);
          os.fill(_T(' '));
        } // of if
      }
      catch(...)
      { bool flag = false;
      
        try
        { os.setstate(ios_base::failbit); }
        catch(...)
        { flag = true; }
        
        if( flag ) 
          throw;
      } 
      
      if( err )
        os.setstate(err);
      
      return os;
    } // of CryptoHash::Dump()

    /**
     *
     */
    template<class charT, class Traits>
    basic_ostream<charT, Traits>& operator <<(basic_ostream<charT, Traits >& os,const CryptoHash& h)
    { return h.Dump(os); }

     /**
     *
     */
    MD5Sum::MD5Sum() :
      m_cryptoCtx(PROV_RSA_FULL,CRYPT_VERIFYCONTEXT)
    { 
    }

    /**
     *
     */
    MD5Sum::~MD5Sum()
    { }

 
    /**
     *
     */
    void MD5Sum::CalcFileHash(LPCTSTR fileName,auto_ptr<CryptoHash>& hash)
    { FileHandle  dataFile(fileName);
      BYTE        pbBuffer[1024];
      DWORD       dwBytesRead;

      hash.reset( new CryptoHash(m_cryptoCtx,CALG_MD5) );
      
      do
      { dwBytesRead = 0;

        THROW_LASTERROREXCEPTION1( ::ReadFile(*dataFile, 
                                              pbBuffer, sizeof(pbBuffer), &dwBytesRead, NULL
                                             ) 
                                 );

        if( dwBytesRead==0 ) 
          break;

        hash->Put(pbBuffer, dwBytesRead);
      } while( dwBytesRead>=sizeof(pbBuffer) );
    } // of MD5Sum::CalcFileHash()
  } // of namespace util
} // of namespace bvr20983

template basic_ostream<TCHAR,char_traits<TCHAR>>& bvr20983::util::operator << <TCHAR,char_traits<TCHAR>>( basic_ostream<TCHAR,char_traits<TCHAR>>&,const bvr20983::util::CryptoHash&);
/*==========================END-OF-FILE===================================*/