#include "fat_sd_spi.h"


static void spi_init(void)
{
	/* Configure spi using CubeMX!!! */
	spi_set_speed(SD_SPEED_400KHZ);
}

static void spi_set_speed(enum sd_speed speed)
{
	//CHECK YOUR APB1 FREQ!!!
	SPI_InitTypeDef spi;
	int prescaler = SPI_BAUDRATEPRESCALER_8;

	if (speed == SD_SPEED_400KHZ)
		prescaler = SPI_BAUDRATEPRESCALER_64;
	else if (speed == SD_SPEED_25MHZ)
		prescaler = SPI_BAUDRATEPRESCALER_8;

	FAT_SD_SPI.Init.BaudRatePrescaler = prescaler;
	HAL_SPI_Init(&FAT_SD_SPI);
}

static uint8_t spi_txrx(uint8_t data)
{
	uint8_t out = 0;
	HAL_SPI_TransmitReceive(&FAT_SD_SPI, &data, &out, sizeof(data), HAL_MAX_DELAY);
	return out;
}


/* crc helpers */
static uint8_t crc7_one(uint8_t t, uint8_t data)
{
	int i;
	const uint8_t g = 0x89;
	t ^= data;
	for (i=0; i<8; i++) {
		if (t & 0x80)
			t ^= g;
		t <<= 1;
	}
	return t;
}
uint8_t crc7(const uint8_t *p, int len)
{
	int j;
	uint8_t crc = 0;
	for (j=0; j<len; j++)
		crc = crc7_one(crc, p[j]);
	return crc>>1;
}
static uint16_t crc16_ccitt(uint16_t crc, uint8_t ser_data)
{
	crc  = (uint8_t)(crc >> 8) | (crc << 8);
	crc ^= ser_data;
	crc ^= (uint8_t)(crc & 0xff) >> 4;
	crc ^= (crc << 8) << 4;
	crc ^= ((crc & 0xff) << 4) << 1;
	return crc;
}
uint16_t crc16(const uint8_t *p, int len)
{
	int i;
	uint16_t crc = 0;
	for (i=0; i<len; i++)
		crc = crc16_ccitt(crc, p[i]);
	return crc;
}


/*** sd functions - on top of spi code ***/

static void sd_cmd(uint8_t cmd, uint32_t arg)
{
	uint8_t crc = 0;
	spi_txrx(0x40 | cmd);
	crc = crc7_one(crc, 0x40 | cmd);
	spi_txrx(arg >> 24);
	crc = crc7_one(crc, arg >> 24);
	spi_txrx(arg >> 16);
	crc = crc7_one(crc, arg >> 16);
	spi_txrx(arg >> 8);
	crc = crc7_one(crc, arg >> 8);
	spi_txrx(arg);
	crc = crc7_one(crc, arg);
	//spi_txrx(0x95);
	spi_txrx(crc | 0x1);	/* crc7, for cmd0 */
}

static uint8_t sd_get_r1()
{
	int tries = 1000;
	uint8_t r;

	while (tries--) {
		r = spi_txrx(0xff);
		if ((r & 0x80) == 0)
			return r;
	}
	return 0xff;
}

static uint16_t sd_get_r2()
{
	int tries = 1000;
	uint16_t r;
	while (tries--) {
		r = spi_txrx(0xff);
		if ((r & 0x80) == 0)
			break;
	}
	if (tries < 0)
		return 0xff;
	r = r<<8 | spi_txrx(0xff);
	return r;
}

/*
 * r1, then 32-bit reply... same format as r3
 */
static uint8_t sd_get_r7(uint32_t *r7)
{
	uint32_t r;
	r = sd_get_r1();
	if (r != 0x01)
		return r;
	r = spi_txrx(0xff) << 24;
	r |= spi_txrx(0xff) << 16;
	r |= spi_txrx(0xff) << 8;
	r |= spi_txrx(0xff);
	*r7 = r;
	return 0x01;
}


/* Nec (=Ncr? which is limited to [0,8]) dummy bytes before lowering CS,
 * as described in sandisk doc, 5.4. */
static void sd_nec()
{
	int i;
	for (i=0; i<8; i++)
		spi_txrx(0xff);
}


static int sd_init(hwif *hw)
{
	int i;
	int r;
	uint32_t r7;
	uint32_t r3;
	int tries;

	hw->capabilities = 0;

	/* start with 100-400 kHz clock */
	spi_set_speed(SD_SPEED_400KHZ);

	spi_cs_high();
	/* 74+ clocks with CS high */
	for (i=0; i<10; i++)
		spi_txrx(0xff);

	/* reset */
	spi_cs_low();
	sd_cmd(0, 0);
	r = sd_get_r1();
	sd_nec();
	spi_cs_high();
	if (r == 0xff)
		goto err_spi;
	if (r != 0x01) {
		goto err;
	}

	/* ask about voltage supply */
	spi_cs_low();
	sd_cmd(8, 0x1aa /* VHS = 1 */);
	r = sd_get_r7(&r7);
	sd_nec();
	spi_cs_high();
	hw->capabilities |= CAP_VER2_00;
	if (r == 0xff)
		goto err_spi;
	if (r == 0x01)
		;
	else if (r & 0x4) {
		hw->capabilities &= ~CAP_VER2_00;
	} else {
		return -2;
	}


	/* ask about voltage supply */
	spi_cs_low();
	sd_cmd(58, 0);
	r = sd_get_r3(&r3);
	sd_nec();
	spi_cs_high();
	if (r == 0xff)
		goto err_spi;
	if (r != 0x01 && !(r & 0x4)) { /* allow it to not be implemented - old cards */
		return -2;
	}
	else {
		int i;
		for (i=4; i<=23; i++)
			if (r3 & 1<<i)
				break;
		for (i=23; i>=4; i--)
			if (r3 & 1<<i)
				break;
		/* CCS shouldn't be valid here yet */
	}


	tries = 1000;
	uint32_t hcs = 0;
	/* say we support SDHC */
	if (hw->capabilities & CAP_VER2_00)
		hcs = 1<<30;

	/* needs to be polled until in_idle_state becomes 0 */
	do {
		/* send we don't support SDHC */
		spi_cs_low();
		/* next cmd is ACMD */
		sd_cmd(55, 0);
		r = sd_get_r1();
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			goto err_spi;
		/* well... it's probably not idle here, but specs aren't clear */
		if (r & 0xfe) {
			goto err;
		}

		spi_cs_low();
		sd_cmd(41, hcs);
		r = sd_get_r1();
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			goto err_spi;
		if (r & 0xfe) {
			goto err;
		}
	} while (r != 0 && tries--);
	if (tries == -1) {
		goto err;
	}

	/* Seems after this card is initialized which means bit 0 of R1
	 * will be cleared. Not too sure. */


	if (hw->capabilities & CAP_VER2_00) {
		/* ask about voltage supply */
		spi_cs_low();
		sd_cmd(58, 0);
		r = sd_get_r3(&r3);
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			goto err_spi;
		if (r & 0xfe) {
			return -2;
		}
		else {
#if 1
			int i;
			for (i=4; i<=23; i++)
				if (r3 & 1<<i)
					break;
			for (i=23; i>=4; i--)
				if (r3 & 1<<i)
					break;
			/* CCS shouldn't be valid here yet */
			// XXX power up status should be 1 here, since we're finished initializing, but it's not. WHY?
			// that means CCS is invalid, so we'll set CAP_SDHC later
#endif
			if (r3>>30 & 1) {
				hw->capabilities |= CAP_SDHC;
			}

		}
	}


	/* with SDHC block length is fixed to 1024 */
	if ((hw->capabilities & CAP_SDHC) == 0) {
		spi_cs_low();
		sd_cmd(16, 512);
		r = sd_get_r1();
		sd_nec();
		spi_cs_high();
		if (r == 0xff)
			goto err_spi;
		if (r & 0xfe) {
			goto err;
		}
	}


	/* crc on */
	spi_cs_low();
	sd_cmd(59, 0);
	r = sd_get_r1();
	sd_nec();
	spi_cs_high();
	if (r == 0xff)
		goto err_spi;
	if (r & 0xfe) {
		goto err;
	}


	/* now we can up the clock to <= 25 MHz */
	spi_set_speed(SD_SPEED_25MHZ);

	return 0;

 err_spi:
	return -1;
 err:
	return -2;
}

static int sd_read_status(hwif *hw)
{
	uint16_t r2;

	spi_cs_low();
	sd_cmd(13, 0);
	r2 = sd_get_r2();
	sd_nec();
	spi_cs_high();
	if (r2 & 0x8000)
		return -1;
	if (r2)

	return 0;
}

/* 0xfe marks data start, then len bytes of data and crc16 */
static int sd_get_data(hwif *hw, uint8_t *buf, int len)
{
	int tries = 20000;
	uint8_t r;
	uint16_t _crc16;
	uint16_t calc_crc;
	int i;

	while (tries--) {
		r = spi_txrx(0xff);
		if (r == 0xfe)
			break;
	}
	if (tries < 0)
		return -1;

	//for (i=0; i<len; i++)
	//	buf[i] = spi_txrx(0xff);
	dma_complete = 0;
	HAL_SPI_Receive_DMA(&FAT_SD_SPI, buf, len);
	while (dma_complete == 0);

	//_crc16 = spi_txrx(0xff) << 8;
	//_crc16 |= spi_txrx(0xff);

	//calc_crc = crc16(buf, len);
	//if (_crc16 != calc_crc) {
	//	return -1;
	//}

	return 0;
}

static int sd_put_data(hwif *hw, const uint8_t *buf, int len)
{
	uint8_t r;
	int tries = 10;
	uint8_t b[16];
	int bi = 0;
	uint16_t crc;

	spi_txrx(0xfe); /* data start */

	while (len--)
		spi_txrx(*buf++);

	crc = crc16(buf, len);
	/* crc16 */
	spi_txrx(crc>>8);
	spi_txrx(crc);

	/* normally just one dummy read in between... specs don't say how many */
	while (tries--) {
		b[bi++] = r = spi_txrx(0xff);
		if (r != 0xff)
			break;
	}
	if (tries < 0)
		return -1;

	/* poll busy, about 300 reads for 256 MB card */
	tries = 100000;
	while (tries--) {
		if (spi_txrx(0xff) == 0xff)
			break;
	}
	if (tries < 0)
		return -2;

	/* data accepted, WIN */
	if ((r & 0x1f) == 0x05)
		return 0;

	return r;
}

static int sd_read_csd(hwif *hw)
{
	uint8_t buf[16];
	int r;
	int capacity;

	spi_cs_low();
	sd_cmd(9, 0);
	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		return -1;
	}
	if (r & 0xfe) {
		spi_cs_high();
		return -2;
	}

	r = sd_get_data(hw, buf, 16);
	sd_nec();
	spi_cs_high();
	if (r == -1) {
		return -3;
	}

	if ((buf[0] >> 6) + 1 == 1) {
	/* CSD v1 */

	capacity = (((buf[6]&0x3)<<10 | buf[7]<<2 | buf[8]>>6)+1) << (2+(((buf[9]&3) << 1) | buf[10]>>7)) << ((buf[5] & 0xf) - 9);
	/* ^ = (c_size+1) * 2**(c_size_mult+2) * 2**(read_bl_len-9) */

	} else {
	/* CSD v2 */
		/* this means the card is HC */
		hw->capabilities |= CAP_SDHC;

	capacity = buf[7]<<16 | buf[8]<<8 | buf[9]; /* in 512 kB */
	capacity *= 1024; /* in 512 B sectors */

	}

	hw->sectors = capacity;

	/* if erase_blk_en = 0, then only this many sectors can be erased at once
	 * this is NOT yet tested */
	hw->erase_sectors = 1;
	if (((buf[10]>>6)&1) == 0)
		hw->erase_sectors = ((buf[10]&0x3f)<<1 | buf[11]>>7) + 1;

	return 0;
}

static int sd_read_cid(hwif *hw)
{
	uint8_t buf[16];
	int r;

	spi_cs_low();
	sd_cmd(10, 0);
	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		return -1;
	}
	if (r & 0xfe) {
		spi_cs_high();
		return -2;
	}

	r = sd_get_data(hw, buf, 16);
	sd_nec();
	spi_cs_high();
	if (r == -1) {
		return -3;
	}

	return 0;
}


static int sd_readsector(hwif *hw, uint32_t address, uint8_t *buf)
{
	int r;

	spi_cs_low();
	if (hw->capabilities & CAP_SDHC)
		sd_cmd(17, address); /* read single block */
	else
		sd_cmd(17, address*512); /* read single block */

	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		r = -1;
		goto fail;
	}
	if (r & 0xfe) {
		spi_cs_high();
		r = -2;
		goto fail;
	}

	r = sd_get_data(hw, buf, 512);
	sd_nec();
	spi_cs_high();
	if (r == -1) {
		r = -3;
		goto fail;
	}

	return 0;
 fail:
	return r;
}

static int sd_writesector(hwif *hw, uint32_t address, const uint8_t *buf)
{
	int r;

	spi_cs_low();
	if (hw->capabilities & CAP_SDHC)
		sd_cmd(24, address); /* write block */
	else
		sd_cmd(24, address*512); /* write block */

	r = sd_get_r1();
	if (r == 0xff) {
		spi_cs_high();
		r = -1;
		goto fail;
	}
	if (r & 0xfe) {
		spi_cs_high();
		r = -2;
		goto fail;
	}

	spi_txrx(0xff); /* Nwr (>= 1) high bytes */
	r = sd_put_data(hw, buf, 512);
	sd_nec();
	spi_cs_high();
	if (r != 0) {
		r = -3;
		goto fail;
	}

	/* efsl code is weird shit, 0 is error in there?
	 * not that it's properly handled or anything,
	 * and the return type is char, fucking efsl */
	return 0;
 fail:
	return r;
}


/*** public API - on top of sd/spi code ***/

int hwif_init(hwif* hw)
{
	int tries = 10;

	if (hw->initialized)
		return 0;

	spi_init();

	while (tries--) {
		if (sd_init(hw) == 0)
			break;
	}
	if (tries == -1)
		return -1;

	/* read status register */
	sd_read_status(hw);

	sd_read_cid(hw);
	if (sd_read_csd(hw) != 0)
		return -1;

	hw->initialized = 1;
	return 0;
}

int sd_read(hwif* hw, uint32_t address, uint8_t *buf)
{
	int r;
	int tries = 10;

	r = sd_readsector(hw, address, buf);

	while (r < 0 && tries--) {
		if (sd_init(hw) != 0)
			continue;

		// read status register
		sd_read_status(hw);

		r = sd_readsector(hw, address, buf);
	}
	if (tries == -1)

	return r;
}

int sd_write(hwif* hw, uint32_t address,const uint8_t *buf)
{
	int r;
	int tries = 10;

	r = sd_writesector(hw, address, buf);

	while (r < 0 && tries--) {
		if (sd_init(hw) != 0)
			continue;

		/* read status register */
		sd_read_status(hw);

		r = sd_writesector(hw, address, buf);
	}
	if (tries == -1)

	return r;
}

void HAL_SPI_RxCpltCallback(SPI_HandleTypeDef* hspi) {
    dma_complete = 1;
	return;
}
