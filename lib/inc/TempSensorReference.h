/*
 * Copyright 2012-2013 BrewPi/Elco Jacobs.
 * Copyright 2013 Matthew McGowan.
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

#include "TempSensorInterface.h"
#include "defaultDevices.h"

class TempSensorReference final : public TempSensorInterface {
public:
	TempSensorReference() : target(nullptr) {};
	~TempSensorReference() = default;

    void setTarget(TempSensorInterface * s) {
        target = s;
    }

    void unsetTarget(){
    	target = nullptr;
    }

    inline bool isConnected(void) const override final {
        return ref()->isConnected();
    }

    inline bool init() override final {
        return ref()->init();
    }

    void update() override final {
    	ref()->update();
    }

    inline temp_t read() const override final {
        return ref()->read();
    }

private:
    TempSensorInterface * ref() const {
        if (target == nullptr){
           return defaultTempSensor();
        }
        return target;
    }

    TempSensorInterface * target;
};

