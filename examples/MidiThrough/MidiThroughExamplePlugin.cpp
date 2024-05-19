/*
 * DISTRHO Plugin Framework (DPF)
 * Copyright (C) 2012-2024 Filipe Coelho <falktx@falktx.com>
 *
 * Permission to use, copy, modify, and/or distribute this software for any purpose with
 * or without fee is hereby granted, provided that the above copyright notice and this
 * permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD
 * TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL
 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER
 * IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "DistrhoPlugin.hpp"

START_NAMESPACE_DISTRHO

// -----------------------------------------------------------------------------------------------------------

/**
  Plugin that demonstrates MIDI output in DPF.
 */
    class MidiThroughExamplePlugin : public Plugin {
    public:
        MidiThroughExamplePlugin()
                : Plugin(0, 0, 0) {

            _log = fopen("/tmp/midithru.log", "w+");
            log("opened");
        }

        ~MidiThroughExamplePlugin() override {
            if (_log) {
                log("destroyed");
                fclose(_log);
            }
        }

    protected:
        /* --------------------------------------------------------------------------------------------------------
         * Information */

        /**
           Get the plugin label.
           This label is a short restricted name consisting of only _, a-z, A-Z and 0-9 characters.
         */
        const char *getLabel() const override {
            return "MidiThrough";
        }

        /**
           Get an extensive comment/description about the plugin.
         */
        const char *getDescription() const override {
            return "Plugin that demonstrates MIDI output in DPF.";
        }

        /**
           Get the plugin author/maker.
         */
        const char *getMaker() const override {
            return "DISTRHO";
        }

        /**
           Get the plugin homepage.
         */
        const char *getHomePage() const override {
            return "https://github.com/DISTRHO/DPF";
        }

        /**
           Get the plugin license name (a single line of text).
           For commercial plugins this should return some short copyright information.
         */
        const char *getLicense() const override {
            return "ISC";
        }

        /**
           Get the plugin version, in hexadecimal.
         */
        uint32_t getVersion() const override {
            return d_version(1, 0, 5);
        }

        /* --------------------------------------------------------------------------------------------------------
         * Init and Internal data, unused in this plugin */

        void initParameter(uint32_t, Parameter &) override {}

        float getParameterValue(uint32_t) const override { return 0.0f; }

        void setParameterValue(uint32_t, float) override {}

        /* --------------------------------------------------------------------------------------------------------
         * Audio/MIDI Processing */

        /**
           Run/process function for plugins with MIDI input.
           In this case we just pass-through all MIDI events.
         */
        void run(const float **, float **, uint32_t,
                 const MidiEvent *midiEvents, uint32_t midiEventCount) override {
            log("run");
            for (uint32_t i = 0; i < midiEventCount; ++i) {
                log("MIDI %d %d %d %d",
                    static_cast<int>(midiEvents->data[0]),
                    static_cast<int>(midiEvents->data[1]), static_cast<int>(midiEvents->data[2]),
                    static_cast<int>(midiEvents->data[3]));
                writeMidiEvent(midiEvents[i]);
            }
        }

        // -------------------------------------------------------------------------------------------------------

    private:
        FILE *_log;

        int log(const char *format, ...) {
            va_list args;
            va_start(args, format);
            int result = -42;
            if (_log) {
                time_t rawTime;
                time(&rawTime);
                const char *timeString = ctime(&rawTime);
                fprintf(_log, "%*s ",
                        static_cast<int>(strlen(timeString)) - 2, timeString);
                result = vfprintf(_log, format, args);
                fprintf(_log, "\n");
                fflush(_log);
            }
            va_end(args);
            return result;
        }

        /**
           Set our plugin class as non-copyable and add a leak detector just in case.
         */
    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MidiThroughExamplePlugin)
    };

/* ------------------------------------------------------------------------------------------------------------
 * Plugin entry point, called by DPF to create a new plugin instance. */

    Plugin *createPlugin() {
        return new MidiThroughExamplePlugin();
    }

// -----------------------------------------------------------------------------------------------------------

END_NAMESPACE_DISTRHO
