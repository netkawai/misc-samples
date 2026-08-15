#ifndef _ATL_CSTRING_NO_CRT 
    typedef CStringT<wchar_t, StrTraitATL<wchar_t, ChTraitsCRT<wchar_t>>> CAtlStringW;
    typedef CStringT<char, StrTraitATL<char, ChTraitsCRT<char>>> CAtlStringA; 
    typedef CStringT<TCHAR, StrTraitATL<TCHAR, ChTraitsCRT<TCHAR>>> CAtlString; 
#else   // _ATL_CSTRING_NO_CRT 

    typedef CStringT< wchar_t, StrTraitATL< wchar_t > > CAtlStringW; 
    typedef CStringT< char, StrTraitATL< char > > CAtlStringA; 
    typedef CStringT< TCHAR, StrTraitATL< TCHAR > > CAtlString; 
#endif   // _ATL_CSTRING_NO_CRT 

#ifndef _AFX 

typedef CAtlStringW CStringW; 
typedef CAtlStringA CStringA; 
typedef CAtlString CString; 

#endif 


// cstringt.h 

template <typename BaseType, class StringTraits> 
class CStringT : publicCSimpleStringT< BaseType, _CSTRING_IMPL_::_MFCDLLTraitsCheck<BaseType, StringTraits>::c_bIsMFCDLLTraits > 
{ 

public: 

typedef CSimpleStringT< BaseType, _CSTRING_IMPL_::_MFCDLLTraitsCheck<BaseType, StringTraits>::c_bIsMFCDLLTraits > CThisSimpleString; 
typedef StringTraits StrTraits; 

typedef typename CThisSimpleString::XCHAR XCHAR; 
typedef typename CThisSimpleString::PXSTR PXSTR; 
typedef typename CThisSimpleString::PCXSTR PCXSTR; 
typedef typename CThisSimpleString::YCHAR YCHAR; 
typedef typename CThisSimpleString::PYSTR PYSTR; 
typedef typename CThisSimpleString::PCYSTR PCYSTR; 

..... 

}; 

template<typename _BaseType = char , class StringIterator = ChTraitsOS<_BaseType>> 
class StrTraitATL : public StringIterator 
{ 
public: 
    static HINSTANCE FindStringResourceInstance(__in UINT nID) throw() 
    { 
        return ( AtlFindStringResourceInstance(nID)); 
    } 

    static IAtlStringMgr* GetDefaultManager() throw() 
    { 
        return ( &g_strmgr ); 
    } 

}; 

template<typename _CharType = char > 
class ChTraitsCRT : public ChTraitsBase<_CharType> 
{ 

public: 
... 

}; 

template<typename _CharType = char > 
class ChTraitsOS : public ChTraitsBase< _CharType > 
{ 

public: 
... 

}; 

 

// atlsimpstr.h 

template < typename BaseType = char > 
class ChTraitsBase 
{ 
public: 
    typedef char XCHAR; 
    typedef LPSTR PXSTR; 
    typedef LPCSTR PCXSTR; 
    typedef wchar_t YCHAR; 
    typedef LPWSTR PYSTR; 
    typedef LPCWSTR PCYSTR; 
}; 