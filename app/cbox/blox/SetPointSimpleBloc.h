#pragma once

#include "bloc.h"
#include "SetPointSimple.pb.h"
#include "SetPoint.h"

class SetPointSimpleBloc: public Bloc {
private:
    SetPointSimple setpoint;

public:
    SetPointSimpleBloc() :
        setpoint()
{}

    static const size_t persistedMaxSize(){
        static_assert(blox_SetPointSimple_Persisted_size < 128, "varint for settings size will be larger than 1 byte");
        return blox_SetPointSimple_Persisted_size + 1;
    }
    virtual uint8_t readStreamSize() override final {
        /* maximum size of settings  +1 for varint for length in delimited message */
        return persistedMaxSize();
    }

    virtual void writeFrom(cbox::DataIn& dataIn) override final {
        writeFromImpl(dataIn, true);
    }

    void writeFromImpl(cbox::DataIn& dataIn, bool storeToEeprom){
        /* copy old settings, because the update can be sparse and can only overwrite some of the values */
        blox_SetPointSimple_Persisted newData;
        /* copy old settings in case of a sparse update */
        setpoint.copySettingsTo(&newData.settings);
        /* stream in new settings, overwriting copy of old settings */
        size_t maxSize = persistedMaxSize();
        pb_istream_t stream = { &dataInStreamCallback, &dataIn, maxSize};
        bool success = pb_decode_delimited_noinit(&stream, blox_SetPointSimple_Persisted_fields, &newData);
        /* if no errors occur, write new settings to wrapped object */
        if(success){
            setpoint.copySettingsFrom(&newData.settings);
            if(storeToEeprom){
                storeSettings();
            }
        }
    }

    static cbox::Object* create(cbox::ObjectDefinition& defn) {
        auto obj = new_object(SetPointSimpleBloc);
        if(obj != nullptr){
            obj->writeFromImpl(*defn.in, false);
        }
        return obj;
    }

    bool storeSettings(){
        if(eeprom_offset() == eptr_t(-1)){
            return false; /* EEPROM location is not set */
        }
        eptr_t offset = eeprom_offset();
        pb_ostream_t stream = { &eepromOutStreamCallback, &offset, eepromSize(), 0 };
        blox_SetPointSimple_Persisted definition;
        setpoint.copySettingsTo(&definition.settings);
        bool status = pb_encode_delimited(&stream, blox_SetPointSimple_Persisted_fields, &definition);

        return status;
    }

    SetPointSimple & get() {
        return setpoint;
    }

    virtual Interface * getApplicationInterfaceImpl() override final{
        return &setpoint;
    }

    virtual void readTo(cbox::DataOut& out) override final {
        blox_SetPointSimple message; \
        assert_size<sizeof(message.settings), SetPointSimple::sizeof_Settings>();
        setpoint.copySettingsTo(&message.settings);
        static_assert(blox_SetPointSimple_size < 128, "varint for settings size will be larger than 1 byte");
        pb_ostream_t stream = { &dataOutStreamCallback, &out, blox_SetPointSimple_size + 1, 0 };
        pb_encode_delimited(&stream, blox_SetPointSimple_fields, &message);
    }

    virtual cbox::obj_type_t typeID() override {
    	// use function overloading and templates to manage type IDs in a central place (AppTypeRegistry)
    	return resolveTypeID(this);
    }

};
