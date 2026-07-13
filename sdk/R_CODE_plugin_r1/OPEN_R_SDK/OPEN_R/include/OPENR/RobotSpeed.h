//
// RobotSpeed.h
//
// Copyright 2000 Sony Corporation
//

#ifndef H_RobotSpeed
#define H_RobotSpeed

#include <Types.h>

struct RobotSpeed
{
    longword flag;
    slongword vx;
    slongword vy;
    slongword vz;
    slongword wx;
    slongword wy;
    slongword wz;
    
    RobotSpeed( void ):flag(0), vx(0), vy(0), vz(0), wx(0), wy(0), wz(0) {}

    RobotSpeed& operator = (const RobotSpeed& q) {
        if( this != &q ) {
            flag = q.flag;
            vx = q.vx;
            vy = q.vy;
            vz = q.vz;
            wx = q.wx;
            wy = q.wy;
            wz = q.wz;
        }
        return *this;
    }
    
    bool IsValid()   { return flag; }
    bool IsVValid()  { return (flag & 0x0fff0000); }
    bool IsVxValid() { return (flag & 0x0f000000); }
    bool IsVyValid() { return (flag & 0x00f00000); }
    bool IsVzValid() { return (flag & 0x000f0000); }
    bool IsWValid()  { return (flag & 0x00000fff); }
    bool IsWxValid() { return (flag & 0x00000f00); }
    bool IsWyValid() { return (flag & 0x000000f0); }
    bool IsWzValid() { return (flag & 0x0000000f); }
    
    slongword GetVx() { return vx; }
    slongword GetVy() { return vy; }
    slongword GetVz() { return vz; }
    
    slongword GetWx() { return wx; }
    slongword GetWy() { return wy; }
    slongword GetWz() { return wz; }

    void SetVx(const slongword& x) { vx = x; flag |= 0x0f000000; }
    void SetVy(const slongword& x) { vy = x; flag |= 0x00f00000; }
    void SetVz(const slongword& x) { vz = x; flag |= 0x000f0000; }
    void SetWx(const slongword& x) { wx = x; flag |= 0x00000f00; }
    void SetWy(const slongword& x) { wy = x; flag |= 0x000000f0; }
    void SetWz(const slongword& x) { wz = x; flag |= 0x0000000f; }
};

#endif  // H_RobotSpeed
 
