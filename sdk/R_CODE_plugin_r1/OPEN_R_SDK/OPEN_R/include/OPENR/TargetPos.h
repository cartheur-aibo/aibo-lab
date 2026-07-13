//
// TargetPos.h
//
// Copyright 2000 Sony Corporation
//

#ifndef H_TargetPos
#define H_TargetPos

#include <Types.h>
#include <string.h>
#include <OPENR/OStatus.h>
#include <OPENR/OccTypes.h>

/*
  Target Position indicated on sensor coordinate and polar coordinate system
*/
struct Position3D {                     // Total 16 Byte
    Enable      m_enHAngle;
    Enable      m_enVAngle;
    Enable      m_enDistance;
    byte        reserved;
    Angle       m_hAngle;
    Angle       m_vAngle;
    Length      m_distance;

public:
    // Constructor
    Position3D (const Position3D& position)
        {
            *this = position;
        }
    Position3D (const Angle hAngle, const Angle vAngle, const Length distance):
        m_hAngle( hAngle ), m_enHAngle( enable ), 
        m_vAngle( vAngle ), m_enVAngle( enable ), 
        m_distance( distance ), m_enDistance( enable ) {}
    Position3D( const Angle hAngle, const Angle vAngle ) : 
        m_hAngle( hAngle ), m_enHAngle( enable ), 
        m_vAngle( vAngle ), m_enVAngle( enable ), 
        m_distance( 0 ),    m_enDistance( disable ) {}
    Position3D( const Angle hAngle ) : 
        m_hAngle( hAngle ), m_enHAngle( enable ), 
        m_vAngle( 0 ),      m_enVAngle( disable ), 
        m_distance( 0 ),    m_enDistance( disable ) {}
    Position3D( void ) : 
        m_hAngle( 0 ),      m_enHAngle( disable ), 
        m_vAngle( 0 ),      m_enVAngle( disable ), 
        m_distance( 0 ),    m_enDistance( disable ) {}

    // Set
    OStatus SetHAngle( const Angle hAngle ) {
        m_hAngle = hAngle;
        m_enHAngle = enable; 
        return oSUCCESS;
    }
    OStatus SetVAngle( const Angle vAngle ) {
        m_vAngle = vAngle;
        m_enVAngle = enable; 
        return oSUCCESS;
    }
    OStatus SetDistance( const Length distance ) {
        m_distance = distance;
        m_enDistance = enable; 
        return oSUCCESS;
    }
    OStatus     DisableHAngle( void ) {
        m_enHAngle = disable;
        return oSUCCESS;
    }
    OStatus     DisableVAngle( void ) {
        m_enVAngle = disable;
        return oSUCCESS;
    }
    OStatus     DisableDistance( void ) {
        m_enDistance = disable;
        return oSUCCESS;
    }
    OStatus SetPosition( const Position3D& position ) {
        *this = position;
        return oSUCCESS;
    }
    OStatus SetPosition( const Angle hAngle, const Angle vAngle,
                         const Length distance ) {
        m_hAngle = hAngle;  m_enHAngle = enable; 
        m_vAngle = vAngle;  m_enVAngle = enable; 
        m_distance = distance; m_enDistance = enable; 
        return oSUCCESS;
    }
    OStatus SetPosition( const Angle hAngle, const Angle vAngle ) {
        m_hAngle = hAngle;  m_enHAngle = enable; 
        m_vAngle = vAngle;  m_enVAngle = enable; 
        m_enDistance = 0;   m_enDistance = disable; 
        return oSUCCESS;
    }
    OStatus SetPosition( const Angle hAngle ) {
        m_hAngle = hAngle;  m_enHAngle = enable; 
        m_vAngle = 0;       m_enVAngle = disable; 
        m_distance = 0;     m_enDistance = disable; 
        return oSUCCESS;
    }
    Position3D& operator=( const Position3D& position ) {
        if( &position != this ) memcpy( this, &position, sizeof(*this) );
        return *this;
    }

    // Get
    Enable IsEnHAngle( void )   { return m_enHAngle; }
    Enable IsEnVAngle( void )   { return m_enVAngle; }
    Enable IsEnDistance( void ) { return m_enDistance; }
    Angle  GetHAngle( void )    { return m_hAngle; }
    Angle  GetVAngle( void )    { return m_vAngle; }
    Length GetDistance( void )  { return m_distance; }

    // Compare
    bool operator==( const Position3D& position ) {
        bool isSame = true;
        if( m_enHAngle != position.m_enHAngle
            || m_enVAngle != position.m_enVAngle
            || m_enDistance != position.m_enDistance )
        {
            isSame = false;
        }
        else if( m_enHAngle && m_hAngle != position.m_hAngle )
        {
            isSame = false;
        }
        else if( m_enVAngle && m_vAngle != position.m_vAngle )
        {
            isSame = false;
        }
        else if( m_enDistance && m_distance != position.m_distance )
        {
            isSame = false;
        }
        return isSame;
    }
    bool  operator!=( const Position3D& position ) {
        return !(*this == position);
    }
};


/*   indicate posture on Target Position   */
struct Posture3D {                       // Total 16 Byte
    Enable      m_enHAngle;
    Enable      m_enVAngle;
    Enable      m_enRoll;
    byte        m_reserved;
    Angle       m_hAngle;
    Angle       m_vAngle;
    Angle       m_roll;

public:
    // Constructor
    Posture3D( const Posture3D& posture )
        {
            *this = posture;
        }
    Posture3D( const Angle hAngle, const Angle vAngle, const Angle roll ) : 
        m_hAngle( hAngle ), m_enHAngle( enable ), 
        m_vAngle( vAngle ), m_enVAngle( enable ), 
        m_roll( roll ),     m_enRoll( enable ) {}
    Posture3D( const Angle hAngle, const Angle vAngle ) : 
        m_hAngle( hAngle ), m_enHAngle( enable ), 
        m_vAngle( vAngle ), m_enVAngle( enable ), 
        m_roll( 0 ),        m_enRoll( disable ) {}
    Posture3D( const Angle hAngle ) : 
        m_hAngle( hAngle ), m_enHAngle( enable ), 
        m_vAngle( 0 ),      m_enVAngle( disable ), 
        m_roll( 0 ),        m_enRoll( disable ) {}
    Posture3D( void ) : 
        m_hAngle( 0 ),      m_enHAngle( disable ), 
        m_vAngle( 0 ),      m_enVAngle( disable ), 
        m_roll( 0 ),        m_enRoll( disable ) {}

    // Set
    OStatus SetHAngle( const Angle hAngle ) {
        m_hAngle = hAngle;
        m_enHAngle = enable; 
        return oSUCCESS;
    }
    OStatus SetVAngle( const Angle vAngle ) {
        m_vAngle = vAngle;
        m_enVAngle = enable; 
        return oSUCCESS;
    }
    OStatus SetRoll( const Angle roll ) {
        m_roll = roll;
        m_enRoll = enable; 
        return oSUCCESS;
    }
    OStatus DisableHAngle( void ) { 
        m_enHAngle = disable; 
        return oSUCCESS;
    }
    OStatus DisableVAngle( void ) { 
        m_enVAngle = disable; 
        return oSUCCESS;
    }
    OStatus  DisableRoll( void ) { 
        m_enRoll = disable; 
        return oSUCCESS;
    }
    OStatus SetPosture( const Posture3D& posture ) {
        *this = posture;
        return oSUCCESS;
    }
    OStatus SetPosture( const Angle hAngle, const Angle vAngle,
                        const Angle roll ) {
        m_hAngle = hAngle;  m_enHAngle = enable; 
        m_vAngle = vAngle;  m_enVAngle = enable; 
        m_roll = roll;      m_enRoll = enable; 
        return oSUCCESS;
    }
    OStatus SetPosture( const Angle hAngle, const Angle vAngle ) {
        m_hAngle = hAngle;  m_enHAngle = enable; 
        m_vAngle = vAngle;  m_enVAngle = enable; 
        m_roll = 0;         m_enRoll = disable; 
        return oSUCCESS;
    }
    OStatus SetPosture( const Angle hAngle ) {
        m_hAngle = hAngle;  m_enHAngle = enable; 
        m_vAngle = 0;       m_enVAngle = disable; 
        m_roll = 0;         m_enRoll = disable; 
        return oSUCCESS;
    }

    // Operator
    Posture3D& operator=( const Posture3D& posture ) {
        if( &posture != this ) memcpy( this, &posture, sizeof(*this) );
        return *this;
    }

    // Get
    Enable IsEnHAngle( void ) { return m_enHAngle; }
    Enable IsEnVAngle( void ) { return m_enVAngle; }
    Enable IsEnRoll( void )   { return m_enRoll; }
    Angle  GetHAngle( void )  { return m_hAngle; }
    Angle  GetVAngle( void )  { return m_vAngle; }
    Angle  GetRoll( void )    { return m_roll; }

    // Compare
    bool operator==( const Posture3D& posture ) {
        bool isSame = true;
        if( m_enHAngle != posture.m_enHAngle
            || m_enVAngle != posture.m_enVAngle
            || m_enRoll != posture.m_enRoll )
        {
            isSame = false;
        }
        else if( m_enHAngle && m_hAngle != posture.m_hAngle )
        {
            isSame = false;
        }
        else if( m_enVAngle && m_vAngle != posture.m_vAngle )
        {
            isSame = false;
        }
        else if( m_enRoll && m_roll != posture.m_roll )
        {
            isSame = false;
        }
        return isSame;
    }
    bool operator!=( const Posture3D& posture ) {
        return !(*this == posture);
    }
};


struct TargetPos {                      // Total 32 Byte
    Position3D  m_position;             // 16 Byte
    Posture3D   m_posture;              // 16 Byte

public:
    // Constructor
    TargetPos( const TargetPos& tpos )
        {
            *this = tpos;
        }
    TargetPos( const Position3D& position, const Posture3D& posture ) :
        m_position( position ), m_posture( posture ) {}
    TargetPos( const Position3D& position ) : 
        m_position( position ) {}
    TargetPos( void ) {}

    // Set
    OStatus SetPosition( const Position3D& position ) {
        m_position = position;
        return oSUCCESS;
    }
    OStatus SetPosture( const Posture3D& posture ) {
        m_posture = posture;
        return oSUCCESS;
    }

    TargetPos&  operator=( const TargetPos& tpos ){
        if( &tpos != this ) memcpy( this, &tpos, sizeof(*this) );
        return *this;
    }

    // Get
    Position3D& GetPosition( void ) { return m_position; }
    Posture3D&  GetPosture( void )  { return m_posture; }

    // Compare
    bool operator==( TargetPos& tp ) {
        return ( GetPosition() == tp.GetPosition()
                 && GetPosture() == tp.GetPosture() );
    }
    bool operator!=( TargetPos& tp ) {
        return !(*this == tp);
    }
};

#endif  // H_TargetPos
