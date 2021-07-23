#ifndef __TapTempo_hpp__
#define __TapTempo_hpp__

// #define TAP_THRESHOLD     64// 256 // 78Hz at 20kHz sampling rate, or 16th
// notes at 293BPM

template<int TRIGGER_LIMIT>
class TapTempo
{
  private:
    uint32_t limit; // represents the current period of the tap tempo
    uint32_t trig;  // records the time in samples since the last tap
    uint16_t speed; // ??
    bool ison;      // whether the tap tempo button is being tapped this frame

  public:
    // tempo is in samples?
    TapTempo(uint32_t tempo)
      : limit(tempo),
        trig(TRIGGER_LIMIT),
        speed(2048),
        ison(false)
    {}

	bool isOn() { return ison; }

	// used to record a "tap" of the tap tempo
    void trigger(bool on) { trigger(on, 0); }
    void trigger(bool on, int delay)
    {
        // if(trig < TAP_THRESHOLD)
        //   return;
        if (on && ! ison) {
            if (trig < TRIGGER_LIMIT) { limit = trig + delay; }
            trig = 0;
            //      debugMessage("limit/delay", (int)limit, (int)delay);
        }
        ison = on;
    }

    void setLimit(uint32_t value) { limit = value; }

	// this seems like a super janky way of allowing the tap tempo to also
	// represent speed or something...
    void setSpeed(int16_t s)
    {
        //if (abs(speed - s) > 16) {
            int64_t delta = (int64_t)limit * (speed - s) / 2048;
            limit = max(1, limit + delta);
            speed = s;
        //}
    }

    float getPeriod() { return float(limit) / TRIGGER_LIMIT; }

    float getFrequency() { return TRIGGER_LIMIT / float(limit); }

	// used to measure the distance between taps
    void clock()
    {
        if (trig < TRIGGER_LIMIT) trig++;
    }
    void clock(uint32_t steps)
    {
        trig += steps;
        if (trig > TRIGGER_LIMIT) trig = TRIGGER_LIMIT;
    }
};

#endif // __TapTempo_hpp__
