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
#ifndef SPECIFICWORKER_H
#define SPECIFICWORKER_H

#include <genericworker.h>

/**
       \brief
       @author authorname
*/

class SpecificWorker : public GenericWorker
{
Q_OBJECT
private:
	bool girando = false;
	enum class STATE {INITAVANZAR, AVANZAR, PARAR, INITGIRAR, GIRAR, CONTROL, IDLE};
	STATE s;
	QTime reloj;
	TLaserData laser_data;
	
	struct tag
	{
	 int id;
	 float tx,tz,ry;	
	 tag(){};
	 tag( int id_, float tx_, float tz_, float ry_)
	 {
		 tx = tx_*1000; tz = tz_*1000; ry = ry_; id = id_;
	 }
	};
	
	struct tagslocalT
	{
		QMutex mutex;
		void update( const tagsList &t)
		{
			mutex.lock();
			tags.clear();
			for(auto i: t)
			{
				tag myT(i.id, i.tx, i.tz, i.ry);
				tags.push_back(myT);
			}
			mutex.unlock();
		};
		bool existsId(int id_, tag &tt)
		{
			mutex.lock();
			for(auto i: tags)
			if( i.id == id_)
			{
				tt=i;
				return true;
			}
			mutex.unlock();
			return false;
		}
		std::vector<tag> tags;
	};
	
	tagslocalT tagslocal;
	
public:
	SpecificWorker(MapPrx& mprx, QObject *parent = 0);	
	~SpecificWorker();
	bool setParams(RoboCompCommonBehavior::ParameterList params);
	void initGirar();
	bool initAvanzar();
	void avanzar();
	bool girar();
	void parar();
	bool estaCerca();
	bool control();
	void  newAprilTag(const tagsList& tags);
	bool leerMarca( int id);

public slots:
 	void compute(); 
};

#endif