/*
 *    Copyright (C) 2006-2010 by RoboLab - University of Extremadura
 *
 *    This file is part of RoboComp
 *
 *    RoboComp is free software: you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation, either version 3 of the License, or
 *    (at your option) any later version.
 *
 *    RoboComp is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *    GNU General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with RoboComp.  If not, see <http://www.gnu.org/licenses/>.
 */
 
 #include "specificworker.h"
#include <qt4/QtCore/QDate>

/**
* \brief Default constructor
*/

SpecificWorker::SpecificWorker(MapPrx& mprx, QObject *parent) : GenericWorker(mprx, parent)	
{
	s = STATE::INITAVANZAR;
}

/**
* \brief Default destructor
*/
SpecificWorker::~SpecificWorker()
{

}

void SpecificWorker::compute( )
{
	laser_data=laser_proxy->getLaserData();
	switch(s)
	{
		case STATE::INITAVANZAR:
			initAvanzar();
			break;
		case STATE::AVANZAR:
			avanzar();
			break;
		case STATE::PARAR:
			parar();
			break;
		case STATE::INITGIRAR:
			initGirar();
			break;
		case STATE::GIRAR:
			girar();
			break;
		case STATE::CONTROL:
			control();
			break;
		case STATE::IDLE:
			
			break;
	}
}

bool SpecificWorker::initAvanzar()
{
	if(estaCerca()){
		s=STATE::INITGIRAR;
		return true;
	}else{
		try {
			differentialrobot_proxy->setSpeedBase(300,0);
			s=STATE::AVANZAR;
			qDebug()<<"Estado InitAvanzar";
		} catch (const Ice::Exception &E) {
			std::cout<< E << endl;
		}
		return false;
	}
}



void SpecificWorker::avanzar(){
	qDebug()<<"Estado Avanzar";
	if( estaCerca() )
			s=STATE::PARAR;

}


bool SpecificWorker:: leerMarca( int id){
/*	
	for(auto i: tagslocal){
	  
		if(i.id==id){
			
			return true;
	}
	
}

return false;*/
}



void SpecificWorker::parar(){
	
	try {
		differentialrobot_proxy->setSpeedBase(0,0);
		qDebug()<<"Estado Parar";
		s=STATE::INITGIRAR;
		} catch (const Ice::Exception &E) {
			std::cout<< E << endl;
		}
}

void SpecificWorker::initGirar()
{
        try 
        {
           differentialrobot_proxy->setSpeedBase ( 0,0.7 );
           s=STATE::GIRAR;
          qDebug() <<"Estado InitGirar";
        } catch ( const Ice::Exception &E ) 
		{   std::cout<< E << endl; }
}

bool SpecificWorker::girar(){
	
	tag t;
	if(tagslocal.existsId(3,t))
	{
		parar();
		s=STATE::CONTROL;
		return true;
	}
}

bool SpecificWorker::control()
{
	const float P= 1;
	const float Q= 1;
	
	tag t;
	if(tagslocal.existsId(3, t))
	{
			if(t.tz<300)
			{
				s=STATE::PARAR;
				return false;
			
			}
			
			float vrot=t.tx * P;
			if( vrot > 0.8) vrot = 0.8;
			if( vrot < -0.8) vrot = -0.8;
			float vadv=t.tz * Q;
			if(vadv > 600) vadv = 600;
			try 
			{
				differentialrobot_proxy->setSpeedBase ( 0,0.7 );
				s=STATE::GIRAR;
				qDebug() <<"Estado InitGirar";
        } catch ( const Ice::Exception &E )	{   std::cout<< E << endl; }
		
		}
		return true;
}

bool SpecificWorker::setParams(RoboCompCommonBehavior::ParameterList params)
{
    timer.start(Period);
    return true;
};
 


bool SpecificWorker::estaCerca(){
	qDebug()<<"Tamaño del array"<<laser_data.size();
	for(int i=1; i<laser_data.size();i++){
		qDebug()<<"Distancia laser"<<laser_data[i].dist;
		if(laser_data[i].dist<500 and fabs(laser_data[i].angle) < 1.2){
			return true;
		}
	}
	return false;
}

void SpecificWorker::newAprilTag(const tagsList& tags)
{
	
	tagslocal.update( tags) ;
	
}
