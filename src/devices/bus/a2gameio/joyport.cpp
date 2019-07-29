// license:BSD-3-Clause
// copyright-holders:R. Belmont
/*********************************************************************

    Sirius JoyPort - connects 2 digital joysticks to Apple II and II Plus

    This doesn't work on the IIe or IIgs because the buttons are
    active low, meaning those systems always go into self-test if a
    JoyPort is connected.  Also, the annunciator useage could clash
    with double-hi-res on those systems.

    IIc and IIc Plus are out because they don't have annunciator outputs.

*********************************************************************/

#include "emu.h"
#include "bus/a2gameio/joyport.h"

//**************************************************************************
//  GLOBAL VARIABLES
//**************************************************************************

// device type definition
DEFINE_DEVICE_TYPE(APPLE2_JOYPORT, apple2_joyport_device, "a2joyprt", "Sirius JoyPort")

//**************************************************************************
//  PARAMETERS
//**************************************************************************

#define JOYSTICK_DELTA          80
#define JOYSTICK_SENSITIVITY    50
#define JOYSTICK_AUTOCENTER     80

//**************************************************************************
//  INPUT PORTS
//**************************************************************************

static INPUT_PORTS_START( apple2_joyport )
	PORT_START("joystick_p1")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP )
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT )
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN )
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT )
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 )

	PORT_START("joystick_p2")
	PORT_BIT( 0x01, IP_ACTIVE_LOW, IPT_JOYSTICK_UP ) PORT_PLAYER(2)
	PORT_BIT( 0x02, IP_ACTIVE_LOW, IPT_JOYSTICK_RIGHT ) PORT_PLAYER(2)
	PORT_BIT( 0x04, IP_ACTIVE_LOW, IPT_JOYSTICK_DOWN ) PORT_PLAYER(2)
	PORT_BIT( 0x08, IP_ACTIVE_LOW, IPT_JOYSTICK_LEFT ) PORT_PLAYER(2)
	PORT_BIT( 0x10, IP_ACTIVE_LOW, IPT_BUTTON1 ) PORT_PLAYER(2)
INPUT_PORTS_END

//**************************************************************************
//  DEVICE IMPLEMENTATION
//**************************************************************************

apple2_joyport_device::apple2_joyport_device(const machine_config &mconfig, const char *tag, device_t *owner, u32 clock)
	: device_t(mconfig, APPLE2_JOYPORT, tag, owner, clock)
	, device_a2gameio_interface(mconfig, *this)
	, m_player1(*this, "joystick_p1")
	, m_player2(*this, "joystick_p2")
{
}

ioport_constructor apple2_joyport_device::device_input_ports() const
{
	return INPUT_PORTS_NAME(apple2_joyport);
}

void apple2_joyport_device::device_start()
{
	save_item(NAME(m_an0));
	save_item(NAME(m_an1));
}

READ_LINE_MEMBER(apple2_joyport_device::sw0_r)
{
	u8 port_read = m_an1 ? m_player2->read() : m_player1->read();

	return BIT(port_read, 4);
}

READ_LINE_MEMBER(apple2_joyport_device::sw1_r)
{
	u8 port_read = m_an1 ? m_player2->read() : m_player1->read();

	return m_an1 ? BIT(port_read, 0) : BIT(port_read, 3);
}

READ_LINE_MEMBER(apple2_joyport_device::sw2_r)
{
	u8 port_read = m_an1 ? m_player2->read() : m_player1->read();

	return m_an1 ? BIT(port_read, 2) : BIT(port_read, 1);
}

WRITE_LINE_MEMBER(apple2_joyport_device::an0_w)
{
	m_an0 = state;
}

WRITE_LINE_MEMBER(apple2_joyport_device::an1_w)
{
	m_an1 = state;
}
