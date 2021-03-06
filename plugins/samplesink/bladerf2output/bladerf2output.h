///////////////////////////////////////////////////////////////////////////////////
// Copyright (C) 2018 Edouard Griffiths, F4EXB                                   //
//                                                                               //
// This program is free software; you can redistribute it and/or modify          //
// it under the terms of the GNU General Public License as published by          //
// the Free Software Foundation as version 3 of the License, or                  //
//                                                                               //
// This program is distributed in the hope that it will be useful,               //
// but WITHOUT ANY WARRANTY; without even the implied warranty of                //
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the                  //
// GNU General Public License V3 for more details.                               //
//                                                                               //
// You should have received a copy of the GNU General Public License             //
// along with this program. If not, see <http://www.gnu.org/licenses/>.          //
///////////////////////////////////////////////////////////////////////////////////

#ifndef PLUGINS_SAMPLESINK_BLADERF2OUTPUT_BLADERF2OUTPUT_H_
#define PLUGINS_SAMPLESINK_BLADERF2OUTPUT_BLADERF2OUTPUT_H_

#include <QString>
#include <libbladeRF.h>

#include "dsp/devicesamplesink.h"
#include "bladerf2/devicebladerf2shared.h"
#include "bladerf2outputsettings.h"

class DeviceSinkAPI;
class BladeRF2OutputThread;
struct bladerf_gain_modes;

class BladeRF2Output : public DeviceSampleSink {
public:
    class MsgConfigureBladeRF2 : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        const BladeRF2OutputSettings& getSettings() const { return m_settings; }
        bool getForce() const { return m_force; }

        static MsgConfigureBladeRF2* create(const BladeRF2OutputSettings& settings, bool force)
        {
            return new MsgConfigureBladeRF2(settings, force);
        }

    private:
        BladeRF2OutputSettings m_settings;
        bool m_force;

        MsgConfigureBladeRF2(const BladeRF2OutputSettings& settings, bool force) :
            Message(),
            m_settings(settings),
            m_force(force)
        { }
    };

    class MsgStartStop : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        bool getStartStop() const { return m_startStop; }

        static MsgStartStop* create(bool startStop) {
            return new MsgStartStop(startStop);
        }

    protected:
        bool m_startStop;

        MsgStartStop(bool startStop) :
            Message(),
            m_startStop(startStop)
        { }
    };

    class MsgReportGainRange : public Message {
        MESSAGE_CLASS_DECLARATION

    public:
        int getMin() const { return m_min; }
        int getMax() const { return m_max; }
        int getStep() const { return m_step; }

        static MsgReportGainRange* create(int min, int max, int step) {
            return new MsgReportGainRange(min, max, step);
        }

    protected:
        int m_min;
        int m_max;
        int m_step;

        MsgReportGainRange(int min, int max, int step) :
            Message(),
            m_min(min),
            m_max(max),
            m_step(step)
        {}
    };

    BladeRF2Output(DeviceSinkAPI *deviceAPI);
    virtual ~BladeRF2Output();
    virtual void destroy();

    virtual void init();
    virtual bool start();
    virtual void stop();
    BladeRF2OutputThread *getThread() { return m_thread; }
    void setThread(BladeRF2OutputThread *thread) { m_thread = thread; }

    virtual QByteArray serialize() const;
    virtual bool deserialize(const QByteArray& data);

    virtual void setMessageQueueToGUI(MessageQueue *queue) { m_guiMessageQueue = queue; }
    virtual const QString& getDeviceDescription() const;
    virtual int getSampleRate() const;
    virtual quint64 getCenterFrequency() const;
    virtual void setCenterFrequency(qint64 centerFrequency);

    void getFrequencyRange(uint64_t& min, uint64_t& max, int& step);
    void getSampleRateRange(int& min, int& max, int& step);
    void getBandwidthRange(int& min, int& max, int& step);
    void getGlobalGainRange(int& min, int& max, int& step);

    virtual bool handleMessage(const Message& message);

    virtual int webapiSettingsGet(
                SWGSDRangel::SWGDeviceSettings& response,
                QString& errorMessage);

    virtual int webapiSettingsPutPatch(
                bool force,
                const QStringList& deviceSettingsKeys,
                SWGSDRangel::SWGDeviceSettings& response, // query + response
                QString& errorMessage);

    virtual int webapiReportGet(
            SWGSDRangel::SWGDeviceReport& response,
            QString& errorMessage);

    virtual int webapiRunGet(
            SWGSDRangel::SWGDeviceState& response,
            QString& errorMessage);

    virtual int webapiRun(
            bool run,
            SWGSDRangel::SWGDeviceState& response,
            QString& errorMessage);

private:
    bool openDevice();
    void closeDevice();
    BladeRF2OutputThread *findThread();
    void moveThreadToBuddy();
    bool applySettings(const BladeRF2OutputSettings& settings, bool force);
    int getNbChannels();
    bool setDeviceCenterFrequency(struct bladerf *dev, int requestedChannel, quint64 freq_hz);
    void webapiFormatDeviceSettings(SWGSDRangel::SWGDeviceSettings& response, const BladeRF2OutputSettings& settings);
    void webapiFormatDeviceReport(SWGSDRangel::SWGDeviceReport& response);

    DeviceSinkAPI *m_deviceAPI;
    QMutex m_mutex;
    BladeRF2OutputSettings m_settings;
    struct bladerf* m_dev;
    BladeRF2OutputThread* m_thread;
    QString m_deviceDescription;
    DeviceBladeRF2Shared m_deviceShared;
    bool m_running;
};

#endif /* PLUGINS_SAMPLESINK_BLADERF2OUTPUT_BLADERF2OUTPUT_H_ */
