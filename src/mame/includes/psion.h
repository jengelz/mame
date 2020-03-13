// license:BSD-3-Clause
// copyright-holders:Sandro Ronco
/***************************************************************************

        Psion Organiser II series

****************************************************************************/
#ifndef MAME_INCLUDES_PSION_H
#define MAME_INCLUDES_PSION_H

#pragma once

#include "cpu/m6800/m6801.h"
#include "machine/nvram.h"
#include "machine/psion_pack.h"
#include "machine/timer.h"
#include "video/hd44780.h"
#include "sound/beep.h"
#include "emupal.h"


// ======================> psion_state

class psion_state : public driver_device
{
public:
	psion_state(const machine_config &mconfig, device_type type, const char *tag)
		: driver_device(mconfig, type, tag)
		, m_maincpu(*this, "maincpu")
		, m_lcdc(*this, "hd44780")
		, m_beep(*this, "beeper")
		, m_pack1(*this, "pack1")
		, m_pack2(*this, "pack2")
		, m_nvram1(*this, "nvram1")
		, m_nvram2(*this, "nvram2")
		, m_nvram3(*this, "nvram3")
		, m_sys_register(*this, "sys_register")
		, m_stby_pwr(1)
		, m_ram(*this, "ram")
	{ }

	void psion_2lines(machine_config &config);
	void psion_4lines(machine_config &config);
	void psionlam(machine_config &config);
	void psioncm(machine_config &config);
	void psionlz(machine_config &config);
	void psionla(machine_config &config);
	void psionp350(machine_config &config);

	DECLARE_INPUT_CHANGED_MEMBER(psion_on);

protected:
	required_device<hd6301x_cpu_device> m_maincpu;
	required_device<hd44780_device> m_lcdc;
	required_device<beep_device> m_beep;
	required_device<datapack_device> m_pack1;
	required_device<datapack_device> m_pack2;
	required_device<nvram_device> m_nvram1;
	required_device<nvram_device> m_nvram2;
	optional_device<nvram_device> m_nvram3;

	uint16_t m_kb_counter;
	uint8_t m_enable_nmi;
	optional_shared_ptr<uint8_t> m_sys_register;
	uint8_t m_tcsr_value;
	uint8_t m_stby_pwr;
	uint8_t m_pulse;

	uint8_t m_port2_ddr;  // datapack i/o ddr
	uint8_t m_port2;      // datapack i/o data bus

	// RAM/ROM banks
	required_shared_ptr<uint8_t> m_ram;
	std::unique_ptr<uint8_t[]> m_paged_ram;
	uint8_t m_rom_bank;
	uint8_t m_ram_bank;
	uint8_t m_ram_bank_count;
	uint8_t m_rom_bank_count;

	virtual void machine_start() override;
	virtual void machine_reset() override;
	void nvram_init(nvram_device &nvram, void *data, size_t size);

	uint8_t kb_read();
	void update_banks();
	void port2_ddr_w(uint8_t data);
	void port2_w(uint8_t data);
	uint8_t port2_r();
	void tcsr_w(uint8_t data);
	uint8_t tcsr_r();
	uint8_t rcp5c_r();
	uint8_t port5_r();
	void port6_w(uint8_t data);
	uint8_t port6_r();
	void io_rw(address_space &space, uint16_t offset);
	DECLARE_WRITE8_MEMBER( io_w );
	DECLARE_READ8_MEMBER( io_r );
	void psion_palette(palette_device &palette) const;
	TIMER_DEVICE_CALLBACK_MEMBER(nmi_timer);

	HD44780_PIXEL_UPDATE(lz_pixel_update);
	void psion_int_reg(address_map &map);
	void psioncm_mem(address_map &map);
	void psionla_mem(address_map &map);
	void psionlam_mem(address_map &map);
	void psionlz_mem(address_map &map);
	void psionp350_mem(address_map &map);
};


class psion1_state : public psion_state
{
public:
	psion1_state(const machine_config &mconfig, device_type type, const char *tag)
		: psion_state(mconfig, type, tag)
	{ }

	void psion1(machine_config &config);

private:
	virtual void machine_reset() override;

	DECLARE_READ8_MEMBER( reset_kb_counter_r );
	DECLARE_READ8_MEMBER( inc_kb_counter_r );
	DECLARE_READ8_MEMBER( switchoff_r );

	HD44780_PIXEL_UPDATE(psion1_pixel_update);
	void psion1_mem(address_map &map);
};

#endif // MAME_INCLUDES_PSION_H
