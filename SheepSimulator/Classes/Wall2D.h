#pragma once
#ifndef WALL_H
#define WALL_H
//------------------------------------------------------------------------
//
//  Name:   Wall2D.h
//
//  Desc:   class to create and render 2D walls. Defined as the two 
//          vectors A - B with a perpendicular normal. 
//          
//
//  Author: Mat Buckland (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include <fstream>
#include <cocos2d.h>

class Wall2D 
{
protected:

  cocos2d::Vec2    m_vA,
              m_vB,
              m_vN;

  void CalculateNormal()
  {
	  cocos2d::Vec2 temp = m_vB - m_vA;
	  temp.normalize();

    m_vN.x = -temp.y;
    m_vN.y = temp.x;
  }

public:

  Wall2D(){}

  Wall2D(cocos2d::Vec2 A, cocos2d::Vec2 B):m_vA(A), m_vB(B)
  {
    CalculateNormal();
  }

  Wall2D(cocos2d::Vec2 A, cocos2d::Vec2 B, cocos2d::Vec2 N):m_vA(A), m_vB(B), m_vN(N)
  { }

  Wall2D(std::ifstream& in){Read(in);}

  cocos2d::Vec2 From()const  {return m_vA;}
  void     SetFrom(cocos2d::Vec2 v){m_vA = v; CalculateNormal();}

  cocos2d::Vec2 To()const    {return m_vB;}
  void     SetTo(cocos2d::Vec2 v){m_vB = v; CalculateNormal();}
  
  cocos2d::Vec2 Normal()const{return m_vN;}
  void     SetNormal(cocos2d::Vec2 n){m_vN = n;}
  
  cocos2d::Vec2 Center()const{return (m_vA+m_vB)/2.0;}

 void Read(std::ifstream& in)
  {
    double x,y;

    in >> x >> y;
    SetFrom(cocos2d::Vec2(x,y));

    in >> x >> y;
    SetTo(cocos2d::Vec2(x,y));

     in >> x >> y;
    SetNormal(cocos2d::Vec2(x,y));
  }
  
};

#endif