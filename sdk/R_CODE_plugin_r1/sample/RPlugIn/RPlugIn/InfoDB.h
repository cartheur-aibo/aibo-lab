//
// Copyright 2001,2006 Sony Corporation
//
// Permission to use, copy, modify, and redistribute this software for
// non-commercial use is hereby granted.
//
// This software is provided "as is" without warranty of any kind,
// either expressed or implied, including but not limited to the
// implied warranties of fitness for a particular purpose.
//

#include <OPENR/ODebug.h>

//==================================================================
// Information Database
//==================================================================

const word MWIID_BIT    = 0x0001 ;
const word EXP1_BIT     = 0x0002 ;
const word CATL_BIT     = 0x0004 ;
const word CATM_BIT     = 0x0008 ;
const word CATS_BIT     = 0x0010 ;
const word MIN_BIT      = 0x0020 ;
const word MAX_BIT      = 0x0040 ;
const word RESO_BIT     = 0x0080 ;
const word UNIT_BIT     = 0x0100 ;
const word EXP2_BIT     = 0x0200 ;
const word SEN_BIT      = 0x0400 ;
const word EXP3_BIT     = 0x0800 ;
const word PART_BIT     = 0x1000 ;
const word POS_BIT      = 0x2000 ;

struct InformationData
{

    longword    m_iid;      // MWIID
    byte        m_exp1;     // Expansion flag of categorize
    byte        m_catL;     // Category L
    byte        m_catM;     // Category M
    byte        m_catS;     // Category S
    longword    m_min;      // Min value
    longword    m_max;      // Max value
    longword    m_reso;     // Resolution
    longword    m_unit;     // Unit
    byte        m_exp2;     // Expansion flag of sensor type
    byte        m_type;     // Sensor type
    byte        pad1;       // padding
    byte        pad2;       // padding
    byte        m_exp3;     // Expansion flag of sensor position
    byte        pad3;       // padding
    byte        m_part;     // parts
    byte        m_num;      // Number

    InformationData(): m_iid(0), m_exp1(0), m_catL(0), m_catM(0), m_catS(0),
        m_min(0), m_max(0), m_reso(0), m_unit(0),
        m_exp2(0), m_type(0), m_exp3(0), m_part(0), m_num(0) {};

    void        SetMWIID(longword iid)      { m_iid  = iid;  }
    void        SetInfoExp1(byte exp1)      { m_exp1 = exp1; }
    void        SetInfoCatL(byte catL)      { m_catL = catL; }
    void        SetInfoCatM(byte catM)      { m_catM = catM; }
    void        SetInfoCatS(byte catS)      { m_catS = catS; }
    void        SetInfoMin(longword min)    { m_min  = min;  }
    void        SetInfoMax(longword max)    { m_max  = max;  }
    void        SetInfoReso(longword reso)  {m_reso  = reso; }
    void        SetInfoUnit(longword unit)  { m_unit = unit; }
    void        SetInfoExp2(byte exp2)      { m_exp2 = exp2; }
    void        SetInfoType(byte type)      { m_type = type; }
    void        SetInfoExp3(byte exp3)      { m_exp3 = exp3; }
    void        SetInfoPart(byte part)      { m_part = part; }
    void        SetInfoNum(byte num)        { m_num  = num;  }

    longword    GetMWIID()    { return m_iid;  }
    byte        GetInfoExp1() { return m_exp1; }
    byte        GetInfoCatL() { return m_catL; }
    byte        GetInfoCatM() { return m_catM; }
    byte        GetInfoCatS() { return m_catS; }
    longword    GetInfoMin()  { return m_min;  }
    longword    GetInfoMax()  { return m_max;  }
    longword    GetInfoReso() { return m_reso; }
    longword    GetInfoUnit() { return m_unit; }
    byte        GetInfoExp2() { return m_exp2; }
    byte        GetInfoType() { return m_type; }
    byte        GetInfoExp3() { return m_exp3; }
    byte        GetInfoPart() { return m_part; }
    byte        GetInfoNum()  { return m_num;  }

};

//==================================================================
// Search Information Database
//==================================================================
struct InfoDB
{
    InformationData*    m_infoData;     // Data at retrieval destination
    longword            m_nData;        // Information Database Size
    ODesignDataID       m_id;           // Design data ID

    InformationData     m_SearchData;   // Retrieval former data
    word                m_InfoBIT;      // Retrieval mask


    InfoDB(): m_nData(0), m_InfoBIT(0) {};
    virtual ~InfoDB() {
        OPENR::DeleteDesignData(m_id);   // Shared memory opening
    }

    void SetMWIID(longword iid)     { m_SearchData.SetMWIID( iid );      m_InfoBIT = m_InfoBIT | MWIID_BIT; }
    void SetInfoExp1(byte exp1)     { m_SearchData.SetInfoExp1( exp1 );  m_InfoBIT = m_InfoBIT | EXP1_BIT;  }
    void SetInfoCatL(byte catL)     { m_SearchData.SetInfoCatL( catL );  m_InfoBIT = m_InfoBIT | CATL_BIT;  }
    void SetInfoCatM(byte catM)     { m_SearchData.SetInfoCatM( catM );  m_InfoBIT = m_InfoBIT | CATM_BIT;  }
    void SetInfoCatS(byte catS)     { m_SearchData.SetInfoCatS( catS );  m_InfoBIT = m_InfoBIT | CATS_BIT;  }
    void SetInfoMin(longword min)   { m_SearchData.SetInfoMin( min );    m_InfoBIT = m_InfoBIT | MIN_BIT;   }
    void SetInfoMax(longword max)   { m_SearchData.SetInfoMax( max );    m_InfoBIT = m_InfoBIT | MAX_BIT;   }
    void SetInfoReso(longword reso) { m_SearchData.SetInfoReso( reso );  m_InfoBIT = m_InfoBIT | RESO_BIT;  }
    void SetInfoUnit(longword unit) { m_SearchData.SetInfoUnit( unit );  m_InfoBIT = m_InfoBIT | UNIT_BIT;  }
    void SetInfoExp2(byte exp2)     { m_SearchData.SetInfoExp2( exp2 );  m_InfoBIT = m_InfoBIT | EXP2_BIT;  }
    void SetInfoType(byte type)     { m_SearchData.SetInfoType( type );  m_InfoBIT = m_InfoBIT | SEN_BIT;   }
    void SetInfoExp3(byte exp3)     { m_SearchData.SetInfoExp3( exp3 );  m_InfoBIT = m_InfoBIT | EXP3_BIT;  }
    void SetInfoPart(byte part)     { m_SearchData.SetInfoPart( part );  m_InfoBIT = m_InfoBIT | PART_BIT;  }
    void SetInfoNum(byte num)       { m_SearchData.SetInfoNum( num );    m_InfoBIT = m_InfoBIT | POS_BIT;   }

    // It searches infoData and returns MWIID
    OStatus FindAllMWIID( longword* iid )
    {
        bool chkbit = true;

        for( longword i = 0 ; i< m_nData ; i++ ){
            if ( (m_InfoBIT & EXP1_BIT) && !(GetInfoExp1(i) == m_SearchData.GetInfoExp1()) ) chkbit = false;
            if ( (m_InfoBIT & CATL_BIT) && !(GetInfoCatL(i) == m_SearchData.GetInfoCatL()) ) chkbit = false;
            if ( (m_InfoBIT & CATM_BIT) && !(GetInfoCatM(i) == m_SearchData.GetInfoCatM()) ) chkbit = false;
            if ( (m_InfoBIT & CATS_BIT) && !(GetInfoCatS(i) == m_SearchData.GetInfoCatS()) ) chkbit = false;
            if ( (m_InfoBIT & MIN_BIT ) && !(GetInfoMin(i)  == m_SearchData.GetInfoMin())  ) chkbit = false;
            if ( (m_InfoBIT & MAX_BIT ) && !(GetInfoMax(i)  == m_SearchData.GetInfoMax())  ) chkbit = false;
            if ( (m_InfoBIT & RESO_BIT) && !(GetInfoReso(i) == m_SearchData.GetInfoReso()) ) chkbit = false;
            if ( (m_InfoBIT & UNIT_BIT) && !(GetInfoUnit(i) == m_SearchData.GetInfoUnit()) ) chkbit = false;
            if ( (m_InfoBIT & EXP2_BIT) && !(GetInfoExp2(i) == m_SearchData.GetInfoExp2()) ) chkbit = false;
            if ( (m_InfoBIT & SEN_BIT ) && !(GetInfoType(i) == m_SearchData.GetInfoType()) ) chkbit = false;
            if ( (m_InfoBIT & EXP3_BIT) && !(GetInfoExp3(i) == m_SearchData.GetInfoExp3()) ) chkbit = false;
            if ( (m_InfoBIT & PART_BIT) && !(GetInfoPart(i) == m_SearchData.GetInfoPart()) ) chkbit = false;
            if ( (m_InfoBIT & POS_BIT ) && !(GetInfoNum(i)  == m_SearchData.GetInfoNum())  ) chkbit = false;

            if ( chkbit == true )
            {
                *iid = GetMWIID(i);
                m_InfoBIT = 0;
                return oSUCCESS;
            }
            chkbit = true;
        }
        m_InfoBIT = 0;
        return oFAIL;
    }

private:

    void SetInfoData( InformationData *infoData, longword nData) { m_infoData = infoData; m_nData = nData; }

    longword GetMWIID(longword nIndex)    { return (m_infoData+nIndex)->GetMWIID();    }
    byte     GetInfoExp1(longword nIndex) { return (m_infoData+nIndex)->GetInfoExp1(); }
    byte     GetInfoCatL(longword nIndex) { return (m_infoData+nIndex)->GetInfoCatL(); }
    byte     GetInfoCatM(longword nIndex) { return (m_infoData+nIndex)->GetInfoCatM(); }
    byte     GetInfoCatS(longword nIndex) { return (m_infoData+nIndex)->GetInfoCatS(); }
    longword GetInfoMin(longword nIndex)  { return (m_infoData+nIndex)->GetInfoMin();  }
    longword GetInfoMax(longword nIndex)  { return (m_infoData+nIndex)->GetInfoMax();  }
    longword GetInfoReso(longword nIndex) { return (m_infoData+nIndex)->GetInfoReso(); }
    longword GetInfoUnit(longword nIndex) { return (m_infoData+nIndex)->GetInfoUnit(); }
    byte     GetInfoExp2(longword nIndex) { return (m_infoData+nIndex)->GetInfoExp2(); }
    byte     GetInfoType(longword nIndex) { return (m_infoData+nIndex)->GetInfoType(); }
    byte     GetInfoExp3(longword nIndex) { return (m_infoData+nIndex)->GetInfoExp3(); }
    byte     GetInfoPart(longword nIndex) { return (m_infoData+nIndex)->GetInfoPart(); }
    byte     GetInfoNum(longword nIndex)  { return (m_infoData+nIndex)->GetInfoNum();  }

    longword GetInfoDataSize() { return m_nData; }

public:

    //============================================================================
    // Read Information Database
    //============================================================================
    OStatus ReadInfoDB()
    {
        ODesignDataID   id;     // Design Data ID
        byte*           data;   // Address of design data
        size_t          size;   // Size of design data
        OStatus         result; // Return value

        size_t      ssize;
        longword    nData;

        result = OPENR::FindDesignData( "MWIDB", &id, &data, &size );
        m_id = id;

        if ( result == oFAIL ) {
            // FindDesignData error
            DPRINTF(("FindDesignData Error %d", result ));
            return oFAIL;
        }

        // magic
        if( memcmp( "OIDB", data , sizeof(longword) ))
        {
            DPRINTF(( "MagicID error\n" ));
            return oFAIL;
        }
        data += sizeof(longword);

        // major version
        data += sizeof(word);
        // minor version
        data += sizeof(word);

        // Comment Section ID
        if( memcmp( "COME", data , sizeof(longword) ))
        {
            DPRINTF(( "Comment SectionId error\n" ));
            return oFAIL;
        }
        data += sizeof(longword);

        // Comment Section size
        ssize = *((size_t*)data);
        data += sizeof(size_t);
        // Comment
        data += ssize;

        // Reserved Section ID
        if( memcmp( "ODID", data, sizeof(longword) ))
        {
            DPRINTF(( "Reserved SectionId error\n" ));
            return oFAIL;
        }
        data += sizeof(longword);

        // Reserved Section size
        ssize = *((size_t*)data);
        data += sizeof(size_t);
        // Reserved
        data += ssize;

        // Information Database Section ID
        if( memcmp( "IFDB", data, sizeof(longword) ))
        {
            DPRINTF(( "InfDB SectionId error\n" ));
            return oFAIL;
        }
        data += sizeof(longword);

        // Information Database Section size
        //      ssize = *((size_t*)data);
        data += sizeof(size_t);

        // Information Data
        nData = *((longword*)data);
        data += sizeof(longword);

        SetInfoData((InformationData*)data, nData );

        return oSUCCESS;
    }

    //
    // for debugging
    //
    void debugDump()
    {
        for(longword i=0 ; i<GetInfoDataSize() ; i++ )
        {
            DPRINTF(( "%d:%02x:%02x:%02x:%02x:%d:%d:%d:%d:%02x:%02x:%02x:%02x:%X\n",
                GetMWIID(i),
                GetInfoExp1(i),
                GetInfoCatL(i),
                GetInfoCatM(i),
                GetInfoCatS(i),
                GetInfoMin(i),
                GetInfoMax(i),
                GetInfoReso(i),
                GetInfoUnit(i),
                GetInfoExp2(i),
                GetInfoType(i),
                GetInfoExp3(i),
                GetInfoPart(i),
                GetInfoNum(i) ));
        }
    }
};
