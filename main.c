#include <stdio.h>
#include <stdint.h>
#include "/home/home/Projects/SoftwareI2C.c"



uint8_t ssd1306_send_command(uint8_t b2s)
{
  uint16_t display = 0x78;
  return SoftwareI2C.write16(display, 0x0000 | b2s);
}

uint8_t ssd1306_send_data(uint8_t b2s)
{
  uint16_t display = 0x78;
  return SoftwareI2C.write16(display, 0x4000 | b2s);
}

void ssd1306_init() {
    // Turn display off
    ssd1306_send_command(0xAE);

    // Set display clock divide ratio / oscillator frequency
    ssd1306_send_command(0xD5);
    ssd1306_send_command(0x80);

    // Set multiplex ratio
    ssd1306_send_command(0xA8);
    ssd1306_send_command(0x3F); // 64MUX

    // Set display offset
    ssd1306_send_command(0xD3);
    ssd1306_send_command(0x00);

    // Set start line
    ssd1306_send_command(0x40 | 0x00);

    // Charge pump
    ssd1306_send_command(0x8D);
    ssd1306_send_command(0x14);

    // Memory mode
    ssd1306_send_command(0x20);
    ssd1306_send_command(0x00); // horizontal addressing mode

    // Segment remap
    ssd1306_send_command(0xA0 | 0x01);

    // COM output scan direction
    ssd1306_send_command(0xC8);

    // COM pins hardware configuration
    ssd1306_send_command(0xDA);
    ssd1306_send_command(0x12);

    // Set contrast
    ssd1306_send_command(0x81);
    ssd1306_send_command(0xCF);

    // Set pre-charge period
    ssd1306_send_command(0xD9);
    ssd1306_send_command(0xF1);

    // Set VCOMH deselect level
    ssd1306_send_command(0xDB);
    ssd1306_send_command(0x40);

    // Entire display ON
    ssd1306_send_command(0xA4);

    // Set normal display
    ssd1306_send_command(0xA6);

    // Turn display on
    ssd1306_send_command(0xAF);
}

// Clear the display
void ssd1306_clear_display() {
    ssd1306_send_command(0x20); // Set memory addressing mode
    ssd1306_send_command(0x00); // Horizontal addressing mode
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_send_command(0xB0 | page); // Set page address
        ssd1306_send_command(0x00); // Set lower column address
        ssd1306_send_command(0x10); // Set upper column address
        for (uint8_t column = 0; column < 128; column++) {
            ssd1306_send_data(0x00); // Clear pixel
        }
    }
}

// Fill the display
void ssd1306_fill_display() {
    ssd1306_send_command(0x20); // Set memory addressing mode
    ssd1306_send_command(0x00); // Horizontal addressing mode
    for (uint8_t page = 0; page < 8; page++) {
        ssd1306_send_command(0xB0 | page); // Set page address
        ssd1306_send_command(0x00); // Set lower column address
        ssd1306_send_command(0x10); // Set upper column address
        for (uint8_t column = 0; column < 128; column++) {
            ssd1306_send_data(0xFF); // Set pixel
        }
    }
  }

uint8_t ssd1306_x;
uint8_t ssd1306_y;
const uint8_t FONT_5X7[96][5] = {
    {0x00, 0x00, 0x00, 0x00, 0x00}, // Space
    {0x00, 0x00, 0x5F, 0x00, 0x00}, // !
    {0x00, 0x07, 0x00, 0x07, 0x00}, // "
    {0x14, 0x7F, 0x14, 0x7F, 0x14}, // #
    {0x24, 0x2A, 0x7F, 0x2A, 0x12}, // $
    {0x23, 0x13, 0x08, 0x64, 0x62}, // %
    {0x36, 0x49, 0x55, 0x22, 0x50}, // &
    {0x00, 0x05, 0x03, 0x00, 0x00}, // '
    {0x00, 0x1C, 0x22, 0x41, 0x00}, // (
    {0x00, 0x41, 0x22, 0x1C, 0x00}, // )
    {0x08, 0x2A, 0x1C, 0x2A, 0x08}, // *
    {0x08, 0x08, 0x3E, 0x08, 0x08}, // +
    {0x00, 0x50, 0x30, 0x00, 0x00}, // ,
    {0x08, 0x08, 0x08, 0x08, 0x08}, // -
    {0x00, 0x60, 0x60, 0x00, 0x00}, // .
    {0x20, 0x10, 0x08, 0x04, 0x02}, // /
    {0x3E, 0x51, 0x49, 0x45, 0x3E}, // 0
    {0x00, 0x42, 0x7F, 0x40, 0x00}, // 1
    {0x42, 0x61, 0x51, 0x49, 0x46}, // 2
    {0x21, 0x41, 0x45, 0x4B, 0x31}, // 3
    {0x18, 0x14, 0x12, 0x7F, 0x10}, // 4
    {0x27, 0x45, 0x45, 0x45, 0x39}, // 5
    {0x3C, 0x4A, 0x49, 0x49, 0x30}, // 6
    {0x01, 0x71, 0x09, 0x05, 0x03}, // 7
    {0x36, 0x49, 0x49, 0x49, 0x36}, // 8
    {0x06, 0x49, 0x49, 0x29, 0x1E}, // 9
    {0x00, 0x36, 0x36, 0x00, 0x00}, // :
    {0x00, 0x56, 0x36, 0x00, 0x00}, // ;
    {0x00, 0x08, 0x14, 0x22, 0x41}, // <
    {0x14, 0x14, 0x14, 0x14, 0x14}, // =
    {0x41, 0x22, 0x14, 0x08, 0x00}, // >
    {0x02, 0x01, 0x51, 0x09, 0x06}, // ?
    {0x32, 0x49, 0x79, 0x41, 0x3E}, // @
    {0x7E, 0x11, 0x11, 0x11, 0x7E}, // A
    {0x7F, 0x49, 0x49, 0x49, 0x36}, // B
    {0x3E, 0x41, 0x41, 0x41, 0x22}, // C
    {0x7F, 0x41, 0x41, 0x22, 0x1C}, // D
    {0x7F, 0x49, 0x49, 0x49, 0x41}, // E
    {0x7F, 0x09, 0x09, 0x09, 0x01}, // F
    {0x3E, 0x41, 0x49, 0x49, 0x7A}, // G
    {0x7F, 0x08, 0x08, 0x08, 0x7F}, // H
    {0x00, 0x41, 0x7F, 0x41, 0x00}, // I
    {0x20, 0x40, 0x41, 0x3F, 0x01}, // J
    {0x7F, 0x08, 0x14, 0x22, 0x41}, // K
    {0x7F, 0x40, 0x40, 0x40, 0x40}, // L
    {0x7F, 0x02, 0x04, 0x02, 0x7F}, // M
    {0x7F, 0x04, 0x08, 0x10, 0x7F}, // N
    {0x3E, 0x41, 0x41, 0x41, 0x3E}, // O
    {0x7F, 0x09, 0x09, 0x09, 0x06}, // P
    {0x3E, 0x41, 0x51, 0x21, 0x5E}, // Q
    {0x7F, 0x09, 0x19, 0x29, 0x46}, // R
    {0x46, 0x49, 0x49, 0x49, 0x31}, // S
    {0x01, 0x01, 0x7F, 0x01, 0x01}, // T
    {0x3F, 0x40, 0x40, 0x40, 0x3F}, // U
    {0x1F, 0x20, 0x40, 0x20, 0x1F}, // V
    {0x7F, 0x20, 0x18, 0x20, 0x7F}, // W
    {0x63, 0x14, 0x08, 0x14, 0x63}, // X
    {0x03, 0x04, 0x78, 0x04, 0x03}, // Y
    {0x61, 0x51, 0x49, 0x45, 0x43}, // Z
    {0x00, 0x00, 0x7F, 0x41, 0x41}, // [
    {0x02, 0x04, 0x08, 0x10, 0x20}, // "\"
    {0x41, 0x41, 0x7F, 0x00, 0x00}, // ]
    {0x04, 0x02, 0x01, 0x02, 0x04}, // ^
    {0x40, 0x40, 0x40, 0x40, 0x40}, // _
    //{0x00, 0x01, 0x02, 0x04, 0x00}, // `
    {0x00, 0x05, 0x03, 0x00, 0x00}, // ` ??
    {0x20, 0x54, 0x54, 0x54, 0x78}, // a
    {0x7F, 0x48, 0x44, 0x44, 0x38}, // b
    {0x38, 0x44, 0x44, 0x44, 0x20}, // c
    {0x38, 0x44, 0x44, 0x48, 0x7F}, // d
    {0x38, 0x54, 0x54, 0x54, 0x18}, // e
    {0x08, 0x7E, 0x09, 0x01, 0x02}, // f
    {0x0C, 0x52, 0x52, 0x52, 0x3E}, // g
    {0x7F, 0x08, 0x04, 0x04, 0x78}, // h
    {0x00, 0x44, 0x7D, 0x40, 0x00}, // i
    {0x20, 0x40, 0x44, 0x3D, 0x00}, // j
    {0x7F, 0x10, 0x28, 0x44, 0x00}, // k
    {0x00, 0x41, 0x7F, 0x40, 0x00}, // l
    {0x7C, 0x04, 0x18, 0x04, 0x78}, // m
    {0x7C, 0x08, 0x04, 0x04, 0x78}, // n
    {0x38, 0x44, 0x44, 0x44, 0x38}, // o
    {0x7C, 0x14, 0x14, 0x14, 0x08}, // p
    {0x08, 0x14, 0x14, 0x18, 0x7C}, // q
    {0x7C, 0x08, 0x04, 0x04, 0x08}, // r
    {0x48, 0x54, 0x54, 0x54, 0x20}, // s
    {0x04, 0x3F, 0x44, 0x40, 0x20}, // t
    {0x3C, 0x40, 0x40, 0x20, 0x7C}, // u
    {0x1C, 0x20, 0x40, 0x20, 0x1C}, // v
    {0x3C, 0x40, 0x30, 0x40, 0x3C}, // w
    {0x44, 0x28, 0x10, 0x28, 0x44}, // x
    {0x0C, 0x50, 0x50, 0x50, 0x3C}, // y
    {0x44, 0x64, 0x54, 0x4C, 0x44}, // z
};

void ssd1306_cursor(uint8_t x, uint8_t y)
{
  ssd1306_x = x;
  ssd1306_y = y;

  x *= 8;
  ssd1306_send_command(0x20); // Set memory addressing mode
  ssd1306_send_command(0x00); // Horizontal addressing mode
  ssd1306_send_command(0xB0 | y); // Set page address
  ssd1306_send_command(0x00 | (x & 0x0F)); // Set lower column address
  ssd1306_send_command(0x10 | ((x >> 4) & 0x0F));
}

void ssd1306_putchar(uint8_t c)
{
  if (c == '\n')
  {
    ssd1306_x = 0;
    ssd1306_y++;
    ssd1306_cursor(ssd1306_x, ssd1306_y);
  }
  else
  {
    for (uint8_t i = 0; i < 5; i++)
      ssd1306_send_data(FONT_5X7[c - 32][i]);
    ssd1306_send_data(0);
    ssd1306_send_data(0);
    ssd1306_send_data(0);
    ssd1306_x++;
    if (ssd1306_x >= 16)
    {
      ssd1306_y++;
      ssd1306_x = 0;
      ssd1306_cursor(ssd1306_x, ssd1306_y);
    }
  }
}

void ssd1306_putstr(uint8_t *s)
{
  for (uint8_t i = 0; s[i] != 0; i++)
  {
    ssd1306_putchar(s[i]);
  }
}

uint8_t chachabytes[64];
uint8_t chachaposition;
uint32_t chachablock;

uint32_t lr(uint32_t a, uint8_t b)
{
	return (a << b) | (a >> (32 - b));
}

void QR(uint32_t *cc, uint8_t a, uint8_t b, uint8_t c, uint8_t d)
{
	cc[a] += cc[b]; cc[d] ^= cc[a]; cc[d] = lr(cc[d], 16);
	cc[c] += cc[d]; cc[b] ^= cc[c]; cc[b] = lr(cc[b], 12);
	cc[a] += cc[b]; cc[d] ^= cc[a]; cc[d] = lr(cc[d], 8);
	cc[c] += cc[d]; cc[b] ^= cc[c]; cc[b] = lr(cc[b], 7);
}

void DR(uint32_t *cc)
{
	QR(cc, 0, 4,  8, 12);
	QR(cc, 1, 5,  9, 13);
	QR(cc, 2, 6, 10, 14);
	QR(cc, 3, 7, 11, 15);
	QR(cc, 0, 5, 10, 15);
	QR(cc, 1, 6, 11, 12);
	QR(cc, 2, 7,  8, 13);
	QR(cc, 3, 4,  9, 14);
}

void CB(uint32_t *cc)
{
	uint8_t i;
	uint32_t x[16];
	for (i = 0; i < 16; i++)
	{
		x[i] = cc[i];
	}
	for (i = 0; i < 10; i++)
	{
		DR(cc);
	}
	for (i = 0; i < 16; i++)
	{
		cc[i] += x[i];
	}
}

void S(uint32_t *cc, uint8_t *cs)
{
	for (uint8_t i = 0; i < 16; i++)
	{
		cs[4 * i] = (cc[i] & 0xFF);
		cs[4 * i + 1] = ((cc[i] >> 8) & 0xFF);
		cs[4 * i + 2] = ((cc[i] >> 16) & 0xFF);
		cs[4 * i + 3] = ((cc[i] >> 24) & 0xFF);
	}
}

void dochacha
(
  uint32_t blk0,
  uint32_t key0,
  uint32_t key1,
  uint32_t key2,
  uint32_t key3,
  uint32_t key4,
  uint32_t key5,
  uint32_t key6,
  uint32_t key7,
  uint32_t non0,
  uint32_t non1,
  uint32_t non2
)
{
	uint32_t cc[] =
	{
       0x61707865, 0x3320646e, 0x79622d32, 0x6b206574,
       key0, key1, key2, key3,
       key4, key5, key6, key7,
       blk0, non0, non1, non2
	};

	CB(cc);
	S(cc, chachabytes);
}

uint32_t rr(uint32_t a, uint32_t b)
{
    return (a >> b) | (a << (32 - b));
}


uint32_t shablock[16] =
{
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,

    0x80000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000000,
    0x00000100
};

uint32_t h[] =
{
    0x6a09e667,
    0xbb67ae85,
    0x3c6ef372,
    0xa54ff53a,
    0x510e527f,
    0x9b05688c,
    0x1f83d9ab,
    0x5be0cd19
};

uint32_t k[] =
{
   0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
   0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
   0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
   0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
   0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
   0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
   0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
   0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void sha256_process_block(uint32_t *p)
{

    uint8_t i;
    uint32_t w[64];
    for (i = 0; i < 64; i++) w[i] = 0;
    for (i = 0; i < 16; i++) w[i] = p[i];
    
    uint32_t A, B, C, D, E, F, G, H;
    uint32_t s0, s1, t1, t2, ch, maj;
    for (i = 16; i < 64; i++)
    {
        s0 = rr(w[i - 15], 7) ^ rr(w[i - 15], 18) ^ (w[i - 15] >> 3);
        s1 = rr(w[i - 2], 17) ^ rr(w[i - 2], 19) ^ (w[i - 2] >> 10);
        w[i] = w[i - 16] + s0 + w[i - 7] + s1;
    }

    i = 0;
    
    A = h[0];
    B = h[1];
    C = h[2];
    D = h[3];
    E = h[4];
    F = h[5];
    G = h[6];
    H = h[7];

    for (i = 0; i < 64; i++)
    {
        s1 = rr(E, 6) ^ rr(E, 11) ^ rr(E, 25);
        ch = (E & F) ^ ((~E) & G);
        t1 = H + s1 + ch + k[i] + w[i];
        s0 = rr(A, 2) ^ rr(A, 13) ^ rr(A, 22);
        maj = (A & B) ^ (A & C) ^ (B & C);
        t2 = s0 + maj;

        H = G;
        G = F;
        F = E;
        E = D + t1;
        D = C;
        C = B;
        B = A;
        A = t1 + t2;
    }

    h[0] += A;
    h[1] += B;
    h[2] += C;
    h[3] += D;
    h[4] += E;
    h[5] += F;
    h[6] += G;
    h[7] += H;

}


uint8_t EntropyTest_GrabBit(uint8_t *from, uint32_t position, uint32_t size)
{
    if (position / 8 >= size) return 0;
    return (from[position / 8] >> (8 - (position % 8) - 1)) & 0b1;
}

uint16_t EntropyTest_GrabBits(uint8_t *from, uint32_t position, uint32_t size, uint8_t count)
{
    uint16_t r = 0;
    for (uint16_t i = 0; i < count; i++)
    {
        r <<= 1;
        r |= EntropyTest_GrabBit(from, position + i, size);
    }
    return r;
}

double EntropyTest_Once(uint8_t *b, uint32_t size, uint8_t level)
{
    uint16_t symbols = 1 << level;
    uint32_t totals[symbols];
    for (uint16_t i = 0; i < symbols; i++)
    {
        totals[i] = 0;
    }
    for (uint32_t i = 0; i < size * 8; i += level)
    {
        uint16_t index = EntropyTest_GrabBits(b, i, size, level);
        totals[index]++;
    }
    double score = 0;
    uint32_t s = size * 8;
    s = s % level != 0 ? s + (level - (s % level)) : s; 
    s = s / level;

    for (uint16_t i = 0; i < symbols; i++)
    {
        double p = (double)totals[i] / (double)s;
        if (p > 0) score += p * -(log(p) / log(2));
    }
    score /= level;
    return score;
}

uint8_t extract_from_analog(uint8_t type)
{
    uint8_t a, b;
    if (type == 0)
    {
      a = (analogRead(A0)) & 1;
      b = (analogRead(A1)) & 1;
    }
    else if (type == 1)
    {
      a = (analogRead(A2)) & 1;
      b = (analogRead(A3)) & 1;
    }

    return a ^ b;
}

uint8_t extract()
{
  while (1)
  {
    uint8_t a = extract_from_analog(0);
    uint8_t b = extract_from_analog(1);
    if (a == 0 && b == 0) continue;
    if (a == 1 && b == 1) continue;
    return (a == 0 && b == 1) ? 0 : 1;
  }
}


uint8_t buff[320];

void sha256_load(uint32_t offset, uint32_t offset2)
{
  uint32_t *bigbuff = (uint32_t*)buff;
  for (uint8_t i = 0; i < 8; i++)
  {
    shablock[i] = bigbuff[i + offset];
  }
  for (uint8_t i = 8; i < 16; i++)
  {
    shablock[i] = bigbuff[i + offset2];
  }
  sha256_process_block(shablock);
  for (uint8_t i = 0; i < 8; i++)
  {
    bigbuff[i + offset] = h[i];
  }
}

void sha256_apply_to_all()
{
  uint32_t *bigbuff = (uint32_t*)buff;
  sha256_load(0, 8);
  sha256_load(8, 16);
  sha256_load(16, 24);
  sha256_load(24, 32);
  sha256_load(32, 40);
  sha256_load(40, 48);
  sha256_load(48, 0);

  /*
  Serial.print("----------------------------------------------------\n");
  Serial.print("{ ");
  for (uint8_t i = 0; i < 8; i++)
  {
      if (i > 0) Serial.print(",");
      Serial.print(h[i], HEX);
      Serial.print(" ");
  }
  Serial.print("}\n");*/
}

void chacha_apply_to_all()
{
  uint32_t *bigbuff = (uint32_t*)buff;
  for (uint32_t k = 0; k < 4; k++)
  {
    for (uint32_t i = 0;;i++)
    {
      dochacha
      (
        i,
        bigbuff[0 + k * 11],
        bigbuff[1 + k * 11],
        bigbuff[2 + k * 11],
        bigbuff[3 + k * 11],
        bigbuff[4 + k * 11],
        bigbuff[5 + k * 11],
        bigbuff[6 + k * 11],
        bigbuff[7 + k * 11],
        bigbuff[8 + k * 11],
        bigbuff[9 + k * 11],
        bigbuff[10 + k * 11]
      );
      for (uint8_t j = 0; j < 64; j++)
      {
        Serial.write(chachabytes[j]);
      }
      if (i == 4294967295) break;
    }
  }
}



void setup() {
  Serial.begin(1000000);


  SoftwareI2C.begin(PIN3, PIN2);  
  ssd1306_init();
  ssd1306_clear_display();
  ssd1306_cursor(0, 0);
  ssd1306_putstr("Gathering\nentropy . . .");

  ssd1306_cursor(0, 2);
  ssd1306_putstr("-> 0%");

  uint32_t *bigbuff = (uint32_t*)buff;
  *(bigbuff) = 0b10000000;
  *(bigbuff + 16) = 0b10000000;
  *(bigbuff + 32) = 0b10000000;
  *(bigbuff + 48) = 0b10000000;
}

uint16_t buffpos = 0;
uint16_t buffsubpos = 0;
uint8_t shouldSkip = 1;
uint8_t str[128];
void loop()
{
  if (buffpos == 0 && buffsubpos == 0 && shouldSkip == 0)
  {
    sprintf(str, "-> 100%%\n-> Verifying...");
    ssd1306_cursor(0, 2);
    ssd1306_putstr(str);
    uint8_t shaAlreadyApplied = 0;
    double lowest = 1;
    for (int i = 1; i <= 5; i++)
    {
      double score = EntropyTest_Once(buff, sizeof(buff), i);
      if (score < lowest) lowest = score;
    }
    if (!(lowest >= 0.99))
    {
      sha256_apply_to_all();
      lowest = 1;
      shaAlreadyApplied = 1;
      for (int i = 1; i <= 5; i++)
      {
        double score = EntropyTest_Once(buff, sizeof(buff), i);
        if (score < lowest) lowest = score;
      }
    }
    uint8_t pct = lowest * 100.0;
    if (lowest >= 0.99)
    {
      if (!shaAlreadyApplied) sha256_apply_to_all();

      sprintf(str, "-> PASS (%i%%)  \nReady.", pct);
      ssd1306_cursor(0, 4);
      ssd1306_putstr(str);

      chacha_apply_to_all();

      ssd1306_clear_display();
      ssd1306_cursor(0, 0);
      ssd1306_putstr("Gathering\nentropy . . .");
      ssd1306_cursor(0, 2);
      ssd1306_putstr("-> 0%");
      
    }
    else
    {
      sprintf(str, "-> FAIL (%i%%)  ", pct);
      ssd1306_cursor(0, 4);
      ssd1306_putstr(str);
    }
  }

  buff[buffpos] <<= 1;
  buff[buffpos] |= extract();
  buffsubpos++;
  if (buffsubpos >= 8)
  {
    buffsubpos = 0;
    buffpos = (buffpos + 1) % sizeof(buff);
    shouldSkip = 0;

    if (buffpos != 0 || buffsubpos != 0)
    {
      double pct = (buffpos * 100.0) / ((double)sizeof(buff));
      sprintf(str, "-> %i%% ", (int)pct);
      ssd1306_cursor(0, 2);
      ssd1306_putstr(str);
    }
  }
}

