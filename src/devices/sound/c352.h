// license:BSD-3-Clause
// copyright-holders:R. Belmont, superctr
#ifndef MAME_SOUND_C352_H
#define MAME_SOUND_C352_H

#pragma once


//**************************************************************************
//  TYPE DEFINITIONS
//**************************************************************************

// ======================> asc_device

class c352_device : public device_t,
					public device_sound_interface,
					public device_rom_interface
{
public:
	// construction/destruction
	c352_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock, int divider)
		: c352_device(mconfig, tag, owner, clock)
	{
		set_divider(divider);
	}

	c352_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock);

	void set_divider(int divider) { m_divider = divider; }

	DECLARE_READ16_MEMBER(read);
	DECLARE_WRITE16_MEMBER(write);

protected:
	// device-level overrides
	virtual void device_start() override;
	virtual void device_reset() override;
	virtual void device_clock_changed() override;

	// device_sound_interface overrides
	virtual void sound_stream_update(sound_stream &stream, stream_sample_t **inputs, stream_sample_t **outputs, int samples) override;

	// device_rom_interface overrides
	virtual void rom_bank_updated() override;

private:
	enum
	{
		C352_FLG_BUSY       = 0x8000,   // channel is busy
		C352_FLG_KEYON      = 0x4000,   // Keyon
		C352_FLG_KEYOFF     = 0x2000,   // Keyoff
		C352_FLG_LOOPTRG    = 0x1000,   // Loop Trigger
		C352_FLG_LOOPHIST   = 0x0800,   // Loop History
		C352_FLG_FM         = 0x0400,   // Frequency Modulation
		C352_FLG_PHASERL    = 0x0200,   // Rear Left invert phase 180 degrees
		C352_FLG_PHASEFL    = 0x0100,   // Front Left invert phase 180 degrees
		C352_FLG_PHASEFR    = 0x0080,   // invert phase 180 degrees (e.g. flip sign of sample)
		C352_FLG_LDIR       = 0x0040,   // loop direction
		C352_FLG_LINK       = 0x0020,   // "long-format" sample (can't loop, not sure what else it means)
		C352_FLG_NOISE      = 0x0010,   // play noise instead of sample
		C352_FLG_MULAW      = 0x0008,   // sample is mulaw instead of linear 8-bit PCM
		C352_FLG_FILTER     = 0x0004,   // don't apply filter
		C352_FLG_REVLOOP    = 0x0003,   // loop backwards
		C352_FLG_LOOP       = 0x0002,   // loop forward
		C352_FLG_REVERSE    = 0x0001    // play sample backwards
	};

	struct c352_voice_t
	{
		u32 pos;
		u32 counter;

		s16 sample;
		s16 last_sample;

		u16 vol_f;
		u16 vol_r;
		u8 curr_vol[4];

		u16 freq;
		u16 flags;

		u16  wave_bank;
		u16 wave_start;
		u16 wave_end;
		u16 wave_loop;

	};

	void fetch_sample(c352_voice_t &v);
	void ramp_volume(c352_voice_t &v, int ch, u8 val);

	u16 read_reg16(offs_t offset);
	void write_reg16(offs_t offset, u16 data, u16 mem_mask = 0);

	sound_stream *m_stream;

	int m_sample_rate_base;
	int m_divider;

	c352_voice_t m_c352_v[32];

	s16 m_mulawtab[256];

	u16 m_random;
	u16 m_control; // control flags, purpose unknown.
};


// device type definition
DECLARE_DEVICE_TYPE(C352, c352_device)

#endif // MAME_SOUND_C352_H
