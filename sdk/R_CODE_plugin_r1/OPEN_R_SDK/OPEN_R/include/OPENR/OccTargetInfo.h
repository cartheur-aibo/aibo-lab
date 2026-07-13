//
// OccTargetInfo.h
//
// Copyright 2000 Sony Corporation
//

#ifndef H_OccTargetInfo
#define H_OccTargetInfo

#include <string.h>
#include <Types.h>
#include <OPENR/OccTypes.h>
#include <OPENR/SensorId.h>
#include <OPENR/TargetPos.h>
#include <OPENR/TMatrix.h>

/*   Target Status   */
typedef byte    TargetStatus;
const TargetStatus tsNotSupport = 0;
const TargetStatus tsUnknown        = 1;
const TargetStatus tsFind       = 2;
const TargetStatus tsLose       = 3;

struct TransformInfo {                  // Total 68 Byte
    CoordinateID    m_cid;              //  4 Byte
    T_Matrix        m_tmatrix;          // 64 Byte

public:
    // Constructor
    TransformInfo( const TransformInfo& transinfo )
        {
            *this = transinfo;
        }
    TransformInfo( const CoordinateID& cid, const T_Matrix& tmatrix ) : 
        m_cid( cid ), m_tmatrix( tmatrix ) {}
    TransformInfo( const CoordinateID& cid ) : m_cid(cid) {}
    TransformInfo( void ) : m_cid(cidINVALID)             {}

    // Set
    OStatus SetTransformInfo(const CoordinateID& cid,
                             const T_Matrix& tmatrix) {
        m_cid = cid; m_tmatrix = tmatrix;
        return oSUCCESS;
    }
    OStatus SetCoordinateID( const CoordinateID& cid ) {
        m_cid = cid;
        return oSUCCESS;
    }
    OStatus SetTMatrix( const T_Matrix& tmatrix ) {
        m_tmatrix = tmatrix;
        return oSUCCESS;
    }
    OStatus SetTransformInfo( const TransformInfo& transinfo ) {
        *this = transinfo;
        return oSUCCESS; 
    }

    // Operator
    TransformInfo& operator=( const TransformInfo& transinfo ) {
        if( &transinfo != this ) memcpy( this, &transinfo, sizeof(*this) );
        return *this;
    }

    // Get
    CoordinateID& GetCoordinateID( void ) { return m_cid; }
    T_Matrix&     GetTMatrix( void )      { return m_tmatrix; }
};


/*   Target Size   */
struct Size3D {                         // Total 16 Byte
    Enable      m_enWidth;
    Enable      m_enHeight;
    Enable      m_enDepth;
    byte        reserved;
    Length      m_width;
    Length      m_height;
    Length      m_depth;

public:
    // Constructor
    Size3D( const Size3D& size )
        {
            *this = size;
        }
    Size3D( const Length width, const Length height, const Length depth ) : 
        m_width( width ),   m_enWidth( enable ), 
        m_height( height ), m_enHeight( enable ), 
        m_depth( depth ),   m_enDepth( enable ), reserved( 0x00 ) {}
    Size3D( const Length length ) : 
        m_width( length ),  m_enWidth( enable ), 
        m_height( length ), m_enHeight( enable ), 
        m_depth( length ),  m_enDepth( enable ), reserved( 0x00 ) {}
    Size3D( void ) : 
        m_width( 0 ),       m_enWidth( disable ), 
        m_height( 0 ),      m_enHeight( disable ), 
        m_depth( 0 ),       m_enDepth( disable ), reserved( 0x00 ) {}

    // Set
    OStatus SetWidth( const Length width ) {
        m_width = width;
        m_enWidth = enable; 
        return oSUCCESS;
    }
    OStatus SetHeight( const Length height ) {
        m_height = height;
        m_enHeight = enable; 
        return oSUCCESS;
    }
    OStatus SetDepth( const Length depth ) { 
        m_depth = depth;
        m_enDepth = enable; 
        return oSUCCESS;
    }
    OStatus DisableWidth( void ) {
        m_enWidth = disable; 
        return oSUCCESS;
    }
    OStatus DisableHeight( void ) {
        m_enHeight = disable; 
        return oSUCCESS;
    }
    OStatus DisableDepth( void ) {
        m_enDepth = disable; 
        return oSUCCESS;
    }
    OStatus SetSize( const Size3D& size ) {
        *this = size;
        return oSUCCESS;
    }
    OStatus SetSize( const Length width, const Length height,
                     const Length depth ) {
        m_width = width;    m_enWidth = enable;
        m_height = height;  m_enHeight = enable;
        m_depth = depth;    m_enDepth = enable; 
        return oSUCCESS;
    }
    OStatus SetSize( const Length length ) {
        m_width = length;   m_enWidth = enable; 
        m_height = length;  m_enHeight = enable; 
        m_depth = length;   m_enDepth = enable; 
        return oSUCCESS;
    }

    Size3D& operator=( const Size3D& size ) {
        if( &size != this ) memcpy( this, &size, sizeof(*this) );
        return *this;
    }

    // Get
    Enable  IsEnWidth( void )  { return m_enWidth; }
    Enable  IsEnHeight( void ) { return m_enHeight; }
    Enable  IsEnDepth( void )  { return m_enDepth; }
    Length  GetWidth( void )   { return m_width; }
    Length  GetHeight( void )  { return m_height; }
    Length  GetDepth( void )   { return m_depth; }
};


struct OccTargetInfo {                  // Total 136 Byte
    OpenrTime       m_openrtime;        //  4 Byte
    AppID           m_tid;              //  4 Byte
    TargetStatus    m_tstatus;          //  1 Byte
    static const int    m_numofsubinfo = 3;
    byte            m_subinfo[m_numofsubinfo];  // 3 Byte
    TargetPos       m_tpos;             // 32 Byte
    SensorId        m_sid;              //  8 Byte
    TransformInfo   m_transinfo;        // 68 Byte
    Size3D          m_tsize;            // 16 Byte

public:
    // Constructor
    OccTargetInfo( const OccTargetInfo& targetinfo )
        {
            *this = targetinfo;
        }
    OccTargetInfo( const OpenrTime openrtime, const AppID tid,
                   const TargetStatus tstatus, 
                   const TargetPos& tpos, const SensorId& sid,
                   const TransformInfo& transinfo, 
                   const Size3D& tsize ) : 
        m_openrtime( openrtime ), m_tid( tid ), m_tstatus( tstatus ), 
        m_tpos( tpos ), m_sid( sid ), m_transinfo( transinfo ),
        m_tsize( tsize )
        {
            for(int i=0; i<m_numofsubinfo; i++) {
                m_subinfo[i] = 0;
            }
        }
    OccTargetInfo( void ) :
        m_openrtime( 0 ), m_tid( INVALID_APPID ), m_tstatus( tsUnknown ), 
        m_tsize( 0 )
        {
            for(int i=0; i<m_numofsubinfo; i++) {
                m_subinfo[i] = 0;
            }
        }

    // Set
    OStatus SetOpenrTime( const OpenrTime openrtime ) {
        m_openrtime = openrtime;
        return oSUCCESS;
    }
    OStatus SetAppID( const AppID tid ) {
         m_tid = tid;
        return oSUCCESS;
    }
    OStatus SetSubInfo( const int idx, const byte subinfo ) {
         m_subinfo[idx] = subinfo;
        return oSUCCESS;
    }
    OStatus SetTargetStatus( const TargetStatus tstatus ) {
         m_tstatus = tstatus;
        return oSUCCESS;
    }
    OStatus SetTargetPos( const TargetPos& tpos ) {
        m_tpos = tpos;
        return oSUCCESS;
    }
    OStatus SetSensorID( const SensorId& sid ) {
        m_sid = sid;
        return oSUCCESS;
    }
    OStatus SetTransformInfo( const TransformInfo& transinfo ) {
        m_transinfo = transinfo;
        return oSUCCESS;
    }
    OStatus SetSize( const Size3D& tsize ) {
        m_tsize = tsize;
        return oSUCCESS;
    }
    OStatus SetTargetInfo( const OccTargetInfo& targetinfo ) {
        *this = targetinfo;
        return oSUCCESS;
    }

    OccTargetInfo&  operator=( const OccTargetInfo& targetinfo ) {
        if( &targetinfo != this ) memcpy( this, &targetinfo, sizeof(*this) );
        return *this;
    }

    // Get
    OpenrTime       GetOpenrTime( void )        { return m_openrtime; }
    AppID           GetAppID( void )            { return m_tid; }
    byte            GetSubInfo( const int idx ) { return m_subinfo[idx]; }
    TargetStatus    GetTargetStatus( void )     { return m_tstatus; }
    TargetPos&      GetTargetPos( void )        { return m_tpos; }
    SensorId&       GetSensorID( void )         { return m_sid; }
    TransformInfo&  GetTransformInfo( void )    { return m_transinfo; }
    Size3D&         GetSize( void )             { return m_tsize; }
};

#endif  // H_OccTargetInfo
