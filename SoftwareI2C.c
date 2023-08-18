static int8_t SoftwareI2C_SDA;
static int8_t SoftwareI2C_SCL;
static uint32_t SoftwareI2C_DELAY= 0;

static void SoftwareI2C_set(int8_t pin, int8_t val)
{
    if (val == LOW)
    {
        digitalWrite(pin, LOW);
        pinMode(pin, OUTPUT);
    }
    else
    {
        pinMode(pin, INPUT_PULLUP);
    }
    delayMicroseconds(SoftwareI2C_DELAY);
}

//Read a digital pin (open collector).
static int8_t SoftwareI2C_get(int8_t pin)
{
    pinMode(pin, INPUT_PULLUP);
    return digitalRead(pin);
}

static void SoftwareI2C_begin(uint8_t sda, uint8_t scl)
{
    SoftwareI2C_SDA = sda;
    SoftwareI2C_SCL = scl;
    //release the bus
    SoftwareI2C_set(sda, HIGH);
    SoftwareI2C_set(scl, HIGH);
}

static void SoftwareI2C_start()
{
    //Start condition (SDA low followed by SCL, in that exact order)
    SoftwareI2C_set(SoftwareI2C_SDA, LOW);
    SoftwareI2C_set(SoftwareI2C_SCL, LOW);
}

static void SoftwareI2C_stop()
{
    //Start condition (SDA low followed by SCL, in that exact order)
    SoftwareI2C_set(SoftwareI2C_SCL, HIGH);
    SoftwareI2C_set(SoftwareI2C_SDA, HIGH);
}

static uint8_t SoftwareI2C_write(uint8_t databyte)
{
    for (int8_t i = 0; i < 8; i++)
    {
        //Bits cannot change while scl is high
        SoftwareI2C_set(SoftwareI2C_SDA, databyte >> 7);
        SoftwareI2C_set(SoftwareI2C_SCL, HIGH);
        SoftwareI2C_set(SoftwareI2C_SCL, LOW);
        databyte <<= 1;
    }

    //release the databus since we need to receive ACK
    SoftwareI2C_set(SoftwareI2C_SDA, HIGH);

    SoftwareI2C_set(SoftwareI2C_SCL, HIGH);
    int8_t ACK = !SoftwareI2C_get(SoftwareI2C_SDA);
    SoftwareI2C_set(SoftwareI2C_SCL, LOW);

    return ACK;
}

static uint8_t SoftwareI2C_read()
{
    uint8_t ret = 0;
    for (int8_t i = 0; i < 8; i++)
    {
        //Bits cannot change while scl is high
        ret <<= 1;
        SoftwareI2C_set(SoftwareI2C_SCL, HIGH);
        ret |= SoftwareI2C_get(SoftwareI2C_SDA);
        SoftwareI2C_set(SoftwareI2C_SCL, LOW);
    }

    //send an ACK
    SoftwareI2C_set(SoftwareI2C_SDA, LOW);
    SoftwareI2C_set(SoftwareI2C_SCL, HIGH);
    SoftwareI2C_set(SoftwareI2C_SCL, LOW);

    return ret;
}

static int16_t SoftwareI2C_SearchForAddress()
{
    for (uint16_t i = 0; i <= 255; i++)
    {
        SoftwareI2C_start();
        if (SoftwareI2C_write(i))
        {
            SoftwareI2C_stop();
            return i;
        }
        SoftwareI2C_stop();
    }
    return -1;
}

static uint8_t SoftwareI2C_wordOut(uint8_t a, uint16_t b)
{
    int8_t ret = 0;
    SoftwareI2C_start();
    if (SoftwareI2C_write(a << 0))
    {
        if (SoftwareI2C_write(b >> 8))
        {
        	if (SoftwareI2C_write(b & 0xFF))
		{
            		ret = 1;
		}
        }
    }
    SoftwareI2C_stop();
    return ret;

}

static uint8_t SoftwareI2C_byteOut(uint8_t a, uint8_t b)
{
    int8_t ret = 0;
    SoftwareI2C_start();
    if (SoftwareI2C_write(a << 1))
    {
        if (SoftwareI2C_write(b))
        {
            ret = 1;
        }
    }
    SoftwareI2C_stop();
    return ret;
}

static uint8_t SoftwareI2C_byteIn(uint8_t a, uint8_t *b)
{
    int8_t ret = 0;
    SoftwareI2C_start();
    if (SoftwareI2C_write(a << 0))
    {
        *b = SoftwareI2C_read();
    }
    SoftwareI2C_stop();
    return ret;
}

struct _SoftwareI2C
{
    void (*begin)(uint8_t, uint8_t);
    uint8_t (*write8)(uint8_t, uint8_t);
    uint8_t (*write16)(uint8_t, uint16_t);
    uint8_t (*read)(uint8_t, uint8_t*);
    int16_t (*search)();
} SoftwareI2C =
{
    .begin = SoftwareI2C_begin,
    .write8 = SoftwareI2C_byteOut,
    .write16 = SoftwareI2C_wordOut,
    .read = SoftwareI2C_byteIn,
    .search = SoftwareI2C_SearchForAddress
};
