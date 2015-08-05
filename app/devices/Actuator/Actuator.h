/*
 * Copyright 2013 Matthew McGowan
 * Copyright 2013 BrewPi/Elco Jacobs.
 *
 * This file is part of BrewPi.
 * 
 * BrewPi is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * BrewPi is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with BrewPi.  If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include <stdint.h>
#include "Devices.h"


/*
 * An actuator simply turns something on or off.                        
 */

class Actuator
{
public:
    Actuator(){}
    virtual ~Actuator() {}
    virtual void setActive(bool active) = 0;
	virtual bool isActive() = 0;
	virtual void write(uint8_t val) = 0;
	virtual Actuator ** getDeviviceTarget(){
	    return 0;  // recursive call for super classes until this level is reached.
	}
	virtual Actuator * getBareActuator(){
	    return this;
	}
};

class DriverActuator : public Actuator
{
public:
    DriverActuator(Actuator * _target){
        target = _target;
    }
    virtual ~DriverActuator(){};

    Actuator * target;
    Actuator ** getDeviviceTarget(){
        if( target->getDeviviceTarget() == 0){
            return &target; // this is bottom
        }
        else{
            return target->getDeviviceTarget();  // this is not bottom
        }
    }

    virtual Actuator * getBareActuator(){
        return *getDeviviceTarget();
    }
};

/*
 * An actuator that simply remembers the set value. This is primary used for testing.
 */
class ValueActuator : public Actuator
{
public:
	ValueActuator() : state(false) {}
	ValueActuator(bool initial) : state(initial) {}

	virtual void setActive(bool active) { state = active; }
	virtual bool isActive() { return state; }
	virtual void write(uint8_t val) {}

private:
	bool state;	
};
